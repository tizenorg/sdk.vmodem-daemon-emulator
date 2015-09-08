/*
 *  telephony-emulator
 *
 * Copyright (c) 2000 - 2013 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact:
 * Sooyoung Ha <yoosah.ha@samsung.com>
 * YeongKyoon Lee <yeongkyoon.lee@samsung.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * Contributors:
 * - S-Core Co., Ltd
 *
 */

#include "vgsm.h"
#include "lxtutil.h"
#include "vgsm_phone.h"

int vgsm_power_modem_on_req_send(LXT_HANDLE * handle)
{
    if( handle == NULL )
	return -1;

    return lxt_msg_send_message(	handle->fd,
	    GSM_POWER,
	    GSM_POWER_MODEM_ON_REQ,
	    0,
	    NULL);
}

int vgsm_power_battery_status(LXT_HANDLE* handle, battery_status_type_e level)
{
    unsigned char data = (unsigned char )level;

    printf("level : %d, %x\n", data, data);

    if( handle == NULL )
	return -1;

    return lxt_msg_send_message(	handle->fd,
	    GSM_POWER,
	    GSM_POWER_BATTERY_STATUS_NOTI,
	    0x01,
	    &data);
}

