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


#ifndef __DB_PHONEBOOK_H__
#define __DB_PHONEBOOK_H__

#include <sqlite3.h>

#define PB_SUCCESS	100
#define PB_FAIL	0

typedef struct _PB PB;
struct _PB {
	int db_st_type;
//	int index;
	char name[30];
	char number[20];
	char address[30];
	int cnt;
};
	
typedef enum {
	DB_REQ=0,	
	DB_DC =1,
	DB_EN =2,
	DB_FDN,
	DB_LDN,
	DB_MC,
	DB_ME,
	DB_MT,
	DB_OWN,
	DB_RC,
	DB_ADN,
	DB_SDN
}PB_ST_type;



int db_sim_pb_restore_callback(void * ref, int ncol, char ** cols, char ** name);


int db_sim_pb_cnt_callback(void * ref, int ncol, char ** cols, char ** name);


int db_sim_pb_get_callback(void * ref, int ncol, char ** cols, char ** name);


int db_sim_pb_setted_storage_callback(void * ref, int ncol, char ** cols, char ** name);


int db_sim_pb_create(void);


int db_sim_pb_add(PB *pb);


int db_sim_pb_remove(PB *pb);


int db_sim_pb_cnt_mgr(sqlite3 * db,int pb_type,int action);


int db_sim_pb_restore(PB *pb);


int db_sim_pb_get(PB *pb);


int db_sim_pb_set_mgr(int pb_type,int action);


int db_sim_pb_get_check(PB *pb);


#endif

