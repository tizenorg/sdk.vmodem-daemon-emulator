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

#ifndef	_SERVER_COMMON_CALL_H_
#define _SERVER_COMMON_CALL_H_
#include "vgsm_call.h"
#include "vgsm_ss.h"
#include "at_recv.h"

void set_call_list( gsm_call_direction_e_type dir, gsm_call_status_e_type stat, gsm_call_type_e_type type, char *number, int num_len );
void set_call_list_status( int call_id, int mpty, gsm_call_status_e_type stat );
int delete_call_list( int call_id );
void clear_call_list(void);
void get_call_list( gsm_call_list_t *list );
int release_all_held_call(void);
int release_all_active_call(void);
int drop_call( int call_id );
int is_prev_ss_state(ss_hold_act_state_e_type prev_state);
void set_ss_state(ss_hold_act_state_e_type state);
void set_rx_ss_state(void);
int swap_call(void);
int split_call( int call_id );
int join_call(void);
int check_call(void);
int get_active_call_count(void);
int get_hold_call_count(void);
int make_call_id(void);
void set_call_id( int call_id );
int get_call_id(void);
void set_call_type( int type );
int get_call_type(void);
void set_call_line_id( int line_id );
int get_call_line_id(void);
int check_call_error(void);
int get_callcount(void);
int valid_call_type(int call_type);

#endif // _SERVER_COMMON_CALL_H_

