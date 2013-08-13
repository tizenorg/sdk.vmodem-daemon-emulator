/*
 *  telephony-emulator
 *
 * Copyright (c) 2000 - 2011 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact: 
 * Sooyoung Ha <yoosah.ha@samsung.com>
 * Sungmin Ha <sungmin82.ha@samsung.com>
 * YeongKyoon Lee <yeongkyoon.lee@samsung.com>
 * 
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation; either version 2.1 of the License, or (at your option)
 * any later version.
 * 
 * This library is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc., 51
 * Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 * Contributors:
 * - S-Core Co., Ltd
 * 
 */

/////////////////////////////////////////////////////////////////////
// server_tx_calll.c

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "state.h" // linuxtapi.h
#include "misc.h"

#include "phoneserver.h"

#include "vgsm_call.h"
#include "oem_tx_call.h"
#include "server_tx_call.h"
#include "server_common_call.h"
#include "server_tx_ss.h"
#include "logmsg.h"
#include "at_gen_resp.h"
#include "at_func.h"

// caution: MAX_GSM_DIALED_DIGITS_NUMBER and MAX_GSM_DIALED_DIGITS_NUM are confusingly similar

int server_tx_call_msg(void)
{
	_ENTER();

	STATE state;

	get_current_state_machine(&state );

	switch(STATE_SUB_CMD(state))
	{
		case GSM_CALL_STATUS:
			server_tx_call_status();
			break;
		case GSM_CALL_LIST:
			server_tx_call_list_resp();
			break;
		default:
			log_msg(MSGL_WARN, "unknown subcommand %d\n", STATE_SUB_CMD(state));
	}

	_LEAVE();

	return 1;
}

//int TxCall_CallStatus()
int server_tx_call_status(void) // it means call state.
{
	int ret = 0;
	STATE prev;
	int i;
	int call_state = 0;

	TRACE(MSGL_VGSM_INFO, "\n");

	LXT_MESSAGE packet;
	gsm_call_list_t * callList = malloc(sizeof(gsm_call_list_t));
	if(!callList)
		return -1;
	memset(callList, 0, sizeof(gsm_call_list_t));

	get_prev_state_machine( &prev );

	//	if( STATE_SUB_CMD(prev) == GSM_CALL_OUTGOING )
	if( STATE_TYPE(prev) == STATE_STANDBY ) // it means GSM_CALL_OUTGOING in state_machine.
	{
		call_state = GSM_CALL_STATE_OUTGOING;
		log_msg(MSGL_VGSM_INFO, "STATE_STANBY\n");
		set_call_list_status( get_call_id(), 0, GSM_CALL_STATUS_DIALING );
		get_call_list(callList);
	}
	else if( STATE_TYPE(prev) == STATE_CALL_WAITING_OUTGOING || STATE_TYPE(prev) == STATE_CALL_WAITING_INCOMING )
	{
		call_state = GSM_CALL_STATE_CONNECTED;

		// find client id of incoming call & set active
		get_call_list(callList);
		for( i=0; i<MAX_CALL_COUNT; ++i ) {
			if(callList->CallInfo[i].stat  == GSM_CALL_STATUS_INCOMING || callList->CallInfo[i].stat == GSM_CALL_STATUS_WAITING){
				log_msg(MSGL_VGSM_INFO,"call id(%d) is GSM_CALL_STATUS_INCOMING \n", i);
				set_call_id( i );
				break;
			}
		}
		set_call_list_status( get_call_id(), 0, GSM_CALL_STATUS_ACTIVE );
		at_gen_resp_send(AT_GEN_ERR_CONNECT);

		// re get call list
		get_call_list(callList);

		for(i = 0; i < callList->CallCount; i++){
			log_msg(MSGL_VGSM_INFO,"call id : %d, number : %s, multiparty : %d, stat : %d \n",
					callList->CallInfo[i].idx, callList->CallInfo[i].number,
					callList->CallInfo[i].mpty, callList->CallInfo[i].stat);
		}

		packet.data = (gsm_call_list_t*)callList;
		packet.group  = GSM_CALL;
		packet.action = GSM_CALL_CONNECTED_LINE_NOTI;
		packet.length = sizeof(gsm_call_list_t);

		FuncServer->Cast(&GlobalPS, LXT_ID_CLIENT_EVENT_INJECTOR, &packet);
		if(get_call_id() >= callList->CallCount) {
			callback_callist();
		}
	}
	else if( STATE_TYPE(prev) == STATE_CALL_RELEASED )
	{
		call_state = GSM_CALL_STATE_RELEASED;
		at_gen_resp_send(AT_GEN_ERR_RELEASE);		

		get_call_list(callList);
		packet.data = (gsm_call_list_t*)callList;
		packet.group  = GSM_CALL;
		packet.action = GSM_CALL_STATUS_NOTI;
		packet.length = sizeof(gsm_call_list_t);

		FuncServer->Cast(&GlobalPS, LXT_ID_CLIENT_EVENT_INJECTOR, &packet);
		set_call_list_status( get_call_id(), 0, GSM_CALL_STATUS_NONE );
		get_call_list(callList);
	}
	/* Why does not it exist in case of STATE_CALL_CONVERSATION */

	/*	 In call status msg, incoming msg do not exist.
		 else if( STATE_SUB_CMD(prev) == GSM_CALL_INCOMING )
		 {
		 pdata[2] = GSM_CALL_STATE_INCOMING;
		 }
	 */

	unsigned char pdata[128];
	int at_stat = change_stat_for_at(callList->CallInfo[get_call_id()].stat);
	TRACE(MSGL_VGSM_INFO, "call id:%d, orignal stat: %d, send stat: %d\n", get_call_id(), callList->CallInfo[get_call_id()].stat, at_stat);
	
	if(strcmp(callList->CallInfo[get_call_id()].number, ""))
	{
		sprintf((char*)pdata, "%d,%d,%d,%d,%d,%s,%d", get_call_id() + 1, change_dir_for_at(callList->CallInfo[get_call_id()].dir), 
								at_stat, AT_CALL_MODE_VOICE, AT_CALL_MPTY_FALSE, 
								callList->CallInfo[get_call_id()].number,callList->CallInfo[get_call_id()].num_type); 
	}
	else
	{
		sprintf((char*)pdata, "%d,%d,%d,%d,%d", get_call_id() + 1, change_dir_for_at(callList->CallInfo[get_call_id()].dir), at_stat, AT_CALL_MODE_VOICE, AT_CALL_MPTY_FALSE);
	}
 
	free(callList);
	set_current_call_status_error( 0x00 );

	ret = oem_tx_call_status_noti(pdata, strlen((char*)pdata));

	_LEAVE();

	return ret;
}

int server_tx_call_list_noti(void)
{
	_ENTER();

	int i = 0, j = 0, ret = 0;
	int CallCount = 0;
	int len = 0;
	unsigned char *data = NULL;
	LXT_MESSAGE packet;
	gsm_call_list_t 	* callList = malloc(sizeof(gsm_call_list_t));

	get_call_list(callList);
	CallCount = callList->CallCount;
	// FIXME: probably want MAX_GSM_DIALED_DIGITS_NUM here
	len = 32 + (8+MAX_GSM_DIALED_DIGITS_NUMBER);
	data = malloc(len*2);
	log_msg(MSGL_VGSM_INFO,"CallCount %d\n", CallCount);
	j = 1;
	for (i=0; i < MAX_CALL_COUNT; i++) {
		if( callList->CallInfo[i].stat == GSM_CALL_STATUS_NONE )
			continue;
		log_msg(MSGL_VGSM_INFO,"index = %d    : Call_STATUS %d\n", i, callList->CallInfo[i].stat);
		
		assert(valid_call_type(callList->CallInfo[i].call_type));
		memset( data, 0, len * 2 );
		sprintf((char*)data, "%d,%d,%d,%d,%d,%s,%d%s", callList->CallInfo[i].idx + 1,
                                                        change_dir_for_at(callList->CallInfo[i].dir),
                                                        change_stat_for_at(callList->CallInfo[i].stat), AT_CALL_MODE_VOICE, callList->CallInfo[i].mpty, 
							callList->CallInfo[i].number, callList->CallInfo[i].num_type, CRLF);
                log_msg(MSGL_VGSM_INFO, "%s", data);
		ret = oem_tx_call_status_noti(data, strlen((char*)data));
	}
	
	/* send to EI */
	packet.data = (gsm_call_list_t*)callList;
	packet.length = sizeof(gsm_call_list_t);
	packet.group  = GSM_CALL;
	packet.action = GSM_CALL_CALL_LIST_IND;

	FuncServer->Cast(&GlobalPS, LXT_ID_CLIENT_EVENT_INJECTOR, &packet);

	free(data);
	free(callList);

	_LEAVE();

	return ret;
}

int change_stat_for_at(int gsm_stat)
{
	int stat = 0;
	switch(gsm_stat)
	{
	case GSM_CALL_STATUS_NONE:
		stat = AT_CALL_STAT_RELEASED;
		break;
	case GSM_CALL_STATUS_ACTIVE:
		stat = AT_CALL_STAT_ACTIVE;
		break;
	case GSM_CALL_STATUS_HELD:
		stat = AT_CALL_STAT_HELD;
		break;
	case GSM_CALL_STATUS_DIALING:
		stat = AT_CALL_STAT_DIALING;
		break;
	case GSM_CALL_STATUS_ALERT:
		stat = AT_CALL_STAT_ALERTING;
		break;
	case GSM_CALL_STATUS_INCOMING:
		stat = AT_CALL_STAT_INCOMING;
		break;
	case GSM_CALL_STATUS_WAITING:
		stat = AT_CALL_STAT_WAITING;
		break;
	case GSM_CALL_STATUS_MAX:
	default:
		stat = AT_CALL_STAT_DIALING;
		break;
	}

	return stat;
}

int change_dir_for_at(int gsm_dir)
{
	int dir = 0;

	switch(gsm_dir)
	{
	case GSM_CALL_DIR_MO:
		dir = 0;
		break;
	case GSM_CALL_DIR_MT:
		dir = 1;
		break;
	default:
		dir = 0;
		break;
	}
	return dir;
}

int server_tx_call_list_resp(void)
{
	int i = 0, ret = 0;
	int CallCount = 0;
	int len = 0;
	unsigned char *data = NULL;
	gsm_call_list_t * callList = malloc(sizeof(gsm_call_list_t));

	_ENTER();

	get_call_list(callList);
	CallCount = callList->CallCount;
	len = 32 + (8+MAX_GSM_DIALED_DIGITS_NUMBER);
	data = malloc(len*2);
	log_msg(MSGL_VGSM_INFO,"CallCount %d\n", CallCount);
	for (i=0; i < MAX_CALL_COUNT; i++) {
		if( callList->CallInfo[i].stat == GSM_CALL_STATUS_NONE )
			continue;
		log_msg(MSGL_VGSM_INFO,"index = %d    : Call_STATUS %d\n",  i, callList->CallInfo[i].stat);

		assert(valid_call_type(callList->CallInfo[i].call_type));
		memset( data, 0, len * 2);
		sprintf((char*)data, "%s%d,%d,%d,%d,%d,%s,%d%s", CLCC,callList->CallInfo[i].idx + 1, 
							change_dir_for_at(callList->CallInfo[i].dir),
							change_stat_for_at(callList->CallInfo[i].stat), 
							AT_CALL_MODE_VOICE, callList->CallInfo[i].mpty, 
							callList->CallInfo[i].number, callList->CallInfo[i].num_type, CRLF);
		log_msg(MSGL_VGSM_INFO, "%s", data);	
		ret = oem_tx_call_list_resp(data, strlen((char*)data));
	}
	at_gen_resp_send(AT_GEN_ERR_NO_ERROR);

	free(data);
	free(callList);

	_LEAVE();

	return ret;
}

static void cast_call_incomming(unsigned char call_id)
{
	_ENTER();

	unsigned char data[1];
	LXT_MESSAGE packet;
	TAPIMessageInit(&packet);

	/*   check general response error & call status error    */

	data[0] = call_id;		//The object is simulator,   0x01 : Outgoing Call, 0x02 : Answer Call, 0x03 : Release Call

	packet.data = data;
	packet.length = 1;
	packet.group  = GSM_CALL;
	packet.action = GSM_CALL_ID;

	FuncServer->Cast(&GlobalPS, LXT_ID_CLIENT_EVENT_INJECTOR, &packet);

	log_msg(MSGL_VGSM_INFO," -------------------------incoming call id : %d\n",call_id);
}


int server_tx_call_incoming_noti( LXT_MESSAGE * packet ) // on network side, this is outgoing call.
{
	int num_len = 0, ret = 0, tmp = 0;
	char number[MAX_GSM_DIALED_DIGITS_NUMBER];
	char *p, data[MAX_GSM_DIALED_DIGITS_NUMBER*2];
	unsigned char ss_present_indi ;
	unsigned char ss_call_type ;
	unsigned char ss_no_cli_cause ;
	unsigned char call_id ;
	STATE state;
	gsm_call_list_t list;
	gsm_call_status_e_type call_status = GSM_CALL_STATUS_INCOMING;
	char clir = GSM_CALL_CLIR_STATUS_NOT_CHANGED;		//gsm_clir_status_e_type
	int call_exist = 0;  // If there is no call in call list then send the incoming noti.
	unsigned short call_type;

	int i;
	call_barring_entry_t * resp_entry = get_call_barring_entry() ;

	TRACE(MSGL_VGSM_INFO, "\n");

	if(!resp_entry)
		TRACE(MSGL_VGSM_INFO, "CB entry is NULL!!!\n");
	else {
		for(i=0; i<resp_entry[0].count; i++) {
			TRACE(MSGL_VGSM_INFO,"i : %d,  type : %d\n", i, resp_entry[i].type);
			if(resp_entry[i].type == 4 && resp_entry[i].ss_mode == 3) { // 'All incoming calls' has set
				TRACE(MSGL_VGSM_ERR, "Incoming Call Barring is set \n");
				callback_callist();
				return -1;
			}
		}
	}

	get_current_state_machine( &state );
	if( ( state.state_type == STATE_CALL_WAITING_OUTGOING ) ||
			( state.state_type == STATE_CALL_WAITING_INCOMING ) )
	{
		log_msg(MSGL_WARN,"state_type = %d\n",state.state_type);
		return 0;
	}

	if( check_call() == 0 )
	{
		log_msg(MSGL_WARN, "1 active and 1 hold \n");
		// delete for multi call ( 6 call ) return 0;
	}

	get_call_list( &list );
	if( list.CallCount > 0 ) {
		call_status = GSM_CALL_STATUS_WAITING;

		// sub_cmd = GSM_CALL_WAITING;
		call_exist = 1; // If there are any call in call list, then send the waiting noti instead the incoming noti.

		call_waiting_entry_t * cw_entry = find_call_waiting_entry(1);
		if(cw_entry->ss_mode == SS_MODE_DEACT) {
			TRACE(MSGL_VGSM_ERR, "Call waiting is blocked \n");
			callback_callist();
			return -1;
		}
	}

	assert(packet->length >= 8);
	p = packet->data;

	call_id = make_call_id();
	call_type = p[0] << 8;
	cast_call_incomming(call_id);
	set_call_type(call_type);

	memset(number, 0, MAX_GSM_DIALED_DIGITS_NUMBER);

	clir = p[2];

	if(clir == GSM_CALL_CLIR_STATUS_INVOCATION)
	{
		log_msg(MSGL_VGSM_INFO,"  GSM_CALL_CLIR_STATUS_INVOCATION -> no num  \n");
		num_len = 0; // check that 'num_len' should be MAX_GSM_DIALED_DIGITS_NUMBER or not.
		memset(number, 0, MAX_GSM_DIALED_DIGITS_NUMBER);
		ss_present_indi = p[5];
		ss_no_cli_cause = p[6];

	}
	else
	{
		tmp = (int)p[3];
		if(tmp < 0){
			TRACE(MSGL_VGSM_INFO, "ERROR!! Invalid value of packet.data.\n");
			callback_callist();
			return -1;
		} else if ( tmp >= MAX_GSM_DIALED_DIGITS_NUMBER){
			TRACE(MSGL_VGSM_INFO, "The number is too long. It will be cut.\n");
			num_len = MAX_GSM_DIALED_DIGITS_NUMBER - 1;
		} else {
			num_len = tmp;
		}
		memcpy(number, &p[7], num_len);
		number[num_len] = '\0';
		log_msg(MSGL_VGSM_INFO,"  call num len %d  \n", num_len);
		ss_present_indi = 0;
		ss_no_cli_cause = 0;
	}

	set_call_list( GSM_CALL_DIR_MT, call_status, call_type, number, num_len );

	//090314
	callback_callist();

	char* number_type;
	if(number[0] == '+')
        	number_type = "129";
        else
                number_type = "145";    // default
	
	if(call_exist == 0)
	{
		sprintf(data, "%d,%d,%d,%d,%d,%s,%s", call_id + 1, AT_CALL_DIR_MT, AT_CALL_STAT_INCOMING, AT_CALL_MODE_VOICE, AT_CALL_MPTY_FALSE, number, number_type);
		TRACE(MSGL_VGSM_INFO, "data: %s\n", data);
		ret = oem_tx_call_incoming_noti(data, strlen(data));
	}
	else
	{
		sprintf(data, "%s,%s,%d", number, number_type, AT_CALL_CLASS_VOICE);
		TRACE(MSGL_VGSM_INFO, "data: %s\n", data);
		ret = oem_tx_call_waiting_noti(data, strlen(data));

		memset(data, '\0', strlen(data));
		sprintf(data, "%d,%d,%d,%d,%d,%s,%s", call_id + 1, AT_CALL_DIR_MT, AT_CALL_STAT_WAITING, AT_CALL_MODE_VOICE, AT_CALL_MPTY_FALSE, number, number_type);
                TRACE(MSGL_VGSM_INFO, "data: %s\n", data);
		return oem_tx_call_status_noti(data, strlen(data));
	}

	//log_msg(MSGL_VGSM_INFO,"!!!!!!!!server_tx_call_incoming_noti!!!!!!!!!!! number : %d, number len : %d \n", number[0], num_len);
	if( GSM_CALL_TYPE_FORWARDED == p[1] )
	{
		ss_call_type = 0x33 ;
		// forwardcall
	}
	else
	{
		ss_call_type = 0x31 ; //calling party
	}

	log_msg(MSGL_VGSM_INFO,"  call type = %d,  num len=%d  PI=%d  Cli-cause %d \n", p[0], num_len,ss_present_indi,ss_no_cli_cause);

	server_tx_ss_cli(ss_call_type, ss_present_indi, ss_no_cli_cause); // TODO:  it will be changed..

	_LEAVE();

	return ret;
}

//int TxCall_ExecRelease( LXT_MESSAGE * packet )
int server_tx_call_release_exec(LXT_MESSAGE * packet )
{
	int i, ret = 0;
	int call_id = 0;
	unsigned char *p = 0;
	STATE state;
	int call_gen_resp_err =-1;

	log_msg(MSGL_VGSM_INFO, "\n");

	get_current_state_machine( &state );

	if( state.state_type == STATE_STANDBY )
	{
		// general error send
		call_gen_resp_err = check_call_error();
		//oem_tx_call_gen_resp(call_gen_resp_err);
		set_general_response_error(0x8000);
	}

	// STATE_CALL_WAITING_INCOMING

	gsm_call_list_t *callList = malloc(sizeof(gsm_call_list_t));
	if(!callList)
		return -1;

	p = (unsigned char *)packet->data; // call id

	log_msg(MSGL_VGSM_INFO," release call id = %d \n", p[0]);

	call_id = p[0];

	get_call_list(callList);
	for (i=0; i < MAX_CALL_COUNT; i++)
	{
		if( callList->CallInfo[i].stat == GSM_CALL_STATUS_NONE )
			continue;
		if( callList->CallInfo[i].idx == call_id)
		{
			ret = server_tx_call_release_internal( call_id );
			break;
		}
	}

	_LEAVE();
	free(callList);
	return ret;
}

//int TxCall_ExecAnswer()
int server_tx_call_answer_exec(void)
{
	STATE next;

	TRACE(MSGL_VGSM_INFO, "\n");

	/*
	   GSM_CallList list;

	   get_call_list( &list );
	   if( list.CallCount <= 0 )
	   return 0;

	   if( ( list.CallInfo[get_call_id()].stat == GSM_CALL_STATE_ACTIVE ) ||
	   ( list.CallInfo[get_call_id()].stat == GSM_Call_Held ) )
	   return 0;
	 */
	int i = 0;
	int alerted_id = -1;

	gsm_call_list_t * callList = malloc(sizeof(gsm_call_list_t));

	if(!callList) {
		return -1;
	}

	get_call_list(callList);

	if(callList->CallCount < 1) {
		TRACE(MSGL_VGSM_ERR, "The call-count is %d!!\n", callList->CallCount);
		free(callList);
		return -1;
	}

	for (i=0; i < MAX_CALL_COUNT; i++) {
		if( callList->CallInfo[i].stat == GSM_CALL_STATUS_ALERT ) {
			alerted_id = callList->CallInfo[i].idx;
			break;
		}
	}

	if(alerted_id != get_call_id()) {
		TRACE(MSGL_VGSM_INFO, "Current g_call_id(%d) is not the alerted_id(%d)\n", get_call_id(), alerted_id);
		set_call_id(alerted_id);
	}


	set_current_state( STATE_CALL_WAITING_OUTGOING, GSM_CALL_CMD, GSM_CALL_ANSWER);

	if( find_next_state( &next, STATE_FLAG_ALL ) ) {
		set_state_machine( next );
		send_msg();
	}

	free(callList);

	_LEAVE();
	return 1;
}


int server_tx_call_alert_ind(void *ptr_data, int data_len )
{
	//    STATE next;

	TRACE(MSGL_VGSM_INFO, "\n");

	//send resp ( no error ) to Phone
	//	oem_tx_call_gen_resp(0x8000);

	//send noti ( outgoing  )to Phone
	//	set_current_state( STATE_STANDBY, GSM_CALL_CMD, GSM_CALL_OUTGOING);

	/*	
		if( find_next_state( &next, STATE_FLAG_ALL ) ) {
	// call server_tx_call_msg()
	set_state_machine( next );
	send_msg();
	}
	 */

	//send noti ( alert  )to Phone
	unsigned short call_type = get_call_type();
	int i = 0;
	int dialing_id = -1;

	gsm_call_list_t * callList = malloc(sizeof(gsm_call_list_t));

	if(!callList) {
		return -1;
	}

	get_call_list(callList);

	if(callList->CallCount < 1) {
		TRACE(MSGL_VGSM_ERR, "The call-count is %d!!\n", callList->CallCount);
		free(callList);
		return -1;
	}

	for (i=0; i < MAX_CALL_COUNT; i++) {
		if( callList->CallInfo[i].stat == GSM_CALL_STATUS_DIALING ) {
			dialing_id = callList->CallInfo[i].idx;
			break;
		}
	}

	if(dialing_id != get_call_id()) {
		TRACE(MSGL_VGSM_INFO, "Current g_call_id(%d) is not the dialing_id(%d)\n", get_call_id(), dialing_id);
		set_call_id(dialing_id);
	}

	char sndbuf[SEND_BUF_SIZE];
	memset(sndbuf, '\0', sizeof(sndbuf));
	sprintf(sndbuf, "%d,%d,%d,%d,%d", get_call_id() + 1, AT_CALL_DIR_MO, AT_CALL_STAT_ALERTING, AT_CALL_CLASS_VOICE, AT_CALL_MPTY_FALSE);

	set_current_call_status_error( 0x00 );

	set_call_list_status( get_call_id(), 0, GSM_CALL_STATUS_ALERT );

	callback_callist();		// renewal call_list in the EI

	free(callList);
	return oem_tx_call_status_noti(sndbuf, strlen(sndbuf));
}


//int release_call( int call_id )
int server_tx_call_release_internal(int call_id)
{
	char data[1];
	LXT_MESSAGE packet;
	STATE next;
	gsm_call_list_t * list = malloc(sizeof(gsm_call_list_t));
	if(!list)
		return -1;

	TRACE(MSGL_VGSM_INFO, "\n");

	set_call_id( call_id ); // We should set here because the server_tx_call_status() may need the g_call_id.

	TAPIMessageInit(&packet);

	data[0] = 0x03;	//0x01 : Outgoing Call, 0x02 : Answer Call, 0x03 : Release Call

	packet.data = data;
	packet.length = 1;
	packet.group  = GSM_CALL;
	packet.action = GSM_CALL_STATE;

	FuncServer->Cast(&GlobalPS, LXT_ID_CLIENT_EVENT_INJECTOR, &packet);

	set_current_state( STATE_CALL_RELEASED, GSM_CALL_CMD, GSM_CALL_RELEASE);

	set_ss_state(SS_NONE);

	log_msg(MSGL_VGSM_INFO,"deleted call id : %d \n", call_id);
        delete_call_list( call_id );

	if( find_next_state( &next, STATE_FLAG_ALL ) ) {
		set_state_machine( next );
		send_msg();
	}

	get_call_list(list);
	if(list){
		packet.data = list;
		packet.length = sizeof(gsm_call_list_t);
	}/* Execution cannot reach this statement
	else{
		packet.data = NULL;
		packet.length = 0;
	}*/

	packet.group  = GSM_CALL;
	packet.action = GSM_CALL_STATUS_NOTI;

	FuncServer->Cast(&GlobalPS, LXT_ID_CLIENT_EVENT_INJECTOR, &packet);

	free(list);

	return 1;
}

int server_tx_call_all_release(void)
{
	int i = 0;
	gsm_call_list_t * list = malloc(sizeof(gsm_call_list_t));
	get_call_list(list);

	TRACE(MSGL_VGSM_INFO, "\n");

	for( i=0; i<MAX_CALL_COUNT; ++i ) {
		if( list->CallInfo[i].stat != GSM_CALL_STATUS_NONE )
			server_tx_call_release_internal( list->CallInfo[i].idx );
	}

	free(list);

	return 1;
}


void server_tx_call_burst_dtmf_noti(int success)
{
	int n = 0;
	unsigned char data[1];

	data[n++] = success;

	oem_tx_call_burst_dtmf_noti(data, n);
}

int server_tx_call_line_id_resp(void)
{
	int n = 0;
	unsigned char data[1];

	data[n++] = get_call_line_id();

	oem_tx_call_line_id_resp(data, n);
	return 1;
}


void callback_callist(void)
{
	_ENTER();

	LXT_MESSAGE packet;
	gsm_call_list_t * callList = malloc(sizeof(gsm_call_list_t));

	get_call_list(callList);

	/* send to EI */
	packet.data = (gsm_call_list_t*)callList;
	packet.length = sizeof(gsm_call_list_t);
	packet.group  = GSM_CALL;
	packet.action = GSM_CALL_CALL_LIST_IND;		// 0x29	in vgsm_phone.h

	FuncServer->Cast(&GlobalPS, LXT_ID_CLIENT_EVENT_INJECTOR, &packet);
	free(callList);
	_LEAVE();
}
