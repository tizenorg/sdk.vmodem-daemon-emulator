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

#include "at_tx_security.h"
#include "oem_tx_security.h"
#include "server_common_security.h"

int oem_tx_sim_sec_get_imsi_res(void* ptr_data, int data_len)
{
    SIM_DEBUG("\n");
    return at_tx_sim_sec_get_imsi_res(ptr_data, data_len);
}


int oem_tx_sim_sec_set_pin_status_res(int error)
{
    SIM_DEBUG("error value=[%x]\n",error);
    return at_tx_sim_sec_set_pin_status_res(error);
}


int oem_tx_sim_sec_get_pin_status_res(void* ptr_data, int data_len)
{
    SIM_DEBUG("\n");
    return at_tx_sim_sec_get_pin_status_res(ptr_data, data_len);
}


int oem_tx_sim_sec_get_lock_info_res(void* ptr_data, int data_len)
{
    SIM_DEBUG("\n");
    return at_tx_sim_sec_get_lock_info_res(ptr_data, data_len);
}

int oem_tx_sim_sec_check_password_res(void* ptr_data)
{
    SIM_DEBUG("\n");
    return at_tx_sim_sec_check_password_res(ptr_data);
}

int oem_tx_sim_sec_change_password_res(void* ptr_data)
{
    SIM_DEBUG("\n");
    return at_tx_sim_sec_change_password_res(ptr_data);
}


int oem_tx_sim_sec_set_phone_lock_res(int error)
{
    SIM_DEBUG("error value=[%x]\n",error);
    return at_tx_sim_sec_set_phone_lock_res(error);
}


int oem_tx_sim_sec_get_phone_lock_res(void* ptr_data, int data_len)
{
    SIM_DEBUG("\n");
    return at_tx_sim_sec_get_phone_lock_res(ptr_data, data_len);
}


int oem_tx_sim_sec_set_rsim_access_res(int error)
{
    SIM_DEBUG("\n");
    return at_tx_sim_sec_set_rsim_access_res(error);
}


int oem_tx_sim_sec_card_type_noti(void* ptr_data, int data_len)
{
    SIM_DEBUG("\n");
    return at_tx_sim_sec_card_type_noti(ptr_data, data_len);
}


int oem_tx_sim_sec_pin_status_noti(void* ptr_data, int data_len)
{
    SIM_DEBUG("\n");
    return at_tx_sim_sec_pin_status_noti(ptr_data, data_len);
}

