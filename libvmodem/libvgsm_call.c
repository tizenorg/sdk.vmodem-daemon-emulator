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


/*	++ 2008-08-13
	++ Name : vgsm_call_make
	++ Description : This function is called from on_injecting_call_mt_start_clicked() in Event Injector.
	++ 				 When "Dial" button of Event Injector is clicked, this function will work.
 */
int vgsm_call_make(LXT_HANDLE* handle, gsm_call_info_t callinfo)
{
    int nretn = 0;
    int	length = callinfo.numlen + 8;


    length = sizeof(gsm_call_info_t);

    if( handle == NULL )
	return -1;

    unsigned char * pdata = (unsigned char *) malloc(length);

    if (!pdata)
	return (-1);

    memset(pdata, 0, length);

    pdata[0] = (unsigned char)callinfo.calltype;
    pdata[1] = (unsigned char)callinfo.forwardedstatus;
    pdata[2] = (unsigned char)callinfo.clirstatus;

    /*	++ 2008-08-13
	++ Type of numlen is unsigned short.
	++ Type of pdata is char.
	++ I think that this is mis-casting.
     */
    pdata[3] = (unsigned char)callinfo.numlen;
    pdata[4] = (unsigned char)callinfo.numtype;

    pdata[5] = (unsigned char)callinfo.present_ind;
    //	pdata[6] = (unsigned char)callinfo.redirected_num;
    pdata[6] = (unsigned char)callinfo.no_cli_cause;

    if(callinfo.numlen > 0)
	memcpy(&pdata[7], callinfo.number, callinfo.numlen);

    nretn = lxt_msg_send_message(	handle->fd,
	    GSM_CALL,
	    GSM_CALL_MAKE_REQ,
	    length,
	    pdata);

    if (pdata)
	free(pdata);

    return nretn;
}

int vgsm_call_disconnect_cause(LXT_HANDLE* handle, int call_error_cause)
{
    //unsigned char data = (unsigned char )call_error_cause;
    unsigned char data[2];

    if( handle == NULL )
	return -1;

    data[0] = (unsigned short)call_error_cause & 0x00ff;
    data[1] = ((unsigned short)call_error_cause >> 8) & 0x00ff;

    printf("!!!!!!! value !!!!!!! disconnect : %x\n",GSM_EMULATOR_SET_SS_GENERAL_RESPONSE_ERROR);

    return lxt_msg_send_message(	handle->fd,
	    GSM_CALL,
	    GSM_EMULATOR_SET_SS_GENERAL_RESPONSE_ERROR,
	    2,
	    data);
}

int vgsm_call_hangup(LXT_HANDLE* handle, int call_id)
{
    unsigned char data = (unsigned char )call_id;

    if( handle == NULL )
	return -1;

    return lxt_msg_send_message(	handle->fd,
	    GSM_CALL,
	    GSM_CALL_HANGUP_REQ,
	    0x01,
	    &data);
}

int vgsm_call_answer(LXT_HANDLE* handle)
{
    if( handle == NULL )
	return -1;

    return lxt_msg_send_message(	handle->fd,
	    GSM_CALL,
	    GSM_CALL_ANSWER_REQ,
	    0x00,
	    NULL);
}

int vgsm_call_alert(LXT_HANDLE* handle)
{
    if( handle == NULL )
	return -1;

    return lxt_msg_send_message(	handle->fd,
	    GSM_CALL,
	    GSM_CALL_ALERT_IND,
	    0x00,
	    NULL);
}


