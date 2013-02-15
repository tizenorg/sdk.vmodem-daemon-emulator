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

#if 0

#ifndef _SAMSUNGTAPI_H_
#define _SAMSUNGTAPI_H_

#ifdef __cplusplus
extern "C"
{
#endif

    // ++++++++++++++++++++++++++++++++++++++++++++++include about standard library

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++include about qt

    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++include user define
#include "linuxtapi.h"
#include "phoneprotocol.h"
    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++define area
    
    typedef enum
    {
        SAMSUNG_SERVICE_MODE_MANUAL_TEST                                        = 0x01,
        SAMSUNG_SERVICE_MODE_NAM_EDITING                                        = 0x02,
        SAMSUNG_SERVICE_MODE_DEBUG                                              = 0x03,
        SAMSUNG_SERVICE_MODE_AUTO_TEST                                          = 0x04,
        SAMSUNG_SERVICE_MODE_BASIC_NAM_EDITING                                  = 0x05,
        SAMSUNG_SERVICE_MODE_PHONE_TEST                                         = 0x06
    }
    SAMSUNG_SERVICE_MODE;

    /*
    typedef enum
    {
        SAMSUNG_DEFAULT_CONFIGURATION_MODE_WARM                                 = 0x00,
        SAMSUNG_DEFAULT_CONFIGURATION_MODE_COOL                                 = 0x01,
        SAMSUNG_DEFAULT_CONFIGURATION_MODE_COLD                                 = 0x02
    }
    SAMSUNG_DEFAULT_CONFIGURATION_MODE;


    typedef enum
    {
        SAMSUNG_SIOMODE_BAUDRATE_38400                                          = 0x00,
        SAMSUNG_SIOMODE_BAUDRATE_57600                                          = 0x01,
        SAMSUNG_SIOMODE_BAUDRATE_115200                                         = 0x02
    }
    SAMSUNG_SIOMODE_BAUDRATE;

    typedef enum
    {
        SAMSUNG_SIOMODE_SLOT_MODE_ON                                            = 0x00,
        SAMSUNG_SIOMODE_SLOT_MODE_NON                                           = 0x01

    } SAMSUNG_SIOMODE_SLOT_MODE;

    typedef enum
    {
        SAMSUNG_SIOMODE_VOICE_OP_8K                                             = 0x00,
        SAMSUNG_SIOMODE_VOICE_OP_EVRC                                           = 0x01,
        SAMSUNG_SIOMODE_VOICE_OP_13K                                            = 0x02,
        SAMSUNG_SIOMODE_VOICE_OP_DEFAULT                                        = 0x03
    }
    SAMSUNG_SIOMODE_VOICE_OP;

    typedef enum
    {
        SAMSUNG_SIOMODE_MODE_HFK                                                = 0x00,
        SAMSUNG_SIOMODE_MODE_DIAG                                               = 0x01

    } SAMSUNG_SIOMODE_MODE;

    typedef enum
    {
        SAMSUNG_FACTORY_MODE_DISABLED                                           = 0x00,
        SAMSUNG_FACTORY_MODE_ENABLED                                            = 0x01
    }
    SAMSUNG_FACTORY_MODE;
    */


    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++global variable area
    int samsung_servicemode_enterServiceMode(LXT_HANDLE* handle, SAMSUNG_SERVICE_MODE mode);
    int samsung_servicemode_endServiceMode(LXT_HANDLE* handle);
    int samsung_servicemode_processKeyCode(LXT_HANDLE* handle, char ascii);

    /*
    int samsung_servicemode_factoryModeEnable(LXT_HANDLE* handle, SAMSUNG_FACTORY_MODE enable);

    int samsung_configuration_setDefaultConfiguration(LXT_HANDLE* handle, SAMSUNG_DEFAULT_CONFIGURATION_MODE mode);
    int samsung_siomode_getSIOBaudRate(LXT_HANDLE* handle);
    int samsung_siomode_setSIOBaudRate(LXT_HANDLE* handle, SAMSUNG_SIOMODE_BAUDRATE baud_rate);
    int samsung_siomode_getSlotMode(LXT_HANDLE* handle);
    int samsung_siomode_setSlotMode(LXT_HANDLE* handle, SAMSUNG_SIOMODE_SLOT_MODE slot_mode);
    int samsung_siomode_getVoiceOp(LXT_HANDLE* handle);
    int samsung_siomode_setVoiceOp(LXT_HANDLE* handle, SAMSUNG_SIOMODE_VOICE_OP voice_op);
    int samsung_siomode_getSIOMode(LXT_HANDLE* handle);
    int samsung_siomode_setSIOMode(LXT_HANDLE* handle, SAMSUNG_SIOMODE_MODE sio_mode);
    int samsung_siomode_setLifeTimeClear(LXT_HANDLE* handle);
    */

#ifdef __cplusplus
}
#endif

#endif // _SAMSUNGTAPI_H_

#endif // if 0

