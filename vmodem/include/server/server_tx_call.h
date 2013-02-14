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

#ifndef _SERVER_TX_CALL_
#define _SERVER_TX_CALL_

#include "linuxtapi.h"

int server_tx_call_incoming_noti( LXT_MESSAGE * packet );

int server_tx_call_list_noti(void);

int server_tx_call_list_resp(void);

int server_tx_call_msg(void);

int server_tx_call_status(void);

int server_tx_call_release_exec( LXT_MESSAGE * packet );

int server_tx_call_release_internal(int call_id);

int server_tx_call_answer_exec(void);

int server_tx_call_all_release(void);

int server_tx_call_alert_ind(void *ptr_data, int data_len );

void server_tx_call_burst_dtmf_noti(int success);

int server_tx_call_line_id_resp(void);

// 090314
int callback_list_noti(void);
void callback_callist(void);

int change_stat_for_at(int gsm_stat);
int change_dir_for_at(int gsm_dir);

#endif
