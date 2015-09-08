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

#ifndef __DB_SS_H__
#define __DB_SS_H__


#include "vgsm_ss.h"


int vgsm_ss_database_init(void);
int vgsm_ss_database_add(ss_cmd_e_type type, void * entry);
int vgsm_ss_database_remove(ss_cmd_e_type type, int tel_class, int ss_type);
int vgsm_ss_database_restore(ss_cmd_e_type type);

void set_call_waiting_entry(call_waiting_entry_t * entry, int num);
call_waiting_entry_t * get_call_waiting_entry(void);
call_waiting_entry_t * find_call_waiting_entry(int tel_class);

void set_call_barring_entry(call_barring_entry_t * entry, int num);
call_barring_entry_t * get_call_barring_entry(void);
call_barring_entry_t *  find_call_barring_entry(int tel_class, int type);

void set_call_forwarding_entry(call_forwarding_entry_t * entry, int num);
call_forwarding_entry_t * get_call_forwarding_entry(void);
call_forwarding_entry_t * find_call_forwarding_entry(int tel_class, int type);

void set_outgoing_call_barring_state(int state);
int get_outgoing_call_barring_state(void);

/////////////////////
char* get_callbarring_pwd(void);
void set_callbarring_pwd(char* cb_pwd);
int increase_callbarring_pwd_fail_count(void);
void clear_callbarring_pwd_fail_count(void);

//090225
int  vgsm_ss_sqlite_cw_check_exist (int);

#endif

