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

#ifndef	_vgsm_h_
#define _vgsm_h_

#include <time.h>
#include <string.h>
#include <stdlib.h>

#include "lxtutil.h"
#include "linuxtapi.h"
#include "vgsm_phone.h"
#include "vgsm_call.h"
#include "vgsm_data.h"
#include "vgsm_gprs.h"
#include "vgsm_display.h"
#include "vgsm_sim.h"
#include "vgsm_pb.h"
#include "vgsm_edb.h"
#include "sms_util.h"
#include "vgsm_power.h"
///////////////////////////////090213//
#include "vgsm_ss.h"
//////////////////////////////////////////////
//#include "../vgsm/db_gprs.h"
//080117
//#include "../vgsm/sat/sat_values.h"

typedef struct tagTAT {
    char* atmsg;
} TAT;

typedef struct _TCallbackInfo {
    time_t time;
    int rx;
    TAT at;
} TCallbackInfo;

// init
LXT_HANDLE* vgsm_injector_initialize(LXT_CALLBACK cb);
LXT_HANDLE* vgsm_manager_rx_initialize(LXT_CALLBACK cb);
LXT_HANDLE* vgsm_manager_tx_initialize(LXT_CALLBACK cb);
LXT_HANDLE* vgsm_manager_initialize(LXT_CALLBACK cb);
int vgsm_injector_release(LXT_HANDLE* handle);

int vgsm_injector_get_fd(LXT_HANDLE *handle);
int vgsm_socket_read_callback(LXT_HANDLE* handle);

int vgsm_client_release_notify(LXT_HANDLE* handle);

//int vgsm_release(LXT_HANDLE* handle);

int vgsm_convert_callbackinfo( void *data, TCallbackInfo *info );
int vgsm_get_at_length_callbackinfo( TCallbackInfo info );
int vgsm_convert_atinfo( void *data, TAT *at );
int vgsm_get_at_length_atinfo( TAT at );

// SMS
/* add user option by cosmos in 20090213 */
int vgsm_sms_sendMessage(LXT_HANDLE* handle, SmsAddressInfo SCA, TPDU_SMS_DELIVER tpdu_deliver, BYTE pid);

int vgsm_sms_sendAck(LXT_HANDLE* handle, int error);
int vgsm_cb_sendMessage(LXT_HANDLE* handle, CELLBROADCASTING cb_msg); //june

// Call
int vgsm_call_make(LXT_HANDLE* handle, gsm_call_info_t callinfo);
int vgsm_call_hangup(LXT_HANDLE* handle, int call_id);
int vgsm_call_answer(LXT_HANDLE* handle);
// added by okdear for vodafone
int vgsm_call_alert(LXT_HANDLE* handle);
int vgsm_call_disconnect_cause(LXT_HANDLE* handle, int call_error_cause);

// Supplementary Service
int vgsm_ss_conference(LXT_HANDLE* handle);
int vgsm_ss_hold(LXT_HANDLE* handle);
int vgsm_ss_unhold(LXT_HANDLE* handle);
    // 090213
int vgsm_ss_setAOC(LXT_HANDLE* handle, _AOC_t *aoc, int size);
int vgsm_ss_setCB(LXT_HANDLE* handle, _setCB_t *setCB, int size);
int vgsm_ss_setCF(LXT_HANDLE* handle, _setCF_t *setCF, int size);
int vgsm_ss_setCW(LXT_HANDLE* handle, _setCW_t *setCW, int size);
int vgsm_ss_setUSSD(LXT_HANDLE* handle, _USSD_data_t *ussd, int size);
int vgsm_ss_getUSSD(LXT_HANDLE* handle);
int vgsm_ss_getAOC(LXT_HANDLE* handle);
int vgsm_ss_restoreEI(LXT_HANDLE* handle);

// Network
int vgsm_network_registration(LXT_HANDLE* handle, gsm_network_reg_t reg);
/* network plmn, nitz information by cosmos in 20090212 */
int vgsm_network_plmn_information(LXT_HANDLE* handle, gsm_network_plmn_info_t reg);
int vgsm_network_nitz_information(LXT_HANDLE* handle, gsm_network_nitz_info_t reg);
int vgsm_network_plmn_list(LXT_HANDLE* handle, gsm_network_plmn_list_t plmn_list);
int vgsm_network_current_plmn(LXT_HANDLE *handle, gsm_network_plmn_record_t plmn);

// Data
int vgsm_data_status(LXT_HANDLE* handle, GSM_DataCallStatus status);

// GPRS
int vgsm_gprs_ip_configuration(LXT_HANDLE* handle, gsm_GprsConfList list);

// Display
int vgsm_display_icon_information(LXT_HANDLE* handle, disp_icon_info_t info);

// set error cause
int vgsm_set_call_general_response_error(LXT_HANDLE* handle, int error);
int vgsm_set_ss_general_response_error(LXT_HANDLE* handle, int error);
int vgsm_set_call_status_error(LXT_HANDLE* handle, int type, int error);

// Event Manager
int vgsm_set_hdlc_mode(LXT_HANDLE *handle, int mode);
int vgsm_send_hdlc(LXT_HANDLE * handle, const char *hdlc, int hdlc_len);
int vgsm_clear_hdlc(LXT_HANDLE * handle);


int vgsm_set_sca(LXT_HANDLE* handle, SmsAddressInfo SCA);

// power
int vgsm_power_modem_on_req_send(LXT_HANDLE * handle);  // this is temporary code.
int vgsm_power_battery_status(LXT_HANDLE* handle, battery_status_type_e level);

//sim
int vgsm_sim_init(LXT_HANDLE * handle);
int vgsm_get_sim_db(LXT_HANDLE * handle);
int vgsm_sim_status_set(LXT_HANDLE * handle,gsm_sec_status_type status);
int vgsm_sim_change_password(LXT_HANDLE * handle,gsm_sec_lock_e_type type,char *password,int length);
int vgsm_sim_facility_set(LXT_HANDLE * handle,gsm_sec_lock_e_type type,gsm_sec_lock_mode_e_type flag);
int vgsm_get_siminfo_db(LXT_HANDLE * handle, int *list );
int vgsm_get_simdata_db(LXT_HANDLE * handle, int *list);
int vgsm_sim_savedata(LXT_HANDLE * handle, _SIMD_t *simd_t );
int vgsm_sim_reinit(LXT_HANDLE * handle);

//pb
int vgsm_make_pb_db(LXT_HANDLE * handle);
int vgsm_get_all_pb_db(LXT_HANDLE * handle,gsm_pb_storage_type_e_type storage_type);
int vgsm_get_pb_db(LXT_HANDLE * handle,gsm_pb_storage_type_e_type storage_type,char* index_);
int vgsm_add_pb_db(LXT_HANDLE * handle,gsm_pb_storage_type_e_type storage_type,char* index_,char* name,char* number);
int vgsm_delete_pb_db(LXT_HANDLE * handle,gsm_pb_storage_type_e_type storage_type,char* index_);

// SAT
int vgsm_proactive_cmd_send(LXT_HANDLE * handle,int command);
int vgsm_sat_display_text(LXT_HANDLE * handle,char priority,char  clear_msg,char* textstring,char coding_scheme,char icon_identifier_flag,char icon_qualifier,char* icon_identifier,char immediate_response,char duration,char duration_unit );
int vgsm_sat_get_inkey(LXT_HANDLE * handle,char cmd_detail,char* textstring,char coding_scheme,char icon_identifier_flag,char icon_qualifier,char* icon_identifier,char immediate_response,char duration,char duration_unit );
int vgsm_sat_get_input(LXT_HANDLE * handle,char cmd_detail,char* textstring,char coding_scheme,char icon_identifier_flag,char icon_qualifier,char* icon_identifier,char immediate_response,char duration,char duration_unit );
int vgsm_sat_set_up_menu(LXT_HANDLE * handle,unsigned char cmd_detail,int num,void * set_up_item );
int vgsm_sat_select_item(LXT_HANDLE * handle,unsigned char cmd_detail,int num,void * set_up_item );
int vgsm_sat_send_sms(LXT_HANDLE * handle,unsigned char cmd_detail,unsigned char address,char *number,unsigned char* sms_tpdu   );
int vgsm_sat_set_up_call(LXT_HANDLE * handle,unsigned char cmd_detail,char* dial_number,char *alpha_user,/*char *alpha_call ,*/unsigned char address,unsigned char options,char duration_unit);

#endif /* _vgsm_h_ */

