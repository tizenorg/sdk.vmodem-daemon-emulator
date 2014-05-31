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

#include "at_tx_func.h"
#include "at_tx_misc.h"
#include "at_send.h"
#include "at_gen_resp.h"
#include "at_tx_misc.h"
#include "at_func.h"

int at_tx_misc_version(void *data, int len)
{
    TRACE(MSGL_VGSM_INFO, "+CGMR resp %s\n", (char*)data);
    at_msg_send(ACK_SEQ_RESPONSE, data, len);
    return at_gen_resp_send(AT_GEN_ERR_NO_ERROR);
}

static const int at_tx_max_serial_number_data = 0x20;

int at_tx_misc_me_serial(int sn_index, const void *sn, int len)
{
    TRACE(MSGL_VGSM_INFO, "+CGSN resp %s\n", (char*)sn);
    at_msg_send(ACK_SEQ_RESPONSE, (void*)sn, strlen(sn));
    return at_gen_resp_send(AT_GEN_ERR_NO_ERROR);
}

int at_tx_misc_time_info(char* atmsg, const struct tm *tm)
{
    unsigned char data[11];
    int n = 0;

    data[n++] = 0x01;	// time info from network
    data[n++] = 0x00;	// daylight time invalid
    data[n++] = tm->tm_year + 1900 - 2000;
    data[n++] = tm->tm_mon + 1;
    data[n++] = tm->tm_mday;
    data[n++] = tm->tm_hour;
    data[n++] = tm->tm_min;
    data[n++] = tm->tm_sec;
    data[n++] = 0;		// timezone offset ...
    data[n++] = 0;		// daylight adjust
    data[n++] = tm->tm_wday;

    assert(n <= sizeof data);

    char sndbuf[SEND_BUF_SIZE];
    memset(sndbuf, '\0', sizeof(sndbuf));

    sprintf(sndbuf, "%s%d,0,%d/%d/%d,%d:%d:%d%s", CTZR, data[0], data[2], data[3], data[4], data[5], data[6], data[7], CRLF);
    at_msg_send(ACK_SEQ_RESPONSE, sndbuf, strlen(sndbuf));
    return at_gen_resp_send(AT_GEN_ERR_NO_ERROR);
}

