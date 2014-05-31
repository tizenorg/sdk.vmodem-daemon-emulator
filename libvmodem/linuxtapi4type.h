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

#ifndef _LINUXTAPI4TYPE_H_
#define _LINUXTAPI4TYPE_H_


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
        LXT_CARD_STATUS_ALL_BLOCKED                                             = 0x0B,
        LXT_CARD_STATUS_CARD1_IMSI_ERROR                                        = 0x0C,
        LXT_CARD_STATUS_CARD2_IMSI_ERROR                                        = 0x0D,
        LXT_CARD_STATUS_CARD_ENABLED                                            = 0x0E,
        LXT_CARD_STATUS_CARD_DISABLED                                           = 0x0F,
        LXT_CARD_STATUS_CARD1_BROKEN                                            = 0x10,
        LXT_CARD_STATUS_CARD2_BROKEN                                            = 0x11
    }
    LXT_CARD_STATUS;

    // current status
    typedef enum
    {
        LXT_PIN_PROCESS_STATUS_NETWORK_LIST                                     = 0x00,
        LXT_PIN_PROCESS_STATUS_PIN_CHECK                                        = 0x01,
        LXT_PIN_PROCESS_NETWORK_START                                           = 0x02,
        LXT_PIN_PROCESS_MENU_SELECTION_PUK1                                     = 0x03,
        LXT_PIN_PROCESS_PHONE_SETUP                                             = 0x04
    }
    LXT_PIN_PROCESS_STATUS;


    // slot capability
    typedef enum
    {
        LXT_SLOT_CAPABILITY_UNKNOWN                                             = 0x00,
        LXT_SLOT_CAPABILITY_USIM                                                = 0x01,
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
        LXT_SLOT_STATE_PERM_BLOCKED                                             = 0x04,
        LXT_SLOT_STATE_UNDEFINE                                                 = 0xff
    }
    LXT_SLOT_STATE;

    // dn mode
    typedef enum
    {
        LXT_DN_MODE_ADN                                                         = 0x00,
        LXT_DN_MODE_FDN                                                         = 0x01
    }
    LXT_DN_MODE;

    typedef enum
    {
        FDN_STATE_DISABLE                                                       = 0x00,
        FDN_STATE_ENABLE                                                        = 0x01
    }
    LXT_FDN_STATE;

    typedef enum
    {
        LXT_GSDI_NONE                     = 0x00,
        LXT_GSDI_CHV1_DISABLE             = 0x01,   // CHV1 disabled function
        LXT_GSDI_ADN                      = 0x02,   // GSM Abbreviated Dialling Numbers
        LXT_GSDI_FDN                      = 0x03,   // Forbidden Dialling Numbers
        LXT_GSDI_SMS                      = 0x04,   // Short Mesasge Services
        LXT_GSDI_AOC                      = 0x05,   // Advice of Charge
        LXT_GSDI_CCP                      = 0x06,   // Configuration Capability Params
        LXT_GSDI_PLMN_SELECTOR            = 0x07,   // PLMN Selector
        LXT_GSDI_MSISDN                   = 0x08,   // MSISDN
        LXT_GSDI_EXT1                     = 0x09,   // Extension 1
        LXT_GSDI_EXT2                     = 0x0A,   // Extension 2
        LXT_GSDI_SMSP                     = 0x0B,   // SMS Parameters
        LXT_GSDI_LND                      = 0x0C,   // Last Number Dialled
        LXT_GSDI_GID1                     = 0x0D,   // Group Identifier 1
        LXT_GSDI_GID2                     = 0x0E,   // Group Identifier 2
        LXT_GSDI_SPN                      = 0x0F,   // Service Provider Name
        LXT_GSDI_SDN                      = 0x10,   // Service Dialling Numbers
        LXT_GSDI_EXT3                     = 0x11,   // Enxtension 3
        LXT_GSDI_VGCS_GID_LIST            = 0x12,   // VGCS Group Identifier List
        LXT_GSDI_VBS_GID_LIST             = 0x13,   // VBS Gropu Identifier List
        LXT_GSDI_ENH_ML_SVC               = 0x14,   // Enhnaged Multi Level Precedence and Premption Service
        LXT_GSDI_AA_EMLPP                 = 0x15,   // Automatic Answer for eMLPP
        LXT_GSDI_DATA_DL_SMSCB            = 0x16,   // Data download via SMS-CB
        LXT_GSDI_DATA_DL_SMSPP            = 0x17,   // Data download via SMS-PP
        LXT_GSDI_MENU_SEL                 = 0x18,   // Menu Selection
        LXT_GSDI_CALL_CONTROL             = 0x19,   // Call Control
        LXT_GSDI_MO_SMS_CONTROL           = 0x1A,   // MO SMS control
        LXT_GSDI_PROACTIVE_SIM            = 0x1B,   // Proactive SIM
        LXT_GSDI_CBMID_RANGES             = 0x1C,   // Cell Broadcast Message Identifiers
        LXT_GSDI_BDN                      = 0x1D,   // Barred Dialing Numbers
        LXT_GSDI_EXT4                     = 0x1E,   // Extension 4
        LXT_GSDI_DEPERSON_KEYS            = 0x1F,   // Depersonalization Keys
        LXT_GSDI_COOP_NETWK_LIST          = 0x20,   // Co-operative Network List
        LXT_GSDI_SMSR                     = 0x21,   // Short Message Status Reports
        LXT_GSDI_NETWK_ALERTING_MS        = 0x22,   // Network's Indication of Alerting in the MS
        LXT_GSDI_MO_SMS_BY_SIM            = 0x23,   // Mobile Originated Short Messagse control by SIM
        LXT_GSDI_GPRS                     = 0x24,   // GPRS
        LXT_GSDI_IMAGES                   = 0x25,   // Image (IMG)
        LXT_GSDI_SOLSA                    = 0x26,   // Support of Loca Service Area
        LXT_GSDI_USSD                     = 0x27,   // USSD String Data object supported in Call Control
        LXT_GSDI_RUN_AT_CMD               = 0x28,   // RUN AT COMMAND command
        LXT_GSDI_LOCAL_PHONEBOOK          = 0x29,   // Local Phonebook, UADF/Phonebook/ADN
        LXT_GSDI_GLOBAL_PHONEBOOK         = 0x2A,   // Global Phonebook on USIM (always present)
        LXT_GSDI_OCI_OCT                  = 0x2B,   // Outgoing Call information/timer
        LXT_GSDI_ICI_ICT                  = 0x2C,   // Incoming Call Information/timer
        LXT_GSDI_CBMID                    = 0x2D,   // Cell Broadcast Message Identifier
        LXT_GSDI_UPLMN_SEL_WACT           = 0x2E,   // User controlled PLMN Selector with Access Technology
        LXT_GSDI_GSM_ACCESS_IN_USIM       = 0x2F,   // USIM ADF Access to USIM files for GSM Roaming
        LXT_GSDI_RPLMN_LACT               = 0x30,   // RPLMN Last used Access Technology
        LXT_GSDI_SP_DISP_INFO             = 0x31,   // Service Provider Display Information
        LXT_GSDI_EST                      = 0x32,   // Enable Service Table in USIM
        LXT_GSDI_ACL                      = 0x33,   // APN Control list
        LXT_GSDI_GSM_SECURITY_CONTEXT     = 0x34,   // GSM Security Context
        LXT_GSDI_CPBCCH                   = 0x35,   // CP BCCH
        LXT_GSDI_INV_SCAN                 = 0x36,   // Investigation Scan
        LXT_GSDI_MEXE                     = 0x37,   // MExE info
        LXT_GSDI_OPLMN_SEL_WACT           = 0x38,   // Operator controlled PLMN Selector with Access Technology
        LXT_GSDI_HPLMN_WACT               = 0x39,   // HPLMN Selector with Access Technology
        LXT_GSDI_EXT5                     = 0x3A,   // Extention 5
        LXT_GSDI_PLMN_NTWRK_NAME          = 0x3B,   // PLMN Network Name
        LXT_GSDI_OPLMN_LIST               = 0x3C,   // Operator PLMN List
        LXT_GSDI_MAILBOX_DIAL             = 0x3D,   // Mailbox Dialing Numbers
        LXT_GSDI_MSG_WAIT                 = 0x3E,   // Message Wait Indication
        LXT_GSDI_CALL_FWD_IND             = 0x3F,   // Call Forward Indication Status
        LXT_GSDI_MMS                      = 0x40,   // Multimedia Messaging Service
        LXT_GSDI_EXT8                     = 0x41,   // Extension 8
        LXT_GSDI_GPRS_USIM                = 0x42,   // Call control on GPRS by USIM
        LXT_GSDI_MMS_USR_P                = 0x43,   // MMS User Connectivity Parameters
        LXT_GSDI_EXT_CCP                  = 0x44,   // extended capability configuration param - in GSM
        LXT_GSDI_MAX_SVR_RSP              = 0x45
    }
    LXT_GSDI;   // generic sim driver interface



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

    typedef enum
    {
        FDN_STATE_SUCCESS                                                       = 0x00,
        FDN_STATE_PIN2_AUTH_FAIL                                                = 0x01,
        FDN_STATE_NOT_SUPPORT_FAIL                                              = 0x02,
        FDN_STATE_ENABLE_FAIL                                                   = 0x03,
        FDN_STATE_DISABLE_FAIL                                                  = 0x04
    }
    LXT_FDN_STATE_FAIL_REASON;

    typedef enum
    {
        LXT_PHONE_REBUILD_STATUS_START                                          = 0x01,
        LXT_PHONE_REBUILD_STATUS_END                                            = 0x02,
        LXT_PHONE_REBUILD_STATUS_UNDEFINE                                       = 0xFF
    }
    LXT_PHONE_REBUILD_STATUS;

    typedef enum
    {
        LXT_SIM_COVER_STATUS_CLOSE                                              = 0x01,
        LXT_SIM_COVER_STATUS_OPEN                                               = 0x02,
        LXT_SIM_COVER_STATUS_UNDEFINE                                           = 0xFF
    }
    LXT_SIM_COVER_STATUS;

    typedef struct _lxt_card
    {
        LXT_SLOT_STATE state;           // card status : empty, disable, enable, block, p.block
        char iccid[16];                 // iccid
        int provider;                   // provider : mcc, mnc
        LXT_SLOT_CAPABILITY capability; // capability : cdma only, gsm only, dual
        LXT_DN_MODE dn_mode;            // dn mode : adn, fdn
    }
    LXT_CARD;

    typedef struct _lxt_dual_card
    {
        LXT_NETWORK_SELECTION_MODE selection_mode;
        LXT_NETWORK_MODE booted_network_mode;
        LXT_CARD card1;
        LXT_CARD card2;
    }
    LXT_DUAL_CARD;



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


    typedef struct _lxt_network_information
    {
        int mcc;
        int mnc;
        char rat;
        char plmn_id[3];
    }
    LXT_NETWORK_INFORMATION;



#ifdef __cplusplus
}
#endif


#endif // _LINUXTAPI4TYPE_H_


