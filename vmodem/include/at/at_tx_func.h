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

/*
/////////////////////////////////////////////////////////////////////
// at_tx_func.h
*/

#ifndef __AT_TX_FUNC_H__
#define __AT_TX_FUNC_H__

#include <vgsm_network.h>
//#include <vgsm_sim.h>
//#include <mzgsmpb.h>
#include <vgsm_sat.h>
#include <vgsm_ss.h>
#include <vgsm_gprs.h>
#include <vgsm_call.h>
#include <vgsm_display.h>
#include <vgsm_data.h>
#include <linuxtapi.h>

#include "state.h"
#include "logmsg.h"
#include "at_msg.h"
//#include "db_phonebook.h"

// 14. GSM_SAT_CMD
int TxSAT_ATGetProactiveCommand(GSM_SatProactiveCmd const* cmd);
int TxGCMD_SendGeneralResponse(char* atmsg);

// EMULATOR
int ATTxEmulator_HDLC_Req(LXT_MESSAGE const* packet);
int ATTxEmulator_HDLC_Mode(LXT_MESSAGE const* packet);
int ATTxEmulator_HDLC_Clear(void);
int ATTxEmulator_Set_General_Response_Error(LXT_MESSAGE const* packet);
int ATTxEmulator_Set_SS_General_Response_Error(LXT_MESSAGE const* packet);
int ATTxEmulator_Set_Call_Status_Error(LXT_MESSAGE const* packet);
int ATTxEmulator_Set_SCA(LXT_MESSAGE const* packet);
int ATTxEmulator_Set_SS_Response_Error(LXT_MESSAGE const* packet, int ss_error_type);
void ATset_sca( SmsAddressInfo SCA );

#endif /* __AT_TX_FUNC_H__ */

