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

/*********************************************************************
* $Id: gsmstate.h,v 1.1.1.1 2008-12-08 04:45:34 okdear Exp $
*
* Created : 20050329
*
*********************************************************************/

/* ------------------------------------------------------------------------
   This is the most important header file for understanding the library architecture.
   This goes like an instance of the structure, _GSM_StateMachine.
   So, you can figure out the whole architecture by analyzing this structure members.

   The flow is as below.
   +----------------------------------------------------------------+
   | Phone Layer (AT command generate)  | (AT command reply)        |
   +----------------------------------------------------------------+
       |                                      |
       | Request                              | Reply Call
       |                                      |
   +----------------------------------------------------------------+
   | AT Protocol layer                  | AT Procotol StateMachine  |
   +----------------------------------------------------------------+
       |                                      |
       | Write                                | Read
       |                                      |
   +----------------------------------------------------------------+
   | Device Layer (DPRAM)                                           |
   +----------------------------------------------------------------+

   ------------------------------------------------------------------------*/

#ifndef __gsm_state_h
#define __gsm_state_h

#include <time.h>
#include <stdio.h>
#include <unistd.h>

#include <vgsm_error.h>

typedef struct _GSM_StateMachine           GSM_StateMachine;

//#include "protocol.h"
#include "dpram.h"

#include <vgsm_network.h>
#include <vgsm_sim.h>
#include <vgsm_call.h>
#include <vgsm_ss.h>
//#include <mzgsmpb.h>
#include <vgsm_display.h>

/* --------------------------------------------------------------------------
   Device layer
   include the dpram read, write, setting functions
   nothing special.
   --------------------------------------------------------------------------*/
/**
 *  I used the I519/I819 original codes as it is,
 * Àand this is defined as function pointer.
 */
typedef struct {
    GSM_Error (*OpenDevice)    (GSM_StateMachine *s);
    GSM_Error (*CloseDevice)   (GSM_StateMachine *s);
    int       (*ReadDevice)    (GSM_StateMachine *s, void *buf, size_t nbytes);
    int       (*WriteDevice)   (GSM_StateMachine *s, void *buf, size_t nbytes);
    int       (*PowerOnDevice) (GSM_StateMachine *s, int first_boot);
    int       (*PowerOffDevice)(GSM_StateMachine *s);
    bool      (*PowerStatus)   (GSM_StateMachine *s);
    int       (*WriteMagicCode)(GSM_StateMachine *s, unsigned char );
} GSM_Device_Functions;

extern GSM_Device_Functions DPRAMDevice;

//
typedef struct {
    union {
	char fake;
	GSM_Device_DPRAMData       DPRAM;
    } Data;
    GSM_Device_Functions *Functions;
} GSM_Device;


/* --------------------------------------------------------------------------
   Phone layer
   --------------------------------------------------------------------------*/

#define MAX_MANUFACTURER_LENGTH     50
#define MAX_MODEL_LENGTH            50
#define MAX_VERSION_LENGTH          50
#define GSM_MAX_NUMBER_LENGTH       50
/*
 * Phone Data :
 * gather all datas in here that the PhoneServer would refer to.
 *
 * These structure members are used as global variables.
 */
typedef struct {
    double VerNum;
    bool ExternalMode;
    bool EnableCLIP;
    bool EnableCallWaiting;
    bool EnableUSSD;
    int  AocMode;
    bool EnableCSSI;
    bool EnableCSSU;
    GSM_Error DispatchError;
    int  NetworkRegistration;
    char Manufacturer[MAX_MANUFACTURER_LENGTH];
    char Model[MAX_MODEL_LENGTH];
    char Version[MAX_VERSION_LENGTH];
    char * AT_Result;
    char * External_Result;
    bool FacLockStatus;
    bool PBReady;
    int  PIN_retry;
    unsigned short CMECode;
    unsigned short CMSCode;
    bool PhonePower;
    bool RamDump;
    bool TCInfo;
    char dumpaddr[256];

    int  ALS;
    int  Band;
    int                      ArrayIndex;
    bool PBMirroringDone;
    bool AutoRedial;
    /*
       GSM_PHONEBOOK_ALL       *PBAllItem;
       GSM_PB_ONE_ITEM         *PBOneItem;
       GSM_PB_StorageInfo      *pbinfo;
       GSM_SM_StorageInfo      *sminfo;
     */
    //    GSM_CLIR_Data           *CLIRData;
    //   GSM_AOC_Data            *AocData;
    gsm_call_list_t            *CallList;
    //    GSM_CCWAList            *CCWAList;
    //    GSM_CCFCList            *CCFCList;
    //    GSM_NetworkReg          *NetworkInfo;   // Network Info
    /*
       GSM_SecurityCodeType    *SecurityStatus; // Security Status
     */
    //    GSM_CurrentOperator     *OperatorInfo;
    /*
       GSM_CRSM_Result         *CRSM_Result;
     */
    //    GSM_IndicatorEvent      *IndValue;
    //    GSM_Protocol_Message    *RequestMsg;
    //    GSM_Protocol_Message    *SentMsg;
    //    GSM_Phone_RequestID     RequestID;
} GSM_Phone_Data;

// phone function

/*
// user reserved area
typedef struct {
GSM_Error (*SetIncomingCall)    (GSM_StateMachine *s, bool enable);
} GSM_AT_Functions_User;
 */


/**
 * User defined function set, which use making notification packet for
 * phone client(e.g dialer, indicator, smsdaemon, etc..)
 */
// extern GSM_AT_Functions_User    FuncUser;

/**
 * Set of AT command wrapper.
 */



typedef struct {
    GSM_Phone_Data           Data;
} GSM_Phone;



// The hightest structure
struct _GSM_StateMachine {
    bool opened;    //
    int  ReplyNum;  // The count of calling the AT command. normally this is 1.
    GSM_Device             Device;
    GSM_Phone              Phone;
};

/* ------------------------ Others ---------------------- */

GSM_Error GSM_InitConnection        (GSM_StateMachine *s, int ReplyNum);
GSM_Error GSM_TerminateConnection   (GSM_StateMachine *s);

void GSM_OSErrorInfo(GSM_StateMachine *s, char const* description);

#endif
