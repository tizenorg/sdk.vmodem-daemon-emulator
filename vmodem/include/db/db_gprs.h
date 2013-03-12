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

#ifndef __DB_GPRS_H__
#define __DB_GPRS_H__

#include "vgsm_gprs.h"


typedef struct _GprsDbEntry {
	char apn[255];
	char intf[10];
	GSM_GprsIPConfiguration ipconf;
} GprsDbEntry;


typedef struct _GprsConfList {
	int num;
	GprsDbEntry *pentry;
} GprsConfList;


#ifdef __cplusplus
extern "C"
{
#endif

// initialize the database
int vgsm_gprs_database_init(void);

// add a registration
int vgsm_gprs_database_add(GprsDbEntry *entry);

// remove a registration
int vgsm_gprs_database_remove(GprsDbEntry *entry);

// restore all previously save registrations
int vgsm_gprs_database_restore(void);

#ifdef __cplusplus
};
#endif


extern GprsDbEntry g_gprs_ipconf;
extern GprsConfList g_gprs_conflist;

#endif // __DB_GPRS_H__

