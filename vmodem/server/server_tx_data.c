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

#include "oem_tx_data.h"
#include "server_tx_data.h"
#include "linuxtapi.h"
#include "at_msg.h"
#include "vgsm_data.h"
#include <string.h>

int server_tx_data_CallStatus(LXT_MESSAGE const* packet)
{
	int len = 4;
	unsigned char data[4];

	memset( data, 0, sizeof(unsigned char)*4 );

	if (packet && packet->data) {
		unsigned char *ptr = (unsigned char *)packet->data;

		data[0] = ptr[0];
		data[1] = ptr[1];
		data[2] = ptr[2];
		data[3] = ptr[3];
	}
	else {
		data[0] = 0x01; /* CID */
		data[1] = GSM_DATA_TE2_DATA_CALL_RESULT_OK; /* DATA_CALL_STATE */
		data[2] = GSM_DATA_CALL_REASON_NORMAL; /* DATA_CALL_STATE_RSN */
		data[3] = GSM_DATA_CALL_INTERNAL_PS_CALL; /* EXTERNAL_PS_CALL */
	}

	return oem_tx_data_CallStatus(data, len);
}

int server_tx_data_DataCallStatus(LXT_MESSAGE const* packet)
{
    int len = 4;
    unsigned char data[4];

    memset( data, 0, sizeof(unsigned char)*4 );

    if (packet && packet->data) {
	unsigned char *ptr = (unsigned char *)packet->data;

	data[0] = ptr[0];
	data[1] = ptr[1];
	data[2] = ptr[2];
	data[3] = ptr[3];
    }
    else {
	data[0] = 0x01; /* CID */
	data[1] = GSM_DATA_TE2_DATA_CALL_RESULT_OK; /* DATA_CALL_STATE */
	data[2] = GSM_DATA_CALL_REASON_NORMAL; /* DATA_CALL_STATE_RSN */
	data[3] = GSM_DATA_CALL_INTERNAL_PS_CALL; /* EXTERNAL_PS_CALL */
    }

    return oem_tx_data_DataCallStatus(data, len);
}
