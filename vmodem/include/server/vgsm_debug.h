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

#ifndef _VGSM_DEBUG_H_
#define _VGSM_DEBUG_H_

#include <stdio.h>
#include"logmsg.h"

#ifdef _VGSM_DEBUG
//#define VGSM_DEBUG(frmt, args...)  {do {printf("[VGSM] [%s:%d] " frmt ,  __func__,  __LINE__, ##args); } while (0) ;}
/*--> 2008-09-20
#define VGSM_DEBUG(frmt, args...)  {do {log_msg(MSGL_VGSM_INFO,frmt ,  ##args); } while (0) ;}
#define VGSM_ERR(frmt, args...)  {do {log_msg(MSGL_VGSM_WARN,frmt ,  ##args); } while (0) ;}
<--*/
/*++> 2008-09-20 */
#define VGSM_DEBUG(frmt, args...)	log_msg(MSGL_VGSM_INFO, frmt, ##args)
/*<++*/
#else
#define VGSM_DEBUG(frmt, args...)
#endif

#endif

