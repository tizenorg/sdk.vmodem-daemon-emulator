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

/*  oem_rx_network.c */

#include "logmsg.h"
#include "server_rx_network.h"
#include "oem_rx_network.h"
#include "oem_tx_network.h"

int  oem_rx_net_plmn_list_get(void)
{
    return server_rx_net_plmn_list_get();
}

int  oem_rx_net_reg_get(void)
{
    return server_rx_net_reg_get();
}

/* Update for public open */
/*  WIRELESS */
static const unsigned char default_identity[64] = {
    0x01, 0x01, 0x00, 0x10, 0x02, 0xd7, 0xa4, 0xb4, 0xc8, 0x2c,
    0x4e, 0xa7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x20, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x31, 0x31, 0x31, 0x31,
    0x31, 0x23 };

int  oem_rx_net_current_plmn_get(void)
{
    int ret;

    ret = server_rx_net_current_plmn_get();

    return ret;
}

int  oem_rx_net_plmn_selection_get(void)
{
    return server_rx_net_plmn_selection_get();
}

int oem_rx_net_plmn_selection_set(void * ptr_data, int data_len)
{
    return server_rx_net_plmn_selection_set(ptr_data, data_len);
}

int  oem_rx_net_band_set(void * ptr_data, int data_len)
{
    return server_rx_net_band_set(ptr_data, data_len);
}

int  oem_rx_net_service_domain_get(void)
{
    return server_rx_net_service_domain_get();
}

int  oem_rx_net_band_get(void)
{
    return server_rx_net_band_get();
}

int  oem_rx_net_mode_sel_get(char * api)
{
    return server_rx_net_mode_sel_get(api);
}

int  oem_rx_net_mode_sel_set(char * api)
{
    return server_rx_net_mode_sel_set(api);
}

int  oem_rx_net_pref_plmn_set(char * api)
{
    return server_rx_net_pref_plmn_set(api);
}

int  oem_rx_net_pref_plmn_get(char * api)
{
    return server_rx_net_pref_plmn_get(api);
}

