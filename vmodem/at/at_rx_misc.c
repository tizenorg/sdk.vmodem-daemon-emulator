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
#include <time.h>

#include <vgsm_phone.h>

#include "phoneserver.h"
#include "at_rx_func.h"
#include "at_send.h"
#include "at_recv.h"
#include "oem_rx_misc.h"
#include "at_rx_misc.h"
#include "at_tx_misc.h"

extern GSM_StateMachine GlobalS;

int at_rx_misc_me_version(char* atmsg)
{
	unsigned char flags = 0x01;
	return oem_rx_misc_me_version(flags);
}

int at_rx_misc_time_info_get(char* atmsg)
{
	struct timeval tv;
	struct timezone tz;
	struct tm tm;

	gettimeofday(&tv, &tz);
	gmtime_r(&tv.tv_sec, &tm);

	return at_tx_misc_time_info(atmsg, &tm);
}

int at_RxMISC_DefaultMisc(char* atmsg)
{
	TRACE(MSGL_VGSM_INFO, "\n");
	return 1;
}

int at_rx_misc_me_serial(char* atmsg)
{
	int serial_num_index = 0;
	const char *serial_num_data;
	int serial_num_len;

	TRACE(MSGL_VGSM_INFO, "\n");

	// IMEI
	//serial_num_data = "3520990017614823\r\n"; // IMEISV
	serial_num_data = "990007429826041\r\n";
	serial_num_len = strlen(serial_num_data);

	return at_tx_misc_me_serial( serial_num_index, serial_num_data, serial_num_len );
}

