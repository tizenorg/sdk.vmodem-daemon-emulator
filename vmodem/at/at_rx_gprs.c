/*
 *  telephony-emulator
 *
 * Copyright (c) 2000 - 2011 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact: 
 * SungMin Ha <sungmin82.ha@samsung.com>
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

#include "at_msg.h"
#include "oem_rx_gprs.h"
#include "at_rx_gprs.h"

int at_rx_gprs_SetPDPContext(char* atmsg)
{
	return oem_rx_gprs_SetPDPContext(atmsg, strlen(atmsg));
}

int at_rx_gprs_SetPSActDeact(char* atmsg)
{
	return oem_rx_gprs_SetPSActDeact(atmsg, strlen(atmsg));
}

int at_rx_gprs_DefaultGPRS(char* atmsg)
{
	TRACE(MSGL_VGSM_INFO, "\n");
	return 1;
}
