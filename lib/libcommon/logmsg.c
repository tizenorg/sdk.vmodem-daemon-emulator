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

#include "logmsg.h"

const char *msglevel[7] = { "[ fatal ]", "[ error ]", "[warning]", "[ info  ]", "[ debug ]",
    "[**dbg1**]", "[**dbg2**]"
};
int msg_level = MSGL_DEFAULT;
static char *daemon_name = NULL;

static char module[MODNAME_MAX] = { 0, };
static char logfile[256] = { 0, };


/**
 * @brief 	real_log_msg
 * @param	level
 * @param	format
 * @param ...
 * @date    Nov 18. 2008
 * */
void real_log_msg(int level, const char *format, ...)
{
    char tmp[MSGSIZE_MAX] = { 0, };
    char txt[MSGSIZE_MAX] = { 0, };
    char timeinfo[256] = { 0, };
    FILE *fp;
    struct tm *tm_time;
    struct timeval tval;

    va_list va;

    /* If directed daemon name exist and is not same as module name, return  */
    if (daemon_name != NULL) {
	if (daemon_name[0] !=  module[0]) {
	    return;
	}
    }

    if (level > msg_level)
	return;

    va_start(va, format);
    vsnprintf(tmp, MSGSIZE_MAX, format, va);
    va_end(va);

    /* when logfile no exit, just print */

    if (strlen(logfile) <= 0) {
	fprintf(stdout, "%s %s / %s", msglevel[level], module, tmp);
	return;
    }

    tmp[MSGSIZE_MAX - 2] = '\n';
    tmp[MSGSIZE_MAX - 1] = 0;

    if ((msg_level == MSGL_ALL) || (msg_level != MSGL_FILE))
	fprintf(stdout, "%s %s / %s", msglevel[level], module, tmp);

    if ((msg_level == MSGL_ALL) || (msg_level == MSGL_FILE)) {
	gettimeofday(&tval, NULL);
	tm_time = gmtime(&(tval.tv_sec));
	strftime(timeinfo, sizeof(timeinfo), "%Y/%m/%d %H:%M:%S", tm_time);

	sprintf(txt, "%s %s %s / %s", timeinfo, msglevel[level], module, tmp);

	if ((fp = fopen(logfile, "a+")) == NULL) {
	    fprintf(stdout, "log file can't open. (%s)\n", logfile);
	    return;
	}

	fputs(txt, fp);
	fclose(fp);

	return;
    }

    return;
}

/**
 * @brief 	log msg initialize
 * @param	mod: simulator, vinit
 * @date    Nov 18. 2008
 * */

void log_msg_init(char *mod, int level)
{
    char *env = getenv("SIM_VERBOSE");
    gchar *conf_path = NULL;

    /* 1.1 when getting SIM_VERBOSE env */

    if (env) {
	if (strstr(env, ":")) {
	    daemon_name=strtok(env, ":");
	    msg_level = atoi(strtok(NULL, ":"));
	    printf("a=%s, bj=%d\n", daemon_name, msg_level);

	}
	else {
	    msg_level = atoi(env);
	}

	/* 1.1.1 check message level range */

	if (!(0 < msg_level && msg_level < 7) && msg_level != MSGL_FILE) {
	    fprintf(stdout, "\n>>> debug_level must be 0,1,2,3,4,9 <otherwise: MSGL_INFO<3>\n");
	    msg_level = MSGL_DEFAULT;
	}
    }

    if (level) {
	msg_level = level;

	/* 1.1.1 check message level range */

	if (!(0 < msg_level && msg_level < 7) && msg_level != MSGL_FILE) {
	    fprintf(stdout, "\n>>> debug_level must be 0,1,2,3,4,9 <otherwise: MSGL_INFO<3>\n");
	    msg_level = MSGL_DEFAULT;
	}
    }

    /* 1.2 when not getting SIM_VERBOSE env (MSGL_DEFAULT = 3) */

    else
	msg_level = MSGL_DEFAULT;

    /* 2 check module name */

    if (!mod) {
	fprintf(stderr, "module name needed\n");
	exit(0);
    }

    strncpy(module, mod, strlen(mod));

    if ((msg_level == MSGL_ALL) || (msg_level == MSGL_FILE)) {
	//		conf_path = get_conf_path();
	conf_path = get_path();
	sprintf(logfile, "%s/simulator.log", conf_path);
	fprintf(stdout, "\n>>> save log-messages to %s\n", logfile);
    }

    else
	fprintf(stdout, "\n>>> msg_level: %d, module_name: %s\n\n", msg_level, module);

    if (conf_path)
	g_free(conf_path);

    log_msg(MSGL_DEBUG, "log level init complete\n");

    return;
}
/**
 * vim:set tabstop=4 shiftwidth=4 foldmethod=marker wrap:
 *
 */


/**
 * @brief 	return log level
 * @date    Feb 23. 2009
 * */

int get_log_level(void)
{
    return msg_level;
}

