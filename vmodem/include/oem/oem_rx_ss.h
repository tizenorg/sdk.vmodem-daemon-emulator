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

#ifndef _OEM_RX_SS_
#define _OEM_RX_SS_

int oem_rx_ss_manage_call_set(char* ptr_data, int data_len);
int oem_rx_ss_cw_get(int tel_class);
int oem_rx_ss_cb_get(int tel_class, int cb_type);
int oem_rx_ss_cf_get(int tel_class, int cf_type);

int oem_rx_ss_cw_set(int tel_class, int ss_mode);
//int oem_rx_ss_cf_set(int ss_mode, int type, int tel_class,  int replyTime, int num_len, char *number, int num_type, int satype, char* subaddr);
int oem_rx_ss_cf_set(int ss_mode, int type, int tel_class,  int replyTime, int num_len, char *number, int num_type);
int oem_rx_ss_cb_set(int cb_type, int ss_mode, int tel_class, char *passwd);

int oem_rx_ss_cb_passwd_set(char* curr_passwd, char* new_passwd);

int oem_rx_ss_cli(char * api);

#endif

