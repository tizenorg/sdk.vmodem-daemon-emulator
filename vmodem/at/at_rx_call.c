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

/*	at_rx_call.c	*/

#include <assert.h>
#include "oem_rx_call.h"
#include "at_msg.h"
#include "at_recv.h"
#include "at_rx_call.h"

int at_rx_call_originate_exec(char* atmsg)
{
    return oem_rx_call_originate_exec(atmsg, strlen(atmsg));
}

int at_rx_call_list_get(char* atmsg)
{
    return oem_rx_call_list_get();
}

int at_rx_call_release_exec(char* atmsg)
{
    return oem_rx_call_release_exec();
}

int at_rx_call_answer_exec(char* atmsg)
{
    return oem_rx_call_answer_exec();
}

int at_rx_call_burst_dtmf_exec(char* atmsg)
{
    return oem_rx_call_burst_dtmf_exec();
}

int at_rx_call_line_id_set(char* atmsg)
{
    return oem_rx_call_line_id_set(atmsg, strlen(atmsg));
}

int at_rx_call_line_id_get(char* atmsg)
{
    return oem_rx_call_line_id_get( );
}


