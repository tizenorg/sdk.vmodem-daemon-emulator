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

#include "server_tx_sms.h"
#include "server_rx_sms.h"
#include "vgsm_phone.h"
#include "linuxtapi.h"
#include "state.h"
#include "phoneserver.h"
#include "oem_tx_sms.h"
#include "misc.h"
#include "at_recv.h"
#include "at_gen_resp.h"
#include "sms_util.h"
#include "flight.h"
#include <vconf/vconf.h>
#include <vconf/vconf-keys.h>

extern SmsAddressInfo g_sca;
extern int smsSentStatus;


int server_rx_sms_DR(void *ptr_data, int data_len)
{
	return -1;
}

static int sms_msg_hook_modify(void * data, int len)
{       
	int position;
	unsigned char *rawdata = (unsigned char *)data;
	TPDU_SMS_SUBMIT tpdu_submit;
	memset(&tpdu_submit, 0, sizeof(TPDU_SMS_SUBMIT));

	/* position is the hit of user data location */
	/* Mobile Oriented sms (1) */
	position = DecodeSmsSubmitTpdu(&tpdu_submit, len - 3, (char*)&rawdata[3], 1);

	TRACE(MSGL_VGSM_INFO, "DecodeSmsSubmitTpdu => [return %d], udhl:%d\n", position, rawdata[position]);

	TRACE(MSGL_VGSM_INFO, "[%d][%s]\n"
		, tpdu_submit.desAddr.dialnumLen, tpdu_submit.desAddr.diallingNum);
	TRACE(MSGL_VGSM_INFO, "[%d][%s]\n"
		, tpdu_submit.udl, tpdu_submit.userData);

	/* modify length */
	TRACE(MSGL_VGSM_INFO, "length is modified [%d] => [%d] \n"
		, rawdata[(position+2)], tpdu_submit.udl);
	rawdata[(position+2)] = (unsigned char)tpdu_submit.udl;

	/* modify user data */
	memset(&rawdata[position+3], 0, tpdu_submit.udl);
	memcpy(&rawdata[position+3], tpdu_submit.userData, tpdu_submit.udl);

	return 0;
}

int server_rx_sms_SendMsg(void *ptr_data, int data_len)
{
	unsigned char rawdata[0x100];
	int rawdata_len = 0, rssi = 5;
	bool flightMode = false;
	unsigned char data[data_len];

	LXT_MESSAGE	packet;
	TAPIMessageInit(&packet);
	
	memcpy(data, ptr_data, data_len);
	TRACE(MSGL_VGSM_INFO, "smsSentStatus:%d\n", smsSentStatus);
	// pick out each field
	unsigned char *to_event_injector = data;

	// data send to event injector includes
	packet.data   = to_event_injector;
	packet.group  = GSM_SMS;
	packet.action = GSM_SMS_SEND_MSG_REQ;
	packet.length = data_len;
	
	sms_msg_hook_modify(packet.data, packet.length);

	// check RSSI level and flight mode
	flightMode = is_flight_mode();	
	if(vconf_get_int(VCONFKEY_TELEPHONY_RSSI, &rssi))
	{
		TRACE(MSGL_WARN, "vconf_get_int(%s) fail\n", VCONFKEY_TELEPHONY_RSSI);
	}

	if(smsSentStatus == 0 && rssi != 0 && flightMode == false)
	{
		FuncServer->Cast(&GlobalPS, LXT_ID_CLIENT_EVENT_INJECTOR, &packet);

		// send a response
		EncodeSmsSubmitReportTpdu(rawdata, &rawdata_len);
		server_tx_sms_send_msg(rawdata, rawdata_len);
	}
	else
	{
		at_gen_resp_send(AT_CMS_ERR_UNKNOWN_ERR);
	}
	// 0x04 : Short Message Service Commands
	// 0x01 : Send the SMS-SUBMIT, SMS-COMMAND or SMS-SUBMIT-REPORT message
	set_current_state( STATE_ANY, 0x04, 0x01);

	return 1;
}

int get_sca( SmsAddressInfo *SCA )
{
	memcpy( SCA, &g_sca, sizeof( SmsAddressInfo ) );

	return 1;
}

int server_rx_sms_SCA(void *ptr_data, int data_len )
{
	int len = 0;
	unsigned char data[20];
	SmsAddressInfo SCA;

	get_sca(&SCA);

	len = SmsUtilEncodeAddrField(data, SCA.diallingNum, SCA.dialnumLen, 0x01, 0x01);

	return oem_tx_sms_SCA(data, len);
}

int server_rx_sms_SCA_set(char * api)
{
	at_unimplemented(api);
	return 1;
}

int server_rx_sms_cbsetconfig(char * api)
{
	at_unimplemented(api);
	return 1;
}

int server_rx_sms_ressvcoption(char * api)
{
	at_unimplemented(api);
	return 1;
}

int server_rx_sms_param_set(char* api)
{
	at_unimplemented(api);
	return 1;
}

int server_rx_sms_param_get(char* api)
{
	at_unimplemented(api);
	return 1;
}

int server_rx_sms_memstatus(char* api)
{
	at_unimplemented(api);
	return 1;
}

int server_rx_sms_stored_msg_status(char* api)
{
	at_unimplemented(api);
	return 1;
}

int server_rx_sms_param_count(char* api)
{
	at_unimplemented(api);
	return 1;
}

int server_rx_sms_mem_status(char* api)
{
	at_unimplemented(api);
	return 1;
}
