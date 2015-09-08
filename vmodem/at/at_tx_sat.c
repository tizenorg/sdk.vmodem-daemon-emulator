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
// at_tx_sat.c

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "at_send.h"
#include "at_tx_func.h"

static const int max_proactive_command_length = 0x100;

int TxSAT_ATGetProactiveCommand(GSM_SatProactiveCmd const* pProactiveCmd)
{
    unsigned char data[2 + max_proactive_command_length];
    int n = 0;

    TRACE(MSGL_VGSM_INFO, "\n");

    // this is always passed because (pProactiveCmd->length&0xff00) is always 0, so comment out
    //assert(!(pProactiveCmd->length&0xff00));

    data[n++] = (pProactiveCmd->length)&0xff;
    // this is always 0 because (pProactiveCmd->length >> 8) is 0, so fix like below
    //data[n++] = (pProactiveCmd->length >> 8)&0xff;
    data[n++] = 0;

    memcpy(&data[n], pProactiveCmd->cmd, pProactiveCmd->length);
    n += pProactiveCmd->length;

    assert(n <= sizeof data);

    TRACE(MSGL_VGSM_INFO, "\n");
    return 0;
    //	return at_msg_send(ACK_SEQ_RESPONSE, data, n);
}

