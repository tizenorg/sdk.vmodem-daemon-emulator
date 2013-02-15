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
// at_rx_sat.c

#include <stdlib.h>
#include <string.h>

#include <vgsm_phone.h>

#include "phoneserver.h"
#include "at_send.h"
#include "at_rx_func.h"
#include "at_rx_sat.h"
#include "at_tx_func.h"
#include "at_recv.h"

//! Proactive Commands Id
#define SAT_CMD_REFRESH					0x01
#define SAT_CMD_SETUP_EVENTLIST			0x05
#define SAT_CMD_SETUP_CALL				0x10
#define SAT_CMD_SEND_SS					0x11
#define SAT_CMD_SEND_USSD					0x12
#define SAT_CMD_SEND_SHORT_MESSAGE		0x13
#define SAT_CMD_SEND_DTMF					0x14
#define SAT_CMD_LAUNCH_BROWSER			0x15
#define SAT_CMD_PLAY_TONE					0x20
#define SAT_CMD_DISPLAY_TEXT				0x21
#define SAT_CMD_GET_INKEY					0x22
#define SAT_CMD_GET_INPUT					0x23
#define SAT_CMD_SELECT_ITEM				0x24
#define SAT_CMD_SETUP_MENU				0x25
#define SAT_CMD_SETUP_IDLE_MODE_TEXT		0x28
#define SAT_CMD_RUN_AT_COMMAND			0x34
#define SAT_CMD_OPEN_CHANNEL				0x40
#define SAT_CMD_CLOSE_CHANNEL				0x41
#define SAT_CMD_RECEIVE_DATA				0x42
#define SAT_CMD_SEND_DATA					0x43

//envelope
#define SAT_CMD_MENU_SELECTION			0xd3

int at_rx_sat_envelope_cmd(char* atmsg)
{
	at_unimplemented("TelTapiSatMenuSelectionEnvelope or TelTapiSatEventDownloadEnvelope");
	return 1;
}

int at_rx_sat_proactive_cmd(char* atmsg)
{
	at_unimplemented("TelTapiSatGetSendUiDispalyStatus or TelTapiSatGetSendUiUserConfirm or TelTapiSatSendAppEcecutionResult");
	return 1;
}

int at_RxSAT_DefaultSAT(char* atmsg)
{
	return -1;
}


int at_RxSAT_TerminalResponse(char* atmsg)
{
	unsigned char *p_rcv= 0;
	unsigned char *tmp	= 0;
	LXT_MESSAGE packet;
	TAPIMessageInit(&packet);
	char CmdId;
	int data_size;
	unsigned char *pdata = 0;
	TRACE(MSGL_VGSM_INFO, "unimplemented!\n");
	return 1;
}


//RxSAT_TerminalResponse
int at_RxSAT_EnvelopeResponse(char* atmsg)
{
	unsigned char *p_rcv_term = 0;
	unsigned char *tmp = 0;
	LXT_MESSAGE packet;
	TAPIMessageInit(&packet);

	unsigned char CmdId;
	TRACE(MSGL_VGSM_INFO, "unimplemented!\n");
	return 1;
}


int at_RxSAT_ResProfileDownload(char* atmsg)
{
//	TxGCMD_SendGeneralResponse(atmsg);
	return 1;

}

