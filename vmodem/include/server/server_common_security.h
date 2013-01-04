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

#ifndef	_SERVER_COMMON_SECURITY_H_
#define _SERVER_COMMON_SECURITY_H_

#include <stdio.h>
#include "vgsm_sim.h"
#include "logmsg.h"

#define SIM_DEBUG(frmt, args...)  log_msg(MSGL_VGSM_SIM, frmt, ##args);

typedef enum {
	SIM_STATE_READY		=	0,
	SIM_STATE_PIN_REQ		=	1,
	SIM_STATE_PUK_REQ	,
	SIM_STATE_PIN2_REQ,
	SIM_STATE_PUK2_REQ,
	SIM_STATE_FRIZEN,
	SIM_STATE_NO_SIM,
	SIM_STATE_FDN_STATE,
	SIM_STATE_PIN2_OK
} SIM_STATE;


int server_sim_db_init(void);

int server_simdata_init(void);

int server_sec_verify_password(char *password,int type,int length);

int server_sec_status_mgr(int type);

int server_sec_set_sec_status(int status);

int server_sec_get_sec_status(void);

int server_sec_get_remain_cnt(int type);

int server_sec_change_password(char *password,int type,int length);

char * server_sec_get_password(int type);

void server_sec_reset_remain_cnt(int type);

int server_sec_get_facility(int type);

int _trans_db_sim_state_type(int type);

int server_sec_set_sec_db_info(SimSecurity *sim_sec,int  ncol);

void server_sec_set_cnt_zero(int type);

int  server_sec_get_sec2_status(void);

int server_sec_set_sec2_status(int status);

void server_sec_set_facility(int type,int flag);

#endif /* _SERVER_COMMON_SECURITY_H_ */

