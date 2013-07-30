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

#ifndef __AT_TX_SMS_H__
#define __AT_TX_SMS_H__

#include "at_msg.h"
#include "logmsg.h"

void sms_response_for_eventinjector(void);

int at_tx_sms_deviceReady(void *data, int len);

int at_tx_sms_NotiIncomingMsg(void *data, int len);

int at_tx_sms_sendDeliverReport(void *data, int len);

int at_tx_sms_sendMsg(void *data, int len);

int at_tx_sms_sendCBSMsg(void *data, int len);

int at_tx_sms_SCA(void *data, int len);

int at_tx_sms_cbconfig_resp(unsigned char enabled, unsigned char selected_id, unsigned char list_max, unsigned char list_count, unsigned short *list);

int at_tx_sms_stored_msg_count_resp(void);
int at_tx_sms_deliver_report_noti(int result_status);

int at_tx_sms_param_count_resp(unsigned char mem_store, unsigned char record_count);
int at_tx_sms_param_resp(unsigned char mem_store, unsigned char record_index, unsigned char record_len, unsigned char *record);


#endif /* __AT_TX_SMS_H__ */
