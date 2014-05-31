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

#include "server_rx_phonebook.h"
#include "server_common_security.h"
#include "oem_rx_phonebook.h"

int oem_rx_sim_pb_get_access_req(void* ptr_data, int data_len)
{
    SIM_DEBUG("\n");
    return server_rx_sim_pb_get_access_req(ptr_data, data_len);
}


int oem_rx_sim_pb_set_access_req(void* ptr_data, int data_len)
{
    SIM_DEBUG("\n");
    return server_rx_sim_pb_set_access_req(ptr_data, data_len);
}


int oem_rx_sim_pb_get_storage_cnt_req(void* ptr_data, int data_len)
{
    SIM_DEBUG("\n");
    return server_rx_sim_pb_get_storage_cnt_req(ptr_data, data_len);
}


int oem_rx_sim_pb_get_entry_info_req(void* ptr_data, int data_len)
{
    SIM_DEBUG("\n");
    return server_rx_sim_pb_get_entry_info_req(ptr_data, data_len);
}


int oem_rx_sim_pb_get_storage_list_req(void* ptr_data, int data_len)
{
    SIM_DEBUG("\n");
    return server_rx_sim_pb_get_storage_list_req(ptr_data, data_len);
}

/**************************************090226*************************************************/
int oem_rx_sim_pb_exec_access_req(void* ptr_data, int data_len)
{
    unsigned char *p = (unsigned char*)(ptr_data);
    int sim_mode = p[0];				// action type or mode

    SIM_DEBUG("\n");
    switch(sim_mode)
    {
	case 0x03:	//TAPI_SIM_PB_ACCESS_MODE_EDIT:
	    return server_rx_sim_pb_update_record("TelTapisimPBUpdateRecord");
	    break;
	case 0x02:	//TAPI_SIM_PB_ACCESS_MODE_DEL:
	    return server_rx_sim_pb_del_record("TelTapisimPBDeleteRecord");
	    break;

    }
    return 0;
}

int oem_rx_sim_pb_get_capability_info(void* ptr_data, int data_len)
{
    SIM_DEBUG("\n");
    return server_rx_sim_pb_get_capability_info("TelTapiSimPbGet3GCapabilityInfo");
}
/************************************** 090226*************************************************/
