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

	assert(!(pProactiveCmd->length&0xff00));

	data[n++] = (pProactiveCmd->length)&0xff;
	data[n++] = (pProactiveCmd->length >> 8)&0xff;
	memcpy(&data[n], pProactiveCmd->cmd, pProactiveCmd->length);
	n += pProactiveCmd->length;

	assert(n <= sizeof data);

	TRACE(MSGL_VGSM_INFO, "\n");
	return 0;
//	return at_msg_send(ACK_SEQ_RESPONSE, data, n);
}

