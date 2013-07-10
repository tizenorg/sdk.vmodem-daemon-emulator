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

#include <gsmstate.h>
#include <stdarg.h>
#include <stdlib.h>

#include "phoneserver.h"
#include "logmsg.h"
#include "vgsm_debug.h"
#include "gsm.h"

GSM_StateMachine           GlobalS;

GSM_Device_Functions     * Device;

GSM_Error GSM_Init(bool checkerror)
{
    GSM_Error error;

    // nsclass: phone state
    GlobalPS.Data.StatePhoneOn = 0;
    GlobalPS.Data.CurrentPhoneState = LXT_STATE_OFF;
    GlobalPS.Data.GSMDataRequest = 0;

    error = GSM_InitConnection(&GlobalS,1);
    if  (error != ERR_NONE)
        return error;

    Device = GlobalS.Device.Functions;	// &DPRAMDevice

    return ERR_NONE;
}

GSM_Error GSM_PhonePowerOn(int first_boot)
{
    GSM_Error error = ERR_NONE;

	TRACE(MSGL_VGSM_INFO,"\n");

    Device->PowerOnDevice(&GlobalS, first_boot);	// &DPRAMDevice->dpram_poweron

    return error;
}

bool GSM_PhonePowerStatus(void)
{
	TRACE(MSGL_VGSM_INFO,"\n");

    return Device->PowerStatus(&GlobalS);	// &DPRAMDevice
}

void GSM_Terminate(void)
{
    GSM_Error error;

    error=GSM_TerminateConnection(&GlobalS);
    TRACE(MSGL_VGSM_ERR, "error is %d.\n", error);
}

/*
GSM_Error SendInitCommand()
{
    GSM_Error error;

    error = MiscFunc->SetDefault(&GlobalS);
    if  (error != ERR_NONE) return error;

    error = MiscFunc->SetDefault(&GlobalS);
    if  (error != ERR_NONE) return error;

    error = MiscFunc->SetEquipmentError(&GlobalS, 1);
    if  (error != ERR_NONE) return error;

    error = CallFunc->SetCallExtendedFormat(&GlobalS, true);
    if  (error != ERR_NONE) return error;

    error = CallFunc->SetIntermediateCR(&GlobalS, true);
    if  (error != ERR_NONE) return error;

    error = CallFunc->SetCallMode(&GlobalS, 0);
    if  (error != ERR_NONE) return error;

    error = NetworkFunc->SetNetworkReg(&GlobalS, 1);
    if  (error != ERR_NONE) return error;

    error = GPRSFunc->SetGPRSRegStatus(&GlobalS, 1);
    if  (error != ERR_NONE) return error;

    error = SSFunc->SetCID(&GlobalS, true);
    if  (error != ERR_NONE) return error;

    error = SSFunc->SetEnabledCallWaiting(&GlobalS, true);
    if  (error != ERR_NONE) return error;

    error = SSFunc->SetSSNotification(&GlobalS, false, true);
    if  (error != ERR_NONE) return error;

    error = SSFunc->SetEnableNotifyAOC(&GlobalS, 2);
    if  (error != ERR_NONE) return error;

    error = SSFunc->SetEnabledUSSD(&GlobalS, true);
    if  (error != ERR_NONE) return error;

    error = SMSFunc->SetSMSFormat(&GlobalS, 0);
    if  (error != ERR_NONE) return error;

    error = SMSFunc->SetSMSInitReceivingMode(&GlobalS);
    if  (error != ERR_NONE) return error;
}
*/

GSM_Error GSM_Start(void)
{
	return ERR_NONE;
}

//GSM_Error GSM_ModemInit()
//{
//    GSM_Error error;
//
//    error = MiscFunc->SetIndicatorEvent(&GlobalS);
//    error = MiscFunc->SetEquipmentError(&GlobalS, 1);
//    error = SSFunc->SetSSNotification(&GlobalS, true, true);
//    error = CallFunc->SetCallExtendedFormat(&GlobalS, false);
//    /*
//    error = SendInitCommand();
//    if  (error != ERR_NONE)
//        return error;
//    */
//    return ERR_NONE;
//}
//

