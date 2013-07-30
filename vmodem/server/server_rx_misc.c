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

#include <string.h>
#include <assert.h>

#include "server_rx_misc.h"
#include "server_common_call.h"
#include "oem_tx_misc.h"
#include "at_func.h"

static const char *sw_version = "vmodem sw 0.1";
static const char *hw_version = "vmodem hw 0.1";
static const char *rf_cal_date = "2009/09/09";
static const char *product_code = "Protector";
static const char *model_id = "i8290";

#define MAX_VERSION_FIELD_LEN 0x20

int server_rx_misc_me_version(unsigned char flags)
{
	TRACE(MSGL_VGSM_INFO, "\n");

	char sndbuf[SEND_BUF_SIZE];
	memset(sndbuf, '\0', sizeof(sndbuf));
	sprintf(sndbuf, "%s%s,%s,%s,%s,%s%s", CGMR, sw_version, hw_version, rf_cal_date, product_code, model_id, CRLF);

/*
#define ADD_VERSION_STRING(mask, string) \
	if (mask) { \
		strcat(sndbuf, string);\
	}

	ADD_VERSION_STRING(0x01, sw_version)
	ADD_VERSION_STRING(0x02, hw_version)
	ADD_VERSION_STRING(0x04, rf_cal_date)
	ADD_VERSION_STRING(0x08, product_code)
	ADD_VERSION_STRING(0x10, model_id)

#undef ADD_VERSION_STRING

	// FIXME: add CDMA fields PRL and ERI

*/		
	return oem_tx_misc_version(sndbuf, strlen(sndbuf));
}

