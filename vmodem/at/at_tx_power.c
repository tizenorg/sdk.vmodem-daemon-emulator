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

/////////////////////////////////////////////////////////////////////
// at_tx_power.c

#include <stdlib.h>
#include <string.h>

#include "at_send.h"
#include "at_tx_power.h"
#include "logmsg.h"
#include "at_func.h"
#include "at_gen_resp.h"
#include "flight.h"

int at_tx_power_up_resp(void *data, int len)
{
	TRACE(MSGL_VGSM_INFO, "+CPAS resp\n");
	at_msg_send(ACK_SEQ_RESPONSE, data, len);
	return at_gen_resp_send(AT_GEN_ERR_NO_ERROR);
}

int at_tx_power_online_noti(void *data, int len)
{	
	TRACE(MSGL_VGSM_INFO, "%s noti\n", SCFUN);
	char sndbuf[SEND_BUF_SIZE];
	memset(sndbuf, '\0', sizeof(sndbuf));
	bool status = is_flight_mode();
	int value = 0;
	
	if(status)
		value = FLIGHT_MODE_ON;
	else
		value = FLIGHT_MODE_OFF;

	sprintf(sndbuf, "%s%d%s", SCFUN, value, CRLF);
	
	TRACE(MSGL_VGSM_INFO, "%s\n", sndbuf);

	return at_msg_send(ACK_SEQ_NOTIFICATION, sndbuf, strlen(sndbuf));
}

int at_tx_battery_status_noti(void *data, int len)
{
	TRACE(MSGL_VGSM_INFO, "noti");
	return at_msg_send(ACK_SEQ_NOTIFICATION, data, len);
}

