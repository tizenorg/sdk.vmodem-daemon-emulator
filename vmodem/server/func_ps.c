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

#include <stdlib.h>
#include <string.h>
#include "phoneserver.h"
//#include "data_reqclient.h"
//#include "data_connclient.h"                w
#include "at_tx_func.h"
#include "misc.h"
//#include "ps_request_response_mgr.h"

/* todo : nsclass
extern GSM_AT_Functions_Security * SecurityFunc;
extern GSM_StateMachine            GlobalS;
extern ResponseFunc              * FuncResponse;
extern GSM_AT_Functions_Network  * NetworkFunc;
*/

static void RetrievePhoneState(void)
{
}

static const char* getStateName(LXT_PHONE_STATE state)
{

    switch(state)
    {
    case LXT_STATE_OFF :
        return "STATE_OFF";

    case LXT_STATE_ON :
        return "STATE_ON";

    case LXT_STATE_UIM_NOT_READY :
        return "STATE_UIM_NOT_READY";

    case LXT_STATE_UIM_READY :
        return "STATE_UIM_READY";

    case LXT_STATE_NO_SERVICE :
        return "STATE_NO_SERVICE";

    case LXT_STATE_STANDBY :
        return "STATE_STANDBY";

    case LXT_STATE_WAITING_OUTGOING_CALL :
        return "STATE_WAITING_OUTGOING_CALL";

    case LXT_STATE_WAITING_INCOMING_CALL :
        return "STATE_WAITING_INCOMING_CALL";

    case LXT_STATE_WAITING_DATA_SERVICE :
        return "STATE_WAITING_DATA_SERVICE";

    case LXT_STATE_CONVERSATION :
        return "STATE_CONVERSATION";

    case LXT_STATE_DATA_SERVICED :
        return "STATE_DATA_SERVICED";

    case LXT_STATE_DIAL_UP_DATA_SERVICED :
        return "STATE_DIAL_UP_DATA_SERVICED";

    case LXT_STATE_RELEASED :
        return "STATE_RELEASED";

    default :
        return "STATE_UNDEFINED";

    }

    return 0;

}

static void svr_svrmgr_write_state(LXT_PHONE_STATE stateBefore, LXT_PHONE_STATE stateCurr)
{
	char file_name[128] = {0, };
	char data[128] = {0,};
	int fd = 0;
	int rc;

	//sprintf(file_name,"/tmp/.phone_state");
	sprintf(file_name,"/tmp/.vgsm_state");

	fd = open(file_name, O_CREAT|O_WRONLY|O_TRUNC,00777);

	sprintf(	data, "[STATE NAME]  before = %s \t after = %s\n",
				getStateName(stateBefore),
				getStateName(stateCurr));

	if(fd >=0)
	{
		rc = write(fd, data, strlen(data));
		close(fd);
	}
}

static void SetInternalState(PhoneServer * ps, LXT_PHONE_STATE state)
{
    ps->Data.CurrentPhoneState = state;
}

static LXT_PHONE_STATE GetInternalState(PhoneServer * ps)
{
    return ps->Data.CurrentPhoneState;
}

static void OnPhoneOff(void)
{
	TRACE(MSGL_VGSM_INFO, "\n");

	// 1. clear request client.
	//GetReqClientMgr()->m_pFunc->RemoveAllClient();

	// 2. connected client.
	//GetConnClientMgr()->m_pFunc->CloseAllConnection();
}

static void OnPhoneOn(void)
{
	TRACE(MSGL_VGSM_INFO, "\n");
}

static void BroadcastInternalStatus(PhoneServer *ps, LXT_PHONE_STATE state)
{
    LXT_MESSAGE packet;

    unsigned char * data = NULL;
    LXT_PHONE_STATE CurrentPhoneState;

    CurrentPhoneState = FuncPhoneServer->GetInternalState(&GlobalPS);	// &PhoneServerHandle->GetInternalState

	// todo : nsclass : I must decomment out below function when phone is corrected.
	TRACE(MSGL_VGSM_INFO, "Attempt to change (%s -> %s)\n",
		printCurrentState(CurrentPhoneState), printCurrentState(state));

	if (LXT_STATE_OFF == CurrentPhoneState) {
		if (LXT_STATE_ON != state) {
			TRACE(MSGL_VGSM_INFO, "Can't change state(%s), because state isn't LXT_STATE_ON\n", printCurrentState(state));
			return;
		}
	}

	if (LXT_STATE_ON != state) {
		if (GlobalPS.Data.StatePhoneOn != 1) {
			TRACE(MSGL_VGSM_INFO, "Can't change state(%s), because Phone state is NOT StatePhoneOn", printCurrentState(state));
			return;
		}
	}

	if (LXT_STATE_UIM_NOT_READY == CurrentPhoneState) {
		if ((LXT_STATE_WAITING_OUTGOING_CALL != state)						// For SOS call with No-SIM, civily, YL29
			&& (LXT_STATE_UIM_READY != state && LXT_STATE_OFF != state)) {
			TRACE(MSGL_VGSM_INFO, "Can't change state(%s)\n", printCurrentState(state));
			return;
		}
	}

	if (LXT_STATE_WAITING_OUTGOING_CALL == CurrentPhoneState) {
		if (LXT_STATE_CONVERSATION != state && LXT_STATE_DATA_SERVICED != state && LXT_STATE_OFF != state &&
			LXT_STATE_RELEASED != state) {
			TRACE(MSGL_VGSM_INFO, "Can't change state(%s)\n", printCurrentState(state));
			return;
		}
	}

	if (LXT_STATE_WAITING_INCOMING_CALL == CurrentPhoneState) {
		if (LXT_STATE_CONVERSATION != state && LXT_STATE_DATA_SERVICED != state && LXT_STATE_OFF != state &&
			LXT_STATE_RELEASED != state) {
			TRACE(MSGL_VGSM_INFO, "Can't change state(%s)\n", printCurrentState(state));
			return;
		}
	}

	if (LXT_STATE_CONVERSATION == CurrentPhoneState || LXT_STATE_DATA_SERVICED == CurrentPhoneState) {
		if (LXT_STATE_STANDBY == state) {
			TRACE(MSGL_VGSM_INFO, "Can't change state(%s)\n", printCurrentState(state));
			return;
		}
	}

    if (CurrentPhoneState == state)
    {
        TRACE(MSGL_VGSM_INFO, "PhoneState is equal %d\n", state);
        return;
    }

    TRACE(MSGL_VGSM_INFO, "Success to change (%s -> %s)\n",
				printCurrentState(CurrentPhoneState),
				printCurrentState(state) );

    FuncPhoneServer->SetInternalState(&GlobalPS, state);	// &PhoneServerHandle;

    TAPIMessageInit(&packet);

    packet.group = LXT_GRP_INTERNAL;
    packet.action = LXT_PHN_INTERNAL_STATE_CHANGE_NOTIFICATION;
    packet.length = 0x01;
    data = (unsigned char *)malloc(packet.length);
    data[0] = (unsigned char )state;
    packet.data = (void *)data;

    FuncServer->BroadCast(&GlobalPS, &packet);	// &ServerHandle->server_broadcast

	if (state == LXT_STATE_ON) {
		GlobalPS.Data.StatePhoneOn = 1;
		OnPhoneOn();
	} else if (state == LXT_STATE_OFF) {
		GlobalPS.Data.StatePhoneOn = 0;
		OnPhoneOff();
	}

    free(data);
    svr_svrmgr_write_state(CurrentPhoneState,state);
}

static void CastCurrentInternalStatus(PhoneServer *ps, int clientid)
{
    LXT_MESSAGE packet;

    unsigned char * data = NULL;
    LXT_PHONE_STATE CurrentState;

    CurrentState = FuncPhoneServer->GetInternalState(&GlobalPS);

    TRACE(MSGL_VGSM_INFO, "LXT_PDA_INTERNAL_STATE_RESPONSE <%s> <%s>\n",
						clientName[clientid], printCurrentState(CurrentState) );
    TAPIMessageInit(&packet);

    packet.group = LXT_GRP_INTERNAL;
    packet.action = LXT_PHN_INTERNAL_STATE_RESPONSE;
    packet.length = 0x01;
    data = (unsigned char *)malloc(packet.length);
    data[0] = (unsigned char )CurrentState;
    packet.data = (void *)data;

    FuncServer->Cast(&GlobalPS, clientid, &packet);
    free(data);
}

static void CallListId_Init(PhoneServer *ps)
{
    int ii;

    memset( &ps->Data.prevCallList, 0x00, sizeof(gsm_call_list_t));

    for (ii = 0; ii < MAX_CALL_COUNT; ii ++)
        ps->Data.prevCallList.CallInfo[ii].idx = 0x00;
}

PhoneServerFunc PhoneServerHandle = {
    SetInternalState,
    GetInternalState,
    BroadcastInternalStatus,
    CastCurrentInternalStatus,
    CallListId_Init,
    RetrievePhoneState,
};

