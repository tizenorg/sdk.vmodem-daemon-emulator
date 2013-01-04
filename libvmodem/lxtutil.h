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

#ifndef _LXTUTIL_H_
#define _LXTUTIL_H_

#ifdef __cplusplus
extern "C"
{
#endif

// ++++++++++++++++++++++++++++++++++++++++++++++include about standard library
#include <stdio.h>
#include <stdlib.h>
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++include user define

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++define area

// ++++++++++++++++++++++++++++++++++++++++++++++++define of function prototype
int lxt_util_readRawBytes(int fd, void *data, int size);
int lxt_util_writeRawBytes(int fd, const void *data, int size);
void lxt_util_rawdataPrint(void *rawdata, int rawdatasize, const char *title);
int lxt_msg_send_to_server(int fd, const void *data, int size);
// EMTRACE_I878 [
int lxt_msg_send_message(int fd, unsigned char g, unsigned char a, unsigned int len, const void *data);
// EMTRACE_I878, civily 20051022 ]

#ifdef __cplusplus
}
#endif


#endif // _LXTUTIL_H_
