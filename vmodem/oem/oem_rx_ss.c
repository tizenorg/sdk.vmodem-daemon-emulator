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

/*  oem_rx_ss.c */

#include "server_rx_call.h"
#include "server_rx_ss.h"
#include "oem_rx_ss.h"

#include "vgsm_debug.h"

int oem_rx_ss_manage_call_set(char* ptr_data, int data_len)
{
	return server_rx_ss_manage_call_set(ptr_data, data_len);
}

int oem_rx_ss_cw_get(int tel_class)
{
	VGSM_DEBUG("\n");
	return server_rx_ss_cw_get(tel_class);
}

int oem_rx_ss_cf_get(int tel_class, int cf_type)
{
	VGSM_DEBUG("\n");
	return server_rx_ss_cf_get(tel_class, cf_type);
}

int oem_rx_ss_cb_get(int tel_class, int cb_type)
{
	VGSM_DEBUG("\n");
	return server_rx_ss_cb_get(tel_class, cb_type);
}

int oem_rx_ss_cw_set(int tel_class, int ss_mode)
{
	VGSM_DEBUG("\n");
	return server_rx_ss_cw_set(tel_class, ss_mode);
}

/**************************************090226*************************************************/
int oem_rx_ss_cli(char * api)
{
	return server_rx_ss_cli(api);
}
/**************************************090226*************************************************/


//int oem_rx_ss_cf_set(int ss_mode, int type, int tel_class,  int replyTime, int num_len, char *number, int num_type, int satype, char* subaddr)
int oem_rx_ss_cf_set(int ss_mode, int type, int tel_class,  int replyTime, int num_len, char *number, int num_type)
{
	VGSM_DEBUG("\n");
	return server_rx_ss_cf_set(ss_mode, type, tel_class,  replyTime, num_len, number, num_type);
}

int oem_rx_ss_cb_set(int cb_type, int ss_mode, int tel_class, char *passwd)
{
	VGSM_DEBUG("\n");
	return server_rx_ss_cb_set(cb_type, ss_mode, tel_class, passwd);
}

int oem_rx_ss_cb_passwd_set(char* curr_passwd, char* new_passwd)
{
	VGSM_DEBUG("\n");
	return server_rx_ss_cb_passwd_set(curr_passwd, new_passwd);
}


