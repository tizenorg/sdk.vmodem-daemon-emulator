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

/*  oem_rx_call.c */

#include "server_rx_call.h"
#include "oem_rx_call.h"

int  oem_rx_call_list_get(void)
{
	return server_rx_call_list_get();
}

int oem_rx_call_answer_exec(void)
{
	return server_rx_call_answer_exec();
}

int oem_rx_call_originate_exec(void * ptr_data, int data_len)
{
	return server_rx_call_originate_exec(ptr_data, data_len);
}

int oem_rx_call_release_exec(void)
{
	return server_rx_call_release_exec();
}

int oem_rx_call_burst_dtmf_exec(void)
{
	return server_rx_call_burst_dtmf_exec();
}

int oem_rx_call_line_id_set(void * ptr_data, int data_len)
{
	return server_rx_call_line_id_set(ptr_data, data_len);
}

int oem_rx_call_line_id_get(void)
{
	return server_rx_call_line_id_get();
}



