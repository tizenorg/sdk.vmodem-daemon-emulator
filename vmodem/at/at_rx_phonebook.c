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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "oem_rx_phonebook.h"
#include "at_rx_phonebook.h"
#include "at_recv.h"


int at_rx_sim_pb_default(char* atmsg)
{
	TRACE(MSGL_VGSM_INFO, "\n");
	return 1;
}


/*
여러개의 entry를 get 하게는 안하고 하나의 entry만 get 하게.. 지금 현재 모뎀 코드가 그래서..
Phonebook도 2G feature 로 구현.. 지금 현재 모뎀 코드가 그래서..
*/
int at_rx_sim_pb_access(char* atmsg)
{
	TRACE(MSGL_VGSM_INFO, "unimplemented!\n");
	return 1;
}


int at_rx_sim_pb_get_storage_cnt(char* atmsg)
{
	TRACE(MSGL_VGSM_INFO, "unimplemented!\n");
	return  1;
}


int at_rx_sim_pb_get_entry_info(char* atmsg)
{
	TRACE(MSGL_VGSM_INFO, "unimplemented!\n");
	return 0;
}


int at_rx_sim_pb_get_storage_list(char* atmsg)
{
	TRACE(MSGL_VGSM_INFO, "unimplemented!\n");
	return 0;
}

int at_rx_sim_pb_get_capability_info(char* atmsg)
{
	TRACE(MSGL_VGSM_INFO, "\n");

	return oem_rx_sim_pb_get_capability_info(atmsg, strlen(atmsg));
}

