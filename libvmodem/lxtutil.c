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

// ++++++++++++++++++++++++++++++++++++++++++++++include about standard library
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++include user define
#include "lxtutil.h"
#include "linuxtapi.h"

#include <errno.h>
#include <string.h>

#include "libvgsm_debug.h"
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++define area

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++global variable area

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++define public function

// read raw bytes
int lxt_util_readRawBytes(int fd, void *data, int size)
{
    int rc;
    LXT_MESSAGE tmp_buf;

    // check
    if ( (data == 0) || (fd < 0) )
    {
#ifndef _NO_ESPRESSO_DEBUG_
	LIBVGSM_DEBUG("Source Invalid \n");
#endif // _NO_ESPRESSO_DEBUG_

	return -1;
    }

    rc = read(fd, &tmp_buf, size);

    ((LXT_MESSAGE*)data)->length = (unsigned short)tmp_buf.length;
    ((LXT_MESSAGE*)data)->group = (unsigned char)tmp_buf.group;
    ((LXT_MESSAGE*)data)->action = (unsigned char)tmp_buf.action;

    if (rc <= 0)
    {
#ifndef _NO_ESPRESSO_DEBUG_
	fprintf(stderr,"File read error:%s\n",strerror(errno));
	LIBVGSM_DEBUG("Read Fail and size=%d,  rc=%d\n",size,rc);
#endif // _NO_ESPRESSO_DEBUG_

	return -1;
    }

#ifndef _NO_ESPRESSO_DEBUG_
    assert(rc == size);
#endif

    return rc;
}

// write raw bytes
int lxt_util_writeRawBytes(int fd, const void *data, int size)
{
    int rc;

    LIBVGSM_DEBUG("%s\n", __FUNCTION__);

    // check
    if ( (data == 0) || (fd < 0) )
    {
#ifndef _NO_ESPRESSO_DEBUG_
	LIBVGSM_DEBUG("Source Invalid\n");
#endif // _NO_ESPRESSO_DEBUG_

	return -1;
    }

    rc = write(fd, data, size);

    if (rc <= 0)
    {
#ifndef _NO_ESPRESSO_DEBUG_
	LIBVGSM_DEBUG("Write Failed\n");
#endif // _NO_ESPRESSO_DEBUG_

	return -1;
    }

#ifndef _NO_ESPRESSO_DEBUG_
    assert(rc == size);
#endif

    return rc;

}

void lxt_util_rawdataPrint(void *rawdata, int rawdatasize, const char *title)
{
    int i;
    int len;
    unsigned char *p;

    LIBVGSM_DEBUG("%s\n", __FUNCTION__);

    if ( (rawdatasize > 0) && (rawdata == NULL) )
    {
	return;
    }

    printf(title);

    // save pointer
    p = (unsigned char *)rawdata;

    // save length
    len = rawdatasize;

    for (i = 0; i < len; i++)
    {
	if (!(i%16))
	    printf("\n");
	printf("%02x ", p[i]);

    }
    printf("\n");
}

// tx data to phone server
int lxt_msg_send_to_server(int fd, const void *data, int size)
{
    int rc = -1;

    LIBVGSM_DEBUG("%s\n", __FUNCTION__);

    // check
    if ( (data == 0) || (fd < 0) )
	return rc;

    // tx to phone server
    rc = lxt_util_writeRawBytes(fd, data, size);

    return rc;
}

// tx lxt messge to server
int lxt_msg_send_message(int fd, unsigned char g, unsigned char a, unsigned int len, const void *data)
{
    LXT_MESSAGE packet;
    int rc;

    LIBVGSM_DEBUG("lxt_msg_send_message : fd=%d, total size = 4+%d \n",fd, len);

    // save group
    packet.group = g;

    // save action
    packet.action = a;

    // save length
    packet.length = len;

    // tx to phone server
    rc = lxt_msg_send_to_server(fd, &packet, 4);

    if(rc != 4 )
	printf("[ %s ] [WARNING] write fail : rc = %d \n", __FUNCTION__, rc);

    if (len > 0)
    {
	// tx to phone server if exists data
	rc = lxt_msg_send_to_server(fd, data, len);
	if(rc != len )
	    printf("[ %s ] [WARNING] write fail : rc = %d \n", __FUNCTION__, rc);
    }

    return rc;
}

