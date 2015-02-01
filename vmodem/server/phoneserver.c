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

#include <dpram.h>
#include <gsmstate.h>

#include "phoneserver.h"
#include "logmsg.h"

PhoneServer            GlobalPS;

FunctionsDpram       * FuncDpram;

FunctionsServer      * FuncServer;	// &ServerHandle
/******************/
PhoneServerFunc      * FuncPhoneServer;

int PhoneServer_Init(PhoneServer * server, GSM_StateMachine * s) // server = GlobalPS
{
    // Dpram FD
    GSM_Device_DPRAMData * d = &s->Device.Data.DPRAM;

    TRACE(MSGL_VGSM_INFO, "\n");

    if( vgsm_server_port==0 )
	server->Dpram.fd = d->hPhone;

    // Dpram
    server->Dpram.Functions = NULL;
    server->Dpram.Functions = &DpramHandle;

    // Socket Server
    server->ServerSocket.Functions = NULL;
    server->ServerSocket.Functions = &ServerHandle;
    server->ServerSocket.Functions->Init(server);

    server->Functions1 = NULL;
    server->Functions1 = &PhoneServerHandle;

    server->Data.m_waitingmode = false;
    server->Data.m_deviceCall  = false;

    return 0;
}

int PhoneServer_Connect()
{
    // Dpram
    FuncDpram = GlobalPS.Dpram.Functions;	// &DpramHandle

    // Socket Server
    FuncServer = GlobalPS.ServerSocket.Functions;	// &ServerHandle

    if(FuncServer->Open(&GlobalPS) < 0 )		// &ServerHandle->server_open
	return -1;

    if(FuncServer->OpenInet(&GlobalPS) < 0)	// &ServerHandle->server_inet_open
	return -1;

    FuncPhoneServer = GlobalPS.Functions1;	// &PhoneServerHandle

    FuncPhoneServer->CallListId_Init(&GlobalPS);	// &PhoneServerHandle->CallListId_Init

    return 0;
}


const char * clientName[] =
{
    "LXT_ID_CLIENT_RESERVED",			//= 0x00, // ID of Reserved
    "LXT_ID_CLIENT_INDICATOR",			//= 0x01, // ID of GUI Server
    "LXT_ID_CLIENT_PHONE",				//= 0x02, // ID of Phone Client
    "LXT_ID_CLIENT_SMS",				//= 0x03, // ID of SMS Client
    "LXT_ID_CLIENT_RUIM",				//= 0x04, // ID of RUIM Manager
    "LXT_ID_CLIENT_SERVICEMODE",		//= 0x05, // ID of Service Mode
    "LXT_ID_CLIENT_DATASERVICE",		//= 0x06, // ID of Data service
    "LXT_ID_CLIENT_DIAL_ON_DEMAND",		//= 0x07, // ID of Dial on demand
    "LXT_ID_CLIENT_UTK",				//= 0x08, // ID of UTK Client
    "LXT_ID_CLIENT_ADDRESS",			//= 0x09, // ID of Address Book
    "LXT_ID_CLIENT_PHONE_SETUP",		//= 0x0A, // ID of Phone setup
    "LXT_ID_CLIENT_CARD_MANAGER",		//= 0x0B, // ID of CARD Manager
    "LXT_ID_CLIENT_SMS_DAEMON",			//= 0x0C, // ID of SMS Daemon
    "LXT_ID_CLIENT_GPS",				//= 0x0D, // ID of GPS
    "LXT_ID_CLIENT_SECURITY_SETUP",		//= 0x0E, // ID of Security Setup
    "LXT_ID_CLIENT_SAT",				//= 0x0F, // ID of sim application toolkit
    "LXT_ID_CLIENT_DATA_1",				//= 0x10, // ID of data 1
    "LXT_ID_CLIENT_DATA_2",				//= 0x11, // ID of data 2
    "LXT_ID_CLIENT_DATA_3",				//= 0x12, // ID of data 3
    "LXT_ID_CLIENT_DATA_4",				//= 0x13, // ID of data 4
    "LXT_ID_CLIENT_DATA_5",				//= 0x14, // ID of data 5
    "LXT_ID_CLIENT_DATA_6",				//= 0x15, // ID of data 6
    "LXT_ID_CLIENT_EVENT_INJECTOR",		//= 0x16, // ID of event injector
    "LXT_ID_CLIENT_EVENT_MANAGER_RX",	//= 0x17, // ID of event injector
    "LXT_ID_CLIENT_EVENT_MANAGER_TX",	//= 0x18, // ID of event injector
    "LXT_ID_CLIENT_EVENT_MANAGER",		//= 0x19, // ID of event manager
};

