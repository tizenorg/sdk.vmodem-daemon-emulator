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
// at_tx_emulator.c
 */

#include <stdlib.h>
#include <string.h>
#include <vgsm_phone.h>
#include <misc.h>
#include "state.h"
#include "gsmstate.h"
#include "at_send.h"
#include "at_tx_func.h"
#include "at_func.h"

extern GSM_StateMachine GlobalS;
extern SmsAddressInfo g_sca;

int ATTxEmulator_HDLC_Req(LXT_MESSAGE const* packet)
{
    TRACE(MSGL_VGSM_INFO, "\n");

    // HDLC->AT
    if( get_hdlc_mode() == 1 )
	GlobalS.Device.Functions->WriteDevice(&GlobalS, packet->data, packet->length);

    return 1;
}

int ATTxEmulator_HDLC_Mode(LXT_MESSAGE const* packet)
{
    char *data = (char *)packet->data;

    set_hdlc_mode(data[0]);

    return 1;
}

int ATTxEmulator_HDLC_Clear()
{
    ATFreeAllMultiNodeList();

    return 1;
}

int ATTxEmulator_Set_General_Response_Error(LXT_MESSAGE const* packet)
{
    char *data = (char *)packet->data;

    int error = (data[1] & 0xFF);
    error <<= 8;
    error |= (data[0] & 0xFF);


    set_general_response_error(error);

    return 1;
}


int ATTxEmulator_Set_SS_Response_Error(LXT_MESSAGE const* packet, int ss_error_type)
{
    char *data = (char *)packet->data;
    int error = (data[1] & 0xFF);
    error <<= 8;
    error |= (data[0] & 0xFF);
    TRACE(MSGL_VGSM_INFO,"type = %d\n", ss_error_type );

    set_ss_transfer_response_error(error, ss_error_type );
    return 1;
}

int ATTxEmulator_Set_SS_General_Response_Error(LXT_MESSAGE const* packet)
{
    char *data = (char *)packet->data;
    int error = (data[1] & 0xFF);
    error <<= 8;
    error |= (data[0] & 0xFF);
    set_ss_general_response_error(error);


    return 1;
}

int ATTxEmulator_Set_Call_Status_Error(LXT_MESSAGE const* packet)
{
    char *data = (char *)packet->data;
    int error = (data[0] & 0xFF);

    set_call_status_error(error);

    return 1;
}

void ATset_sca( SmsAddressInfo SCA )
{
    memcpy( &g_sca, &SCA, sizeof( SmsAddressInfo ) );
}

int ATTxEmulator_Set_SCA(LXT_MESSAGE const* packet)
{
    SmsAddressInfo SCA;

    memcpy(&SCA, (char *)packet->data, sizeof(SmsAddressInfo));

    ATset_sca(SCA);

    return 1;
}
