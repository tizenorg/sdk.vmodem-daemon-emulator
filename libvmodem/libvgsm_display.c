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

#include "vgsm.h"

int vgsm_display_icon_information(LXT_HANDLE* handle, disp_icon_info_t info)
{
	int nretn = 0;
	int	length = 5; 

	if( handle == NULL )
		return -1;

	unsigned char * pdata = (unsigned char *) malloc(length);

	if (!pdata)
		return (-1);

	memset(pdata, 0, length);

	pdata[0] = (unsigned char)info.icon_type;
	pdata[1] = (unsigned char)info.rssi;
	pdata[2] = (unsigned char)info.battery;
	pdata[3] = (unsigned char)info.act;
	pdata[4] = (unsigned char)info.reg_status;

	nretn = lxt_msg_send_message(	handle->fd,
									GSM_DISPLAY,
									GSM_DISPLAY_INDICATOR_NOTI,
									length,
									pdata);

	if (pdata)
		free(pdata);

	return nretn;
}

