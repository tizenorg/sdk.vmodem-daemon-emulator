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

#include <assert.h>

#include "at_send.h"
#include "logmsg.h"
#include "at_gen_resp.h"
#include "at_tx_sms.h"
#include "at_func.h"
#include "phoneserver.h"
#include <stdio.h>

int at_tx_sms_deviceReady(void *data, int len)
{
	TRACE(MSGL_VGSM_INFO, "%%SCDEV noti\n");
	char sndbuf[SEND_BUF_SIZE];
	memset(sndbuf, '\0', sizeof(sndbuf));

	sprintf(sndbuf, "%sREADY%s", CDEV, CRLF);
	log_msg(MSGL_VGSM_INFO,"GSM_SMS_DEVICE_READY sent!!!\n");
	
	return at_msg_send(ACK_SEQ_NOTIFICATION, sndbuf, strlen(sndbuf));
}

int at_tx_sms_NotiIncomingMsg(void *data, int len)
{
	TRACE(MSGL_VGSM_INFO, "+CMT noti\n");
	TRACE(MSGL_VGSM_INFO, "%s", (char*)data);
	return at_msg_send(ACK_SEQ_NOTIFICATION, data, len);
}

int at_tx_sms_sendDeliverReport(void *data, int len)
{
	TRACE(MSGL_VGSM_INFO, "%%SCSDR noti unimplement\n");
	return 0;
//	return at_msg_send(ACK_SEQ_NOTIFICATION, data, len);
}

int at_tx_sms_sendMsg(void *data, int len)
{
	TRACE(MSGL_VGSM_INFO, "+CMGS resp\n");
	TRACE(MSGL_VGSM_INFO, "%s", (char*)data);
	at_msg_send(ACK_SEQ_RESPONSE, data, len);
	return at_gen_resp_send(AT_GEN_ERR_NO_ERROR);
}

int at_tx_sms_sendCBSMsg(void *data, int len)
{
	TRACE(MSGL_VGSM_INFO, "noti\n");
	return 0;
//	return at_msg_send(ACK_SEQ_NOTIFICATION, data, len);
}

int at_tx_sms_SCA(void *data, int len)
{
	TRACE(MSGL_VGSM_INFO, "resp\n");
	return 0;
//	return at_msg_send(ACK_SEQ_RESPONSE, data, len);
}


int at_tx_sms_cbconfig_resp(unsigned char enabled, unsigned char selected_id,
		unsigned char list_max, unsigned char list_count, unsigned short *list)
{
	unsigned char data[4 + 0x100*2];
	int n = 0;

	assert(n <= sizeof data);

	data[n++] = enabled;
	data[n++] = selected_id;
	data[n++] = list_max;
	data[n++] = list_count;

	memcpy(&data[n], list, list_count*2);
	n += list_count * 2;

	TRACE(MSGL_VGSM_INFO, "enabled(%d), selected_id(%d), list max(%d), list count(%d) \n"
		, enabled, selected_id, list_max, list_count);

	TRACE(MSGL_VGSM_INFO, "resp\n");
	return 0;
//	return at_msg_send(ACK_SEQ_RESPONSE, data, n);
}

int at_tx_sms_stored_msg_count_resp(void)
{
	TRACE(MSGL_VGSM_INFO, "+CPMS resp\n");
	
	char sndbuf[SEND_BUF_SIZE];
	memset(sndbuf, '\0', sizeof(sndbuf));

	sprintf(sndbuf, "%s0,0%s", CPMS, CRLF);

	TRACE(MSGL_VGSM_INFO, "%s", sndbuf);
	
	at_msg_send(ACK_SEQ_RESPONSE, sndbuf, strlen(sndbuf));
	return at_gen_resp_send(AT_GEN_ERR_NO_ERROR);
}

void sms_response_for_eventinjector()
{
	LXT_MESSAGE smsres_packet;

	// for sms receive check        
        TAPIMessageInit(&smsres_packet);
        smsres_packet.group  = GSM_SMS;                 // 0x04
        smsres_packet.action = GSM_SMS_RECEIVE_RES;     // 0x0C
        FuncServer->Cast(&GlobalPS, LXT_ID_CLIENT_EVENT_INJECTOR, &smsres_packet);
}

int at_tx_sms_deliver_report_noti(int result_status)
{
	TRACE(MSGL_VGSM_INFO, "+CNMA noti\n");
	int rc = 0;
	
	rc = at_gen_resp_send(AT_GEN_ERR_NO_ERROR);
	sms_response_for_eventinjector();
	return rc;
}

int at_tx_sms_param_count_resp(unsigned char mem_store, unsigned char record_count)
{
	unsigned char data[2];
	int n = 0;

	data[n++] = mem_store;
	data[n++] = record_count;

	TRACE(MSGL_VGSM_INFO, "mem_store(%d) record_count(%d) \n", mem_store, record_count);

	TRACE(MSGL_VGSM_INFO, "resp\n");
	return 0;
//	return at_msg_send(ACK_SEQ_RESPONSE, data, n);
}

int at_tx_sms_param_resp(unsigned char mem_store, unsigned char record_index, unsigned char record_len, unsigned char *record)
{
	unsigned char data[256];
	int n = 0;

	data[n++] = mem_store;
	data[n++] = record_index;
	data[n++] = record_len;

	memcpy(&data[n], record, record_len);
	n += record_len;

	TRACE(MSGL_VGSM_INFO, "mem_store(%d) record_index(%d), record_len(%d), record(%p) \n"
		, mem_store, record_index, record_len, record);

	TRACE(MSGL_VGSM_INFO, "resp\n");
	return 0;
//	return at_msg_send(ACK_SEQ_RESPONSE, data, n);
}

