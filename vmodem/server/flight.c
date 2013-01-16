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

// start here
#include <string.h>
#include "oem_rx_power.h"
#include "at_rx_func.h"
#include "flight.h"
#include <vconf/vconf.h>
#include <vconf/vconf-keys.h>

/* For Flight Mode */
int g_svc_type = VCONFKEY_TELEPHONY_SVCTYPE_3G; 

bool is_flight_mode(void)
{
	bool ret = false;
	int status;

	if(vconf_get_bool(VCONFKEY_TELEPHONY_FLIGHT_MODE, &status)) {
		TRACE(MSGL_WARN, "vconf_get_bool(%s) fail\n", VCONFKEY_TELEPHONY_FLIGHT_MODE);
		return ret; 
	}    

	if(status == 1){ 
		ret = true;
		TRACE(MSGL_VGSM_INFO, "============ Flight mode On  ==========\n");
	}else{
		ret = false;
		TRACE(MSGL_VGSM_INFO, "============ Flight mode Off ==========\n");
	}    

	TRACE(MSGL_VGSM_INFO, "============ Flight mode value: %d ==========\n", ret);

	return ret; 
}


