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
// at_tx_ss.c

#include "at_send.h"
#include "at_tx_ss.h"
#include "at_gen_resp.h"
#include "at_func.h"

//#define 	ENCODE_TYPE_GSM			0x0F	 /**< the value of the GSM encoding fromat for ussd */

int at_tx_ss_ssinfo_noti(void *data, int len)
{
    TRACE(MSGL_VGSM_INFO, "unimplemented\n");
    return 0;
    //	return at_msg_send(ACK_SEQ_RESPONSE, data, len);
}

int at_tx_ss_cw_resp(void *data, int len)
{
    TRACE(MSGL_VGSM_INFO, "+CCWA resp\n");
    at_msg_send(ACK_SEQ_RESPONSE, data, len);
    return at_gen_resp_send(AT_GEN_ERR_NO_ERROR);
}

int at_tx_ss_cb_resp(void *data, int len)
{
    TRACE(MSGL_VGSM_INFO, "CLCK resp\n");
    char* cdata = (char*)data;
    char sndbuf[SEND_BUF_SIZE];
    memset(sndbuf, '\0', sizeof(sndbuf));

    sprintf(sndbuf, "%s%d,%d%s", CLCK, cdata[3], cdata[2], CRLF);
    TRACE(MSGL_VGSM_INFO, "%s", sndbuf);
    return at_msg_send(ACK_SEQ_RESPONSE, sndbuf, strlen(sndbuf));
}

int at_tx_ss_cf_resp(void *data, int len)
{
    TRACE(MSGL_VGSM_INFO, "CCFC resp\n");
    TRACE(MSGL_VGSM_INFO, "%s", (char*)data);
    at_msg_send(ACK_SEQ_RESPONSE, data, len);
    return at_gen_resp_send(AT_GEN_ERR_NO_ERROR);
}

int at_tx_ss_aoc_resp (void *data, int len)
{
    TRACE(MSGL_VGSM_INFO, "resp\n");
    return 0;
    //	return at_msg_send(ACK_SEQ_RESPONSE, data, len);
}

int at_tx_ss_aoc_noti (void *data, int len)
{
    TRACE(MSGL_VGSM_INFO, "noti\n");
    return 0;
    //	return at_msg_send(ACK_SEQ_NOTIFICATION, data, len);
}

int at_tx_ss_gen_resp(void *data)
{
    TRACE(MSGL_VGSM_INFO, "resp\n");
    return at_gen_resp_send((char*)data);
}
