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

#ifndef	_OEM_RX_PHONEBOOK_H_
#define	_OEM_RX_PHONEBOOK_H_

int oem_rx_sim_pb_get_access_req(void* ptr_data, int data_len);

int oem_rx_sim_pb_set_access_req(void* ptr_data, int data_len);

int oem_rx_sim_pb_get_storage_cnt_req(void* ptr_data, int data_len);

int oem_rx_sim_pb_get_entry_info_req(void* ptr_data, int data_len);

int oem_rx_sim_pb_get_storage_list_req(void* ptr_data, int data_len);

int oem_rx_sim_pb_exec_access_req(void* ptr_data, int data_len);

int oem_rx_sim_pb_get_capability_info(void* ptr_data, int data_len);

#endif
