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

/*  at_gen_resp.c */

#include "at_send.h"
#include "at_gen_resp.h"
#include "at_func.h"

static char* g_recv_atmsg;
static int g_msgseq ;

void at_set_recv_cmd(char* atmsg)
{
    g_recv_atmsg = atmsg;
}

int get_at_msgseq(void)
{
    return g_msgseq;
}

int at_gen_resp_send(char* gen_resp_err)
{
    char sndbuf[SEND_BUF_SIZE];
    memset(sndbuf, '\0', sizeof(sndbuf));
    sprintf(sndbuf, "%s%s", gen_resp_err, CRLF);

    TRACE(MSGL_VGSM_INFO, "gen_resp_send = %s", sndbuf);
    return at_msg_send(ACK_SEQ_RESPONSE, sndbuf, strlen(sndbuf));
}

