/*
 *  telephony-emulator
 *
 * Copyright (c) 2000 - 2011 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact: 
 * SungMin Ha <sungmin82.ha@samsung.com>
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

#ifndef	_OEM_TX_PHONEBOOK_H_
#define	_OEM_TX_PHONEBOOK_H_

int oem_tx_sim_pb_get_storage_cnt_res(void* ptr_data, int data_len);

int oem_tx_sim_pb_get_entry_info_res(void* ptr_data, int data_len);

int oem_tx_sim_pb_get_storage_list_res(void* ptr_data, int data_len);

int oem_tx_sim_pb_set_access_res(int error);

int oem_tx_sim_pb_get_access_res(void* ptr_data, int data_len);

#endif

