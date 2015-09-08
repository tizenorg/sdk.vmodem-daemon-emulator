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

extern BOOL EncodeCB_GSM(CELLBROADCASTING cb_msg, char *rawdata, int *rawdata_len);

// sms group function +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/* modified sms msg click by cosmos in 20090213 */
int vgsm_sms_sendMessage(LXT_HANDLE* handle, SmsAddressInfo SCA, TPDU_SMS_DELIVER tpdu_deliver, BYTE pid)
{

    char data[500];
    int len = 0;

    if( handle == NULL )
	return -1;

    memset( data, 0, 500 );

    /* EncodeSmsDeliverTpdu argument added : pid, dcs */;
    EncodeSmsDeliverTpdu( SCA, tpdu_deliver, data, &len, pid);

    return lxt_msg_send_message(	handle->fd,
	    GSM_SMS,
	    GSM_SMS_SEND_MSG_REQ,
	    len,
	    data);

}

int vgsm_sms_sendAck(LXT_HANDLE* handle, int error)
{
    unsigned char data[2];

    if( handle == NULL )
	return -1;

    data[0] = (unsigned short)error & 0x00ff;
    data[1] = ((unsigned short)error >> 8) & 0x00ff;

    return lxt_msg_send_message(	handle->fd,
	    GSM_SMS,
	    GSM_SMS_SEND_ACK_REQ,
	    0x02,
	    &data);
}

int vgsm_cb_sendMessage(LXT_HANDLE* handle, CELLBROADCASTING cb_msg )
{
    char data[500];
    int len = 0;

    if( handle == NULL )
	return -1;

    memset( data, 0, 500 );

    // EncodeSmsDeliverTpdu( SCA, tpdu_deliver, data, &len );
    EncodeCB_GSM(cb_msg, data, &len);
    return lxt_msg_send_message(	handle->fd,
	    GSM_SMS,
	    GSM_SMS_INCOMING_CB_MSG_REQ,
	    len,
	    data);
}


