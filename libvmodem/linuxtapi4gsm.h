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

#ifndef _LINUXTAPI4GSM_H_
#define _LINUXTAPI4GSM_H_


#ifdef __cplusplus
extern "C"
{
#endif


// ++++++++++++++++++++++++++++++++++++++++++++++include about standard library
#include <stdio.h>
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++include user define
#include "linuxtapi4gsm_type.h"
#include "phoneprotocol4gsm.h"
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++define area

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++global variable area

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++function prototype

// config =====================================================================
int lxt_gsm_config_get_call_cost(LXT_HANDLE* handle, LXT_CALL_COST_OPTION option);
int lxt_gsm_config_reset_call_cost(LXT_HANDLE* handle);
int lxt_gsm_config_set_max_call_cost(LXT_HANDLE* handle, int max_cost);
int lxt_gsm_config_get_call_unit_price(LXT_HANDLE* handle);
int lxt_gsm_config_set_call_unit_price(LXT_HANDLE* handle, char unit, char* price);
int lxt_gsm_config_get_network_selection_mode(LXT_HANDLE* handle);
int lxt_gsm_config_automatic_network_selection(LXT_HANDLE* handle);
int lxt_gsm_config_manual_network_seleciton(LXT_HANDLE* handle, char mode);
int lxt_gsm_config_manual_network_selection_registration(LXT_HANDLE* handle, char *plmn_id, LXT_GSM_CONFIG_RAT rat);
int lxt_gsm_config_get_caller_id(LXT_HANDLE* handle);
int lxt_gsm_config_set_caller_id(LXT_HANDLE* handle, LXT_CALLER_ID_OPTION option);
int lxt_gsm_config_get_active_line(LXT_HANDLE* handle);
int lxt_gsm_config_set_active_line(LXT_HANDLE* handle, char line);
int lxt_gsm_config_set_force_gsm_mode(LXT_HANDLE* handle);
int lxt_gsm_config_get_system_identity(LXT_HANDLE* handle);

// call =======================================================================
int lxt_gsm_call_originaton(LXT_HANDLE* handle, char* callnumber);
int lxt_gsm_call_end(LXT_HANDLE* handle, int call_id_num, char* call_id_list);
int lxt_gsm_call_reject(LXT_HANDLE* handle, int call_id);
int lxt_gsm_call_answer(LXT_HANDLE* handle, int call_id);
int lxt_gsm_call_start_dtmf(LXT_HANDLE* handle, char digit, int call_id);
int lxt_gsm_call_stop_dtmf(LXT_HANDLE* handle, int call_id);

// sups =======================================================================
int lxt_gsm_sups_originated_sups(LXT_HANDLE* handle, LXT_GSM_SUPS_ORIGINATED *sups_originated);
int lxt_gsm_sups_network_originated_sups_user_confirm(LXT_HANDLE* handle, LXT_GSM_SUPS_USER_CONFIRM *sups_user_confirm);
int lxt_gsm_sups_get_sups_network_confirm_data(LXT_HANDLE* handle, char block_id);
int lxt_gsm_sups_password_registration(LXT_HANDLE* handle, char* old_pw, char* new_pw, char* new_pw_again, char* ss_code);
//int lxt_gsm_sups_get_barring_password_confirm(LXT_HANDLE* handle, char* barring_pw);
int lxt_gsm_sups_handle_within_call(LXT_HANDLE*, LXT_GSM_MPTY_OPERATION, LXT_GSM_MPTY_PARAM_ID, char*);


#ifdef __cplusplus
}
#endif

#endif // _LINUXTAPI4GSM_H_
