/*
 *  telephony-emulator
 *
 * Copyright (c) 2000 - 2013 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact:
 * Sooyoung Ha <yoosah.ha@samsung.com>
 * YeongKyoon Lee <yeongkyoon.lee@samsung.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * Contributors:
 * - S-Core Co., Ltd
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vgsm_ss.h"
#include "oem_tx_ss.h"
#include "server_common_ss.h"
#include "db_ss.h"
#include "misc.h"
#include "vgsm_phone.h"
#include "state.h" // gen_resp()
#include "phoneserver.h"
#include "server_rx_ss.h"
#include "server_rx_call.h"
#include "vgsm_debug.h"
#include "logmsg.h"
#include "at_func.h"
#include "at_recv.h"
#include "at_gen_resp.h"

#define SS_ERR_NEGATIVE_PW_CHECK 	0x8126
#define SS_PW_ATTEMPS_VIOLATION	0x812b

// 090305
int server_rx_ss_cw_get(int tel_class)
{
    VGSM_DEBUG("\n");

    call_waiting_entry_t * cw_entry;
    char * data = 0;
    int data_len = 64;
    int ss_status, ret;

    log_msg(MSGL_VGSM_INFO,"tel_class = %d \n", tel_class);
    cw_entry = find_call_waiting_entry(tel_class);
    if(cw_entry->ss_mode == SS_MODE_ACT)
	ss_status = AT_SS_STATUS_ACTIVE;
    else
	ss_status = AT_SS_STATUS_NOT_ACTIVE;

    data = malloc(sizeof(char) * data_len);
    strcpy(data, CCWA);
    sprintf(&data[strlen(data)], "%d", ss_status);
    strcat(data, TOKEN);
    sprintf(&data[strlen(data)], "%d", AT_CALL_CLASS_VOICE);
    strcat(data, CRLF);

    log_msg(MSGL_VGSM_INFO,"found & resp: tel_class = %d, ss_mode = %d, ss_status = %d \n"
	    , cw_entry->tel_class, cw_entry->ss_mode, ss_status);

    ret = oem_tx_ss_cw_resp(data, strlen(data));

    free(data);
    return 1;
}

int server_rx_ss_cb_get(int tel_class, int cb_type)
{
    VGSM_DEBUG("\n");

    call_barring_entry_t * cb_entry;

    log_msg(MSGL_VGSM_INFO,"tel_class = %d, cb_type = %d \n", tel_class, cb_type);

    cb_entry = find_call_barring_entry(tel_class, cb_type);

    return at_gen_resp_send(AT_GEN_ERR_NO_ERROR);
}

int server_rx_ss_cf_get(int tel_class, int cf_type)
{
    VGSM_DEBUG("\n");

    call_forwarding_entry_t * cf_entry;
    int ss_status, ret;
    char sndbuf[SEND_BUF_SIZE];
    memset(sndbuf, '\0', sizeof(sndbuf));

    log_msg(MSGL_VGSM_INFO,"tel_class = %d, cf_type = %d \n", tel_class, cf_type);
    cf_entry = find_call_forwarding_entry(tel_class, cf_type);

    log_msg(MSGL_VGSM_INFO," entry->count = %d \n", cf_entry->count);

    if(cf_entry->ss_mode == SS_MODE_REG || cf_entry->ss_mode == SS_MODE_ACT)
	ss_status = AT_SS_STATUS_ACTIVE;
    else
	ss_status = AT_SS_STATUS_NOT_ACTIVE;

    sprintf(sndbuf, "%s%d,%d", CCFC, ss_status, cf_entry->tel_class);
    if(ss_status == AT_SS_STATUS_ACTIVE)
    {
	log_msg(MSGL_VGSM_INFO, "cf number:%s\n", cf_entry->number);
	sprintf(&sndbuf[strlen(sndbuf)], ",%s", cf_entry->number);
    }
    strcat(sndbuf, CRLF);

    ret = oem_tx_ss_cf_resp(sndbuf, strlen(sndbuf));

    return 1;
}

int server_rx_ss_cli(char *api)
{
    at_unimplemented(api);
    return 1;
}

int server_rx_ss_cw_set(int tel_class, int ss_mode)
{
    VGSM_DEBUG("\n");

    int i;
    int db_err;
    LXT_MESSAGE packet;
    TAPIMessageInit(&packet);

    log_msg(MSGL_VGSM_INFO,"tel_class = %d, ss_mode = %d \n", tel_class, ss_mode);

    /* add or update */
    call_waiting_entry_t * entry = malloc(sizeof(call_waiting_entry_t));
    if(entry == NULL){
	log_msg(MSGL_VGSM_ERR,"malloc fail \n");
	return 0;
    }
    memset(entry, 0, sizeof(call_waiting_entry_t));
    entry->tel_class = tel_class;
    entry->ss_mode = ss_mode;
    strcpy(entry->number, SUBSCRIBER_NUM);
    log_msg(MSGL_VGSM_INFO, "%s", entry->number);
    if( ( db_err = vgsm_ss_database_add(SS_CMD_CW, entry) ) > 0){
	log_msg(MSGL_WARN,"vgsm_ss_database_add() is failed!!db_err = %d \n", db_err);
    }

    if(entry){
	free(entry);
    }

    /* resp */
    call_waiting_entry_t * resp_entry = get_call_waiting_entry();
    if(!resp_entry){
	/* Not happen ?? */
	log_msg(MSGL_WARN,"entry is NULL!!!\n");
    }

    if(resp_entry){
	for(i=0; i<resp_entry[0].count; i++){
	    log_msg(MSGL_VGSM_INFO,"i : %d,  tel_class : %d, ss_mode : %d\n"
		    , i, resp_entry[i].tel_class, resp_entry[i].ss_mode);
	}
    }

    resp_entry[0].tel_class = 255;// temporary	
    packet.data = resp_entry;
    packet.group  = GSM_SUPS;
    packet.action = GSM_SUPS_SET_CW_REQ;
    packet.length = (sizeof(call_waiting_entry_t) * resp_entry[0].count);

    FuncServer->Cast(&GlobalPS, LXT_ID_CLIENT_EVENT_INJECTOR, &packet);
    resp_entry[0].tel_class = 1;

    return 0;
}

//int server_rx_ss_cf_set(int ss_mode, int type, int tel_class, int replyTime, int num_len, char *number, int num_type, int satype, char* subaddr)
int server_rx_ss_cf_set(int ss_mode, int type, int tel_class, int replyTime, int num_len, char *number, int num_type)
{
    VGSM_DEBUG("\n");

    int i;
    LXT_MESSAGE packet;
    TAPIMessageInit(&packet);

    call_forwarding_entry_t * entry = malloc(sizeof(call_forwarding_entry_t));
    if(!entry)
	return -1;
    memset(entry, 0, sizeof(call_forwarding_entry_t));

    entry->ss_mode = ss_mode;
    entry->type = type;
    entry->tel_class = tel_class;
    entry->replyTime = replyTime;
    //	entry->number_type = num_type;
    //	entry->satype = satype;
    //	memcpy(entry->subaddr, subaddr, MAX_GSM_DIALED_DIGITS_NUMBER);

    if(num_len)
    {
	strcpy(entry->number, number);
	log_msg(MSGL_VGSM_INFO,"============ %s(%d) \n", entry->number, num_len);
    }


    // 090326
    log_msg(MSGL_VGSM_INFO,"=check=========== (%x)(%x) \n", entry->type, entry->ss_mode);
    if(entry->ss_mode == SS_MODE_DEACT || entry->ss_mode == SS_MODE_DEREG)
    {
	// 090326
	//if(entry->type == SS_CF_TYPE_CF_ALL && entry->ss_mode == SS_MODE_DEACT)		//disable all call forwarding
	if(entry->type == SS_CF_TYPE_CF_ALL)		//disable all call forwarding
	{
	    // 090327  SS_CF_TYPE_CFU ---> SS_CF_TYPE_CF_ALL
	    if(vgsm_ss_database_remove(SS_CMD_CF, entry->tel_class, SS_CF_TYPE_CF_ALL) > 0)
		log_msg(MSGL_WARN,"vgsm_ss_database_remove() SS_CF_TYPE_CFU is failed!!\n");

	}
	// 090326
	//else if(entry->type == SS_CF_TYPE_CFC && entry->ss_mode == SS_MODE_DEACT)		//disable all conditional call forwarding
	else if(entry->type == SS_CF_TYPE_CFC)		//disable all conditional call forwarding
	{
	    if(vgsm_ss_database_remove(SS_CMD_CF, entry->tel_class, SS_CF_TYPE_CFB) > 0)
		log_msg(MSGL_WARN,"vgsm_ss_database_remove() SS_CF_TYPE_CFB is failed!!\n");

	    if(vgsm_ss_database_remove(SS_CMD_CF, entry->tel_class, SS_CF_TYPE_CFNRy) > 0)
		log_msg(MSGL_WARN,"vgsm_ss_database_remove() SS_CF_TYPE_CFNRy is failed!!\n");

	    if(vgsm_ss_database_remove(SS_CMD_CF, entry->tel_class, SS_CF_TYPE_CFNRc) > 0)
		log_msg(MSGL_WARN,"vgsm_ss_database_remove() SS_CF_TYPE_CFNRc is failed!!\n" );
	}
	// add 090326
	else
	{
	    if(entry->type == SS_CF_TYPE_CFU)
	    {
		if(vgsm_ss_database_remove(SS_CMD_CF, entry->tel_class, SS_CF_TYPE_CFU) > 0)
		    log_msg(MSGL_WARN, "vgsm_ss_database_remove() SS_CF_TYPE_CFU is failed!!\n");
	    }
	    else if(entry->type == SS_CF_TYPE_CFB)
	    {
		if(vgsm_ss_database_remove(SS_CMD_CF, entry->tel_class, SS_CF_TYPE_CFB) > 0)
		    log_msg(MSGL_WARN, "vgsm_ss_database_remove() SS_CF_TYPE_CFB is failed!!\n");
	    }
	    else if(entry->type == SS_CF_TYPE_CFNRy)
	    {
		if(vgsm_ss_database_remove(SS_CMD_CF, entry->tel_class, SS_CF_TYPE_CFNRy) > 0)
		    log_msg(MSGL_WARN, "vgsm_ss_database_remove() SS_CF_TYPE_CFNRy is failed!!\n" );
	    }
	    else if(entry->type == SS_CF_TYPE_CFNRc)
	    {
		if(vgsm_ss_database_remove(SS_CMD_CF, entry->tel_class, SS_CF_TYPE_CFNRc) > 0)
		    log_msg(MSGL_WARN,"vgsm_ss_database_remove() SS_CF_TYPE_CFNRc is failed!!\n");
	    }
	}
    }
    else
    {
	//call forwarding table update
	if(vgsm_ss_database_add(SS_CMD_CF, entry) > 0)
	    log_msg(MSGL_VGSM_INFO,"vgsm_ss_database_add() is failed!!\n" );		
    }

    if(entry)
	free(entry);

    // send data to eventinjector -> ui update
    call_forwarding_entry_t * resp_entry = get_call_forwarding_entry();

    //for debug
    if(!resp_entry)
	log_msg(MSGL_WARN,"entry is NULL!!! \n" );
    if(resp_entry)
    {
	for(i=0; i<resp_entry[0].count; i++)
	{
	    log_msg(MSGL_WARN,"i: %d, tel_class : 0x%x, type : %d, number : %s, ss_mode : %d\n",
		    i, resp_entry[i].tel_class, resp_entry[i].type, resp_entry[i].number, resp_entry[i].ss_mode);
	}
    }

    packet.data = resp_entry;
    packet.group  = GSM_SUPS;
    packet.action = GSM_SUPS_SET_CCFC_REQ;
    packet.length = (sizeof(call_forwarding_entry_t) * resp_entry[0].count);

    FuncServer->Cast(&GlobalPS, LXT_ID_CLIENT_EVENT_INJECTOR, &packet);

    return 1;
}

int server_rx_ss_cb_set(int cb_type, int ss_mode, int tel_class, char *passwd)
{
    VGSM_DEBUG("\n");

    int i, gen_resp_err;
    LXT_MESSAGE packet;
    TAPIMessageInit(&packet);
    int cb_pwd_fail_count = 0;

    call_barring_entry_t * entry =  malloc(sizeof(call_barring_entry_t));
    if(!entry)
	return -1;
    memset(entry, 0, sizeof(call_barring_entry_t));

    entry->type =cb_type;
    entry->ss_mode = ss_mode;
    entry->tel_class = tel_class;
    char* tmp_cb_pwd = get_callbarring_pwd();
    VGSM_DEBUG("orig_pwd:%s, input_pwd:%s\n", tmp_cb_pwd, passwd);

    if( strcmp(passwd, tmp_cb_pwd) != 0)
    {
	VGSM_DEBUG("Wrong CallBarring Passwd\n");
	cb_pwd_fail_count = increase_callbarring_pwd_fail_count();
	VGSM_DEBUG("cb_pwd_fail_count = %d\n",cb_pwd_fail_count);

	if(cb_pwd_fail_count >= 3)
	    gen_resp_err = SS_PW_ATTEMPS_VIOLATION; // numberOfPW_AttemptsViolation
	else
	    gen_resp_err = SS_ERR_NEGATIVE_PW_CHECK; // NegativePasswordCheck

	oem_tx_ss_gen_resp(AT_CME_ERR_INCORRECT_PWD);
	free(entry);
	return 1;
    }

    VGSM_DEBUG("Right CallBarring Passwd\n");

    clear_callbarring_pwd_fail_count();

    /* gen resp */
    set_ss_current_general_response_error( get_ss_general_response_error() );
    gen_resp_err = get_ss_current_general_response_error();
    oem_tx_ss_gen_resp(AT_GEN_ERR_NO_ERROR);

    if(gen_resp_err != 0x8000)
    {
	log_msg(MSGL_WARN, "gen_resp_err : %d \n", gen_resp_err);
	free(entry);
	return 1;
    }

    if(entry->tel_class == AT_CALL_SS_CLASS_VIDEO)
    {
	if(entry->type == SS_CB_TYPE_AB)
	{
	    set_outgoing_video_call_barring_state(entry->ss_mode);
	    set_incoming_video_call_barring_state(entry->ss_mode);
	}
	else if(entry->type == SS_CB_TYPE_BAOC)
	{
	    set_outgoing_video_call_barring_state(entry->ss_mode);
	}
	else if(entry->type == SS_CB_TYPE_BAIC)
	{
	    set_incoming_video_call_barring_state(entry->ss_mode);
	}
    }
    else
    {
	if(entry->type == SS_CB_TYPE_AB)
	{
	    set_outgoing_voice_call_barring_state(entry->ss_mode);
	    set_incoming_voice_call_barring_state(entry->ss_mode);
	}
	else if(entry->type == SS_CB_TYPE_BAOC)
	{
	    set_outgoing_voice_call_barring_state(entry->ss_mode);
	}
	else if(entry->type == SS_CB_TYPE_BAIC)
	{
	    set_incoming_voice_call_barring_state(entry->ss_mode);
	}
    }

    if( (entry->type == SS_CB_TYPE_AB ) && (entry->ss_mode == SS_MODE_DEREG || entry->ss_mode == SS_MODE_DEACT)) // cancel all.
    {
	entry->type = SS_CB_TYPE_BAOC;
	if(vgsm_ss_database_add(SS_CMD_CB, entry) > 0)
	    log_msg(MSGL_WARN, "vgsm_ss_database_add() is failed!!\n" );

	entry->type = SS_CB_TYPE_BOIC;
	if(vgsm_ss_database_add(SS_CMD_CB, entry) > 0)
	    log_msg(MSGL_WARN, "vgsm_ss_database_add() is failed!!\n" );

	entry->type = SS_CB_TYPE_BOIC_NOT_HC;
	if(vgsm_ss_database_add(SS_CMD_CB, entry) > 0)
	    log_msg(MSGL_WARN, "vgsm_ss_database_add() is failed!!\n" );

	entry->type = SS_CB_TYPE_BAIC;
	if(vgsm_ss_database_add(SS_CMD_CB, entry) > 0)
	    log_msg(MSGL_WARN, "vgsm_ss_database_add() is failed!!\n" );

	entry->type = SS_CB_TYPE_BIC_ROAM;			
	if(vgsm_ss_database_add(SS_CMD_CB, entry) > 0)
	    log_msg(MSGL_WARN, "vgsm_ss_database_add() is failed!!\n" );

	entry->type = SS_CB_TYPE_AB; //rollback.
    }		
    else
    {
	if(vgsm_ss_database_add(SS_CMD_CB, entry) > 0)
	    log_msg(MSGL_WARN, "vgsm_ss_database_add() is failed!!\n" );		
    }

    if(entry)
	free(entry);

    call_barring_entry_t * resp_entry  = get_call_barring_entry();

    //for debug
    if(!resp_entry)
	log_msg(MSGL_WARN, "entry is NULL!!! \n" );
    if(resp_entry)
    {
	for(i=0; i<resp_entry[0].count; i++)
	{
	    log_msg(MSGL_VGSM_INFO,"i : %d,  tel_class : %d, ss_mode : %d\n",
		    i, resp_entry[i].tel_class, resp_entry[i].ss_mode);
	}
    }

    packet.data = resp_entry;
    packet.group  = GSM_SUPS;
    packet.action = GSM_SUPS_SET_CB_REQ;
    packet.length = (sizeof(call_barring_entry_t) * resp_entry[0].count);

    FuncServer->Cast(&GlobalPS, LXT_ID_CLIENT_EVENT_INJECTOR, &packet);

    return 1;

}

int server_rx_ss_cb_passwd_set(char* curr_passwd, char* new_passwd)
{
    VGSM_DEBUG("\n");
    if(!curr_passwd || !new_passwd)
	return -1;

    int gen_resp_err;
    LXT_MESSAGE packet;
    TAPIMessageInit(&packet);
    int cb_pwd_fail_count = 0;

    char* tmp_cb_pwd = get_callbarring_pwd();

    if( memcmp(tmp_cb_pwd, curr_passwd, 4 ) != 0)
    {
	VGSM_DEBUG("Wrong CallBarring Passwd\n");
	cb_pwd_fail_count = increase_callbarring_pwd_fail_count();
	VGSM_DEBUG("cb_pwd_fail_count = %d\n",cb_pwd_fail_count);

	if(cb_pwd_fail_count >= 3)
	    gen_resp_err = SS_PW_ATTEMPS_VIOLATION; // numberOfPW_AttemptsViolation
	else
	    gen_resp_err = SS_ERR_NEGATIVE_PW_CHECK; // NegativePasswordCheck

	oem_tx_ss_gen_resp(gen_resp_err);
	return 1;
    }

    VGSM_DEBUG("Right CallBarring Passwd\n");

    clear_callbarring_pwd_fail_count();

    /* gen resp */
    set_ss_current_general_response_error( get_ss_general_response_error() );
    gen_resp_err = get_ss_current_general_response_error();
    oem_tx_ss_gen_resp(gen_resp_err);
    if(gen_resp_err != 0x8000)
    {
	log_msg(MSGL_WARN,"gen_resp_err : %d \n", gen_resp_err);
	return 1;
    }

    set_callbarring_pwd(new_passwd);

    VGSM_DEBUG("CallBarring Passwd changed\n");

    packet.data = new_passwd;
    packet.group  = GSM_SUPS;
    packet.action = GSM_SUPS_PWDSET_CB_REQ;
    packet.length = strlen(new_passwd);

    VGSM_DEBUG("CallBarring Passwd changed\n");

    FuncServer->Cast(&GlobalPS, LXT_ID_CLIENT_EVENT_INJECTOR, &packet);
    /* EI receives this event then,
     * popup(callbarring password changed!!) --> callbarring password : new_passwd
     */

    return 1;
}

int server_rx_ss_aoc_get(void)				// 090215   vgsm -> EI
{
    LXT_MESSAGE packet;
    unsigned char * data = 0;
    int		data_len;
    _AOC_t	tmp;

    tmp.acm = 123;
    tmp.ccm = 234;
    tmp.maxacm = 345;
    tmp.ppu = 456;
    tmp.chartype = 7;

    data_len = sizeof(tmp);

    VGSM_DEBUG("\n");

    get_aoc_data( &tmp);

    data = malloc(sizeof(unsigned char) * data_len);
    memset(data, 0, data_len);
    memcpy(data, &tmp, data_len);

    VGSM_DEBUG("Send AOC DATA to EI\n");
    packet.data = data;
    packet.group  = GSM_SUPS;
    packet.action = GSM_SUPS_AOC_GET;
    packet.length = data_len;

    VGSM_DEBUG("Send AOC DATA to EI\n");
    FuncServer->Cast(&GlobalPS, LXT_ID_CLIENT_EVENT_INJECTOR, &packet);

    free(data);
    return 1;
}

int server_rx_ss_ussd_get(void)				// 090215   vgsm -> EI
{
    LXT_MESSAGE packet;
    unsigned char * data = 0;
    int		data_len;
    _USSD_data_t	tmp;

    strcpy(tmp.time, "12:00:00, 2000");
    strcpy(tmp.weather, "Fine");

    data_len = sizeof(tmp);

    VGSM_DEBUG("\n");

    get_ussd_data( &tmp);

    data = malloc(sizeof(unsigned char) * data_len);
    memset(data, 0, data_len);
    memcpy(data, &tmp, data_len);

    VGSM_DEBUG("Send USSD DATA to EI\n");
    VGSM_DEBUG("time : %s\n", tmp.time);
    VGSM_DEBUG("weather : %s\n", tmp.weather);
    packet.data = data;
    packet.group  = GSM_SUPS;
    packet.action = GSM_SUPS_USSD_GET;
    packet.length = data_len;

    VGSM_DEBUG("Send ussd DATA to EI\n");
    FuncServer->Cast(&GlobalPS, LXT_ID_CLIENT_EVENT_INJECTOR, &packet);

#if 1
    free(data);
#endif
    return 1;
}

int server_rx_ss_manage_call_set(char* ptr_data, int data_len)
{
    int rtn=0;
    int signal = 0;
    char* data = strchr(ptr_data, '=');
    char token[] = "\n";
    char* ret = NULL;
    int call_id = 0;

    ret = strtok(data+1, token);
    if(ret)
	signal = atoi(ret);

    if(signal == AT_GSM_SS_CM_0_SEND) 	// release all held call
    {
	rtn = server_rx_call_release_incoming();
	if(rtn == 0)
	    rtn = server_rx_call_release_all_held();
    }
    else if(signal == AT_GSM_SS_CM_1_SEND)    // release all active call and the other (held or waiting) call
    {
	rtn = server_rx_call_release_all_active(); 
    }
    else if(signal == AT_GSM_SS_CM_2_SEND)    // active call on hold and held or waiting call on active
    {
	VGSM_DEBUG("g_ss_hold_response_error: err =%x \n", g_ss_hold_response_error);   
	//at_gen_resp_send( g_ss_hold_response_error);
	if ( g_ss_hold_response_error == 0x8000 )
	    rtn = server_rx_call_swap();
	else
	    VGSM_DEBUG("g_ss_hold_response_error: err =%x \n", g_ss_hold_response_error);
    }
    else if(signal == AT_GSM_SS_CM_3_SEND)    // add a held call to the conversation
    {
	VGSM_DEBUG("g_ss_join_response_error: err =%x \n", g_ss_join_response_error);
	//at_gen_resp_send( g_ss_join_response_error);
	if ( g_ss_join_response_error == 0x8000   )
	    rtn = server_rx_call_join_conf();  // TODO: call id is needed?? check....
	else
	    VGSM_DEBUG("g_ss_join_response_error: err =%x \n", g_ss_join_response_error);
    }
    else if(signal >= 10 && signal < 20 )    // AT_GSM_SS_CM_1X_SEND : connect the two call and disconnect the subscriber from both calls
    {
	call_id = signal - 11;
	rtn = server_rx_call_release_single(call_id);
    }
    else if(signal >= 20 && signal < 30 )	// AT_GSM_SS_CM_2X_SEND : Places all active calls on hold except call X with which communication shall be supported.
    {
	call_id = signal - 21;
	VGSM_DEBUG("server_rx_call_split_conf : err =%x \n", g_ss_split_response_error);
	//at_gen_resp_send( g_ss_split_response_error);
	if ( g_ss_split_response_error == 0x8000   )
	    rtn = server_rx_call_split_conf(call_id);
	else
	    VGSM_DEBUG("server_rx_call_split_conf : err =%x \n", g_ss_split_response_error);
    }
    else
    {
	VGSM_DEBUG (" not supported ID = %d \n" , signal);
    }

    return rtn;
}


