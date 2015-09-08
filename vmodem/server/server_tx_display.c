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
#include <assert.h>

#include "oem_tx_display.h"
#include "server_tx_display.h"
#include "server_common_call.h"
#include "vgsm_display.h"
#include "vgsm_debug.h"
#include "linuxtapi.h"
#include "logmsg.h"

extern unsigned char g_rssi_value;
extern unsigned char g_hdr_rssi_value;
extern unsigned char g_battery_level;

int server_tx_display_icon_info_resp(void)
{
    unsigned char data[10];
    int n = 0;

    /* default value (if needed, make global variable) */
    data[n++] = 0xff; /* ICON_TYPE */
    data[n++] = g_rssi_value; /* RSSI */
    data[n++] = g_hdr_rssi_value; /* RSSI */
    data[n++] = g_battery_level; /* BATTERY */

    return oem_tx_display_icon_info_resp(data, n);
}

int server_tx_display_rssi_info_noti( unsigned char icon_type, unsigned char rssi)
{
    unsigned char data[2];
    int n = 0;

    TRACE(MSGL_VGSM_INFO, "icon %02x rssi %02x\n", icon_type, rssi);
    g_rssi_value = rssi;

    data[n++] = icon_type;
    data[n++] = g_rssi_value;

    return oem_tx_display_rssi_info_noti(data, n);
}

int server_tx_display_icon_info_noti( unsigned char icon_type, unsigned char rssi, unsigned char bat, unsigned char act, unsigned char reg)
{
    unsigned char data[10];
    int n = 0;

    TRACE(MSGL_VGSM_INFO, "icon %02x rssi %02x battery %02x\n", icon_type,rssi, bat);
    g_rssi_value = rssi;
    g_battery_level = bat;

    data[n++] = icon_type;
    data[n++] = g_rssi_value;
    data[n++] = g_hdr_rssi_value;
    data[n++] = g_battery_level;

    return oem_tx_display_icon_info_noti(data, n);
}

