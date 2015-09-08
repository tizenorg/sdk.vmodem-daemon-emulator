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

#include <stdlib.h>
#include <string.h>
#include "misc.h"
#include "vgsm_debug.h"
#include "vgsm_phone.h"
#include "phoneserver.h"
#include "logmsg.h"
#include "server_client.h"

void server_client_release_noti(void)
{
    log_msg(MSGL_VGSM_INFO," Cast\n" );
    LXT_MESSAGE packet;
    TAPIMessageInit(&packet);

    packet.data = NULL;
    packet.length = 0;
    packet.group  = GSM_CLIENT;
    packet.action = GSM_CLIENT_RELEASE_NOTI_ACK;

    FuncServer->Cast(&GlobalPS, LXT_ID_CLIENT_EVENT_INJECTOR, &packet);

    //return 1;
}

