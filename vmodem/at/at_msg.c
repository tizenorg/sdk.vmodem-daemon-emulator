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
// at_msg.c
*/

#include <stdlib.h>
#include <string.h>

#include "at_msg.h"

static HDLCFrame_t s_FrameSentInfo;
static HDLCFrame_t s_FrameRecvInfo;

// Last sent HDLC frame info
void ATSetLastSentHDLCFrameInfo(HDLCFrame_t const* pframe)
{
	s_FrameSentInfo.m_CtrlInfo = pframe->m_CtrlInfo;
	s_FrameSentInfo.m_Length = pframe->m_Length;
}

HDLCFrame_t const* ATGetLastSentHDLCFrameInfo(void)
{
	return &s_FrameSentInfo;
}

// Last receive HDLC frame info
void ATSetLastRecvHDLCFrameInfo(HDLCFrame_t const* pframe)
{
//	TRACE(MSGL_VGSM_INFO, "\n");
	s_FrameRecvInfo.m_CtrlInfo = pframe->m_CtrlInfo;
	s_FrameRecvInfo.m_Length = pframe->m_Length;
}

HDLCFrame_t const* ATGetLastRecvHDLCFrameInfo(void)
{
	return &s_FrameRecvInfo;
}

static char* s_ATSentInfo;
static char* s_ATRecvInfo;

// Last sent AT frame info
void SetLastSentAT(char* atmsg)
{
	s_ATSentInfo = atmsg;
}

char* GetLastSentAT(void)
{
	return s_ATSentInfo;
}

// Last recv AT info
void SetLastRecvAT(char* atmsg)
{
	s_ATRecvInfo = atmsg;
}

char* GetLastRecvAT(void)
{
	return s_ATRecvInfo;
}

// For multi HDLC frame.
typedef struct tagHDLCNode {
	struct tagHDLCNode* m_pNext;
	HDLCFrame_t m_HDLCNode;
} HDLCNode;

HDLCNode* g_ATpHead = NULL;
HDLCNode* g_ATpTail = NULL;

void ATFreeAllMultiNodeList(void)
{
	TRACE(MSGL_VGSM_INFO, "\n");

	HDLCNode* pnode_tmp;
	HDLCNode* pnode = g_ATpHead;
	while (pnode) {
		pnode_tmp = pnode;
		pnode = pnode->m_pNext;

		// free data
		if (pnode_tmp->m_HDLCNode.m_pValue)
			free(pnode_tmp->m_HDLCNode.m_pValue);
		free(pnode_tmp);
	}

	g_ATpHead = g_ATpTail = NULL;
}

void ATPushHDLCMultiFrameInfo(HDLCFrame_t const* pframe)
{
	TRACE(MSGL_VGSM_INFO, "\n");
	if (!pframe)
		return;

	HDLCNode* pnode = malloc(sizeof(HDLCNode));

	if (!pnode)
		return;

	memset(pnode, 0, sizeof(HDLCNode));

	pnode->m_HDLCNode.m_Length = pframe->m_Length;
	pnode->m_HDLCNode.m_CtrlInfo = pframe->m_CtrlInfo;

	if (pframe->m_Length - 3 > 0) {
		pnode->m_HDLCNode.m_pValue = malloc(pframe->m_Length - 3);
		memcpy(pnode->m_HDLCNode.m_pValue, pframe->m_pValue, pframe->m_Length - 3);
	}

	if (!g_ATpHead) {
		g_ATpHead = pnode;
	} else {
		if (g_ATpTail)
			g_ATpTail->m_pNext = pnode;
	}

	g_ATpTail = pnode;
}

HDLCFrame_t const* ATGetLastRecvHDLCMultiFrameInfo(void)
{
//	TRACE(MSGL_VGSM_INFO, "\n");

	if (g_ATpTail)
		return &(g_ATpTail->m_HDLCNode);

	return NULL;
}

char* ConvertHDLCMultiFrameToAT(void)
{
	TRACE(MSGL_VGSM_INFO, "\n");

	unsigned char* atmsg_data = NULL;
	unsigned char* ptr = NULL;
	int at_size = 0;
	HDLCNode* pnode = g_ATpHead;

	while (pnode) {
		at_size += pnode->m_HDLCNode.m_Length - 3;
		pnode = pnode->m_pNext;
	}

	if (at_size > 0) {
		atmsg_data = malloc(at_size);
		ptr = atmsg_data;
		pnode = g_ATpHead;
		while (pnode) {
			memcpy(ptr, pnode->m_HDLCNode.m_pValue, pnode->m_HDLCNode.m_Length - 3);
			ptr += pnode->m_HDLCNode.m_Length - 3;

			pnode = pnode->m_pNext;
		}
	}

	ATFreeAllMultiNodeList();

	return (char*)atmsg_data;
}

