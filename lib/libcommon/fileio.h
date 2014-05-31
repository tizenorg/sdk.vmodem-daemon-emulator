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

#ifndef __SIMULATOR_FILEIO_H__
#define __SIMULATOR_FILEIO_H__

#include <unistd.h>
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <shlobj.h>
#endif
#include "process.h"

gchar *get_simulator_path(void);
gchar *get_bin_path(void);
gchar *get_path(void);
gchar *get_skin_path(void);
gchar *get_data_path(void);
gchar *get_kernel_path(void);
gchar *get_conf_path(void);
gchar *get_simulator_conf_filepath(void);
gchar *get_platform_conf_filepath(void);
char *search_target_name(char *directory_name);
char *find_exec_path_by_bash_profile(char *target_path);
char *find_ld_path_by_bash_profile(char *target_path);
gchar *change_path_to_slash(gchar *org_path);
//#define MIDBUF	64

#ifndef _WIN32
#define PATH_SEP "/"
#else
#define PATH_SEP "\\"
#endif

#endif //__SIMULATOR_FILEIO_H__
