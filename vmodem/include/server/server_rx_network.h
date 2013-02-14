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

#ifndef _SERVER_RX_NETWORK_
#define _SERVER_RX_NETWORK_

int server_rx_net_plmn_list_get(void);
int server_rx_net_reg_get(void);
int server_rx_net_current_plmn_get(void);
int server_rx_net_plmn_selection_get(void);

int server_rx_net_plmn_selection_set(void *ptr_data, int data_len );
int server_rx_net_band_set(void *ptr_data, int data_len );
int server_rx_net_band_get(void);

int server_rx_net_service_domain_get(void);

int server_rx_net_mode_sel_get(char * api);
int server_rx_net_mode_sel_set(char * api);

int server_rx_net_pref_plmn_get(char * api);
int server_rx_net_pref_plmn_set(char * api);

#endif

