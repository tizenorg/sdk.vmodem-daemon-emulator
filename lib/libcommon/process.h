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

#ifndef PROCESS_H
#define PROCESS_H
#ifdef	__cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <strings.h>
#include "logmsg.h"

int run_check_simulator(const char *filename);
int write_pidfile(const char *filename);
int remove_pidfile(const char *filename);

FILE * popen_err(const char *cmdstring, const char *type);
int pclose_err(FILE *fp);

//#define PID_DIR "/var/run/simulator/"
#ifndef _WIN32
#define PID_DIR "/.samsung_sdk/simulator/1/"
#else
#define PID_DIR "/samsung_sdk/simulator/1/"
#endif

#ifdef _WIN32

#include <windows.h>
#include <mbstring.h>

char * mbstok_r (char *string, const char *delim, char **save_ptr);

#endif

#ifdef __cplusplus
}
#endif
#endif
