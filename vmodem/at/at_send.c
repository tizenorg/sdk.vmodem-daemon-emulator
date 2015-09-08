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

/*
/////////////////////////////////////////////////////////////////////
// at_send.c
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "state.h"
#include <misc.h>
#include <phoneserver.h>
#include "gsmstate.h"
#include "at_msg.h"
#include "at_send.h"

extern GSM_StateMachine GlobalS;
static unsigned char get_control_infoid(void)
{
    TRACE(MSGL_VGSM_INFO, "\n");

    static unsigned char s_infoid = 0;

    if (s_infoid >= 128)
	s_infoid = 0;
    ++s_infoid;

    return s_infoid;
}

static unsigned char get_msg_sequence(void)
{
    TRACE(MSGL_VGSM_INFO, "\n");

    static unsigned char s_msg_seq = 0;
    // message-sequence is 0 - 0xFE

    if(s_msg_seq >= 0xFE)
	s_msg_seq = 0;
    else
	s_msg_seq++;
    /***
      static unsigned char s_msg_seq = 0;
      if (s_msg_seq >= 255)
      s_msg_seq = 0;
      ++s_msg_seq;
     ***/
    return s_msg_seq;
}

#define HDLC_FRAME_HEADER_SIZE 3
#define MAX_HDLC_FRAME_SIZE 2048

// make at message with element
static int make_single_at(void *data, int datasize, void *target)
{
    TRACE(MSGL_VGSM_INFO, "\n");

    HDLCFrame_t frame_message = {0,};

    frame_message.m_Length = (unsigned short)(datasize);
    frame_message.m_CtrlInfo = get_control_infoid();

    // save target
    memcpy(target, data, datasize + 1);

    // update last sent HDLC frame
    ATSetLastSentHDLCFrameInfo(&frame_message);

    // update last sent at
    SetLastSentAT(data);

    return datasize + 1;
}

int at_msg_send(unsigned char ack, void *data, int datasize)
{
    _ENTER();
    int rc = 0;
    unsigned char *rawdata;
    if (datasize <= MAX_HDLC_FRAME_SIZE) {
	// allocate memory
	rawdata = malloc(datasize + 1);
	if(!rawdata)
	    return -1;

	// make at rawdata
	rc = make_single_at(data, datasize, rawdata);
	TRACE(MSGL_VGSM_INFO, "rc:%d\n", rc);
	TRACE(MSGL_VGSM_INFO, "data:%s\n", (char*)data);
	TRACE(MSGL_VGSM_INFO, "rawdata:%s\n", rawdata);	
	if (rawdata != NULL) {
	    // tx to phone
	    rc = GlobalS.Device.Functions->WriteDevice(&GlobalS, rawdata, rc - 1);
	    TRACE(MSGL_VGSM_INFO, "rc:%d\n", rc);
	    TRACE(MSGL_VGSM_INFO, "rawdata:%s\n", rawdata);
	}

	// release memory
	free(rawdata);
    } else {
	TRACE(MSGL_VGSM_INFO, "datasize:%d\n", datasize);
	TRACE(MSGL_VGSM_INFO, "data:%s\n", (char*)data);
	return -1;
    }

    return rc;
}

