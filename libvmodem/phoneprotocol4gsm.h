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

#ifndef _PHONEPROTOCOL4GSM_H_
#define _PHONEPROTOCOL4GSM_H_

//  ===========================================================================
//
//                          define of main group
//
//  ===========================================================================
enum LXT_GRP_GSM
{
    LXT_GRP_GSM_CONFIG                                            = 0xA3,
    LXT_GRP_GSM_CALL                                              = 0xA4,
    LXT_GRP_GSM_SUPS                                              = 0xA5,
    LXT_GRP_GSM_EOT                                               = 0xff
};

//  ===========================================================================
//
//                          define of sub command
//
//  ===========================================================================

// define of action group (configuration) +++++++++++++++++++++++++++++++++++
enum LXT_PDA_GSM_CONFIG
{
    LXT_PDA_GSM_CONFIG_GET_CALL_COST_REQUEST                      = 0x41,
    LXT_PDA_GSM_CONFIG_RESET_CALL_COST_REQUEST                    = 0x42,
    LXT_PDA_GSM_CONFIG_SET_MAX_CALL_COST_REQUEST                  = 0x43,
    LXT_PDA_GSM_CONFIG_GET_CALL_UNIT_PRICE_REQUEST                = 0x44,
    LXT_PDA_GSM_CONFIG_SET_CALL_UNIT_PRICE_REQUEST                = 0x45,
    LXT_PDA_GSM_CONFIG_GET_NETWORK_SELECTION_MODE_REQUEST         = 0x46,
    LXT_PDA_GSM_CONFIG_AUTOMATIC_NETWORK_SELECTION_REQUEST        = 0x47,
    LXT_PDA_GSM_CONFIG_MANUAL_NETWORK_SELECTION_REQUEST           = 0x48,
    LXT_PDA_GSM_CONFIG_MANUAL_NETWORK_SELECTION_REG_REQUEST       = 0x49,
    LXT_PDA_GSM_CONFIG_GET_CALLER_ID_REQUEST                      = 0x4A,
    LXT_PDA_GSM_CONFIG_SET_CALLER_ID_REQUEST                      = 0x4B,
    LXT_PDA_GSM_CONFIG_GET_ACTIVE_LINE_REQUEST                    = 0x4C,
    LXT_PDA_GSM_CONFIG_SET_ACTIVE_LINE_REQUEST                    = 0x4D,
    LXT_PDA_GSM_CONFIG_SET_FORCE_GSM_MODE_REQUEST                 = 0x4E,
    LXT_PDA_GSM_CONFIG_GET_SYSTEM_IDENTITY                        = 0x4F
};

enum LXT_PHN_GSM_CONFIG
{
    LXT_PHN_GSM_CONFIG_GET_CALL_COST_RESONSE                      = 0x41,
    LXT_PHN_GSM_CONFIG_RESET_CALL_COST_RESONSE                    = 0x42,
    LXT_PHN_GSM_CONFIG_SET_MAX_CALL_COST_NOTIFICATION             = 0x43,
    LXT_PHN_GSM_CONFIG_GET_CALL_UNIT_PRICE_RESONSE                = 0x44,
    LXT_PHN_GSM_CONFIG_SET_CALL_UNIT_PRICE_RESONSE                = 0x45,
    LXT_PHN_GSM_CONFIG_GET_NETWORK_SELECTION_MODE_NOTIFICATION    = 0x46,
    LXT_PHN_GSM_CONFIG_AUTOMATIC_NETWORK_SELECTION_NOTIFICATION   = 0x47,
    LXT_PHN_GSM_CONFIG_MANUAL_NETWORK_SELECTION_NOTIFICATION      = 0x48,
    LXT_PHN_GSM_CONFIG_MANUAL_NETWORK_REGISTRATION_NOTIFICATION   = 0x49,
    LXT_PHN_GSM_CONFIG_GET_CALLER_ID_RESPONSE                     = 0x4A,
    LXT_PHN_GSM_CONFIG_SET_CALLER_ID_RESPONSE                     = 0x4B,
    LXT_PHN_GSM_CONFIG_GET_ACTIVE_LINE_RESPONSE                   = 0x4C,
    LXT_PHN_GSM_CONFIG_SET_ACTIVE_LINE_RESPONSE                   = 0x4D,
    LXT_PHN_GSM_CONFIG_SERVING_SYSTEM_IDENTITY_INDICATION         = 0x4E
};

// define of action group (call)+++++++++++++++++++++++++++++++++++++++++++++
enum LXT_PDA_GSM_CALL
{
    LXT_PDA_GSM_CALL_CALL_ORIGINATION_REQUEST                     = 0x41,
    LXT_PDA_GSM_CALL_END_CALL_REQUEST                             = 0x42,
    LXT_PDA_GSM_CALL_ANSWER_REQUEST                               = 0x43,
    LXT_PDA_GSM_CALL_START_DTMF_REQUEST                           = 0x44,
    LXT_PDA_GSM_CALL_STOP_DTMF_REQUEST                            = 0x45,
    LXT_PDA_GSM_CALL_REJECT_REQUEST                               = 0x46
};

enum LXT_PHN_GSM_CALL
{
    LXT_PHN_GSM_CALL_INCOMING_CALL_NOTIFICATION                   = 0x41,
    LXT_PHN_GSM_CALL_CALL_STATUS_NOTIFICATION                     = 0x42,
    LXT_PHN_GSM_CALL_START_DTMF_ACK_RESPONSE                      = 0x43,
    LXT_PHN_GSM_CALL_STOP_DTMF_ACK_RESPONSE                       = 0x44,
    LXT_PHN_GSM_CALL_DIAGNOSTIC_CALL_ORIGINATION_NOTIFICATION     = 0x45
};

// define of action group (supplementary)++++++++++++++++++++++++++++++++++++
enum LXT_PDA_GSM_SUPS
{
    LXT_PDA_GSM_SUPS_USER_ORIGINATED_SUPS_REQUEST                 = 0x41,
    LXT_PDA_GSM_SUPS_NETWORK_ORIGINATED_SUPS_USER_CONFIRM         = 0x42,
    LXT_PDA_GSM_SUPS_GET_SUPS_NETWORK_CONFIRM_DATA_REQUEST        = 0x43,
    LXT_PDA_GSM_SUPS_PASSWORD_REGISTRATION_REQUEST                = 0x44,
    LXT_PDA_GSM_SUPS_CALL_HANDLE_SUPS_WITHIN_CALL_REQUEST         = 0x45
	//    LXT_PDA_GSM_SUPS_GET_BARRING_PASSWORD_CONFIRM                 = 0x45,
};

enum LXT_PHN_GSM_SUPS
{
    LXT_PHN_GSM_SUPS_USER_ORIGINATED_SUPS_NOTIFICATION            = 0x41,
    LXT_PHN_GSM_SUPS_NETWORK_ORIGINATED_SUPS_INDICATION           = 0x42,
    LXT_PHN_GSM_SUPS_GET_SUPS_NETWORK_CONFIRM_DATA_RESPONSE       = 0x43,
    LXT_PHN_GSM_SUPS_PASSWORD_REGISTRATION_NOTIFICATION           = 0x44,
    LXT_PHN_GSM_SUPS_GET_BARRING_PASSWORD_INDICATION              = 0x45,
    LXT_PHN_GSM_SUPS_MANAGE_SUPS_WITHIN_CALL_NOTIFICATION         = 0x46,
    LXT_PHN_GSM_SUPS_STATUS_NOTIFICATION                          = 0x47,
    LXT_PHN_GSM_SUPS_CALL_PUT_ON_HOLD_BY_REMOTE_INDICATION        = 0x48,
    LXT_PHN_GSM_SUPS_CALL_RETRIEVED_BY_REMOTE_INDICATION          = 0x49,
    LXT_PHN_GSM_SUPS_EXPLICIT_CALL_TRANSFER_BY_REMOTE_INDICATION  = 0x4A
};



#endif // _PHONEPROTOCOL4GSM_H_
