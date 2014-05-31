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

#ifndef LOGMSG_H
#define LOGMSG_H
#ifdef	__cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>
#include <glib.h>
#include <glib/gstdio.h>
#include <libgen.h>

#include "fileio.h"

#ifndef _WIN32
#define __FILENAME__ basename(__FILE__)
#else
#define __FILENAME__ __FILE__
#endif

#define log_msg(level, fmt, args...)	real_log_msg(level, "%s:%d %s -> " fmt , __FILENAME__, __LINE__, __FUNCTION__, ##args)

#define MODNAME_MAX	64
#define MSGSIZE_MAX 	2048
#define MSGL_DEFAULT	0

#define MSGL_FATAL	0	/* system reboot, rmmod  */
#define	MSGL_ERROR	1	/* exit/terminate/abort */
#define MSGL_WARN	2	/* continue but problems */
#define	MSGL_INFO	3	/* hint, system status */
#define MSGL_DEBUG	4	/* for developement */
#define MSGL_DBG1	5	/* reservation for future */
#define MSGL_DBG2	6	/* reservation for future */
#define MSGL_FILE	9	/* save all above info as file */
#define MSGL_ALL	10	/* save all and all print */

    void real_log_msg(int level, const char *format, ...);
    void log_msg_init(char *mod, int level);
    int get_log_level(void);


#ifdef __cplusplus
}
#endif
#endif /* ifndef LOGMSG_H */
/**
 * vim:set tabstop=4 shiftwidth=4 foldmethod=marker wrap:
 *
 */

