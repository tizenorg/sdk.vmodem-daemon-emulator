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

/*
/////////////////////////////////////////////////////////////////////
// at_rx_func.h
*/

#ifndef __AT_RX_FUNC_H__
#define __AT_RX_FUNC_H__

#include <stdlib.h>

#include "state.h"
#include "at_msg.h"
#include "gsmstate.h"
#include "misc.h"

/////////////////////////////////////////////////////////////////////
int RxPWR_DefaultPower(char* atmsg);
int at_rx_power_up_exec(char* atmsg);
int at_rx_online_exec(char* atmsg);

#endif /* __AT_RX_FUNC_H__ */

