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

#include "safestring.h"

/**
 * @brief	secure wrapper of strncpy
 * @param	dst : dest buffer
 * @param	src : source buffer
 * @param	n : copy size
 * @date    Apr 29. 2008
 * */

gsize s_strncpy(char *dst, const char *src, int n)
{
	if (!src || n == 0)
	{
		log_msg (MSGL_DEBUG, "Cannot copy the string. Null args or size zero.\n");
		return -1;
	}

	if (!dst)
	{
		log_msg (MSGL_ERROR, "Cannot copy the string. Null args or size zero.\n");
		exit(0);
	}

	strncpy(dst, src, n);
	return 0;

}

/**
 * @brief	secure wrapper of strcpy
 * @param	dst : dest buffer
 * @param	src : source buffer
 * @date    Apr 29. 2008
 * */

gsize s_strcpy(char *dst, const char *src)
{
	if (!dst || !src || (strlen(src) == 0))
	{
		log_msg(MSGL_WARN, "Cannot copy the string. Null args or size zero.\n");
		return -1;
	}

	//g_strlcpy(dst, src, (gsize)strlen(src));
	strcpy(dst, src);
	return 0;

}

/**
 * @brief	secure wrapper of strcmp
 * @param	dst : dest buffer
 * @param	src : source buffer
 * @date    Apr 29. 2008
 * */

gsize s_strcmp(const char *dst, const char *src)
{
	int srcsize;
	int dstsize;

	if (!dst || !src) {
		return -1;
	}

	srcsize = strlen(src);
	dstsize = strlen(dst);

	if ((srcsize == 0) && (dstsize == 0)) {
		return 0;
	}

	if (srcsize == 0 || dstsize == 0) {
		return -1;
	}

	if (srcsize==dstsize) {
		return (strcmp(src, dst));
	}
	else {
		return -1;
	}

}


/**
 * @brief	secure wrapper of strncmp
 * @param	dst : dest buffer
 * @param	src : source buffer
 * @date    Apr 29. 2008
 * */

gsize s_strncmp(const char *dst, const char *src, int n)
{
	int srcsize;
	int dstsize;

	if (!dst || !src) {
		return -1;
	}

	srcsize = strlen(src);
	dstsize = strlen(dst);

	if ((srcsize == 0) && (dstsize == 0)) {
		return 0;
	}

	if (srcsize == 0 || dstsize == 0 || n == 0) {
		log_msg (MSGL_WARN, "Cannot compare the string. Null args or size zero.\n");
		return -1;
	}

	if (srcsize==dstsize) {
		return (strncmp(src, dst, n));
	}
	else {
		return -1;
	}
}
