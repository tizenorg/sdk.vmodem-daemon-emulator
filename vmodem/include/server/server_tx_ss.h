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

#ifndef _SERVER_TX_SS_
#define _SERVER_TX_SS_

#include "vgsm_ss.h"
#include "misc.h"

int server_tx_ss_state( ss_state_e_type type );
int server_tx_ss_cli(unsigned char ss_call_type, unsigned char ss_present_indi, unsigned char ss_no_cli_cause );
void server_tx_ss_cw_set_req( LXT_MESSAGE *packet);
void server_tx_ss_ccfc_set_req( LXT_MESSAGE *packet);
void server_tx_ss_cb_set_req( LXT_MESSAGE *packet);
void server_tx_ss_aoc_get_req ( LXT_MESSAGE *packet);
void server_tx_ss_aoc_set_req ( LXT_MESSAGE *packet);
void server_tx_ss_cf_set_req ( LXT_MESSAGE *packet);
void server_tx_ss_ussd_set_req ( LXT_MESSAGE *packet);
void server_tx_ss_ussd_get_req ( LXT_MESSAGE *packet);

#endif

