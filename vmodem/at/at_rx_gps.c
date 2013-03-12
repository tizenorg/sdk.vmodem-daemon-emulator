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

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <vgsm_phone.h>

#include "phoneserver.h"
#include "at_rx_func.h"
#include "at_send.h"
#include "at_recv.h"
#include "at_rx_gps.h"

static unsigned char g_gps_use_ssl;
static unsigned char g_gps_agps_mode;

int at_gps_rx_open(char* atmsg)
{
	TRACE(MSGL_VGSM_INFO, "\n");
	return 1;
}

int at_gps_rx_close(char* atmsg)
{
	TRACE(MSGL_VGSM_INFO, "\n");
	return 1;
}

int at_gps_rx_start(char* atmsg)
{
	TRACE(MSGL_VGSM_INFO, "\n");
	return 1;
}

int at_gps_xtra_set_xtra_enable(char* atmsg)
{
//	const unsigned char *p;

//	p = atmsg[0];

//	TRACE(MSGL_VGSM_INFO, "enabled = %d\n", p[0]);

	return 1;
}

static int at_gps_agps_set_ssl_get(char* atmsg)
{
	unsigned char data[1];
	int n = 0;

	/* build the response */
	data[n++] = g_gps_use_ssl;

//	return at_msg_send(ACK_SEQ_RESPONSE, data, n);
	return 0;
}

static int at_gps_agps_set_ssl_set(char* atmsg)
{
/*	const unsigned char *p = atmsg;

	if (strlen(atmsg) < 2)
		TRACE(MSGL_WARN, "doc says this message should have 2 bytes\n");

	g_gps_use_ssl = p[0];

	TRACE(MSGL_VGSM_INFO, "use_ssl = %d\n", g_gps_use_ssl);

	return at_gps_agps_set_ssl_get(atmsg);
*/	return 1;
}

int at_gps_agps_set_ssl(char* atmsg)
{
/*
	// set
	return at_gps_agps_set_ssl_set(atmsg);
	// get	
	return at_gps_agps_set_ssl_get(atmsg);
*/
	return 0;
}

static int at_gps_agps_mode_get(char* atmsg)
{
	unsigned char data[1];
	int n = 0;

	/* build the response */
	data[n++] = g_gps_agps_mode;

//	return at_msg_send(ACK_SEQ_RESPONSE, data, n);
	return 0;
}

static int at_gps_agps_mode_set(char* atmsg)
{
//	const unsigned char *p = atmsg[0];

//	g_gps_agps_mode = p[0];

//	return at_gps_agps_mode_get(atmsg);
	return 1;
}

int at_gps_agps_mode(char* atmsg)
{
/*
	// set
	return at_gps_agps_mode_set(atmsg);
	// get
	return at_gps_agps_mode_get(atmsg);
*/	
	return 0;
}
