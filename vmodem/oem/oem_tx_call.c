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

/*  oem_tx_call.c */

#include "oem_tx_call.h"
#include "at_tx_call.h"

int oem_tx_call_incoming_noti(void *data, int len)
{
    return at_tx_call_incoming_noti(data, len);	
}

int oem_tx_call_waiting_noti(void *data, int len)
{
    return at_tx_call_waiting_noti(data, len);
}

int oem_tx_call_list_noti(void *data, int len)
{	
    return at_tx_call_list_noti(data, len);
}

int oem_tx_call_list_resp(void *data, int len)
{
    return at_tx_call_list_resp(data, len);
}

int oem_tx_call_status_noti(void *data, int len)
{
    return at_tx_call_status_noti(data, len);
}

int oem_tx_call_burst_dtmf_noti(void *data, int len)
{
    return at_tx_call_burst_dtmf_noti(data, len);

}

int oem_tx_call_line_id_resp(void *data, int len)
{
    return at_tx_call_line_id_resp(data, len);

}

int oem_tx_call_gen_resp(char* resp)
{
    return at_tx_call_gen_resp(resp);
}

