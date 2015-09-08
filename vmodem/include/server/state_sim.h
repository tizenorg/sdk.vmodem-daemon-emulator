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

#ifndef	_STATE_SIM_H_
#define _STATE_SIM_H_

#include <stdio.h>
#include "vgsm_sim.h"

//#define FALSE 0
/*--> 2008-09-29
#define SIM_DEBUG(frmt, args...)  {do { fprintf(stderr, "\n[VGSM-SIM] [%s:%d] " frmt ,  __func__,  __LINE__, ##args); } while (0);}
<--*/
/*++> 2008-09-29 */
#define SIM_DEBUG(frmt, args...)  fprintf(stderr, "\n[VGSM-SIM] [%s:%d] " frmt ,  __func__,  __LINE__, ##args)
/*<++*/

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

int server_sim_db_init();
int state_sim_verify_password(char *password,int type,int length);
char state_sim_status_mgr(int type);
int state_sim_set_sec_status(int status);
int state_sim_get_sec_status();
int state_sim_get_remain_cnt(int type);
int state_sim_change_password(char *password,int type,int length);
char * state_sim_get_password(int type);
void state_sim_reset_remain_cnt(int type);
int state_sim_get_facility(int type);
int _trans_db_sim_state_type(int type);
void state_sim_set_sec_db_info(SimSecurity *sim_sec,int  ncol);
void state_sim_set_cnt_zero(int type);
int  state_sim_get_sec2_status();
int state_sim_set_sec2_status(int status);
void state_sim_set_facility(int type,int flag);

void set_sim_sec_general_response_error( int error );
int get_sim_sec_general_response_error();


#endif /* _STATE_SIM_H_ */

