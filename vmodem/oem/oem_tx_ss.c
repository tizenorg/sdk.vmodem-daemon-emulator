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

/*  oem_tx_ss.c */

#include "at_tx_ss.h"
#include "oem_tx_ss.h"

int oem_tx_ss_ssinfo_noti(void *data, int len)
{
	return at_tx_ss_ssinfo_noti(data, len);
}

int oem_tx_ss_cw_resp(void *data, int len)
{
	return at_tx_ss_cw_resp(data, len);
}

int oem_tx_ss_cb_resp(void *data, int len)
{
	return at_tx_ss_cb_resp(data, len);
}

int oem_tx_ss_cf_resp(void *data, int len)
{
	return at_tx_ss_cf_resp(data, len);
}

int oem_tx_ss_gen_resp(void *data)
{
	return at_tx_ss_gen_resp(data);
}

int oem_tx_ss_aoc_noti(void *data, int len)
{
	return at_tx_ss_aoc_noti(data, len);
}
