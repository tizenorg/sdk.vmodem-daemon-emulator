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

/*  at_tx_call.c */

#include "at_send.h"
#include "at_gen_resp.h"
#include "at_tx_call.h"
#include "at_func.h"
#include "server_tx_call.h"
#include "db_ss.h"
#include "flight.h"
#include <vconf/vconf.h>
#include <vconf/vconf-keys.h>

int at_tx_call_incoming_noti(void *data, int len)
{
	TRACE(MSGL_VGSM_INFO, "RING noti\n");

	/* add block 130321 : flight mode on, rssi zero, call barring set */
	bool flightMode = false;
	bool icb = false; // incomming call barring
	int i = 0;
	int rssi = 5;
        call_barring_entry_t * resp_entry  = get_call_barring_entry();
	
	flightMode = is_flight_mode();

        if(!resp_entry)
		TRACE(MSGL_VGSM_INFO, "entry is NULL!!!\n");
        else {
                for(i=0; i<resp_entry[0].count; i++) {
                        TRACE(MSGL_VGSM_INFO,"i : %d,  type : %d\n", i, resp_entry[i].type);
			if(resp_entry[i].type == 4 && resp_entry[i].ss_mode == 3) { // 'All incoming calls' has set
				icb = true;
			}
                }
        }

	if(vconf_get_int(VCONFKEY_TELEPHONY_RSSI, &rssi)) {
		TRACE(MSGL_WARN, "vconf_get_int(%s) fail\n", VCONFKEY_TELEPHONY_RSSI);
	}

	if(rssi != 0 && icb == false && flightMode == false) {
		TRACE(MSGL_VGSM_INFO, "call OK, %d, %d, %d \n", rssi, icb, flightMode);
		char sndbuf[SEND_BUF_SIZE];
		memset(sndbuf, '\0', sizeof(sndbuf));

		sprintf(sndbuf, "%s%s", RING, CRLF);	
		at_msg_send(ACK_SEQ_NOTIFICATION, sndbuf, strlen(sndbuf));
		return at_tx_call_status_noti(data, strlen((char*)data));
	} else {
		TRACE(MSGL_VGSM_INFO, "cannot RING a call, %d, %d, %d \n", rssi, icb, flightMode); 
		return -1;
	}

}

int at_tx_call_connect_noti(void *data, int len)
{
	TRACE(MSGL_VGSM_INFO, "CONNECT noti\n");
	return server_tx_call_list_noti();
}

int at_tx_call_waiting_noti(void *data, int len)
{
	TRACE(MSGL_VGSM_INFO, "+CCWA noti\n");
	char sndbuf[SEND_BUF_SIZE];
	memset(sndbuf, '\0', sizeof(sndbuf));

        sprintf(sndbuf, "%s%s%s", CCWA, (char*)data, CRLF);
        return at_msg_send(ACK_SEQ_NOTIFICATION, sndbuf, strlen(sndbuf));
}

int at_tx_call_list_noti(void *data, int len)
{
	TRACE(MSGL_VGSM_INFO, "+CLCC noti\n");
	return at_msg_send(ACK_SEQ_NOTIFICATION, data, len);
}

int at_tx_call_list_resp(void *data, int len)
{
	TRACE(MSGL_VGSM_INFO, "+CLCC resp\n");
	return at_msg_send(ACK_SEQ_RESPONSE, data, len);
}

int at_tx_call_status_noti(void *data, int len)
{
	TRACE(MSGL_VGSM_INFO, "%%SCLCC noti\n");
	char sndbuf[SEND_BUF_SIZE];
	memset(sndbuf, '\0', sizeof(sndbuf));

	sprintf(sndbuf, "%s%s%s", SCLCC, (char*)data, CRLF);
	TRACE(MSGL_VGSM_INFO, "%s", sndbuf);
	return at_msg_send(ACK_SEQ_NOTIFICATION, sndbuf, strlen(sndbuf));
}

int at_tx_call_burst_dtmf_noti(void *data, int len)
{
	TRACE(MSGL_VGSM_INFO, "noti\n");
	return 0;
//	return at_msg_send(ACK_SEQ_NOTIFICATION, data, len);
}

int at_tx_call_line_id_resp(void *data, int len)
{
	TRACE(MSGL_VGSM_INFO, "resp\n");
	return 0;
//	return at_msg_send(ACK_SEQ_RESPONSE, data, len);
}

int at_tx_call_gen_resp(char* resp)
{
	TRACE(MSGL_VGSM_INFO, "call gen resp\n");
	return at_gen_resp_send(resp);
}

