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

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>

#include <linuxtapi.h>

#include "misc.h"

void* PacketDataMalloc(int size)
{
    void *p;

    p = malloc(size);

    if (p == NULL)
	return p;
    else
    {
	memset(p, 0, size);
	return p;
    }
}

void PacketDataFree(void *p)
{
    if (p != NULL)
    {
	free(p);
	p = NULL;
	return;
    }
}

int ReadPacketBytes4(int fd, void *data)
{
    int rc;
    int size = 4;
    LXT_MESSAGE tmp_buf;

    if ( (data == 0) || (fd < 0) )
    {
	return -1;
    }

    rc = read(fd, &tmp_buf, size);

    ((LXT_MESSAGE*)data)->length = (unsigned short)tmp_buf.length;
    ((LXT_MESSAGE*)data)->group = (unsigned char)tmp_buf.group;
    ((LXT_MESSAGE*)data)->action = (unsigned char)tmp_buf.action;


    if (rc <= 0)
    {
	return -1;
    }

    return rc;

}

int ReadBytes(int fd, void *data, int size)
{
    int rc;

    if ( (data == 0) || (fd < 0) )
    {
	return -1;
    }

    rc = read(fd, data, size);

    if (rc <= 0)
    {
	return -1;
    }

    return rc;

}

int WriteBytes(int fd, void *data, int size)
{
    int rc;

    if ( (data == 0) || (fd < 0) )
    {
	return -1;
    }

    rc = write(fd, data, size);

    if (rc <= 0)
    {
	return -1;
    }
    // assert(rc == size);

    return rc;

}

void TAPIMessageFree(LXT_MESSAGE * packet)
{
    packet->length = 0;
    packet->group = 0;
    packet->action = 0;
    PacketDataFree(packet->data);
    packet->data = 0;

    return;
}

void TAPIMessageInit(LXT_MESSAGE *packet)
{
    packet->length = 0;
    packet->group = 0;
    packet->action = 0;
    packet->data = 0;
}

int packed_S32(unsigned char* bytearray)
{
    unsigned char* p = bytearray;
    int i;
    int rc = 0;

    if (!p) return 0;

    for(i = sizeof(int); i >= 0; i--) {
	if(p[i] > 255){
	    return 0;
	}
	else{
	    rc = rc | p[i] << i*8;
	}
    }

    return rc;
}

unsigned short packed_U16(unsigned char* bytearray)
{
    unsigned char* p = bytearray;
    int i;
    unsigned short rc = 0;

    if (!p) return 0;

    for(i = sizeof(unsigned short); i >= 0; i--) rc = rc | p[i] << i*8;

    return rc;
}

char const* printCurrentState(LXT_PHONE_STATE CurrentState)
{
    switch (CurrentState)
    {
	case LXT_STATE_OFF :
	    return "LXT_STATE_OFF";

	case LXT_STATE_ON  :
	    return "LXT_STATE_ON";

	case LXT_STATE_UIM_NOT_READY :
	    return "LXT_STATE_UIM_NOT_READY";

	case LXT_STATE_UIM_READY     :
	    return "LXT_STATE_UIM_READY";

	case LXT_STATE_NO_SERVICE :
	    return "LXT_STATE_NO_SERVICE";

	case LXT_STATE_STANDBY :
	    return "LXT_STATE_STANDBY";

	case LXT_STATE_WAITING_OUTGOING_CALL :
	    return "LXT_STATE_WAITING_OUTGOING_CALL";

	case LXT_STATE_WAITING_INCOMING_CALL :
	    return "LXT_STATE_WAITING_INCOMING_CALL";

	case LXT_STATE_WAITING_DATA_SERVICE :
	    return "LXT_STATE_WAITING_DATA_SERVICE";

	case LXT_STATE_CONVERSATION :
	    return "LXT_STATE_CONVERSATION";

	case LXT_STATE_DATA_SERVICED :
	    return "LXT_STATE_DATA_SERVICED";

	case LXT_STATE_DIAL_UP_DATA_SERVICED :
	    return "LXT_STATE_DIAL_UP_DATA_SERVICED";

	case LXT_STATE_RELEASED :
	    return "LXT_STATE_RELEASED";

	case LXT_STATE_RESERVED :
	    return "LXT_STATE_RESERVED";

	default :
	    return "Unknown";
    }
}

