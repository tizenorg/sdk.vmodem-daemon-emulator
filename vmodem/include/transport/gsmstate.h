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

/*********************************************************************
* $Id: gsmstate.h,v 1.1.1.1 2008-12-08 04:45:34 okdear Exp $
*
* Created : 20050329
*
* Copyright (C) Samsung Electronics
*
*********************************************************************/

/* ------------------------------------------------------------------------
   라이브러리의 구조를 이해하는데 가장 중요한 헤더파일.
   구조체 _GSM_StateMachine의 인스턴스 하나로 움직이는 형태임
   따라서 이 구조체의 멤버들을 검토하면 전체적인 구조를 파악할수 있슴.

   흐름은 아래와 같다.
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
   dpram read, write, setting의 function을 포함
   device fd같은 데이타가 있슴. 특별히 신경쓸 부분은 없슴.
   --------------------------------------------------------------------------*/
/**
 * 코드 내용은 I519/I819의 코드를 그대로 사용하였으며 형태는 라이브러리
 * 의 구조에 맞게 function pointer로 선언하였다.
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
 * PhoneServer에서 참조해야 할 모든 데이타를 여기에 모아두면 된다.
 *
 * 아래의 구조체 멤버에 포함된 변수는 전역 변수 사용하듯이 사용하는 것이
 * 목적이다. I519/I819에서 PhoneServer가 가지고 있던 데이타를 아래의 구조체에
 * 몰아서 사용하면 용이하다.
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
// 사용자 예약 영역
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



// 가장 상위의 구조체
struct _GSM_StateMachine {
    bool opened;    //
    int  ReplyNum;  // AT command를 호출할때의 횟수. 통상 1회이다. 삭제 검토
    GSM_Device             Device;
    GSM_Phone              Phone;
};

/* ------------------------ Others ---------------------- */

GSM_Error GSM_InitConnection        (GSM_StateMachine *s, int ReplyNum);
GSM_Error GSM_TerminateConnection   (GSM_StateMachine *s);

void GSM_OSErrorInfo(GSM_StateMachine *s, char const* description);

#endif
