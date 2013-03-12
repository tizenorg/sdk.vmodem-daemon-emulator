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

#ifndef _AT_RX_NETWORK_H_
#define _AT_RX_NETWORK_H_

#include "at_msg.h"

int at_rx_net_default(char* atmsg);
int at_rx_net_plmn_list_get(void);
int at_rx_net_current_plmn_get(void);
int at_rx_net_reg_get(void);
int at_rx_net_plmn_selection_get(void);

int at_rx_net_plmn_selection_set(char* atmsg);

int at_rx_net_band_set(char* atmsg);
int at_rx_net_band_get(char* atmsg);
int at_rx_net_service_domain_set(char* atmsg);
int at_rx_net_service_domain_get(char* atmsg);
int at_rx_net_mode_sel_set(char* atmsg);
int at_rx_net_mode_sel_get(char* atmsg);
int at_rx_net_pref_plmn_set(char* atmsg);
int at_rx_net_pref_plmn_get(char* atmsg);
int at_rx_net_subscriber_num_get(char* atmsg);

void at_cast_network_band_set(unsigned char band_mode, int band);

#endif

