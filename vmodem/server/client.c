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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "gsmstate.h"
#include "phoneprotocol4gsm.h"
#include "vgsm_phone.h"
#include "vgsm_ss.h"
// #include <mzgsmsat.h> 
// #include <mzgsmsound.h>
#include "vgsm_sms.h"
#include "vgsm_gprs.h"
#include "db_gprs.h"
#include "logmsg.h"

#include "phoneserver.h"
#include "at_tx_func.h"
#include "at_rx_func.h"
#include "misc.h"
#include "at_send.h"
#include "at_tx_sms.h"
#include "state.h"

//#include "db_phonebook.h"
#include "server_common_security.h"
#include "server_common_network.h"
#include "vgsm_sim.h"
#include "server_tx_security.h"
#include "server_tx_phonebook.h"
#include "db_security.h"
#include "db_phonebook.h"

#include "sat_util.h"
#include "sat_values.h"
#include "server_tx_sms.h"
#include "server_tx_data.h"
#include "server_tx_gprs.h"

#include "flight.h"

#include "server_tx_call.h"
#include "server_common_call.h"
#include "server_common_ss.h"
#include "server_tx_ss.h"
#include "server_tx_network.h"
#include "server_tx_display.h"
#include "server_tx_power.h"
#include "server_client.h"
/* for temporary power code. so it will be removed */
#include "at_msg.h"
#include "vgsm_phone.h"
/* end */
#include <vconf/vconf.h>
#include <vconf/vconf-keys.h>

#define MAX_RESULT_LENGTH 1024

GSM_Device_Functions            * Device;
int			smsSentStatus = 0;

extern GSM_StateMachine           GlobalS;

#define MMI_SC_LENGTH2          14
#define MMI_SC_LENGTH3          10

#define MMS_SEND_OK		10
#define MMS_SEND_FAIL		11
#define MMS_MODIFIER		10

#if 0
static char * mmi_sc2[] =
{
    "21", // CFU
    "67", // CF Busy
    "61", // CF No Reply
    "62", // CF Not Reachable
    "43", // Call Waiting
    "30", // CLIP
    "31", // CLIR
    "76", // COLP
    "77", // COLR
    "33", // CB, all outgoing
    "35", // CB, all incoming"
    "03", // CB, change of password
    "04", // Change PIN1
    "05"  // Change PUK1
};

static char * mmi_sc3[] =
{
    "331",// CB, outgoing int'l
    "332",// CB, outgoing int'l except HOME
    "351",// CB, incoming Roaming
    "330",// CB, all calls
    "333",// CB, all outgoing
    "353",// CB, all incoming
    "002",// All CF
    "004",// All Conditional CF
    "042",// Change PIN2
    "052" // Change PUK2
};

static bool isUSSD(char * data)
{
    // ** : register and activate
    // *  : activate
    // *# : interrogate
    // #  : deactivate
    // ## : de-register (and de-activate)

    int start = 0;
    int ii;

    if  (data[0] == '*' && data[1] == '*') // register and activate
	start = 2;
    else
	if  (data[0] == '*' && (data[1] != '*' && data[1] != '#') ) // activate
	    start = 1;
	else
	    if  (data[0] == '*' && data[1] == '#') // interrogate
		start = 2;
	    else
		if  (data[0] == '#' && (data[1] != '#' && data[1] != '*')) // deactivate
		    start = 1;
		else
		    if  (data[0] == '#' && data[1] == '#')  // de-register (and deactivate)
			start = 2;
		    else
			TRACE(MSGL_WARN, "Warning : Bad MMI code <%s>\n", data);

    for (ii = 0; ii < MMI_SC_LENGTH2; ii ++)
    {
	if  (strncmp( (const char *)&data[start], mmi_sc2[ii], 2) == 0) // Normal SS SC
	    return false;
    }

    for (ii = 0; ii < MMI_SC_LENGTH3; ii ++)
    {
	if  (strncmp( (const char *)&data[start], mmi_sc3[ii], 3) == 0) // Normal SS SC
	    return false;
    }

    return true;
}
#endif

#if 0
void do_ext(PhoneServer* ps, TClientInfo * ci, LXT_MESSAGE * packet)
{
    int action = packet->action;
    int length = packet->length;
    int klass = ci->klass;

    char * p = (char *)(packet->data);
    switch (action)
    {
	case GSM_EXT_SET_INTERNAL_STATE_REQ :
	    {
		LXT_PHONE_STATE state = (LXT_PHONE_STATE)p[0];
		TRACE(MSGL_VGSM_INFO, "## GSM_EXT_SET_INTERNAL_STATE_REQ %d\n",
			(int)state);
		FuncPhoneServer->BroadcastInternalStatus( &GlobalPS,
			state);
	    }
	    break;

	case GSM_EXT_SET_AUDIO_PATH_REQ :
	    break;
	case GSM_EXT_SET_VOICE_VOL_REQ :
	    break;
	case GSM_EXT_SET_IMEI_REQ :
	    break;
	case GSM_EXT_SET_BTMAC_REQ :
	    break;
	case GSM_EXT_GET_BTMAC_REQ :
	    break;
    }
}


#endif /* 0 */

// 090326 		// restore data in EI
static void do_restore_ei(PhoneServer* ps, TClientInfo * ci, LXT_MESSAGE * packet)
{
    unsigned char data[MAX_GSM_DIALED_DIGITS_NUMBER+4];

    log_msg(MSGL_VGSM_INFO,"do_restore_EI\n");

    init_ss_info_re();
    init_plmn_list();
    server_sim_db_init();

    gsm_call_list_t * callList = malloc(sizeof(gsm_call_list_t));
    LXT_MESSAGE MOpacket;
    TAPIMessageInit(&MOpacket);
    int i = 0;

    get_call_list(callList);

    for( i=0; i<MAX_CALL_COUNT; ++i )
    {
	if(callList->CallInfo[i].stat == GSM_CALL_STATUS_DIALING || callList->CallInfo[i].stat == GSM_CALL_STATUS_ALERT)
	{
	    data[0] = callList->CallInfo[i].idx;
	    data[1] = callList->CallInfo[i].call_type;
	    data[2] = callList->CallInfo[i].num_len;
	    data[3] = callList->CallInfo[i].stat;

	    memset(&data[4], 0, MAX_GSM_DIALED_DIGITS_NUMBER);
	    memcpy(&data[4], callList->CallInfo[i].number, callList->CallInfo[i].num_len);  
	    MOpacket.data = data;
	    MOpacket.group  = GSM_CALL;
	    MOpacket.action = GSM_CALL_MAKE_REQ;
	    MOpacket.length = callList->CallInfo[i].num_len + 4;

	    FuncServer->Cast(&GlobalPS, LXT_ID_CLIENT_EVENT_INJECTOR, &MOpacket);
	}
    }

    free(callList);
    callback_callist();

    log_msg(MSGL_VGSM_INFO,"do_restore_EI\n");
}

static void do_sat(PhoneServer* ps, TClientInfo * ci, LXT_MESSAGE * packet)
{

    int action = packet->action;
    //    char * p = (char *)(packet->data);

    DP_TEXT *dp_text;
    GET_INKEY *get_inkey;
    GET_INPUT *get_input;

    TRACE(MSGL_VGSM_INFO, "%x\n", action);
    switch (action)
    {
	case GSM_SAT_PROACTIVE_DISPLAY_TEXT	:
	    {
		log_msg(MSGL_VGSM_INFO,"Client - GSM_SAT_PROACTIVE_DISPLAY_TEXT\n");

		dp_text = (DP_TEXT *)(packet->data);
		Display_Text(dp_text);
		break;
	    }

	case GSM_SAT_PROACTIVE_GET_INKEY:
	    {
		log_msg(MSGL_VGSM_INFO,"Client - GSM_SAT_PROACTIVE_GET_INKEY\n");
		get_inkey = (GET_INKEY *)(packet->data);
		Get_Inkey(get_inkey);
		break;
	    }

	case GSM_SAT_PROACTIVE_GET_INPUT:
	    {
		log_msg(MSGL_VGSM_INFO,"Client - GSM_SAT_PROACTIVE_GET_INPUT\n");
		get_input = (GET_INPUT *)(packet->data);
		Get_Input(get_input);
		break;
	    }

	case GSM_SAT_PROACTIVE_SET_MENU:
	    {
		log_msg(MSGL_VGSM_INFO,"Client - GSM_SAT_PROACTIVE_SET_MENU\n");
		Set_Menu(packet->data);
		break;
	    }

	case GSM_SAT_PROACTIVE_SELECT_ITEM:
	    {
		log_msg(MSGL_VGSM_INFO,"Client - GSM_SAT_PROACTIVE_SELECT_ITEM\n");
		Select_Item(packet->data);
		break;
	    }
	case GSM_SAT_PROACTIVE_SEND_SMS:
	    {
		log_msg(MSGL_VGSM_INFO,"Client - GSM_SAT_PROACTIVE_SEND_SMS\n");
		Send_SMS(packet->data);
		break;
	    }
	case GSM_SAT_PROACTIVE_SET_UP_CALL:
	    {
		log_msg(MSGL_VGSM_INFO,"Client - GSM_SAT_PROACTIVE_SET_UP_CALL\n");
		Set_Up_Call(packet->data);
		break;
	    }

#if 0
	case GSM_SAT_ENVELOPE_COMMAND_REQ:
	    {
		GSM_SatEnvelopeCmd cmd;

		cmd.length = length;
		cmd.cmd = p;
		TxSAT_ExecEnvelopeCommand(&cmd);
	    }
	    break;
	case GSM_SAT_PROFILE_DOWNLOAD_SET:
	    {
		GSM_SatProfileDown cmd;

		cmd.result_code = length;
		memcpy(cmd.usat_profile, p, 20);
		TxSAT_SetProfileDownload(&cmd);
	    }
	    break;
	case GSM_SAT_TERMINATE_SESSION_SET:
	    {
		TxSAT_SetTerminateUSATSession(p);
	    }
	    break;
	case GSM_SAT_PROVIDE_LOCAL_INFOMATION_GET:
	    {
		unsigned char infoType;
		infoType = *p;
		TxSAT_GetProvideLocalInfo(infoType);
	    }
	    break;
	case GSM_SAT_POLLING_GET:
	    {
		GSM_SatPollingGetType cmd;
		cmd.pollReqType = *p++;
		cmd.present = *p++;
		cmd.units = *p++;
		cmd.value = *p;
		TxSAT_GetPolling(&cmd);
	    }
	    break;
	case GSM_SAT_REFRESH_GET:
	    {
		GSM_SatRefreshGetType cmd;
		cmd.refreshMode = *p++;
		cmd.numOfFile = *p++;
		cmd.filePathLength = *p++;
		memcpy(cmd.fullFilePath, p, 256);
		p += 256;
		cmd.aidPresent = *p++;
		memcpy(cmd.aid, p, 16);
		TxSAT_GetRefresh(&cmd);
	    }
	    break;
	case GSM_SAT_SETUP_EVENT_LIST_GET:
	    {
		GSM_SatEventListGetType cmd;
		cmd.length = *p++;
		memcpy(cmd.eventList, p, 17);
		TxSAT_GetSetupEventList(&cmd);
	    }
	    break;
#endif

	default :
	    break;
    }
}

static void do_data(PhoneServer* ps, TClientInfo * ci, LXT_MESSAGE * packet)
{
    TRACE(MSGL_VGSM_INFO, "\n");

    //GSM_Error error;

    int action = packet->action;
    //int length = packet->length;
    //int client_id = ci->klass;

    //char * p = (char *)(packet->data);
    TRACE(MSGL_VGSM_INFO, "%x\n", action);
    switch (action)
    {
	case GSM_DATA_STATUS :
	    server_tx_data_CallStatus(packet);
	    break;
    }
}


static void preprocess_do_gprs(LXT_MESSAGE * packet)
{
    int i, num;
    GprsDbEntry entry;
    char *pos;

    // preprocess gprs data from evt injector
    char *p = (char *)(packet->data);
    TRACE(MSGL_VGSM_CALL, "\n");
    // clean up gprs db
    if (g_gprs_conflist.num > 0) {

	for (i = 0; i < g_gprs_conflist.num; i++) {
	    GprsDbEntry rm_entry;

	    strcpy(rm_entry.apn, g_gprs_conflist.pentry[i].apn);

	    vgsm_gprs_database_remove(&rm_entry);
	}
    }

    num = *((int *)p);
    if(num < 0 || num > (254 * sizeof(int)) ) {
	TRACE(MSGL_VGSM_INFO, "ERROR!! Invalid value of packet.data.\n");
	return;
    }

    pos = p + sizeof(int);

    for (i = 0; i < num; i++ ) {
	strcpy(entry.apn, (char *)pos);
	strcpy(entry.intf, (char *)(pos + 255));

	entry.ipconf.cid = pos[265];

	memcpy((char *)(&(entry.ipconf.fieldflag)), pos + 266, 2);
	memcpy((char *)(entry.ipconf.ip_addr), pos + 268, 4);
	memcpy((char *)(entry.ipconf.primary_dns), pos + 272, 4);
	memcpy((char *)(entry.ipconf.secondary_dns), pos + 276, 4);
	memcpy((char *)(entry.ipconf.default_gateway), pos + 280, 4);
	memcpy((char *)(entry.ipconf.subnet_mask), pos + 284, 4);

	// add to gprs db
	vgsm_gprs_database_add(&entry);

	pos += 255 + 10 + 23;
    }

}


static void do_gprs(PhoneServer* ps, TClientInfo * ci, LXT_MESSAGE * packet)
{

    int action = packet->action;

    TRACE(MSGL_VGSM_CALL, "%x\n", action);
    switch (action) {
	case GSM_GPRS_IP_CONFIGURATION:
	    // process VGSM specific job
	    preprocess_do_gprs(packet);
	    server_tx_gprs_IPConfigurationNotify(packet);
	    break;
    }
}


static void do_display_indicator(const LXT_MESSAGE * packet)
{
    const unsigned char *p;

    assert(packet->data);
    assert(packet->length >= 4);

    p = packet->data;

    server_tx_display_icon_info_noti(p[0], p[1], p[2], p[3], GSM_NET_REG_STAT_REG_HOME);
}

static void do_display(PhoneServer* ps, TClientInfo * ci, LXT_MESSAGE * packet)
{
    TRACE(MSGL_VGSM_INFO, "\n");

    //GSM_Error error;

    int action = packet->action;
    //int length = packet->length;
    //int client_id = ci->klass;

    //char * p = (char *)(packet->data);
    TRACE(MSGL_VGSM_CALL, "do_display %x\n", action);
    switch (action)
    {
	case GSM_DISPLAY_INDICATOR_NOTI :
	    do_display_indicator(packet);
	    break;
    }
}

static void do_power_battery_status(LXT_MESSAGE * packet)
{
    unsigned char *p = packet->data;
    unsigned char level;

    assert(packet->length >= 1);
    level = p[0];
    server_tx_battery_status_noti(level);

    /* there seems to be two different messages that notify similar information - send both? */
    if (0)
	server_tx_display_icon_info_noti(0x02, 0, level, 0, 0);	/* only notify the battery level */
}


static void do_power(PhoneServer* ps, TClientInfo * ci, LXT_MESSAGE * packet)
{
    TRACE(MSGL_VGSM_INFO, "\n");

    int action = packet->action;

    TRACE(MSGL_VGSM_INFO, "do_power %x\n", action);


    switch (action)
    {
	case GSM_POWER_MODEM_ON_REQ: 
	    oem_rx_power_up_exec();
	    break;
	case GSM_POWER_BATTERY_STATUS_NOTI:
	    do_power_battery_status(packet);
	    break;
	default:
	    break;
    }
}

static void do_network(PhoneServer* ps, TClientInfo * ci, LXT_MESSAGE * packet)
{
    int action = packet->action;
    TRACE(MSGL_VGSM_INFO, "do_network %x\n", action);

    switch (action)
    {
	case GSM_NETWORK_REG_NOTI:
	    server_tx_net_reg_noti(packet);
	    break;

	case GSM_NETWORK_PLMN_LIST:
	    server_tx_net_plmn_list_noti(packet);
	    break;

	case GSM_NETWORK_CURRENT_PLMN_NOTI:
	    server_tx_net_current_plmn_noti(packet);
	    break;
	case GSM_NETWORK_CURRENT_NITZ_INFO:
	    server_tx_net_current_nitz_noti(packet);
	    break;
    }
}


static void do_sim(PhoneServer * ps, TClientInfo * ci, LXT_MESSAGE * packet)
{
    log_msg(MSGL_VGSM_ERR,"\n");

    int action = packet->action;
    int length = packet->length;
    int i;

    char * p = (char *)(packet->data);

    gsm_sec_lock_e_type type;
    gsm_sec_status_type status;
    gsm_sec_lock_mode_e_type flag;

    char *password = NULL;
    PB * pb;

    STATE current;

    get_current_state_machine( &current );
    TRACE(MSGL_VGSM_INFO, "do_sim %x\n", action);
    TRACE(MSGL_VGSM_INFO, "current_state=[%x]\n\n", STATE_TYPE(current) );

    switch (action)
    {

	/************************************************************************************/
	/*																					*/
	/*									security part										*/
	/*																					*/
	/************************************************************************************/
	case GSM_SIM_INIT_REQ:
	    server_sim_db_init();
	    break;

	    // 090602
	case GSM_SIM_REINIT_REQ:
	    for(i=0;i<7;i++)
	    {
		db_sim_info_add(i,NULL);
		db_sim_data_add(i,NULL);
	    }

	    break;

	case  GSM_SIM_STATUS_SET:
	    {
		status = p[0];

		log_msg(MSGL_VGSM_ERR,"status=[%x]\n",status);

		switch(status)
		{
		    case GSM_SEC_STATUS_PIN_READY:
			server_sec_set_sec_status(GSM_SIM_READY);
			server_sec_reset_remain_cnt(GSM_SIM_READY);
			server_sec_set_facility(GSM_SIM_FACILITY_PIN, GSM_SEC_LOCK_MODE_UNLOCK);
			//server_tx_sim_sec_disable_facility();
			break;

		    case GSM_SEC_STATUS_PIN_REQ:
			server_sec_set_sec_status(GSM_SIM_PIN_REQ_STATE);
			server_sec_reset_remain_cnt(GSM_SIM_PIN_REQ_STATE);
			server_sec_reset_remain_cnt(GSM_SIM_PUK_REQ);
			server_sec_set_facility(GSM_SIM_FACILITY_PIN, GSM_SEC_LOCK_MODE_LOCK);
			break;

		    case GSM_SEC_STATUS_PUK_REQ:
			server_sec_set_sec_status(GSM_SIM_PUK_REQ);
			server_sec_set_cnt_zero(GSM_SIM_PIN_REQ_STATE);
			server_sec_reset_remain_cnt(GSM_SIM_PUK_REQ);
			server_sec_set_facility(GSM_SIM_FACILITY_PIN, GSM_SEC_LOCK_MODE_LOCK);
			break;

		    case GSM_SEC_STATUS_PIN2_REQ:
			server_sec_set_sec2_status(GSM_SIM_PIN2_REQ);
			server_sec_reset_remain_cnt(GSM_SIM_PIN2_REQ);
			server_sec_reset_remain_cnt(GSM_SIM_PUK2_REQ);
			//server_sec_set_facility(GSM_SIM_FACILITY_PIN, GSM_SEC_LOCK_MODE_LOCK);
			break;

		    case GSM_SEC_STATUS_PUK2_REQ:
			server_sec_set_sec2_status(GSM_SIM_PUK2_REQ);
			server_sec_set_cnt_zero(GSM_SIM_PIN2_REQ);
			server_sec_reset_remain_cnt(GSM_SIM_PUK2_REQ);
			//server_sec_set_facility(GSM_SIM_FACILITY_PIN, GSM_SEC_LOCK_MODE_LOCK);
			break;

		    case GSM_SEC_STATUS_PERM_BLOCKED:
			server_sec_set_sec_status(GSM_SIM_FRIZEN);
			server_sec_set_facility(GSM_SIM_FACILITY_PIN, GSM_SEC_LOCK_MODE_LOCK);
			break;

		    case GSM_SEC_STATUS_NO_SIM:
			server_sec_set_sec_status(GSM_SIM_NO_SIM);
			server_sec_set_facility(GSM_SIM_FACILITY_PIN, GSM_SEC_LOCK_MODE_LOCK);
			break;

		    case GSM_SEC_STATUS_PIN2_READY:
			server_sec_set_sec2_status(GSM_SIM_PIN2_OK);
			server_sec_reset_remain_cnt(GSM_SIM_PIN2_OK);
			break;

		    default :
			log_msg(MSGL_VGSM_ERR,"ERROR -Not handled status =[%d] in sim status set \n", status);
			break;
		}
	    }
	    break;

	case GSM_SIM_CHANGE_PASS_SET :
	    {
		type = p[0];
		password = malloc(length-1);
		if(!password)
		    return;
		memcpy(password,&p[1],length-1);

		switch(type)
		{
		    case GSM_SEC_LOCK_TYPE_SC:
			server_sec_change_password(password,GSM_SIM_PIN_REQ_STATE,length-1);
			break;

		    case GSM_SEC_LOCK_TYPE_SC2:
			server_sec_change_password(password,GSM_SIM_PIN2_REQ,length-1);
			break;
#if 1																							// 090215
		    case GSM_SEC_LOCL_TYPE_PUK:
			server_sec_change_password(password,GSM_SIM_PUKSET_REQ,length-1);
			break;
		    case GSM_SEC_LOCL_TYPE_PUK2:
			server_sec_change_password(password,GSM_SIM_PUK2SET_REQ,length-1);
			break;
#endif

		    default :
			log_msg(MSGL_VGSM_ERR,"ERROR -Not handled type =[%d] in change pw \n", type);
			break;
		}
	    }
	    break;

	case GSM_SIM_FACILITY_SET:
	    {
		type = p[0];
		flag =  p[1];

		log_msg(MSGL_VGSM_ERR,"GSM_SIM_FACILITY_SET - type, flag = [%d],[%d]\n", type, flag);

		if(type == GSM_SEC_LOCK_TYPE_SC)
		{
		    server_sec_set_facility(GSM_SIM_FACILITY_PIN,flag);
		}
		else if( type == GSM_SEC_LOCK_TYPE_FD)
		{
		    server_sec_set_facility(GSM_SIM_FACILITY_FDN,flag);
		}
		else
		{
		    log_msg(MSGL_VGSM_ERR,"ERROR - Not handled type =[%d] in facility set \n", type);
		}
	    }
	    break;

	case GSM_SIM_GET_SIM_DB_REQ:
	    {
		log_msg(MSGL_VGSM_ERR,"action ->GSM_SIM_GET_SIM_DB_REQ\n");
		db_sim_sec_restore(VGSM_SIMTypeget);
	    }
	    break;

	case GSM_SIMINFO_REQ:
	    {
		int *list;
		char table[10];

		log_msg(MSGL_VGSM_ERR,"action ->GSM_SIMINFO_REQ\n");
		list = (int*)packet->data;
		log_msg(MSGL_VGSM_ERR,"list : %d\n",*list);

		switch(*list)
		{
		    case 0:
			strcpy(table, "Tusim_li");
			break;
		    case 1:
			strcpy(table, "Tecc");
			break;
		    case 2:
			strcpy(table, "Tsst");
			break;
		    case 3:
			strcpy(table, "Test");
			break;
		    case 4:
			strcpy(table, "Tcphs");
			break;
		    case 5:
			strcpy(table, "Tspn");
			break;
		    case 6:
			strcpy(table, "Timsi");
			break;
		}
		db_sim_info_restore(table);
	    }
	    break;

	case GSM_SIMDATA_REQ:
	    {
		int *list;
		char table[10];

		log_msg(MSGL_VGSM_ERR,"action ->GSM_SIMDATA_REQ\n");
		list = (int*)packet->data;
		log_msg(MSGL_VGSM_ERR,"list : %d\n",*list);

		switch(*list)
		{
		    case 0:
			strcpy(table, "Ticcid");
			break;
		    case 1:
			strcpy(table, "Tusim_li");
			break;
		    case 2:
			strcpy(table, "Tecc");
			break;
		    case 3:
			strcpy(table, "Tsst");
			break;
		    case 4:
			strcpy(table, "Test");
			break;
		    case 5:
			strcpy(table, "Tspn");
			break;
		    case 6:
			strcpy(table, "Timsi");
			break;
		}
		db_sim_data_restore(table);
	    }
	    break;

	case GSM_SIMSAVE_REQ:
	    {
		_SIMD_t *simd_t;

		log_msg(MSGL_VGSM_ERR,"action ->GSM_SIMDATA_REQ\n");

		simd_t = (_SIMD_t*)packet->data;

		log_msg(MSGL_VGSM_ERR,"1. type1 : %d, type2 : %d, size : %d\n",simd_t->type1, simd_t->type2, simd_t->size);
		log_msg(MSGL_VGSM_ERR,"2. data : %x, %x, %x\n",simd_t->tmp[0], simd_t->tmp[1], simd_t->tmp[2]);

		if(simd_t->type1 == 0)
		    db_sim_info_add(simd_t->type2, simd_t);
		else if(simd_t->type1 == 1)
		    db_sim_data_add(simd_t->type2, simd_t);
		else
		    log_msg(MSGL_VGSM_ERR,"ERROR, type1 : %d, type2 : %d\n", simd_t->type1, simd_t->type2);
	    }
	    break;


	    /************************************************************************************/
	    /*																					*/
	    /*									Phonebook part									*/
	    /*																					*/
	    /************************************************************************************/

	case GSM_SIM_MAKE_PB_DB_REQ:
	    {
		log_msg(MSGL_VGSM_ERR,"action ->GSM_SIM_MAKE_PB_DB_REQ\n");
		db_sim_pb_create();
	    }
	    break;

	case GSM_SIM_ADD_PB_DB_REQ:
	    {
		log_msg(MSGL_VGSM_ERR,"action ->GSM_SIM_ADD_PB_DB_REQ\n");

		pb = (PB *)(packet->data);
		db_sim_pb_set_mgr(pb->db_st_type,1);

		log_msg(MSGL_VGSM_ERR,"action ->GSM_SIM_ADD_PB_DB_REQ : %d , %s ,%s, %s\n",pb->db_st_type,pb->name,pb->number,pb->address);

		db_sim_pb_add(pb);
		db_sim_pb_restore(pb);

	    }
	    break;

	case GSM_SIM_GET_PB_DB_REQ:
	    {
		log_msg(MSGL_VGSM_ERR,"action ->GSM_SIM_GET_PB_DB_REQ\n");

		pb = (PB *)(packet->data);

		log_msg(MSGL_VGSM_ERR,"action ->GSM_SIM_GET_PB_DB_REQ : %d , %s \n",pb->db_st_type,pb->name);

		db_sim_pb_get(pb);
		db_sim_pb_restore(pb);
	    }
	    break;

	case GSM_SIM_DELETE_PB_DB_REQ:
	    {
		log_msg(MSGL_VGSM_ERR,"action ->GSM_SIM_DELETE_PB_DB_REQ\n");

		pb = (PB *)(packet->data);
		db_sim_pb_set_mgr(pb->db_st_type,1);

		log_msg(MSGL_VGSM_ERR,"action ->GSM_SIM_DELETE_PB_DB_REQ : %d , %s \n",pb->db_st_type,pb->name);

		db_sim_pb_remove(pb);
		db_sim_pb_restore(pb);
	    }
	    break;

	case GSM_SIM_GET_ALL_PB_DB_REQ:
	    {
		log_msg(MSGL_VGSM_ERR,"action ->GSM_SIM_GET_ALL_PB_DB_REQ\n");

		pb = (PB *)(packet->data);
		//db_sim_pb_set_mgr(pb->db_st_type,1);

		db_sim_pb_restore(pb);
	    }
	    break;

	default:
	    log_msg(MSGL_VGSM_ERR,"ERROR - Not handled action =[%x] \n", action);
	    break;
    }
    if(password)
	free(password);
}


static void do_sups(PhoneServer * ps, TClientInfo * ci, LXT_MESSAGE * packet)
{
    int action = packet->action;
    TRACE(MSGL_VGSM_INFO, "do_sups %x\n", action);

    switch (action)
    {
	case GSM_SUPS_CONFERENCE:
	    server_tx_ss_state( SS_STATE_CONFERENCE );
	    break;
	case GSM_SUPS_HOLD:
	    server_tx_ss_state( SS_STATE_HOLD );
	    break;
	case GSM_SUPS_UNHOLD:
	    server_tx_ss_state( SS_STATE_UNHOLD );
	    break;

	case GSM_SUPS_SET_CW_REQ :
	    server_tx_ss_cw_set_req( packet);
	    break;

	case GSM_SUPS_SET_CCFC_REQ:
	    server_tx_ss_ccfc_set_req(packet);
	    break;

	case GSM_SUPS_SET_CB_REQ:
	    server_tx_ss_cb_set_req(packet);
	    break;
#if 1
	case GSM_SUPS_AOC_GET:
	    server_tx_ss_aoc_get_req(packet);
	    break;
	case GSM_SUPS_AOC_SET:
	    server_tx_ss_aoc_set_req(packet);
	    break;
#endif
	    // 090330 090406 //////////////////////////////////////////////
	case GSM_SUPS_CW_SET:
	    server_tx_ss_cw_set_req(packet);
	    break;

	    /*
	       case GSM_SUPS_CW_GET:
	       server_tx_ss_cw_get_req(packet);
	       break;
	     */

	case GSM_SUPS_CF_SET:
	    server_tx_ss_cf_set_req(packet);
	    break;

	    /*
	       case GSM_SUPS_CF_GET:
	       server_tx_ss_cf_get_req(packet);
	       break;
	     */

	case GSM_SUPS_CB_SET:
	    server_tx_ss_cb_set_req(packet);
	    break;

	    /*
	       case GSM_SUPS_CB_GET:
	       server_tx_ss_cb_get_req(packet);
	       break;
	     */

	    //090406//////////////////////////////////////////////////////////

	case GSM_SUPS_USSD_SET :
	    server_tx_ss_ussd_set_req(packet);
	    break;

	case GSM_SUPS_USSD_GET :
	    server_tx_ss_ussd_get_req(packet);
	    break;

	default :
	    TRACE(MSGL_VGSM_INFO, "do_sups no handle = %x\n", action);
    }
}

static void do_emulator(PhoneServer * ps,
	TClientInfo * ci, LXT_MESSAGE * packet)
{
    int action = packet->action;

    TRACE(MSGL_VGSM_INFO, "do_emulator %x\n", action);
    switch (action)
    {
	case GSM_EMULATOR_HDLC_REQ:
	    ATTxEmulator_HDLC_Req(packet);
	    break;
	case GSM_EMULATOR_HDLC_MODE:
	    ATTxEmulator_HDLC_Mode(packet);
	    break;
	case GSM_EMULATOR_HDLC_CLEAR:
	    ATTxEmulator_HDLC_Clear();
	    break;
	case GSM_EMULATOR_SET_CALL_GENERAL_RESPONSE_ERROR:
	    ATTxEmulator_Set_General_Response_Error(packet);
	    break;
	case GSM_EMULATOR_SET_SS_GENERAL_RESPONSE_ERROR:
	    ATTxEmulator_Set_SS_General_Response_Error(packet);
	    break;
	case GSM_EMULATOR_SET_CALL_STATUS_ERROR:
	    ATTxEmulator_Set_Call_Status_Error(packet);
	    break;
	case GSM_EMULATOR_SET_SCA:
	    ATTxEmulator_Set_SCA(packet);
	    break;
	    // ss error cause set
	case GSM_EMULATOR_SET_SS_HOLD_RESPONSE_ERROR:
	case GSM_EMULATOR_SET_SS_RETRIEVE_RESPONSE_ERROR:
	case GSM_EMULATOR_SET_SS_JOIN_RESPONSE_ERROR:
	case GSM_EMULATOR_SET_SS_SPLIT_RESPONSE_ERROR:
	case GSM_EMULATOR_SET_SS_TRANSFER_RESPONSE_ERROR:
	    ATTxEmulator_Set_SS_Response_Error(packet, action);
	    break;
    }
}

static void do_client(PhoneServer * ps,
	TClientInfo * ci, LXT_MESSAGE * packet)
{
    int action = packet->action;

    TRACE(MSGL_VGSM_INFO, "do_client %x\n", action);
    switch (action)
    {
	case GSM_CLIENT_RELEASE_NOTI:
	    server_client_release_noti();
	    break;
    }
}


static void do_sms(PhoneServer * ps, TClientInfo * ci, LXT_MESSAGE * packet)
{
    unsigned char *p;
    int action = packet->action;
    char vconf_cmd[99];

    TRACE(MSGL_VGSM_INFO, "do_sms %x\n", action);
    switch (action)
    {
	case GSM_SMS_SEND_MSG_REQ:
	    server_tx_sms_notiIncomingMsg(packet);
	    break;
	case GSM_SMS_SEND_ACK_REQ:
	    p = (unsigned char *)packet->data;
	    if((int)p[0] == MMS_SEND_OK || (int)p[0] == MMS_SEND_FAIL){
		sprintf(vconf_cmd, "vconftool set -t int memory/telephony/mms_sent_status %d -i -f", ((int)p[0] - MMS_MODIFIER));
		TRACE(MSGL_VGSM_INFO, "system : %s\n", vconf_cmd);
		system(vconf_cmd);
	    } else {
		smsSentStatus = (int)p[0];
	    }

	    server_tx_sms_ReceiveAckMsg(packet);
	    break;
	case GSM_SMS_INCOMING_CB_MSG_REQ:
	    server_tx_sms_NotiIncomingCBMsg(packet);
	    break;
#if 0
	case GSM_SMS_SEND_MSG_REQ:
	    TxSMS_ExecSendMsg(packet);
	    break;
	case GSM_SMS_READ_MSG_REQ:
	    TxSMS_GetReadMsg(packet);
	    break;
	case GSM_SMS_SAVE_MSG_REQ:
	    TxSMS_ExecSaveMsg(packet);		// by luxmea
	    break;
	case GSM_SMS_DEL_MSG_REQ:
	    TxSMS_ExecDeleteMsg(packet);
	    break;
	case GSM_SMS_SEND_ACK_REQ:
	    TxSMS_SetSendAck(packet);
	    break;
	case GSM_SMS_GET_SEL_MEM_REQ:
	    TxSMS_GetSelectMem(packet);
	    break;
	case GSM_SMS_SET_SEL_MEM_REQ:
	    TxSMS_SetSelectMem(packet);
	    break;
	case GSM_SMS_STORED_MSG_CNT_REQ:
	    TxSMS_GetStoredMsgCount(packet);
	    break;
	case GSM_SMS_GET_MSG_CONFIG_REQ:
	    TxSMS_GetMsgConfig(packet);
	    break;
	case GSM_SMS_SET_MSG_CONFIG_REQ:
	    TxSMS_SetMsgConfig(packet);
	    break;
	case GSM_SMS_GET_SVC_OPTION_REQ:
	    TxSMS_GetSvcOption(packet);
	    break;
	case GSM_SMS_SET_SVC_OPTION_REQ:
	    TxSMS_SetSvcOption(packet);
	    break;
	case GSM_SMS_MEM_FULL_REQ:
	    TxSMS_SetMemFull(packet);
	    break;
	case GSM_SMS_GET_CB_CONFIG_REQ:
	    TxSMS_GetCBConfig(packet);
	    break;
	case GSM_SMS_SET_CB_CONFIG_REQ:
	    TxSMS_SetCBConfig(packet);
	    break;
	case GSM_SMS_SET_MSG_STATUS_REQ:
	    TxSMS_SetMsgStatus(packet);
	    break;
	case GSM_SMS_SET_BLK_LIST_REQ:
	    TxSMS_SetBlkList(packet);
	    break;
	default:
	    TxSMS_DefaultSms(0, 0);
	    break;
#endif /* 0 */
    }

#if 0
    char AT_Result[MAX_RESULT_LENGTH] = {0, };

    memcpy(buffer, p, length);

    error = SMSFunc->SendMessage(&GlobalS, buffer, AT_Result);

    if  (error == ERR_TIMEOUT)
	TRACE(MSGL_VGSM_INFO, "MS TIMEOUT Error\n");
    else
    {
	FuncResponse->ResponseSMS(klass, AT_Result);
    }
#endif
}

#if 0
static int IsFligthMode()
{
#ifdef __arm__
    int fd;
    fd = open("/setting/flightmode.conf\n", O_RDONLY);

    if (fd != -1)
	return 1;

#endif
    return 0;
}
#endif /* 0 */

static void do_internal(PhoneServer * ps, TClientInfo * ci, LXT_MESSAGE * packet)
{
    int klass = ci->klass;
    int clientfd = ci->fd;
    int action = packet->action;
    int length = packet->length;

    TRACE(MSGL_VGSM_INFO, "do_internal %x\n", action);

    char * p = (char *)(packet->data);

    switch (action)
    {
	case LXT_PDA_INTERNAL_ID_REQUEST :
	    {
		int clientid;

		clientid = (int)packed_S32((unsigned char *)p);
		if(clientid == 0)
		    TRACE(MSGL_VGSM_INFO, "ERROR!! Invalid value of clientid.\n");
		ci->klass = clientid;
		TRACE(MSGL_VGSM_INFO, "LXT_PDA_INTERNAL_ID_REQUEST [0x%x]: %s\n", clientid, clientName[clientid]);

		// nsclass : when user change language setting.
		/*
		   if (LXT_ID_CLIENT_INDICATOR == clientid) {
		   if (!IsFligthMode()) {
		   if (LXT_STATE_OFF == FuncPhoneServer->GetInternalState(&GlobalPS)) {
		   GSM_PhonePowerOn(0);
		   }
		   }
		   }
		 */
		/******************/
	    }
	    break;

	case LXT_PDA_INTERNAL_MISSED_MESSAGE_REQUEST :
	    {
		Server * server = &ps->ServerSocket;
		TRACE(MSGL_VGSM_INFO, "LXT_PDA_INTERNAL_MISSED_MESSAGE_REQUEST \n");
		if  (server != NULL)
		{
		    server->Functions->CastMissedMessage(ps, klass, clientfd);
		}
	    }
	    break;

	case LXT_PDA_INTERNAL_STATE_REQUEST :
	    {
		TRACE(MSGL_VGSM_INFO, "LXT_PDA_INTERNAL_MISSED_MESSAGE_REQUEST ..WARNING: it's not implemented\n");
		// nsclass : when user change language setting. phone power off sound
		/*
		   if (LXT_ID_CLIENT_INDICATOR == klass) {
		   if (!IsFligthMode())
		   return;
		   }
		   FuncPhoneServer->CastCurrentInternalStatus(&GlobalPS, klass);
		 */
		/******************/
		break;
	    }

	case LXT_PDA_INTERNAL_LAUNCH_SAT_APP_REQUEST:
	    {
		LXT_MESSAGE packet;
		TAPIMessageInit(&packet);
		TRACE(MSGL_VGSM_INFO, "LXT_PDA_INTERNAL_LAUNCH_SAT_APP_REQUEST\n");
		packet.group = LXT_GRP_INTERNAL;
		packet.action = LXT_PDA_INTERNAL_LAUNCH_SAT_APP_RESPONSE;
		FuncServer->Cast(&GlobalPS, LXT_ID_CLIENT_CARD_MANAGER, &packet);
	    }
	    break;
	case LXT_PDA_INTERNAL_SETUP_SAT_APP_REQUEST:
	    {
		LXT_MESSAGE packet;
		TAPIMessageInit(&packet);
		TRACE(MSGL_VGSM_INFO, "LXT_PDA_INTERNAL_SETUP_SAT_APP_REQUEST\n");
		packet.group = LXT_GRP_INTERNAL;
		packet.action = LXT_PDA_INTERNAL_SETUP_SAT_APP_RESPONSE;
		packet.length = length;
		packet.data = (void *)p;
		FuncServer->Cast(&GlobalPS, LXT_ID_CLIENT_CARD_MANAGER, &packet);
	    }
	    break;

	case LXT_PDA_INTERNAL_PHONE_BATTERY_REQUEST:
	    {
		TRACE(MSGL_VGSM_INFO, "Request phone battery\n");
		log_msg(MSGL_VGSM_INFO,"WARNING: it's not implemented \n");
	    }
	    break;
	default :
	    break;
    }
}

#if 0
void do_power(PhoneServer* ps, TClientInfo * ci, LXT_MESSAGE * packet)
{
    GSM_Error error;

    int action = packet->action;
    int length = packet->length;

    char * p = (char *)(packet->data);

    switch (action)
    {
	case LXT_PDA_POWER_ON_REQUEST :
	    /*
	     * Interanl state\B4\C2 \BD\C7\C1\A6 dpram event\BF\A1\BC\AD active\B8\A6 Ȯ\C0\CE\C7\D1 \C8Ŀ\A1 ó\B8\AE
	     */
	    Device->PowerOnDevice(&GlobalS, 0);
	    break;

	case LXT_PDA_POWER_OFF_REQUEST :
	    /*
	     *
	     */
	    // FuncPhoneServer->BroadcastInternalStatus(&GlobalPS, LXT_STATE_OFF);
	    // error = MiscFunc->PowerOff(&GlobalS);
	    // Device->PowerOffDevice(&GlobalS);

	    /* Phone doesn't seem to respond correctly to the Power-Off request.
	       After Power-Off request, we got Phone-Active signals repeatedly. civily, 2005.09.26 [
	       TxPWR_ExecPhonePowerOff();
	     */ // civily, 2005.09.26 ]
	    TxPWR_ExecPhonePowerOff();
	    FuncPhoneServer->BroadcastInternalStatus(&GlobalPS, LXT_STATE_OFF);
	    break;

	default :
	    TRACE(MSGL_VGSM_INFO, "nexpected request cmd from clients 0x%02x\n", action);
	    break;
    }
}
#endif /* 0 */

static void do_call(PhoneServer * ps, TClientInfo * ci, LXT_MESSAGE * packet)
{
    TRACE(MSGL_VGSM_INFO, "\n");

    int action = packet->action;
    int error ;

    TRACE(MSGL_VGSM_INFO, "do_call %x\n", action);

    switch (action)
    {
	case GSM_CALL_MAKE_REQ :
	    log_msg(MSGL_VGSM_INFO,"GSM_CALL_MAKE_REQ\n");
	    server_tx_call_incoming_noti(packet);
	    break;

	case GSM_CALL_HANGUP_REQ :
	    log_msg(MSGL_VGSM_INFO,"GSM_CALL_HANGUP_REQ\n");
	    server_tx_call_release_exec(packet);
	    break;

	case GSM_CALL_ANSWER_REQ :
	    log_msg(MSGL_VGSM_INFO,"GSM_CALL_ANSWER_REQ\n");
	    {
		int active = get_active_call_count();
		int hold = get_hold_call_count();

		if(active >=1 && hold >= 1)
		{
		    log_msg(MSGL_VGSM_INFO,"======== 1 active + 1held call ========\n");
		    return ;
		}
		else if(active>=1)
		{
		    log_msg(MSGL_VGSM_INFO,"======== 1 active call   ========\n");
		    swap_call();
		    server_tx_call_list_noti();
		}
		else
		{
		    log_msg(MSGL_VGSM_INFO,"======== no established call! ========\n");
		    server_tx_call_answer_exec();
		}
	    }
	    break;
	case GSM_CALL_ALERT_IND:
	    log_msg(MSGL_VGSM_INFO,"GSM_CALL_ALERT_IND\n");
	    server_tx_call_alert_ind(packet->data, packet->length);
	    break;

	    // 090312
	case GSM_EMULATOR_SET_SS_GENERAL_RESPONSE_ERROR:
	    log_msg(MSGL_VGSM_INFO,"GSM_EMULATOR_SET_SS_GENERAL_RESPONSE_ERROR\n");
	    ATTxEmulator_Set_General_Response_Error(packet);
	    error = get_general_response_error();
	    log_msg(MSGL_VGSM_INFO,"======== Disconnect! error set  %x ========\n", error);
	    break;

	default:
	    assert(0);
    }
}

#if 0
void do_sound(PhoneServer * ps, TClientInfo * ci, LXT_MESSAGE * packet)
{
    TRACE(MSGL_VGSM_INFO, "\n" );

    GSM_Error error;

    int klass = ci->klass;
    int action = packet->action;
    int length = packet->length;

    char * ptr = (char *)(packet->data);

    switch (action)
    {
	case GSM_SND_AUDIO_PATH_CTRL_REQ:
	    {
		GSM_SoundAudioPathType type;
		type = *ptr;
		TxSND_SetAudioPathCtrl(type);
	    }
	    break;

	case GSM_SND_SET_SPKR_VOLUME_CTRL_REQ:
	    {
		TxSND_SetVolumeCtrl(ptr[0], ptr[1]);
	    }
	    break;

	case GSM_SND_NOTI_TONE_REQ:
	    {
		GSM_SndNotiToneSet tone;

		tone.mode		= ptr[0];
		tone.volume		= ptr[0];
		tone.count		= ptr[0];
		tone.tone_type	= ptr[0];

		TxSND_SetNotiTone(&tone);
	    }
	    break;

	case GSM_SND_GET_GAIN_CTRL_REQ:
	    TxSND_GetGainCtrl(ptr);
	    break;

	case GSM_SND_SET_GAIN_CTRL_REQ:
	    TxSND_ExecGainCtrl(ptr);
	    break;

	case GSM_SND_SET_MIC_MUTE_CTRL_REQ:
	    {
		GSM_SoundMuteStatusType mute;

		mute = *ptr;
		TxSND_SetMICMuteCtrl(mute);
	    }
	    break;

	default:
	    break;
    }
}

void do_svcmode(PhoneServer * ps, TClientInfo * ci, LXT_MESSAGE * packet)
{
    TRACE(MSGL_VGSM_INFO, "\n" );

    char *ptr = (char *) packet->data;

    switch (packet->action)
    {
	case GSM_SVCMODE_ENTER_REQ:
	    TxSVCMode_SetEnterServiceMode(ptr);
	    break;

	case GSM_SVCMODE_END_REQ:
	    TxSVCMode_SetEndServiceMode(ptr[0]);
	    break;

	case GSM_SVCMODE_PROCESS_KEY_REQ:
	    TxSVCMode_SetProcessKeyCode(ptr[0]);
	    break;
    }
}
#endif /* 0 */

static int client_callback(PhoneServer * ps, int fd, EloopCondition cond, void * data)
{
    log_msg(MSGL_VGSM_INFO," client_callback fd=%d \n",fd);
    TClientInfo * ci = data;

    LXT_MESSAGE packet;
    int rc = -1;
    int group;
    int action;
    int length;
    //int klass = ci->klass;
    int clientfd = ci->fd;
    int rssi = 5;

    //unsigned char * p = 0;

    TAPIMessageInit(&packet);

    rc = ReadPacketBytes4(clientfd, &packet);

    if  (rc <= 0)
    {
	Server * server = &ps->ServerSocket;
	if  (server != NULL)
	{
	    TRACE(MSGL_VGSM_INFO, "Client was disconnected\n");
	    server->Functions->RemoveClient(ps, ci);
	}
	return -1;
    }

    if  (packet.length > 0)
    {
	packet.data = (unsigned char *) PacketDataMalloc(packet.length + 1);
	rc = ReadBytes(clientfd, packet.data, packet.length);
	//p = (unsigned char *)packet.data;
    }

    group = packet.group;
    action = packet.action;
    length = packet.length;

    TRACE(MSGL_VGSM_INFO, "in client_callback, group : %x\n", group);

    if(vconf_get_int(VCONFKEY_TELEPHONY_RSSI, &rssi)) {
	TRACE(MSGL_WARN, "vconf_get_int(%s) fail\n", VCONFKEY_TELEPHONY_RSSI);
    }

    switch (group)
    {
	case GSM_CALL:
	    change_state_machine( GSM_CALL_CMD );
	    if( is_flight_mode() ){
		TRACE(MSGL_VGSM_INFO, "Flight mode on \n");
		callback_callist();
	    } else if(rssi == 0) {
		TRACE(MSGL_VGSM_INFO, "RSSI is zero \n");
		callback_callist();
	    } else {
		TRACE(MSGL_VGSM_INFO, "Call ok \n");
		do_call(ps, ci, &packet);
	    }
	    break;
	case GSM_NETWORK:
	    do_network(ps, ci, &packet);
	    break;
	case GSM_SUPS:
	    do_sups(ps, ci, &packet);
	    break;
	case GSM_SMS:
	    if( is_flight_mode() ){
		TRACE(MSGL_VGSM_INFO, "Flight mode on \n");
		sms_response_for_eventinjector();
	    } else if(rssi == 0) {
		TRACE(MSGL_VGSM_INFO, "RSSI is zero \n");
		sms_response_for_eventinjector();
	    } else {
		TRACE(MSGL_VGSM_INFO, "SMS ok \n");
		do_sms(ps, ci, &packet);
	    }
	    break;
	case GSM_DISPLAY:
	    do_display(ps, ci, &packet);
	    break;
	case GSM_SIM:
	    do_sim(ps, ci, &packet);
	    break;
	case GSM_DATA :
	    do_data(ps, ci, &packet);
	    break;
	case GSM_SAT:
	    do_sat(ps, ci, &packet);
	    break;
	case GSM_EMULATOR :
	    do_emulator(ps, ci, &packet);
	    break;
	case GSM_GPRS :
	    do_gprs(ps, ci, &packet);
	    break;
	case GSM_POWER :
	    do_power(ps, ci, &packet);
	    break;
	case GSM_CLIENT:
	    do_client(ps, ci, &packet);
	    break;
	case LXT_GRP_INTERNAL :
	    do_internal(ps, ci, &packet);
	    break;
	    //090326
	case RESTORE_EI_DATA :
	    do_restore_ei(ps, ci, &packet);


#if 0
	case LXT_GRP_POWER    :
	    /* todo : nsclass
	       if  (GlobalS.Phone.Data.RamDump == true)
	       {
	       FuncResponse->ResponseRamDump(ci->klass);
	       break;
	       }
	     */

	    do_power(ps, ci, &packet);
	    break;

	case GSM_CALL      :
#ifdef __arm__
	    if  (GlobalS.Phone.Data.PhonePower == false)
		break;
#endif
	    do_call(ps, ci, &packet);
	    break;

	case GSM_MISC      :
	    /* todo : nsclass
	       if  (GlobalS.Phone.Data.RamDump == true)
	       {
	       if  (action == GSM_MISC_RAMDUMP_REQ)
	       do_misc(ps, ci, &packet);
	       else
	       FuncResponse->ResponseRamDump(ci->klass);
	       break;
	       }
	     */

	    if  (GlobalS.Phone.Data.PhonePower == false)
		break;
	    do_misc(ps, ci, &packet);
	    break;

	case GSM_SMS       :
	    /* todo : nsclass
	       if  (GlobalS.Phone.Data.RamDump == true)
	       {
	       FuncResponse->ResponseRamDump(ci->klass);
	       break;
	       }
	     */

	    //			if  (GlobalS.Phone.Data.PhonePower == false)
	    //                break;
	    do_sms(ps, ci, &packet);
	    break;

	case GSM_SUPS      :
	    /* todo : nsclass
	       if  (GlobalS.Phone.Data.RamDump == true)
	       {
	       FuncResponse->ResponseRamDump(ci->klass);
	       break;
	       }
	     */

	    if  (GlobalS.Phone.Data.PhonePower == false)
		break;
	    do_sups(ps, ci, &packet);
	    break;

	case GSM_SIM      :
	    /* todo : nsclass
	       if  (GlobalS.Phone.Data.RamDump == true)
	       {
	       FuncResponse->ResponseRamDump(ci->klass);
	       break;
	       }
	     */

	    if  (GlobalS.Phone.Data.PhonePower == false)
		break;
	    do_sim(ps, ci, &packet);
	    break;

	case GSM_NETWORK  :
	    /* todo : nsclass
	       if  (GlobalS.Phone.Data.RamDump == true)
	       {
	       FuncResponse->ResponseRamDump(ci->klass);
	       break;
	       }
	     */
	    if  (GlobalS.Phone.Data.PhonePower == false)
		break;
	    do_network(ps, ci, &packet);
	    break;

	case GSM_DATA :
	    /* todo : nsclass
	       if  (GlobalS.Phone.Data.RamDump == true)
	       {
	       FuncResponse->ResponseRamDump(ci->klass);
	       break;
	       }
	     */

	    if  (GlobalS.Phone.Data.PhonePower == false)
		break;
	    do_data(ps, ci, &packet);
	    break;

	case GSM_SOUND:
	    if  (GlobalS.Phone.Data.PhonePower == false)
		break;
	    do_sound(ps, ci, &packet);
	    break;

	case GSM_EXT :
	    /* todo : nsclass
	       if  (GlobalS.Phone.Data.RamDump == true)
	       {
	       FuncResponse->ResponseRamDump(ci->klass);
	       break;
	       }
	     */

	    if  (GlobalS.Phone.Data.PhonePower == false)
		break;
	    do_ext(ps, ci, &packet);
	    break;

	case GSM_SERVICEMODE:
	    if  (GlobalS.Phone.Data.PhonePower == false)
		break;
	    do_svcmode(ps, ci, &packet);
	    break;

	case GSM_EMULATOR :
	    /* todo : nsclass
	       if  (GlobalS.Phone.Data.RamDump == true)
	       {
	       FuncResponse->ResponseRamDump(ci->klass);
	       break;
	       }
	     */

	    if  (GlobalS.Phone.Data.PhonePower == false)
		break;
	    do_emulator(ps, ci, &packet);
#endif /* 0 */
	default :
	    break;
    }
    TAPIMessageFree(&packet);
    return 1;
}

FunctionsClient ClientHandle =
{
    client_callback
};

