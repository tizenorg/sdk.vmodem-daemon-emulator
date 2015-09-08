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

#ifndef __DB_SECURITY_H__
#define __DB_SECURITY_H__

#define	SIM_SUCCESS	100
#define	SIM_FAIL		0

typedef enum {
    DB_PIN =0,
    DB_PIN2 =1,
    DB_PUK,
    DB_PUK2,
    DB_PIN_CNT,
    DB_PIN2_CNT,
    DB_PUK_CNT,
    DB_PUK2_CNT,
    DB_SIM_STATE,
    DB_F_PIN,
    DB_F_FDN,
    DB_LOCK_SC2_STATE
}Db_table_type;

typedef enum
{
    VGSM_SIMTypeLoad = 1,
    VGSM_SIMTypeget
}VGSM_SIMType;


int db_sim_sec_restore_callback(void * ref, int ncol, char ** cols, char ** name);


int db_sim_sec_create(void);


int db_sim_sec_add(void);


int db_sim_sec_remove(void);


int db_sim_sec_restore(VGSM_SIMType type);


int db_sim_sec_edit(int db_type,char *data,int data2);


int db_sim_info_create(void);


int db_sim_data_create(void);


int db_sim_info_add(int index, _SIMD_t *simd_t);


int db_sim_data_add(int index, _SIMD_t *simd_t);


int db_sim_info_restore(char* table);


int db_sim_data_restore(char* table);


int _sim_get_file_info(gsm_sec_sim_file_id_t file_id,/* in */ unsigned char* pfile_info, /* out */ int* file_info_len/* out */);
int _sim_get_file_data(gsm_sec_sim_file_id_t file_id,/* in */ unsigned char* pfile_data, /* out */ int* file_data_len/* out */);
int _sim_get_file_info_tmp(gsm_sec_sim_file_id_t file_id,/* in */ unsigned char* pfile_info, /* out */ int *file_info_len/* out */);
int _sim_get_file_data_tmp(gsm_sec_sim_file_id_t file_id,/* in */ unsigned char* pfile_data, /* out */ int* file_data_len/* out */);

#endif	/* __DB_SECURITY_H__ */

