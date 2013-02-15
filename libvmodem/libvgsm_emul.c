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

#include "vgsm.h"
#include <string.h>
#include <stdlib.h>
#include "lxtutil.h"
#include "vgsm_hdlc.h"
#include "libvgsm_debug.h"

/* Event Inject Releas Notify */

int vgsm_client_release_notify(LXT_HANDLE* handle)
{
	LIBVGSM_DEBUG("\n");
	if( handle == NULL )
		return -1;

	return lxt_msg_send_message(	handle->fd,
									GSM_CLIENT,
									GSM_CLIENT_RELEASE_NOTI,
									0,
									NULL);
}

int vgsm_set_call_general_response_error(LXT_HANDLE* handle, int error)
{
	unsigned char data[2];

	if( handle == NULL )
		return -1;

	data[0] = (unsigned short)error & 0x00ff;
	data[1] = ((unsigned short)error >> 8) & 0x00ff;

	return lxt_msg_send_message(	handle->fd,
									GSM_EMULATOR,
									GSM_EMULATOR_SET_CALL_GENERAL_RESPONSE_ERROR,
									2,
									data);
}

int vgsm_set_ss_general_response_error(LXT_HANDLE* handle, int error)
{
	unsigned char data[2];

	if( handle == NULL )
		return -1;

	data[0] = (unsigned short)error & 0x00ff;
	data[1] = ((unsigned short)error >> 8) & 0x00ff;

	return lxt_msg_send_message(	handle->fd,
									GSM_EMULATOR,
									GSM_EMULATOR_SET_SS_GENERAL_RESPONSE_ERROR,
									2,
									data);
}

int vgsm_set_call_status_error(LXT_HANDLE* handle, int type, int error)
{
    unsigned char response_type = (unsigned char)type;
    //unsigned char data = (unsigned char)error;
	unsigned char data[2];

    if( handle == NULL )
	return -1;

	data[0] = (unsigned short)error & 0x00ff;
	data[1] = ((unsigned short)error >> 8) & 0x00ff;

	//GSM_EMULATOR_SET_CALL_STATUS_ERROR -> response type
    return lxt_msg_send_message
           (
               handle->fd,
               GSM_EMULATOR,
               response_type,
               2,
               data
           );
}

int vgsm_set_sca(LXT_HANDLE* handle, SmsAddressInfo SCA)
{
    if( handle == NULL )
	return -1;

    return lxt_msg_send_message
           (
               handle->fd,
               GSM_EMULATOR,
               GSM_EMULATOR_SET_SCA,
               sizeof(SmsAddressInfo),
               &SCA
           );
}

int vgsm_convert_callbackinfo( void *data, TCallbackInfo *info )
{
	int len;
	unsigned char *rawdata = (unsigned char *)data;

	if( data == NULL )
		return -1;

	info->time = *(time_t *)rawdata;
	len = sizeof(time_t);

	info->rx = rawdata[len++];
	len += sizeof(unsigned short);
	return 1;
}

int vgsm_get_at_length_callbackinfo( TCallbackInfo info )
{
    return strlen(info.at.atmsg);
}

int vgsm_convert_atinfo( void *data, TAT *at )
{
	return 1;
}

int vgsm_get_at_length_atinfo( TAT at )
{
    return strlen(at.atmsg);
}

int vgsm_set_hdlc_mode(LXT_HANDLE *handle, int mode)
{
    unsigned char data = (unsigned char )mode;

    if( handle == NULL )
	return -1;

    return lxt_msg_send_message
           (
               handle->fd,
               GSM_EMULATOR,
               GSM_EMULATOR_HDLC_MODE,
               0x01,
               &data
           );
}

int vgsm_send_hdlc(LXT_HANDLE * handle, const char *hdlc, int hdlc_len)
{
    if( handle == NULL )
	return -1;

    return lxt_msg_send_message
           (
               handle->fd,
               GSM_EMULATOR,
               GSM_EMULATOR_HDLC_REQ,
               hdlc_len,
               (void *)hdlc
           );
}

int vgsm_clear_hdlc(LXT_HANDLE * handle)
{
    if( handle == NULL )
	return -1;

    FreeAllMultiNodeList();

    return lxt_msg_send_message
           (
               handle->fd,
               GSM_EMULATOR,
               GSM_EMULATOR_HDLC_CLEAR,
               0x00,
               NULL
           );
}


