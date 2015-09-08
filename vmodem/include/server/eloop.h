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

/*********************************************************************
 * $Id: eloop.h,v 1.1.1.1 2008-12-08 04:45:34 okdear Exp $
 *
 * Created : 20050329
 *
 *********************************************************************/

#ifndef __ELOOP_H__
#define __ELOOP_H__

#include <inttypes.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/poll.h>
#include <unistd.h>
#include <stdio.h>

#include "phoneserver.h"

typedef int (*EloopCallback)(void *data);
typedef int (*EloopInputCallback)(PhoneServer * ps, int fd, EloopCondition cond, void *data);


int eloop_add_timeout(unsigned int time_slice, EloopCallback cb, void *data);
void eloop_remove_timeout(int tag);
int eloop_add_input(PhoneServer * ps, int fd, EloopCondition cond, EloopInputCallback cb, void *data);
void eloop_remove_input(int tag);
void eloop_main(void);
void eloop_quit(void);



typedef struct _TimeoutStruct
{
    int64_t expire;
    EloopCallback cb;
    int tag;
    void *data;
    int time_slice;
    struct _TimeoutStruct *next;
    struct _TimeoutStruct *prev;
}
TimeoutStruct;

typedef struct _InOutStruct
{
    PhoneServer * ps;
    int fd;
    EloopCondition cond;
    EloopInputCallback cb;
    int tag;
    void *data;
    struct _InOutStruct *next;
    struct _InOutStruct *prev;
} InOutStruct;

#endif /* __ELOOP_H__ */


// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++end of file


