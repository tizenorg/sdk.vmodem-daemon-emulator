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

/***************************************************************************************
 *
 * File: vgsm_edb.c
 *
 * Release:
 *
 * Description: libvgsm interface.
 *
 * Revision History
 * September/08/2009 Initial revision
 *
 ****************************************************************************************/

#include <string.h>
#include <stdlib.h>

#include "lxtutil.h"
#include "vgsm.h"
#include "vgsm_edb.h"

int vgsm_get_pb_edb(LXT_HANDLE * handle)
{
    if( handle == NULL )
	return -1;

    printf("[LIBVGSM-SIM] vgsm_get_pb_edb\n");

    _Pb_edb *pb_edb;
    pb_edb = malloc(sizeof(_Pb_edb));
    if(!pb_edb)
	return -1;
    memset(pb_edb,0,sizeof(_Pb_edb));
    int ret;

    ret =  lxt_msg_send_message
	(
	 handle->fd,
	 FUNC_EDB,
	 FUNC_GET_PB_DB_REQ,
	 sizeof(_Pb_edb),
	 pb_edb
	);

    if(pb_edb) free(pb_edb);

    return ret;
}


int vgsm_add_pb_edb(LXT_HANDLE * handle, _Pb_edb* pb_edb)
{
    printf("[LIBVGSM-SIM] vgsm_add_pb_db\n");
    int ret;

    if( handle == NULL )
	return -1;

    ret = lxt_msg_send_message
	(
	 handle->fd,
	 FUNC_EDB,
	 FUNC_ADD_PB_DB_REQ,
	 sizeof(*pb_edb),					// size of _Pb_edb structure
	 pb_edb
	);

    //	if(pb_edb) free(pb_edb);

    return ret;
}

int vgsm_del_pb_edb(LXT_HANDLE * handle, char* text)
{
    printf("[LIBVGSM-SIM] vgsm_delete_pb_db\n");

    if( handle == NULL )
	return -1;

    int ret = lxt_msg_send_message
	(
	 handle->fd,
	 FUNC_EDB,
	 FUNC_DEL_PB_DB_REQ,
	 strlen(text),
	 text	
	);

    return ret;
}

int vgsm_get_one_pb_edb(LXT_HANDLE * handle, char* text)
{
    printf("[LIBVGSM-SIM] vgsm_get_one_item_pb_db\n");

    if( handle == NULL )
	return -1;

    int ret = lxt_msg_send_message
	(
	 handle->fd,
	 FUNC_EDB,
	 FUNC_GET_ONE_PB_DB_REQ,
	 strlen(text),
	 text	
	);

    return ret;
}

