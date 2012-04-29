/*
 *  telephony-emulator
 *
 * Copyright (c) 2000 - 2011 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact: 
 * SungMin Ha <sungmin82.ha@samsung.com>
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

#ifndef _TYPES4CARDBOOT_H_
#define _TYPES4CARDBOOT_H_


#ifdef __cplusplus
extern "C" {
#endif


    // ++++++++++++++++++++++++++++++++++++++++++++++include about standard library
#include <stdio.h>
    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++include user define

    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++define area

    // selected slot
    typedef enum
    {
        LXT_RUIM_SLOT_1                                                         = 0x00,
        LXT_RUIM_SLOT_2                                                         = 0x01,
        LXT_RUIM_SLOT_ALL                                                       = 0x02
    }
    LXT_RUIM_SLOT;

    // sim code id
    typedef enum
    {
        LXT_SIM_CODE_ID_PIN1                                                    = 0x00,
        LXT_SIM_CODE_ID_PIN2                                                    = 0x01,
        LXT_SIM_CODE_ID_PUK1                                                    = 0x02,
        LXT_SIM_CODE_ID_PUK2                                                    = 0x03,
        LXT_SIM_CODE_ID_SIM_LOCK                                                = 0x04,
        LXT_SIM_CODE_ID_PHONE_LOCK                                              = 0x05
    }
    LXT_SIM_CODE_ID;

    // selected network mode
    typedef enum
    {
        LXT_SELECTED_NETWORK_MODE_SLOT1_CDMA                                    = 0x00,
        LXT_SELECTED_NETWORK_MODE_SLOT1__GSM                                    = 0x01,
        LXT_SELECTED_NETWORK_MODE_SLOT2_CDMA                                    = 0x02,
        LXT_SELECTED_NETWORK_MODE_SLOT2__GSM                                    = 0x03
    }
    LXT_SELECTED_NETWORK_MODE;

    // network selection method
    typedef enum
    {
        LXT_NETWORK_SELECTION_MODE_DEFAULT                                      = 0x00,
        LXT_NETWORK_SELECTION_MODE_MENU                                         = 0x01
    }
    LXT_NETWORK_SELECTION_MODE;

    // pin check status
    typedef enum
    {
        LXT_PIN_STATUS_DISABLE                                                  = 0x00,
        LXT_PIN_STATUS_ENABLE                                                   = 0x01
    }
    LXT_PIN_STATUS;

    // phone status
    typedef enum
    {
        LXT_CARD_STATUS_CARD1_SEARCHING_START                                   = 0x00,
        LXT_CARD_STATUS_CARD2_SEARCHING_START                                   = 0x01,
        LXT_CARD_STATUS_CARD_SEARCHING_END                                      = 0x02,
        LXT_CARD_STATUS_FORCE_GSM_MODE_RUN                                      = 0x03,
        LXT_CARD_STATUS_FORCE_CDMA_MODE_RUN                                     = 0x04,
        LXT_CARD_STATUS_ALL_EMPTY                                               = 0x05,
        LXT_CARD_STATUS_CARD1_PERM_BLOCKED                                      = 0x06,
        LXT_CARD_STATUS_CARD2_PERM_BLOCKED                                      = 0x07,
        LXT_CARD_STATUS_ALL_PERM_BLOCKED                                        = 0x08,
        LXT_CARD_STATUS_CARD1_BLOCKED                                           = 0x09,
        LXT_CARD_STATUS_CARD2_BLOCKED                                           = 0x0A,
        LXT_CARD_STATUS_ALL_BLOCKED                                             = 0x0B
    }
    LXT_CARD_STATUS;

    // current status
    typedef enum
    {
        LXT_PIN_PROCESS_STATUS_NETWORK_LIST                                     = 0x00,
        LXT_PIN_PROCESS_STATUS_PIN_CHECK                                        = 0x01
    }
    LXT_PIN_PROCESS_STATUS;


    // slot capability
    typedef enum
    {
        LXT_SLOT_CAPABILITY_UNKNOWN                                             = 0x00,
        LXT_SLOT_CAPABILITY_USIM                                                = 0x01
        LXT_SLOT_CAPABILITY_GSM                                                 = 0x02,
        LXT_SLOT_CAPABILITY_GSM_USIM                                            = 0x03,
        LXT_SLOT_CAPABILITY_RUIM                                                = 0x04,
        LXT_SLOT_CAPABILITY_GSM_RUIM                                            = 0x05,
        LXT_SLOT_CAPABILITY_EMV                                                 = 0x06
    }
    LXT_SLOT_CAPABILITY;


    // slot state
    typedef enum
    {
        LXT_SLOT_STATE_NOT_INITIALIZE                                           = 0x00,
        LXT_SLOT_STATE_DISABLED                                                 = 0x01,
        LXT_SLOT_STATE_ENABLED                                                  = 0x02,
        LXT_SLOT_STATE_BLOCKED                                                  = 0x03,
        LXT_SLOT_STATE_PERM_BLOCKED                                             = 0x04
    }
    LXT_SLOT_STATE;


    // network mode (internal)
    typedef enum
    {
        LXT_NETWORK_MODE_SLOT1_CDMA                                             = 0x00,
        LXT_NETWORK_MODE_SLOT1_GSM                                              = 0x01,
        LXT_NETWORK_MODE_SLOT2_CDMA                                             = 0x02,
        LXT_NETWORK_MODE_SLOT2_GSM                                              = 0x03,
        LXT_NETWORK_MODE_UNDEFINED                                              = 0xFF,
    }
    LXT_NETWORK_MODE;

    typedef struct _lxt_card_authentication
    {
        LXT_RUIM_SLOT slot_name;
        LXT_SIM_CODE_ID sim_code_id;
        char size_current_code;
        char current_code[8];
        char size_new_code;
        char new_code[8];
        char is_boot_time;
    }
    LXT_CARD_AUTHENTICATION;



#ifdef __cplusplus
}
#endif


#endif // _TYPES4CARDBOOT_H_


