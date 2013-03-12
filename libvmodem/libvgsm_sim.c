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

/***************************************************************************************
 *
 * Copyright (c) 2008 SAMSUNG Co. Ltd,
 * All rights reserved.
 *
 * File: vgsm_sim.c
 *
 * Release:
 *
 * Description: libvgsm interface.
 *
 * Revision History
 * JANUARY/17/2008   Kyeongchul Kim  Initial revision
 *
****************************************************************************************/

#include <string.h>
#include <stdlib.h>

#include "lxtutil.h"
#include "vgsm.h"


int vgsm_sim_init(LXT_HANDLE * handle)
{
	printf("[LIBVGSM-SIM] vgsm_sim_init\n");

	if( handle == NULL )
		return -1;

	return lxt_msg_send_message
		(
			handle->fd,
			GSM_SIM,
			GSM_SIM_INIT_REQ,
			0,
			NULL
		);
}

int vgsm_sim_reinit(LXT_HANDLE * handle)
{
	printf("[LIBVGSM-SIM] vgsm_sim_reinit\n");

	if( handle == NULL )
		return -1;

	return lxt_msg_send_message
		(
			handle->fd,
			GSM_SIM,
			GSM_SIM_REINIT_REQ,
			0,
			NULL
		);
}

int vgsm_sim_savedata(LXT_HANDLE * handle, _SIMD_t *simd_t )
{
	printf("[LIBVGSM-SIM] vgsm_sim_savedata\n");

	if( handle == NULL )
		return -1;

	return lxt_msg_send_message
		(
			handle->fd,
			GSM_SIM,
			GSM_SIMSAVE_REQ,	// 0xD5
			sizeof(_SIMD_t),
			simd_t
		);
}

int vgsm_get_siminfo_db(LXT_HANDLE * handle, int *list )
{
	printf("[LIBVGSM-SIM] vgsm_get_siminfo_db\n");

	if( handle == NULL )
		return -1;

	return lxt_msg_send_message
		(
			handle->fd,
			GSM_SIM,
			GSM_SIMINFO_REQ,	// 0xD1
			sizeof(int),
			list
		);
}

int vgsm_get_simdata_db(LXT_HANDLE * handle, int *list)
{
	printf("[LIBVGSM-SIM] vgsm_get_simdata_db\n");

	if( handle == NULL )
		return -1;

	return lxt_msg_send_message
		(
			handle->fd,
			GSM_SIM,
			GSM_SIMDATA_REQ,	// 0xD2
			sizeof(int),
			list	
		);
}

int vgsm_get_sim_db(LXT_HANDLE * handle)
{
	printf("[LIBVGSM-SIM] vgsm_get_sim_db\n");

	if( handle == NULL )
		return -1;

	return lxt_msg_send_message
		(
			handle->fd,
			GSM_SIM,
			GSM_SIM_GET_SIM_DB_REQ,
			0,
			NULL
		);
}

int vgsm_make_pb_db(LXT_HANDLE * handle)
{
	printf("[LIBVGSM-SIM] vgsm_make_pb_db\n");

	if( handle == NULL )
		return -1;

	return lxt_msg_send_message
		(
			handle->fd,
			GSM_SIM,
			GSM_SIM_MAKE_PB_DB_REQ,
			0,
			NULL
		);
}


int vgsm_get_all_pb_db(LXT_HANDLE * handle,  gsm_pb_storage_type_e_type storage_type)
{
	printf("[LIBVGSM-SIM] vgsm_get_pb_db\n");

	if( handle == NULL ||(int) storage_type < 0 || storage_type > GSM_PB_ST_GAS )
		return -1;

	PB_ *pb;
	pb = malloc(sizeof(PB_));
	if(!pb)
		return -1;
	memset(pb,0,sizeof(PB_));
	int ret;
	
	pb->db_st_type = storage_type;
	
	printf("vgsm_get_all_pb_db : %d ",pb->db_st_type);
	
    	ret =  lxt_msg_send_message
		(
			handle->fd,
			GSM_SIM,
			GSM_SIM_GET_ALL_PB_DB_REQ,
			sizeof(PB_),
			pb
		);

	if(pb) free(pb);
	
	return ret;
}


int vgsm_get_pb_db(LXT_HANDLE * handle, gsm_pb_storage_type_e_type storage_type, char* index_)
{
	printf("[LIBVGSM-SIM] vgsm_get_pb_db\n");

	if( handle == NULL ||(int)storage_type < 0 || storage_type > GSM_PB_ST_GAS )
		return -1;
	
	PB_ *pb;
	pb = malloc(sizeof(PB_));
	if(!pb)
		return -1;
	memset(pb,0,sizeof(PB_));
	int ret;
	
	pb->db_st_type	= storage_type;
	//pb->index		= atoi(index_);
	memcpy(pb->address,index_,strlen(index_));
	

	printf("vgsm_get_pb_db : %d , %s ",pb->db_st_type,pb->address);
	
	ret =  lxt_msg_send_message
		(
			handle->fd,
			GSM_SIM,
			GSM_SIM_GET_PB_DB_REQ,
			sizeof(PB_),
			pb
		);

	if(pb) free(pb);
	
	return ret;
}


int vgsm_add_pb_db(LXT_HANDLE * handle, gsm_pb_storage_type_e_type storage_type, char* name, char* number, char* index_)
{
	printf("[LIBVGSM-SIM] vgsm_add_pb_db\n");

	if( handle == NULL ||(int)storage_type < 0 || storage_type > GSM_PB_ST_GAS )
		return -1;

	PB_ *pb;
	int ret;
	
	pb = malloc(sizeof(PB_));
	if(!pb)
		return -1;
	memset(pb,0,sizeof(PB_));

	printf(" \n** 1 **");
	pb->db_st_type = storage_type;
	printf(" \n** 2 **");
	memcpy(pb->number,name,strlen(name));
	memcpy(pb->address,number,strlen(number));
	memcpy(pb->name,index_,strlen(index_));

	printf("vgsm_add_pb_db : %d , %s ,%s, %s",pb->db_st_type,pb->name,pb->number,pb->address);
	
	ret = lxt_msg_send_message
		(
			handle->fd,
			GSM_SIM,
			GSM_SIM_ADD_PB_DB_REQ,
			sizeof(PB_),
			pb
		);

	if(pb) free(pb);
	
	return ret;
}


int vgsm_delete_pb_db(LXT_HANDLE * handle, gsm_pb_storage_type_e_type storage_type, char* index_)
{
	printf("[LIBVGSM-SIM] vgsm_delete_pb_db\n");

	if( handle == NULL ||(int)storage_type < 0 || storage_type > GSM_PB_ST_GAS )
		return -1;
	
	PB_ *pb;
	pb = malloc(sizeof(PB_));
	if(!pb)
		return -1;
	int ret;
	memset(pb,0,sizeof(PB_));

	pb->db_st_type	= storage_type;
	//pb->index		= atoi(index_);
	memcpy(pb->name,index_,strlen(index_));
	
	printf("vgsm_delete_pb_db : %d , %s ",pb->db_st_type,pb->name);
	
	ret = lxt_msg_send_message
		(
			handle->fd,
			GSM_SIM,
			GSM_SIM_DELETE_PB_DB_REQ,
			sizeof(PB_),
			pb
		);

	if(pb) free(pb);
	
	return ret;
}


int vgsm_sim_status_set(LXT_HANDLE * handle, gsm_sec_status_type status)
{
	printf("[LIBVGSM-SIM] vgsm_sim_status_set\n");

	unsigned char data = (unsigned char )status;

	if( handle == NULL || (int)status < 0 || status > GSM_SEC_STATUS_NO_SIM)
		return -1;

	return lxt_msg_send_message
		(
			handle->fd,
			GSM_SIM,
			GSM_SIM_STATUS_SET,
			1,
			&data
		);
}


int vgsm_sim_change_password(LXT_HANDLE * handle,	gsm_sec_lock_e_type type, char *password, int length)
{
	printf("[LIBVGSM-SIM] vgsm_sim_change_password\n");

	if( handle == NULL )
		return -1;

	//if ( type != GSM_SEC_LOCK_TYPE_SC && type != GSM_SEC_LOCK_TYPE_SC2 )	// original before 090215
	//090215
	if ( type != GSM_SEC_LOCK_TYPE_SC && type != GSM_SEC_LOCK_TYPE_SC2 && type != GSM_SEC_LOCL_TYPE_PUK2 && type != GSM_SEC_LOCL_TYPE_PUK)
		return -1;

	int ret = -1;
	char *new_password=0;
	new_password = malloc(length + 1); // 1 <= type

  	if(!new_password){
		printf("memory alloc err");
		return ret;
	}

	new_password[0] = (char)type;
	memcpy(&new_password[1],password,length);

	printf(" [LIBVGSM-SIM]  input password =[%s] \n\n",&new_password[1]);

/*	return lxt_msg_send_message
		(
			handle->fd,
			GSM_SIM,
			GSM_SIM_CHANGE_PASS_SET,
			length+1,
			new_password
		); modify to free new_password and use ret */
	ret = lxt_msg_send_message(handle->fd, GSM_SIM, GSM_SIM_CHANGE_PASS_SET, length+1, new_password);

	if(new_password)
		free(new_password);

	return ret;
}


int vgsm_sim_facility_set(LXT_HANDLE * handle, gsm_sec_lock_e_type type, gsm_sec_lock_mode_e_type flag)
{
	printf("[LIBVGSM-SIM] vgsm_sim_facility_set\n");

	unsigned char data[2];
	data[0]= (unsigned char )type;
	data[1]= (unsigned char )flag;

	printf("[LIBVGSM-SIM] vgsm_sim_facility_set - type, flag = [%d],[%d]\n", type, flag);
/*
	if ( type != GSM_SEC_LOCK_TYPE_SC && type != GSM_SEC_LOCK_TYPE_FD )
		return -1;
*/
	if( handle == NULL )
		return -1;

	return lxt_msg_send_message
		(
			handle->fd,
			GSM_SIM,
			GSM_SIM_FACILITY_SET,
			2,
			data
		);
}

