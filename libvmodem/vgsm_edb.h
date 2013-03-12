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

#ifndef _vgsm_edb_h_
#define _vgsm_edb_h_

typedef struct
{
	int cnt;
	int index;
	int isend;
	int id;
	char fn[10];
	char mn[10];
	char sn[10];
	char path[50];
	char mobile[30];
	char email[30];
	char website[50];
	char st1[20];
	char st2[20];
	char city[20];
	char zip[20];
	char country[20];
} _Pb_edb; // 16+30+50+15+30+30+30+15+30 = 246

typedef struct
{
	int cnt;
	int index;
	int isend;
	int id;
	char fn[10];
	char mn[10];
	char sn[10];
	char mobile[30];
} _Tmp_edb; // 16 + 30 + 15 = 61
	
#endif // __vgsm_edb_h_
