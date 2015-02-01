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

#include <assert.h>

#include "oem_tx_sms.h"
#include "server_tx_sms.h"
#include "server_common_call.h"
#include "linuxtapi.h"
#include "at_msg.h"
#include "sms_util.h"
#include "logmsg.h"
#include "at_func.h"
#include <string.h>
#include <stdio.h>

extern int smsSentStatus;

void	server_tx_sms_deviceReady(LXT_MESSAGE const* packet)
{
    oem_tx_sms_deviceReady(NULL, 0);
}

static int sms_msg_hook_modify(void * data, int len)
{
    int position;
    unsigned char *rawdata = (unsigned char *)data;
    TPDU_SMS_SUBMIT tpdu_submit;
    TmDateTime SCTimeStamp;
    time_t c_time;
    struct tm *tmTime;
    unsigned char timeStamp[20];

    memset(&tpdu_submit, 0, sizeof(TPDU_SMS_SUBMIT));
    TRACE(MSGL_VGSM_INFO, "sms_msg_hook_modify start with len(%d) \n", len);

    /* Mobile Oriented sms (0) */
    /* return position : start point of user data */
    position = DecodeSmsSubmitTpdu(&tpdu_submit, len, (char *)rawdata, 0);
    TRACE(MSGL_VGSM_INFO, "DecodeSmsSubmitTpdu => [return %d]\n", position);

    TRACE(MSGL_VGSM_INFO, "[%d][%s]\n"
	    , tpdu_submit.desAddr.dialnumLen, tpdu_submit.desAddr.diallingNum);
    TRACE(MSGL_VGSM_INFO, "[%d][%s]\n"
	    , tpdu_submit.udl, tpdu_submit.userData);

    /* decode time stamp */
    position = DecodeSmsSubmitTpdu(&tpdu_submit, len, (char *)rawdata, 0);
    /* time value is located in (start point of user data) -1  */
    SmsUtilDecodeTimeStamp( &rawdata[(position-1)], &SCTimeStamp );
    TRACE(MSGL_VGSM_INFO, "Event Injector time [%d-%d-%d, %d:%d:%d]\n"
	    , SCTimeStamp.year, SCTimeStamp.month, SCTimeStamp.day
	    , SCTimeStamp.hour, SCTimeStamp.minute, SCTimeStamp.second);

    /*TP-SCTS*/
    c_time = time((time_t *)NULL);
    /* setting utc time => check ? */
    tmTime = gmtime(&c_time);
    SCTimeStamp.year = tmTime->tm_year+1900;
    SCTimeStamp.month = tmTime->tm_mon+1;
    SCTimeStamp.day = tmTime->tm_mday;
    SCTimeStamp.hour = tmTime->tm_hour;
    SCTimeStamp.minute = tmTime->tm_min;
    SCTimeStamp.second = tmTime->tm_sec;

    SmsUtilEncodeTimeStamp(&SCTimeStamp, timeStamp);

    TRACE(MSGL_VGSM_INFO, "Modify time [%d-%d-%d, %d:%d:%d]\n"
	    , SCTimeStamp.year, SCTimeStamp.month, SCTimeStamp.day
	    , SCTimeStamp.hour, SCTimeStamp.minute, SCTimeStamp.second);

    /* modify time area*/
    /* time value is located in (start point of user data) -1  */
    memcpy(&rawdata[(position-1)], timeStamp, 7);

    return 0;
}

int server_tx_sms_notiIncomingMsg(LXT_MESSAGE const* packet)
{
    int i = 0, len = packet->length;
    unsigned char rawdata[0x105], *data = (unsigned char *)packet->data;

    TRACE(MSGL_VGSM_INFO, "\n");

    log_msg(MSGL_VGSM_INFO,"[len: %02x]\n", len);
    for(i=0; i<len; i=i+2)
	log_msg(MSGL_VGSM_INFO,"[%02x] [%02x]\n", data[i], data[i+1]);
    log_msg(MSGL_VGSM_INFO,"\n");

    assert(len < 0x100);

    int n = 0;
    memset(rawdata, '\0', 0x105);
    sprintf((char*)rawdata, "%s%d", CMT, len);
    strcat((char*)rawdata, CRLF);
    n += strlen((char*)rawdata);

    memcpy(&rawdata[n], data, len); // DATA
    sms_msg_hook_modify(&rawdata[n], len);
    n += len;

    return oem_tx_sms_NotiIncomingMsg(rawdata, n);
}

int server_tx_sms_send_msg(unsigned char *data, unsigned char length)
{
    char rawdata[0x100 + 5];
    int n = 0;

    TRACE(MSGL_VGSM_INFO, "\n");

    sprintf(rawdata, "%s1%s", CMGS, CRLF);
    n += strlen(rawdata);

    return oem_tx_sms_sendMsg(rawdata, n);
}

// FIXME: check caller
int server_tx_sms_ReceiveAckMsg(LXT_MESSAGE const* packet)
{
    assert(packet->data);
    assert(packet->length >= 2);

    unsigned char *report = malloc(packet->length + 1);
    int n = 0;

    report[n++] = 0x02; // GSM

    memcpy(&report[n], packet->data, packet->length);
    n += packet->length;

    TRACE(MSGL_VGSM_INFO, "\n");

    int r = oem_tx_sms_sendDeliverReport(report, n);
    free(report);
    return r;
}


int server_tx_sms_NotiIncomingCBMsg(LXT_MESSAGE const * packet)
{
    TRACE(MSGL_VGSM_INFO, "\n");

    unsigned char * data = (unsigned char *)packet->data;
    int len = packet->length;

    unsigned char rawdata[510];
    rawdata[0] = 0x02; // GSM
    rawdata[1] = len;
    memcpy(&rawdata[2], data, len);

    return oem_tx_sms_sendCBSMsg(rawdata, len + 2);
}
