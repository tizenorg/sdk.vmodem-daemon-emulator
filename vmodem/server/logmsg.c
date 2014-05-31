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
 * Copyright (C) 2007 All rights reserved.
 *
 * @file logmsg.c
 * @brief - implementation file
 *
 * @defgroup
 *
 * $Id: logmsg.c,v 1.1.1.1 2008-12-08 04:45:34 okdear Exp $
 *
 * <Revision History>
 *		0000-00-00:
 */
#include <unistd.h>
#include <assert.h>
#include "logmsg.h"

static const char *msglevel[] = {
    "[fatal]",
    "[ err]",
    "[warn]",
    "[info]",
    "[call]",
    "[ sms]",
    "[ sim]",
    "[ ss ]",
    "[ net]",
    "[gprs]",
    "[disp]",
    "[ at ]",
    "[func]",
    "[ all ]",
};

static int msg_level = MSGL_DEFAULT;
static char module[MODNAME_MAX] = { 0, };
static const char logfile[] = "/tmp/vgsm.log";

/**
 * @brief
 *
 * @param level
 * @param format
 * @param ...
 */
void real_log_msg(int level, const char *format, ...)
{
    char tmp[MSGSIZE_MAX] = { 0, };
    char txt[MSGSIZE_MAX] = { 0, };
    char timeinfo[256] = { 0, };
    FILE *fp;
    struct tm *tm_time;
    struct timeval tval;

    va_list va;

    if (level > msg_level )
	return;

    va_start(va, format);
    vsnprintf(tmp, MSGSIZE_MAX, format, va);
    va_end(va);

    tmp[MSGSIZE_MAX - 2] = '\n';
    tmp[MSGSIZE_MAX - 1] = 0;

    if (msg_level == MSGL_FILE) {
	gettimeofday(&tval, NULL);
	tm_time = gmtime(&(tval.tv_sec));
	strftime(timeinfo, sizeof(timeinfo), "%Y/%m/%d %H:%M:%S", tm_time);

	//sprintf(txt, "%s %s %s / %s", timeinfo, msglevel[level], module, tmp);

	sprintf(txt, "%s %s:%s",msglevel[level], module, tmp);
	fp = fopen(logfile, "a+");
	fputs(txt, fp);
	fclose(fp);

	return;
    }

    /// print log message to stderr
    fprintf(stderr, "%s %s / %s", msglevel[level], module, tmp);
    return;
}

/**
 * @brief
 *
 * @param mod
 */
void ge_log_msg_init(char *mod)
{
    char *env = getenv("VGSM_VERBOSE");


    if (env) {
	msg_level = atoi(env);
	/*
	 * check message level range
	 */
	if (!(0 < msg_level && msg_level < MSGL_FILE ) && msg_level != MSGL_FILE) {
	    fprintf(stdout, "\n>>> debug_level must be 0~12 <otherwise: MSGL_ERR<1>\n");
	    msg_level = MSGL_DEFAULT;
	}
    }
    else {
	msg_level = MSGL_DEFAULT;
    }

    if (!mod) {
	fprintf(stderr, "module name needed\n");
	exit(0);
    }

    assert(strlen(mod) < sizeof module);
    strcpy(module, mod);

    if (msg_level == MSGL_FILE) {
	fprintf(stdout, "\n<<< vgsm >>> save log-messages to %s\n", logfile);
    } else
	fprintf(stdout, "\n>>> msg_level: %d, module_name: %s\n\n", msg_level, module);

    return;
}
/**
 * vim:set tabstop=4 shiftwidth=4 foldmethod=marker wrap:
 *
 */

