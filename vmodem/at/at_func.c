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
// at_func.c
 */

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include "state.h"
#include <phoneserver.h>
#include <misc.h>
#include "at_recv.h"
#include "at_msg.h"
#include "at_send.h"
#include "at_gen_resp.h"
#include "at_tx_func.h"
#include "at_func.h"

#define MAX_HDLC_FRAME_SIZE		2048
#define MAX_DATA_RESPONSE 		2048

extern GSM_StateMachine GlobalS;
static char s_at_data[MAX_DATA_RESPONSE+1];
static char *s_at_data_ptr = s_at_data;
static int s_dpram_fd = -255;
static int sms_data_len = 0;
/************************************************/

char* find_new_line(char* ptr, bool sms)
{
    if(sms)
    {
	// find new line
	while(*ptr != '\0')
	{
	    ptr++;
	}
    }
    else
    {
	// find new line
	while (*ptr != '\0' && *ptr != '\r' && *ptr != '\n') 
	{
	    ptr++;
	}
    }

    return ptr;
}

char* read_cmd_line(void)
{
    char s_TmpBuffer[MAX_DATA_RESPONSE+1];
    int cnt;
    char* data_read = NULL;
    char* data_eol = NULL;
    char* ret;
    bool sms = false;
    char* data = NULL;
    char token[] = "\r";
    char* length = NULL;
    int pdu_length = 0, sca_length = 0, total_data_length = 0;
    int start_pduIndex = 0;

    // empty data
    if (*s_at_data_ptr == '\0') 
    {
	s_at_data_ptr = s_at_data;
	*s_at_data_ptr = '\0';
	data_read = s_at_data;
    } 
    else 
    {   
	while(*s_at_data_ptr == '\r' || *s_at_data_ptr == '\n')
	{
	    s_at_data_ptr++;
	}

	data_eol = find_new_line(s_at_data_ptr, sms);

	if (data_eol == NULL) 
	{
	    unsigned int data_len;
	    data_len = strlen(s_at_data_ptr);
	    memcpy(s_at_data, s_at_data_ptr, data_len + 1);
	    data_read = s_at_data + data_len;
	    s_at_data_ptr = s_at_data;
	}
    }

    while (data_eol == NULL) 
    {
	int data_size = MAX_DATA_RESPONSE - (data_read - s_at_data);

	if(data_size == 0) 
	{
	    s_at_data_ptr = s_at_data;
	    *s_at_data_ptr = '\0';
	    data_read = s_at_data;
	}

	while(1)
	{
	    cnt = read(s_dpram_fd, data_read, data_size);
	    if(errno != EINTR || cnt >= 0)
		break;
	}

	if (cnt > 0) {
	    if( !strncmp(s_at_data_ptr, ATCMGS, strlen(ATCMGS)) )
	    {
		TRACE(MSGL_VGSM_INFO, "sms true\n");
		sms = true;
		memset(s_TmpBuffer, '\0', MAX_DATA_RESPONSE+1);
		memcpy(s_TmpBuffer, s_at_data_ptr, MAX_DATA_RESPONSE+1);
		sms_data_len = 0;

		data = strchr(s_TmpBuffer, '=');
		length = strtok(data+1, token);
		if(length)
		{
		    pdu_length = atoi(length);
		}

		start_pduIndex = (strlen(ATCMGS) + strlen(length) + strlen(token) + 1);
		sca_length = hexCharToInt(data_read[start_pduIndex]);
		total_data_length = (pdu_length + sca_length + 1) * 2;	// 1: byte of sca length 
		sms_data_len = start_pduIndex + total_data_length;

		TRACE(MSGL_VGSM_INFO, "sca_length:%d, total_data_length: %d, cnt: %d\n", sca_length, total_data_length, cnt);
		TRACE(MSGL_VGSM_INFO, "(strlen(ATCMGS) + strlen(length) + strlen(token)): %d, sms_data_len:%d\n", (strlen(ATCMGS) + strlen(length) + strlen(token)), sms_data_len);

		while(cnt < sms_data_len + 1)
		{
		    cnt += read(s_dpram_fd, data_read + cnt, MAX_DATA_RESPONSE - (data_read - s_at_data) - cnt);
		}

		data_read[cnt - 2] = '\0';
		data_read[cnt - 1] = '\0';
	    }

	    data_read[cnt] = '\0';

	    while (*s_at_data_ptr == '\r' || *s_at_data_ptr == '\n')
		s_at_data_ptr++;

	    data_eol = find_new_line(s_at_data_ptr, sms);
	    data_read += cnt;
	} else if (cnt <= 0) {
	    if(cnt == 0) {
		TRACE(MSGL_VGSM_INFO, "read cnt = 0");
	    } else {
		TRACE(MSGL_VGSM_INFO, "read error %s", strerror(errno));
	    }

	    return NULL;
	}
    }

    ret = s_at_data_ptr;
    *data_eol = '\0';
    s_at_data_ptr = data_eol + 1;

    TRACE(MSGL_VGSM_INFO, "AT< %s\n", ret);

    return ret;
}

int GSM_ATDispatchDpramData(GSM_StateMachine* pstate)
{
    TRACE(MSGL_VGSM_INFO, "\n");
    HDLCFrame_t frame = {0,};
    char* atmsg = (char*)malloc(MAX_HDLC_FRAME_SIZE);
    const char* scmd;
    int found = 0, nn = 0;

    GSM_Device_DPRAMData *d = &pstate->Device.Data.DPRAM;
    struct timeval      timeout2;
    fd_set              readfds;

    FD_ZERO(&readfds);
    FD_SET(d->hPhone, &readfds);

    timeout2.tv_sec     = 0;
    timeout2.tv_usec    = 1;
    s_dpram_fd = d->hPhone;

    if  (select(d->hPhone+1, &readfds, NULL, NULL, &timeout2))
    {
	memset(atmsg, '\0', MAX_HDLC_FRAME_SIZE);
	atmsg = read_cmd_line();
	if(atmsg == NULL) {
	    return -1;
	}
    }
    TRACE(MSGL_VGSM_INFO, "atmsg length: %d\n", strlen(atmsg));	

    // set default value 
    frame.m_StartMagicCode = 0x7F;
    frame.m_EndMagicCode = 0x7E;
    frame.m_CtrlInfo = 0x7F;
    frame.m_Length = strlen(atmsg);
    if (frame.m_Length <= 0) {
	TRACE(MSGL_WARN, "================================\n");
	TRACE(MSGL_WARN, "Warning !!! Invalid HDLC Frame Packet : Length 0 \n");
	TRACE(MSGL_WARN, "================================\n");
	return -1;
    }

    if (frame.m_Length > 0) {
	// allocate memory by data size
	frame.m_pValue = malloc(frame.m_Length);
	memcpy(frame.m_pValue, atmsg, strlen(atmsg));
    }
    // have no data
    else
    {
	// init
	frame.m_pValue = NULL;
	TRACE(MSGL_VGSM_INFO, "HDLC Frame RX << No data\n");
    }

    // set last recv HDLC frame
    ATSetLastRecvHDLCFrameInfo(&frame);
    // make lxt message
    if (frame.m_pValue) {
	// everything that doesn't start with 'AT' is not a command.
	if(atmsg[0] != 'A' || atmsg[1] != 'T' || atmsg[2] == 0) {
	    TRACE(MSGL_VGSM_INFO, "Unsupported Command %s\n", atmsg);
	    memset(frame.m_pValue, '\0', frame.m_Length);
	    free(frame.m_pValue);
	    return -1;
	}
	atmsg += 2;

	{
	    for (nn = 0; ; nn++) {
		scmd = sDefaultResponses[nn].cmd;
		if (!scmd) // end of list
		    break;

		if (scmd[0] == '!') { // prefix match
		    int  len = strlen(++scmd);

		    if ( !memcmp( scmd, atmsg, len ) ) {
			found = 1;
			break;
		    }
		} else { // full match
		    if ( !strcmp( scmd, atmsg ) ) {
			found = 1;
			break;
		    }
		}
	    }

	    if ( !found )
	    {
		TRACE(MSGL_VGSM_INFO, "Unsupported Command %s\n", atmsg);
		return -1;
	    }
	    else
	    {
		SetLastRecvAT(atmsg);

		if( get_hdlc_mode() == 1 )
		{
		    return 0;
		}

		change_state_machine(sDefaultResponses[nn].main_cmd);

		at_set_recv_cmd(sDefaultResponses[nn].cmd);
		ResponseHandler handler = sDefaultResponses[nn].handler;
		handler(sDefaultResponses[nn].cmd, atmsg);
	    }
	}
	// release memory
	memset(frame.m_pValue, '\0', frame.m_Length);
	free(frame.m_pValue);
    }

    return 1;
}

void prv_byte_to_char(unsigned char aByte, char* hexaCharP)
{
    if (aByte < 0x0a)
	*hexaCharP = (char) (aByte + '0');
    else
	*hexaCharP = (char) (aByte + 0x57);
}

void prv_bytes_to_hexadecimal_string(unsigned char* bytesP, int bytesSize, char* hexaStringP, int* hexaStringSize)
{
    int i;
    int hexaLen;

    if( !hexaStringP || !hexaStringSize)
    {
	TRACE(MSGL_VGSM_INFO, " INVALID_PARAMETER:  !hexaStringP || !hexaStringSize");
	return;
    }

    hexaLen = *hexaStringSize;
    *hexaStringSize = 0;
    for ( i = 0 ; i < bytesSize ; i++ )
    {
	if( hexaLen > 2 )
	{
	    prv_byte_to_char((unsigned char) (*bytesP >> 4), (char*) (hexaStringP++));
	    prv_byte_to_char((unsigned char) (*bytesP & 0xF), (char*) (hexaStringP++));

	    hexaLen -= 2;
	}

	*hexaStringSize += 2;
	bytesP++;
    }
}

uint8_t hexCharToInt(char c)
{
    if (c >= '0' && c <= '9')
    {
	return (c - '0');
    }
    else if (c >= 'A' && c <= 'F')
    {
	return (c - 'A' + 10);
    }
    else if (c >= 'a' && c <= 'f')
    {
	return (c - 'a' + 10);
    }
    else
    {
	TRACE(MSGL_VGSM_INFO, "invalid charater!! %02x\n", c);
	return 0;
    }
}

int hexStringToBytes(char* d, char * s)
{
    int i;
    int sz;

    if(s == NULL || d == NULL)
	return 0;

    sz = strlen(s);

    for(i = 0; i < sz; i += 2)
    {
	d[i/2] = (char)((hexCharToInt(s[i]) << 4) | hexCharToInt(s[i + 1]));
    }

    return i/2;
}
