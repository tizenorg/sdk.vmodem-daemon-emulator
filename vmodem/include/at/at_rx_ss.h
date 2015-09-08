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

#ifndef _AT_RX_SS_H_
#define _AT_RX_SS_H_

#include "at_msg.h"

int at_rx_ss_default(char* atmsg);
int at_rx_ss_manage_call_set(char* atmsg);

int at_rx_ss_cw_get(char* atmsg);			// response
int at_rx_ss_cw_set(char* atmsg);
int at_rx_ss_cf_get(char* atmsg);
int at_rx_ss_cf_set(char* atmsg);
int at_rx_ss_cb_get(char* atmsg);
int at_rx_ss_cb_set(char* atmsg);
int at_rx_ss_cb_passwd_set(char* atmsg);
int at_rx_ss_aoc_set(char* atmsg);
int at_rx_ss_aoc_get(char* atmsg);
int at_rx_ss_cli(void);

#endif

