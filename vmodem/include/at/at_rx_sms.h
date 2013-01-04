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

int at_rx_sms_SendMsg(char* atmsg);
int at_rx_sms_deliver_report_exec(char* atmsg);

int at_rx_sms_SCA(char* atmsg);

int at_rx_sms_DefaultSms(char* atmsg);


// 090310
int at_rx_sms_DR(char* atmsg);

int at_rx_sms_param(char* atmsg);
int at_rx_sms_stored_msg_status(char* atmsg);
int at_rx_sms_param_count_get(char* atmsg);


int at_rx_sms_mem_status_set(char* atmsg);

int at_rx_sms_cbconfig(char* atmsg);
int at_rx_sms_stored_msg_count_get(char* atmsg);
int at_rx_sms_svc_option_set(char* atmsg);

unsigned char* at_rx_sms_param_get(unsigned char* len);
int at_rx_sms_param_set(char* atmsg);
static void at_rx_sms_param_init(void);
int sms_param_set_vconf(void);
