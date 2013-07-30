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

/*
/////////////////////////////////////////////////////////////////////
// at_msg.h
*/

#ifndef __AT_TYPE_H__
#define __AT_TYPE_H__
#include "logmsg.h"

// nsclass
typedef struct attagHDLCFrame {
	unsigned char	m_StartMagicCode;
	unsigned short	m_Length;
	unsigned char	m_CtrlInfo;		// control information id
	void*			m_pValue;
	unsigned char	m_EndMagicCode;
} HDLCFrame_t;

// Last sent HDLC frame info
void ATSetLastSentHDLCFrameInfo(HDLCFrame_t const* pframe);
HDLCFrame_t const* ATGetLastSentHDLCFrameInfo(void);

// Last receive HDLC frame info
void ATSetLastRecvHDLCFrameInfo(HDLCFrame_t const* pframe);
HDLCFrame_t const* ATGetLastRecvHDLCFrameInfo(void);

void ATPushHDLCMultiFrameInfo(HDLCFrame_t const* pframe);
HDLCFrame_t const* ATGetLastRecvHDLCMultiFrameInfo(void);
char* ConvertHDLCMultiFrameToAT(void);
void ATFreeAllMultiNodeList(void);

// Last sent AT frame info
void SetLastSentAT(char* atmsg);
char* GetLastSentAT(void);

// Last recv AT info
void SetLastRecvAT(char* atmsg);
char* GetLastRecvAT(void);

int at_send_to_event_manager( int tx, int id, unsigned char *rawdata, int rawdatasize );
int at_send_to_client( int tx, int id, char* atmsg );

#endif /* __AT_TYPE_H__ */

