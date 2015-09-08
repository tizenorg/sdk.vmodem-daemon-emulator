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

#include <stdlib.h>
#include <string.h>

#include "at_send.h"
#include "at_tx_phonebook.h"
#include "at_gen_resp.h"


int at_tx_sim_pb_get_storage_cnt_res(void* ptr_data, int data_len)
{
    TRACE(MSGL_VGSM_SIM, "res\n");
    return 0;

    //	return at_msg_send(ACK_SEQ_RESPONSE, ptr_data, data_len);
}


int at_tx_sim_pb_get_entry_info_res(void* ptr_data, int data_len)
{
    TRACE(MSGL_VGSM_SIM, "res\n");
    return 0;

    //	return at_msg_send(ACK_SEQ_RESPONSE, ptr_data, data_len);
}


int at_tx_sim_pb_get_storage_list_res(void* ptr_data, int data_len)
{
    TRACE(MSGL_VGSM_SIM, "res\n");
    return 0;

    //	return at_msg_send(ACK_SEQ_RESPONSE, ptr_data, data_len);
}


int at_tx_sim_pb_set_access_res(int error)
{
    TRACE(MSGL_VGSM_SIM, "res\n");
    return 0;
    /*
       char data[4];

       data[0] = AT_GSM_PB_CMD;
       data[1] = AT_GSM_PB_ACCESS;
       data[2] = (error & 0xFF);
       data[3] = ((error >> 8) & 0xFF);

       return at_msg_send(ACK_SEQ_RESPONSE, data, 4);
     */
}


int at_tx_sim_pb_get_access_res(void* ptr_data, int data_len)
{
    TRACE(MSGL_VGSM_SIM, "res\n");
    return 0;
    //	return at_msg_send(ACK_SEQ_RESPONSE, ptr_data, data_len);
}

