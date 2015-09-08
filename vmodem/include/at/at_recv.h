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

/*
/////////////////////////////////////////////////////////////////////
// at_recv.h
 */

#ifndef __AT_RECV_H__
#define __AT_RECV_H__

#include "at_msg.h"


void NOT_SUPPORTED_AT(char* atmsg);

void at_unimplemented(const char* api);

int at_recv_power(char* cmd, char* atmsg);

int at_recv_call(char* cmd, char* atmsg);

int at_recv_data(char* cmd, char* atmsg);

int at_recv_sms(char* cmd, char* atmsg);

int at_recv_security(char* cmd, char* atmsg);

int at_recv_phonebook(char* cmd, char* atmsg);

int at_recv_display(char* cmd, char* atmsg);

int at_recv_network(char* cmd, char* atmsg);

int at_recv_sound(char* cmd, char* atmsg);

int at_recv_misc(char* cmd, char* atmsg);

int at_recv_svcmode(char* cmd, char* atmsg);

int at_recv_ss(char* cmd, char* atmsg);

int at_recv_gprs(char* cmd, char* atmsg);

int at_recv_sat(char* cmd, char* atmsg);

int at_recv_cfg(char* cmd, char* atmsg);

int at_recv_sat(char* cmd, char* atmsg);
int at_recv_generalcmd(char* cmd, char* atmsg);

int at_recv_snd(char* cmd, char* atmsg);

int at_recv_sap(char* cmd, char* atmsg);

int at_recv_svc(char* cmd, char* atmsg);
int at_recv_gps(char* cmd, char* atmsg);

static inline void copy_unsigned_short(unsigned char *p, unsigned short val)
{
    p[0] = val & 0xff;
    p[1] = (val >> 8)&0xff;
}

extern int AT_VERSION;

#endif /* __AT_RECV_H__ */
