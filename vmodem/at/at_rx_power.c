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

/////////////////////////////////////////////////////////////////////

// start here
#include <string.h>
#include "oem_rx_power.h"
#include "at_rx_func.h"

int at_RxPWR_DefaultPower(char* atmsg)
{
    TRACE(MSGL_VGSM_INFO, "\n");
    return 1;
}

int at_rx_power_up_exec(char* atmsg)
{
    TRACE(MSGL_VGSM_INFO, "=== MODEM POWER ON REQUEST is Recevied ===\n");

    return oem_rx_power_up_exec();
}

int at_rx_online_exec(char* atmsg)
{
    char* data = strchr(atmsg, '=');
    int value = atoi(data+1);
    TRACE(MSGL_VGSM_INFO, "=== MODEM ONLINE REQUEST is Recevied ===\n");
    if( value == 0 ){
	TRACE(MSGL_VGSM_INFO, "=== MODEM ONLINE OFF ===\n");
    }else if( value == 1 ){
	TRACE(MSGL_VGSM_INFO, "=== MODEM ONLINE ON ===\n");
    }else if( value == 2 ){
	TRACE(MSGL_VGSM_INFO, "=== MODEM ONLINE MAX ===\n");
    }else{
	TRACE(MSGL_VGSM_INFO, "=== MODEM ONLINE BUG ===\n");
    }

    return oem_rx_online_exec(value);
}


