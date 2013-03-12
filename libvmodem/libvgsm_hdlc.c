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

/////////////////////////////////////////////////////////////////////
// vgsm_hdlc.c

#include <stdlib.h>
#include <string.h>

#include "vgsm_hdlc.h"

#define TRACE(a,s,...)

static HDLCFrame_t s_FrameSentInfo;
static HDLCFrame_t s_FrameRecvInfo;

// Last sent HDLC frame info
void SetLastSentHDLCFrameInfo(HDLCFrame_t const* pframe)
{
	TRACE(MSGL_VGSM_INFO, "%s(%s)", __FUNCTION__, __FILE__);

	s_FrameSentInfo.m_CtrlInfo = pframe->m_CtrlInfo;
	s_FrameSentInfo.m_Length = pframe->m_Length;
}

HDLCFrame_t const* GetLastSentHDLCFrameInfo()
{
	TRACE(MSGL_VGSM_INFO, "%s(%s)", __FUNCTION__, __FILE__);

	return &s_FrameSentInfo;
}

// Last receive HDLC frame info
void SetLastRecvHDLCFrameInfo(HDLCFrame_t const* pframe)
{
	TRACE(MSGL_VGSM_INFO, "%s(%s)", __FUNCTION__, __FILE__);

	s_FrameRecvInfo.m_CtrlInfo = pframe->m_CtrlInfo;
	s_FrameRecvInfo.m_Length = pframe->m_Length;
}

HDLCFrame_t const* GetLastRecvHDLCFrameInfo()
{
	TRACE(MSGL_VGSM_INFO, "%s(%s)", __FUNCTION__, __FILE__);

	return &s_FrameRecvInfo;
}

// For multi HDLC frame.
typedef struct tagHDLCNode {
	struct tagHDLCNode* m_pNext;
	HDLCFrame_t m_HDLCNode;
} HDLCNode;

HDLCNode* g_pHead = NULL;
HDLCNode* g_pTail = NULL;

void FreeAllMultiNodeList()
{
	HDLCNode* pnode_tmp;
	HDLCNode* pnode = g_pHead;
	while (pnode) {
		pnode_tmp = pnode;
		pnode = pnode->m_pNext;

		// free data
		if (pnode_tmp->m_HDLCNode.m_pValue)
			free(pnode_tmp->m_HDLCNode.m_pValue);
		free(pnode_tmp);
	}

	g_pHead = g_pTail = NULL;
}

void PushHDLCMultiFrameInfo(HDLCFrame_t const* pframe)
{
	if (!pframe)
		return;

	TRACE(MSGL_VGSM_INFO, "\n");

	HDLCNode* pnode = malloc(sizeof(HDLCNode));
	if(!pnode) return;
	memset(pnode, 0, sizeof(HDLCNode));

	pnode->m_HDLCNode.m_Length = pframe->m_Length;
	pnode->m_HDLCNode.m_CtrlInfo = pframe->m_CtrlInfo;

	if (pframe->m_Length - 3 > 0) {
		pnode->m_HDLCNode.m_pValue = malloc(pframe->m_Length - 3);
		memcpy(pnode->m_HDLCNode.m_pValue, pframe->m_pValue, pframe->m_Length - 3);
	}

	if (!g_pHead) {
		g_pHead = pnode;
	} else {
		if (g_pTail)
			g_pTail->m_pNext = pnode;
	}

	g_pTail = pnode;
}

HDLCFrame_t const* GetLastRecvHDLCMultiFrameInfo()
{
	TRACE(MSGL_VGSM_INFO, "\n");

	if (g_pTail)
		return &(g_pTail->m_HDLCNode);

	return NULL;
}

TAT* ConvertHDLCMultiFrameToAT()
{
	TRACE(MSGL_VGSM_INFO, "\n");

	unsigned char* pat_data = NULL;
	unsigned char* ptr = NULL;
	int at_size = 0;
	HDLCNode* pnode = g_pHead;

	while (pnode) {
		at_size += pnode->m_HDLCNode.m_Length - 3;
		pnode = pnode->m_pNext;
	}

	if (at_size > 0) {
		pat_data = (unsigned char*)malloc(at_size);
		ptr = pat_data;
		pnode = g_pHead;
		while (pnode) {
			memcpy(ptr, pnode->m_HDLCNode.m_pValue, pnode->m_HDLCNode.m_Length - 3);
			ptr += pnode->m_HDLCNode.m_Length - 3;

			pnode = pnode->m_pNext;
		}
	}

	FreeAllMultiNodeList();

	return (TAT*)pat_data;
}

