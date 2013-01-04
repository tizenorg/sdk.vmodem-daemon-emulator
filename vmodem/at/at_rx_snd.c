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
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "oem_rx_ss.h"
#include "at_msg.h"
#include "state.h"
#include "at_recv.h"

#include "at_tx_func.h"
#include "at_rx_snd.h"
#include "at_tx_snd.h"


static unsigned char g_bmute;
static unsigned char g_audio_path = 1;
static unsigned char g_clock_status; //0:Clock End, 1:Clock Start

int at_rx_snd_mute_control_get(char* atmsg)
{
	unsigned char data[1];
	int len = 0;

	data[len++] = g_bmute;
	
	return at_tx_snd_mute_control_resp(data, len);
}

int at_rx_snd_mute_control_set(char* atmsg)
{
/*	
	unsigned char bmute;

	bmute = atmsg[0];

	TRACE(MSGL_VGSM_INFO, "bmute = %d \n", bmute);

	g_bmute = bmute;
*/
	return 1;
}

int at_rx_snd_audio_path_set(char* atmsg)
{
/*	
	unsigned char audio_path;

	audio_path = atmsg[0];

	TRACE(MSGL_VGSM_INFO, "audio_path = %d \n", audio_path);

	g_audio_path = audio_path;
*/	
	return 1;
}

int at_rx_snd_clock_ctrl_exec(char* atmsg)
{
/*	
	unsigned char clock_status;

	clock_status = atmsg[0];

	TRACE(MSGL_VGSM_INFO, "clock_status = %d \n", clock_status);

	g_clock_status = clock_status;
*/	
	return 1;
}



