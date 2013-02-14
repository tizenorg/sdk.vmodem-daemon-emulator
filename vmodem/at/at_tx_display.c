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

/*    at_tx_display.c      */

#include "at_send.h"
#include "at_gen_resp.h"
#include "at_tx_display.h"
#include "at_func.h"

int at_tx_display_icon_info_noti(void *data, int len)
{
	TRACE(MSGL_VGSM_INFO, "%s noti\n", CIEV);
	char* buf = (char*)data;
	char sndbuf1[SEND_BUF_SIZE], sndbuf2[SEND_BUF_SIZE];
	memset(sndbuf1, '\0', sizeof(sndbuf1));
	memset(sndbuf2, '\0', sizeof(sndbuf2));

	sprintf(sndbuf1, "%s%d,%d%s", CIEV, SIGNAL, buf[1], CRLF);	// rssi
	at_msg_send(ACK_SEQ_NOTIFICATION, sndbuf1, strlen(sndbuf1));

	sprintf(sndbuf2, "%s%d,%d%s", CIEV, BATTCHG, buf[3]/20, CRLF);	// battery
	TRACE(MSGL_VGSM_INFO, "%s, %s", sndbuf1, sndbuf2);
	return at_msg_send(ACK_SEQ_NOTIFICATION, sndbuf2, strlen(sndbuf2));
}

int at_tx_display_icon_info_resp(void *data, int len)
{
	TRACE(MSGL_VGSM_INFO, "resp\n");
	return at_msg_send(ACK_SEQ_RESPONSE, data, len);
}

