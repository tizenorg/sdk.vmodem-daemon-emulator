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

int oem_rx_sms_SendMsg(void * ptr_data, int data_len);

int oem_rx_sms_SCA(void * ptr_data, int data_len);

int oem_rx_sms_DR(void * ptr_data, int data_len);

int oem_rx_sms_cbsetconfig(char *api);

int oem_rx_sms_ressvcoption(char *api);

int oem_rx_sms_param_set(char *api);
int oem_rx_sms_param_get(char *api);

int oem_rx_sms_memstatus(char *api);

int oem_rx_sms_stored_msg_status(char *api);

int oem_rx_sms_param_count(char *api);

int oem_rx_sms_mem_status(char *api);

int oem_rx_sms_SCA_set(char* api);
