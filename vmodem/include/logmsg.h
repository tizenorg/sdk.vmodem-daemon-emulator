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

/**
 * @file logmsg.h
 * @brief - header file
 *
 * @defgroup
 *
 * $Id: logmsg.h,v 1.1.1.1 2008-12-08 04:45:34 okdear Exp $
 *
 * <Revision History>
 * 		0000-00-00:
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

#define log_msg(level, fmt, args...)	real_log_msg(level, "%s:%d [%s] " fmt , __FILE__, __LINE__, __FUNCTION__, ##args)

#define MODNAME_MAX	64
#define MSGSIZE_MAX	2048

#define MSGL_DEFAULT  	1
#define MSGL_FATAL    	0       /* system reboot, rmmod  */
#define MSGL_VGSM_ERR   1       /* exit/terminate/abort */
#define MSGL_WARN     	2       /* continue but problems */
#define MSGL_VGSM_INFO  3       /* hint, system status */
#define MSGL_VGSM_CALL  4       /* information for vgsm */
#define MSGL_VGSM_SMS 	5       /* information for vgsm */
#define MSGL_VGSM_SIM 	6       /* information for vgsm */
#define MSGL_VGSM_SS  	7       /* information for vgsm */
#define MSGL_VGSM_NET 	8       /* information for vgsm */
#define MSGL_VGSM_GPRS  9       /* information for vgsm */
#define MSGL_VGSM_DISP  10      /* information for vgsm */
#define MSGL_VGSM_AT	11      /* error for vgsm */
#define MSGL_FILE       12      /* save all above info as file */

    void real_log_msg(int level, const char *format, ...) __attribute__ ((format (printf, 2, 3)));
    void ge_log_msg_init(char *mod);

#define TRACE(level, fmt, args...) log_msg(level, fmt, ## args)

#ifdef __cplusplus
}
#endif
#endif /* ifndef LOGMSG_H */
/**
 * vim:set tabstop=4 shiftwidth=4 foldmethod=marker wrap:
 *
 */

