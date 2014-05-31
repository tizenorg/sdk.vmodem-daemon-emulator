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

/////////////////////////////////////////////////////////////////////
// vgsm_hdlc.h

#ifndef _vgsm_hdlc_h_
#define _vgsm_hdlc_h_

#include "vgsm.h"

// nsclass 
typedef struct tagHDLCFrame {
    unsigned char 	m_StartMagicCode;
    unsigned short 	m_Length;
    unsigned char 	m_CtrlInfo;		// control information id
    void* 		m_pValue;
    unsigned char 	m_EndMagicCode;
} HDLCFrame_t;

// Last sent HDLC frame info
void SetLastSentHDLCFrameInfo(HDLCFrame_t const* pframe);
HDLCFrame_t const* GetLastSentHDLCFrameInfo();

// Last receive HDLC frame info
void SetLastRecvHDLCFrameInfo(HDLCFrame_t const* pframe);
HDLCFrame_t const* GetLastRecvHDLCFrameInfo();

void PushHDLCMultiFrameInfo(HDLCFrame_t const* pframe);
HDLCFrame_t const* GetLastRecvHDLCMultiFrameInfo();
TAT* ConvertHDLCMultiFrameToAT();
void FreeAllMultiNodeList();

#endif // _vgsm_hdlc_h_


