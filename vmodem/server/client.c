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
#include <stdint.h>
#include <assert.h>
#include "gsmstate.h"
#include "phoneprotocol4gsm.h"
#include "vgsm_phone.h"
#include "vgsm_ss.h"
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

#include <vconf/vconf.h>
#include <vconf/vconf-keys.h>

#define MAX_RESULT_LENGTH 1024

GSM_Device_Functions *Device;
int smsSentStatus = 0;

extern GSM_StateMachine GlobalS;

typedef struct _msg_info {
    char buf[1024];

    uint32_t route;
    uint32_t use;
    uint16_t count;
    uint16_t index;

    unsigned int cclisn;
} msg_info;

#define MMI_SC_LENGTH2          14
#define MMI_SC_LENGTH3          10

#define MMS_SEND_OK             10
#define MMS_SEND_FAIL           11
#define MMS_MODIFIER            10

// 090326 // restore data in EI
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

    for( i=0; i<MAX_CALL_COUNT; ++i ) {
        if(callList->CallInfo[i].stat == GSM_CALL_STATUS_DIALING
                || callList->CallInfo[i].stat == GSM_CALL_STATUS_ALERT) {
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

    DP_TEXT *dp_text;
    GET_INKEY *get_inkey;
    GET_INPUT *get_input;

    TRACE(MSGL_VGSM_INFO, "%x\n", action);
    switch (action)
    {
        case GSM_SAT_PROACTIVE_DISPLAY_TEXT:
            log_msg(MSGL_VGSM_INFO,"Client - GSM_SAT_PROACTIVE_DISPLAY_TEXT\n");
            dp_text = (DP_TEXT *)(packet->data);
            Display_Text(dp_text);
            break;
        case GSM_SAT_PROACTIVE_GET_INKEY:
            log_msg(MSGL_VGSM_INFO,"Client - GSM_SAT_PROACTIVE_GET_INKEY\n");
            get_inkey = (GET_INKEY *)(packet->data);
            Get_Inkey(get_inkey);
            break;
        case GSM_SAT_PROACTIVE_GET_INPUT:
            log_msg(MSGL_VGSM_INFO,"Client - GSM_SAT_PROACTIVE_GET_INPUT\n");
            get_input = (GET_INPUT *)(packet->data);
            Get_Input(get_input);
            break;
        case GSM_SAT_PROACTIVE_SET_MENU:
            log_msg(MSGL_VGSM_INFO,"Client - GSM_SAT_PROACTIVE_SET_MENU\n");
            Set_Menu(packet->data);
            break;
        case GSM_SAT_PROACTIVE_SELECT_ITEM:
            log_msg(MSGL_VGSM_INFO,"Client - GSM_SAT_PROACTIVE_SELECT_ITEM\n");
            Select_Item(packet->data);
            break;
        case GSM_SAT_PROACTIVE_SEND_SMS:
            log_msg(MSGL_VGSM_INFO,"Client - GSM_SAT_PROACTIVE_SEND_SMS\n");
            Send_SMS(packet->data);
            break;
        case GSM_SAT_PROACTIVE_SET_UP_CALL:
            log_msg(MSGL_VGSM_INFO,"Client - GSM_SAT_PROACTIVE_SET_UP_CALL\n");
            Set_Up_Call(packet->data);
            break;
        default :
            break;
    }
}

static void do_data(PhoneServer* ps, TClientInfo * ci, LXT_MESSAGE * packet)
{
    TRACE(MSGL_VGSM_INFO, "\n");

    int action = packet->action;

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

    int action = packet->action;

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

    int *list = NULL;
    char table[10] = {0};
    _SIMD_t *simd_t = NULL;


    get_current_state_machine( &current );
    TRACE(MSGL_VGSM_INFO, "do_sim %x\n", action);
    TRACE(MSGL_VGSM_INFO, "current_state=[%x]\n\n", STATE_TYPE(current) );

    switch (action)
    {

        /****************************************************************************/
        /*                                                                          */
        /*                               security part                              */
        /*                                                                          */
        /****************************************************************************/
        case GSM_SIM_INIT_REQ:
            server_sim_db_init();
            break;

            // 090602
        case GSM_SIM_REINIT_REQ:
            for(i=0;i<7;i++) {
                db_sim_info_add(i,NULL);
                db_sim_data_add(i,NULL);
            }

            break;

        case  GSM_SIM_STATUS_SET:
            status = p[0];

            log_msg(MSGL_VGSM_ERR,"status=[%x]\n",status);

            switch(status)
            {
                case GSM_SEC_STATUS_PIN_READY:
                    server_sec_set_sec_status(GSM_SIM_READY);
                    server_sec_reset_remain_cnt(GSM_SIM_READY);
                    server_sec_set_facility(GSM_SIM_FACILITY_PIN, GSM_SEC_LOCK_MODE_UNLOCK);
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
                    break;

                case GSM_SEC_STATUS_PUK2_REQ:
                    server_sec_set_sec2_status(GSM_SIM_PUK2_REQ);
                    server_sec_set_cnt_zero(GSM_SIM_PIN2_REQ);
                    server_sec_reset_remain_cnt(GSM_SIM_PUK2_REQ);
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
            break;

        case GSM_SIM_CHANGE_PASS_SET :
            type = p[0];
            password = malloc(length-1);
            if(!password) {
                return;
            }
            memcpy(password,&p[1],length-1);

            switch(type)
            {
                case GSM_SEC_LOCK_TYPE_SC:
                    server_sec_change_password(password,GSM_SIM_PIN_REQ_STATE,length-1);
                    break;

                case GSM_SEC_LOCK_TYPE_SC2:
                    server_sec_change_password(password,GSM_SIM_PIN2_REQ,length-1);
                    break;
                case GSM_SEC_LOCL_TYPE_PUK:
                    server_sec_change_password(password,GSM_SIM_PUKSET_REQ,length-1);
                    break;
                case GSM_SEC_LOCL_TYPE_PUK2:
                    server_sec_change_password(password,GSM_SIM_PUK2SET_REQ,length-1);
                    break;
                default :
                    log_msg(MSGL_VGSM_ERR,"ERROR -Not handled type =[%d] in change pw \n", type);
                    break;
            }
            break;

        case GSM_SIM_FACILITY_SET:
            type = p[0];
            flag =  p[1];

            log_msg(MSGL_VGSM_ERR,"GSM_SIM_FACILITY_SET - type, flag = [%d],[%d]\n", type, flag);

            if(type == GSM_SEC_LOCK_TYPE_SC) {
                server_sec_set_facility(GSM_SIM_FACILITY_PIN,flag);
            } else if( type == GSM_SEC_LOCK_TYPE_FD) {
                server_sec_set_facility(GSM_SIM_FACILITY_FDN,flag);
            } else {
                log_msg(MSGL_VGSM_ERR,"ERROR - Not handled type =[%d] in facility set \n", type);
            }
            break;

        case GSM_SIM_GET_SIM_DB_REQ:
            log_msg(MSGL_VGSM_ERR,"action ->GSM_SIM_GET_SIM_DB_REQ\n");
            db_sim_sec_restore(VGSM_SIMTypeget);
            break;

        case GSM_SIMINFO_REQ:
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
            break;

        case GSM_SIMDATA_REQ:
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
            break;

        case GSM_SIMSAVE_REQ:
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
            break;


            /*************************************************************************/
            /*                                                                       */
            /*                             Phonebook part                            */
            /*                                                                       */
            /*************************************************************************/

        case GSM_SIM_MAKE_PB_DB_REQ:
            log_msg(MSGL_VGSM_ERR,"action ->GSM_SIM_MAKE_PB_DB_REQ\n");
            db_sim_pb_create();
            break;

        case GSM_SIM_ADD_PB_DB_REQ:
            log_msg(MSGL_VGSM_ERR,"action ->GSM_SIM_ADD_PB_DB_REQ\n");

            pb = (PB *)(packet->data);
            db_sim_pb_set_mgr(pb->db_st_type,1);

            log_msg(MSGL_VGSM_ERR,"action ->GSM_SIM_ADD_PB_DB_REQ : %d , %s ,%s, %s\n",pb->db_st_type,pb->name,pb->number,pb->address);

            db_sim_pb_add(pb);
            db_sim_pb_restore(pb);

            break;

        case GSM_SIM_GET_PB_DB_REQ:
            log_msg(MSGL_VGSM_ERR,"action ->GSM_SIM_GET_PB_DB_REQ\n");

            pb = (PB *)(packet->data);

            log_msg(MSGL_VGSM_ERR,"action ->GSM_SIM_GET_PB_DB_REQ : %d , %s \n",pb->db_st_type,pb->name);

            db_sim_pb_get(pb);
            db_sim_pb_restore(pb);
            break;

        case GSM_SIM_DELETE_PB_DB_REQ:
            log_msg(MSGL_VGSM_ERR,"action ->GSM_SIM_DELETE_PB_DB_REQ\n");

            pb = (PB *)(packet->data);
            db_sim_pb_set_mgr(pb->db_st_type,1);

            log_msg(MSGL_VGSM_ERR,"action ->GSM_SIM_DELETE_PB_DB_REQ : %d , %s \n",pb->db_st_type,pb->name);

            db_sim_pb_remove(pb);
            db_sim_pb_restore(pb);
            break;

        case GSM_SIM_GET_ALL_PB_DB_REQ:
            log_msg(MSGL_VGSM_ERR,"action ->GSM_SIM_GET_ALL_PB_DB_REQ\n");

            pb = (PB *)(packet->data);

            db_sim_pb_restore(pb);
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
        case GSM_SUPS_AOC_GET:
            server_tx_ss_aoc_get_req(packet);
            break;
        case GSM_SUPS_AOC_SET:
            server_tx_ss_aoc_set_req(packet);
            break;
            // 090330 090406 //////////////////////////////////////////////
        case GSM_SUPS_CW_SET:
            server_tx_ss_cw_set_req(packet);
            break;
        case GSM_SUPS_CF_SET:
            server_tx_ss_cf_set_req(packet);
            break;
        case GSM_SUPS_CB_SET:
            server_tx_ss_cb_set_req(packet);
            break;
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

static void do_emulator(PhoneServer * ps, TClientInfo * ci, LXT_MESSAGE * packet)
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

static void do_client(PhoneServer * ps, TClientInfo * ci, LXT_MESSAGE * packet)
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
    }
}

static void do_internal(PhoneServer * ps, TClientInfo * ci, LXT_MESSAGE * packet)
{
    int klass = ci->klass;
    int clientfd = ci->fd;
    int action = packet->action;
    int length = packet->length;
    int clientid;

    TRACE(MSGL_VGSM_INFO, "do_internal %x\n", action);

    char * p = (char *)(packet->data);
    LXT_MESSAGE tmgs;
    Server * server = &ps->ServerSocket;

    switch (action)
    {
        case LXT_PDA_INTERNAL_ID_REQUEST :

            clientid = (int)packed_S32((unsigned char *)p);
            if(clientid == 0) {
                TRACE(MSGL_VGSM_INFO, "ERROR!! Invalid value of clientid.\n");
            }
            ci->klass = clientid;
            TRACE(MSGL_VGSM_INFO, "LXT_PDA_INTERNAL_ID_REQUEST [0x%x]: %s\n", clientid, clientName[clientid]);
            break;

        case LXT_PDA_INTERNAL_MISSED_MESSAGE_REQUEST :
            TRACE(MSGL_VGSM_INFO, "LXT_PDA_INTERNAL_MISSED_MESSAGE_REQUEST \n");
            if  (server != NULL) {
                server->Functions->CastMissedMessage(ps, klass, clientfd);
            }
            break;

        case LXT_PDA_INTERNAL_STATE_REQUEST :
            TRACE(MSGL_VGSM_INFO, "LXT_PDA_INTERNAL_MISSED_MESSAGE_REQUEST ..WARNING: it's not implemented\n");
            break;

        case LXT_PDA_INTERNAL_LAUNCH_SAT_APP_REQUEST:
            TAPIMessageInit(&tmgs);
            TRACE(MSGL_VGSM_INFO, "LXT_PDA_INTERNAL_LAUNCH_SAT_APP_REQUEST\n");
            tmgs.group = LXT_GRP_INTERNAL;
            tmgs.action = LXT_PDA_INTERNAL_LAUNCH_SAT_APP_RESPONSE;
            FuncServer->Cast(&GlobalPS, LXT_ID_CLIENT_CARD_MANAGER, &tmgs);
            break;
        case LXT_PDA_INTERNAL_SETUP_SAT_APP_REQUEST:
            TAPIMessageInit(&tmgs);
            TRACE(MSGL_VGSM_INFO, "LXT_PDA_INTERNAL_SETUP_SAT_APP_REQUEST\n");
            tmgs.group = LXT_GRP_INTERNAL;
            tmgs.action = LXT_PDA_INTERNAL_SETUP_SAT_APP_RESPONSE;
            tmgs.length = length;
            tmgs.data = (void *)p;
            FuncServer->Cast(&GlobalPS, LXT_ID_CLIENT_CARD_MANAGER, &tmgs);
            break;

        case LXT_PDA_INTERNAL_PHONE_BATTERY_REQUEST:
            TRACE(MSGL_VGSM_INFO, "Request phone battery\n");
            log_msg(MSGL_VGSM_INFO,"WARNING: it's not implemented \n");
            break;
        default :
            break;
    }
}

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

                if(active >=1 && hold >= 1) {
                    log_msg(MSGL_VGSM_INFO,"======== 1 active + 1held call ========\n");
                    return ;
                } else if(active>=1) {
                    log_msg(MSGL_VGSM_INFO,"======== 1 active call   ========\n");
                    swap_call();
                    server_tx_call_list_noti();
                } else {
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

static int client_callback(PhoneServer * ps, int fd, EloopCondition cond, void * data)
{
    log_msg(MSGL_VGSM_INFO," client_callback fd=%d \n",fd);
    TClientInfo * ci = data;

    LXT_MESSAGE packet;
    int rc = -1;
    int group;
    int action;
    int length;
    int clientfd = ci->fd;
    int rssi = 5;

    TAPIMessageInit(&packet);

    msg_info _msg;
    memset(&_msg, 0x00, sizeof(struct _msg_info));
    rc = ReadBytes(clientfd, &_msg, sizeof(struct _msg_info));
    if(rc <= 0) {
        TRACE(MSGL_VGSM_ERR, "read err.\n");
        return -1;
    }
    memcpy(&packet, _msg.buf + 10, 4);
    if (packet.length > 0) {
        packet.data = (unsigned char *) PacketDataMalloc(packet.length + 1);
        memcpy(packet.data, _msg.buf + 10 + 4, packet.length);
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
        case GSM_RSSI:
            server_tx_display_rssi_info_noti(0xff, action);
            break;
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

