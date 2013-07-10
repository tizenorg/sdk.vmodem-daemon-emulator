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

#ifndef _vgsm_phone_h_
#define _vgsm_phone_h_

/* 
 * additional explanation of I858 TAPI
 *
 * I858 TAPI API is not much different from I519/I819. Also
 * Also we devide the directory for
 * 1. separation for parallel project going
 * 2. the only new feature of GSM Only Mode
 *
 *
 * - the basic structure of TAPI packet. RX, TX is same as below.
 * +--------+--------+-------------+---------------------------------------+
 * |MAIN CMD|SUB CMD | Data Length |  DATA                                 |
 * | 1byte  | 1byte  |  2bytes     |   Data Length's Byte(s)               |
 * +--------+--------+-------------+---------------------------------------+
 *
 * Data Length is only for DATA, that is, the Length will be 0 when the DATA is null.
 *
 */

/* 
 * Main command group
 */
#define GSM_CALL                             0x01
#define GSM_NETWORK                          0x02
#define GSM_SUPS                             0x03
#define GSM_SMS                              0x04
#define GSM_MISC                             0x05
#define GSM_DISPLAY                          0x06
#define GSM_SIM                              0x07
#define GSM_DATA                             0x08
#define GSM_SAT				0x09
#define GSM_SOUND							0x0A
#define GSM_EXT                              0x0B
#define GSM_SERVICEMODE						0x0C
#define GSM_EMULATOR                         0x0D
#define GSM_GPRS                             0x0E
#define GSM_POWER                             0x0F
#define GSM_CLIENT		0x10
#define RESTORE_EI_DATA						0x11
#define FUNC_EDB							0x12
#define FUNC_EDB_IND						0x01
#define FUNC_SINGLEDB_IND						0x02

/* --------------------------------------------------------------------------
   GSM_CLIENT : 
   --------------------------------------------------------------------------*/
#define GSM_CLIENT_RELEASE_NOTI		0x01
#define GSM_CLIENT_RELEASE_NOTI_ACK		0x02
/* --------------------------------------------------------------------------
   GSM_CALL : Call Control Functions
   --------------------------------------------------------------------------*/

/*
 * Length : Caller ID length (VAR)
 * Data   : 
 * - Caller ID 
 */
#define GSM_CALL_MAKE_REQ                     0x21
/*
 * Length : 2bytes
 * Data   : 
 * - NUM Calls (1byte) 
 * - Call ID   (1byte)
 */
#define GSM_CALL_HANGUP_REQ                   0x22
/*
 * Length : 1byte
 * Data   :
 * - Call ID   (1byte)
 */
#define GSM_CALL_ANSWER_REQ                   0x23

/*
 * Length : None
 */
#define GSM_CALL_LIST_REQ                     0x24

/*
 * Length : 1byte
 * Data   :
 * - Call ID   (1byte)
 */
#define GSM_CALL_WAITING_STATUS_CFRM           0x25

/*
 * Length : VAR
 * Data   :
 * - GSM_MPTY_OPERATION : 1byte (see mzgsmcall.h)
 * - GSM_MPTY_PARAM_ID  : 1byte
 * - parameter length   : 1byte
 * - param              : VAR
 */
#define GSM_CALL_CONTROL_REQ                  0x26

/*
 * Length : 1byte
 * Data   :
 * - digit : 1 byte
 */
#define GSM_CALL_GENERATE_DTMF_REQ            0x27

// EMTRACE_I878 [ 
// Testing purpose
#define GSM_CALL_GET_FAKE_CALLLIST_REQ		0x28
// EMTRACE_I878, civily 20051014 ]
/*
 * Length : 0
 * Data   :
 * - None
 */
#define GSM_CALL_GET_AUTOREDIAL_REQ           0x29

/*
 * Length : 1
 * Data   :
 * - Enable or Disable
 */
#define GSM_CALL_SET_AUTOREDIAL_REQ           0x2A
#define GSM_CALL_LIST_CFRM                    0x2B

/*
 * Length : Caller ID length (VAR)
 * Data   : 
 * - Caller ID 
 */
#define GSM_CALL_MAKE_TO_DEVICE_REQ           0x2C
#define GSM_CALL_TO_DEVICE_CFRM               0x2D

/* --------------------------------------------------------------------------
   GSM_CALL : Notification or Response
   --------------------------------------------------------------------------*/

#define GSM_CALL_ID                           0x20

/*
 * Length : (VAR)
 * Data   :
 * - Call ID       (1byte)
 * - Forwared flag (1byte)
 * - Call ID Length(1byte)
 * - Call ID       (VAR)
 *
 * CAUTION : CLIR_CAUSE is deleted.
 */

#define GSM_CALL_INCOMING_NOTI                0x21
/*
 * Length : 3bytes
 * Data   :
 * - Call ID      (1byte)
 * - Forwared     (1byte)
 * - Status       (1byte)  // see the value and comment on mzgsmcall.h
 *
 * CAUTION : CAUSE, NUM_CALLS_IN_USE are deleted
 */
#define GSM_CALL_STATUS_NOTI                  0x22
/*
 * Length : VAR 1byte(Reserved1) + 1byte(Reserved2) + 
 * 1byte(Call Count) + (Call Count) * 5 (Call Data)
 * DATA   :
 * - Reserved1      (1byte)
 * - Reserved2      (1byte)
 *
 * - Call Count     (1byte)
 *
 * the 5 byte below is repeated as Call Counts.
 * - Call ID        (1byte)
 * - GSM_Call_Dir   (1byte) see mzgsmcall.h
 * - GSM_Call_Stat  (1byte) see mzgsmcall.h
 * - GSM_Call_Mode  (1byte) see mzgsmcall.h (only about Voice..)
 * - MPTY           (1byte) 0x01 mpty, 0x00 single
 */
#define GSM_CALL_LIST_RES                     0x23

/*
 * Length : VAR 1byte(result) + 1byte(Operation) + 1byte(call Id) + 
 * (Call Count) + VAR(Call Count * 1)
 * DATA   :
 * - result (1byte) 
 * - GSM_MPTY_OPERATION (1byte) see mzgsmcall.h
 * - GSM_MPTY_PARAM_ID  (1byte) see mzgsmcall.h
 * - active call count  (1byte)
 * - active call list   (VAR)
 */
#define GSM_CALL_CONTROL_RES                  0x24

/*
 * Length : 2
 * Data   :
 * - call id (1byte)
 * - status  (1byte) 
 * 0x01 : waiting 상태 GSM_Call_Dialing, GSM_Call_Alerting, GSM_Call_Incoming
 * 0x00 : not waiting 상태 GSM_Call_Active, GSM_Call_Release
 */
#define GSM_CALL_WAITING_STATUS_IND           0x25


/*
 * Length : (VAR)
 * Data   :
 * - Call ID          Length (1byte)
 * - Connected Line          (VAR)
 */

#define GSM_CALL_CONNECTED_LINE_NOTI          0x26

/*
 * Length : 1
 * Data   :
 * - the reason of redialing mode
 */
#define GSM_CALL_AUTOREDIAL_NOTI              0x27

/*
 * Length : 2
 * - Result    : 1 byte 
 * - ALS value : 1 byte 
 */
#define GSM_CALL_GET_AUTO_REDIAL_RES          0x28

/*
 * Length : 0
 */
#define GSM_CALL_CALL_LIST_IND                0x29

/*
 *
 */
#define GSM_CALL_WAITING_STATUS_TO_DEVICE_IND 0x2A

/*
 * Length : 0
 */
#define GSM_CALL_CONNECTION_TONE_NOTI         0x2B

/*
 * Length : 1
 */
#define GSM_CALL_SMASO_NOTI                   0x2C

/*
 * Length : 1
 * result : 1 byte
 */
#define GSM_CALL_MAKE_RES                     0x2D

/*
 * Length : 1
 * result : 1 byte
 */
#define GSM_CALL_ANSWER_RES                   0x2E

/*
 * Length : 1
 * result : 1 byte
 */
#define GSM_CALL_HANGUP_RES                   0x2F

// sunnyfan 2006.01.17
#define GSM_CALL_GET_ALS_MV_REQ				0x41
#define GSM_CALL_GET_ALS_MV_RES				0x42
#define GSM_CALL_SET_ALS_MV_REQ				0x43
#define GSM_CALL_SET_ALS_MV_RES				0x44
// sunnyfan ]

// sunnyfan, 2005.11.17 [
#define GSM_CALL_GENERATE_DTMF_RES            0x50

#define GSM_CALL_GENRES_DTMF					 0x51
// sunnyfan ]

#define GSM_CALL_STATE					0x61
#define GSM_CALL_ALERT_IND				0x62 

/* --------------------------------------------------------------------------
   GSM_NETWORK : Network Functions
   --------------------------------------------------------------------------*/

/*
 * Length : Zero
 * Data   :
 * - None
 */
#define GSM_NETWORK_CURRENT_MODE_REQ          0x31

/*
 * Length : Zero
 * Data   :
 * - None
 */
#define GSM_NETWORK_SET_AUTOMATIC_MODE_REQ    0x32

/*
 * Length : Zero
 * Data   :
 * - operator name (5byte)
 */
#define GSM_NETWORK_SET_MANUAL_MODE_REQ       0x33

/*
 * Length : Zero
 * Data   :
 * - None
 */
#define GSM_NETWORK_INQUIRY_OPERATOR_REQ      0x34

/*
 * Length : Zero
 * Data   :
 * - None
 */
#define GSM_NETWORK_GPRS_STATE_REQ            0x35

/*
 * Length : None
 */
#define GSM_NETWORK_GET_BAND_REQ              0x36

/*
 * Length : 1
 * - Band value : 1 byte 
 */
#define GSM_NETWORK_SET_BAND_REQ              0x37

/*
 * Length : Zero
 * Data   :
 * - None
 */
#define GSM_NETWORK_GET_SIGNAL_REQ            0x38

/*
 * Length : Zero
 * Data   :
 * - None
 */
#define GSM_NETWORK_GET_NETWORK_INFO_REQ      0x39

/*
 * Length : Zero
 * Data   :
 * - None
 */
#define GSM_NETWORK_CANCEL_OPERATOR_REQ       0x3A

/*
 * Length : Zero
 * Data   :
 * - None
 */
#define GSM_NETWORK_GET_INDICATOR_REQ         0x3B

// EMTRACE_I878 [
#define GSM_NETWORK_SET_SVCDOMAIN_REQ		0x3C
#define GSM_NETWORK_SET_POWERONATTACH_REQ	0x3D
#define GSM_NETWORK_GET_SVCDOMAIN_REQ		0x3E
#define GSM_NETWORK_GET_POWERONATTACH_REQ	0x3F
// EMTRACE_I878, civily 2005.10.13 ]
/* --------------------------------------------------------------------------
   GSM_NETWORK : Notification or Response
   --------------------------------------------------------------------------*/

/*
 * Length : 1 + 1 + VAR(Network Name)
 * - State        (1byte) see mzgsmnet.h
 * - Length       (1byte) Network Name Length
 * - Network Name (VAR)
 */
#define GSM_NETWORK_REG_NOTI                  0x31

/*
 * Length : (1) + VAR
 * - String Length 1byte
 * - String        VAR
 *
 */
// CAUTION : This will be fixed to send the only index, not string.
#define GSM_NETWORK_SMNT_NOTI                 0x32

/*
 * Length : 1 + 1 + VAR
 * GSM_OperMode    : 1byte
 * Length MCC+MNC  : 1byte
 * operator name   : VAR
 */
#define GSM_NETWORK_CURRENT_MODE_RES          0x33

/*
 * Length : NONE
 */
#define GSM_NETWORK_SET_AUTOMATIC_MODE_RES    0x34

/*
 * Length : NONE
 */
#define GSM_NETWORK_SET_MANUAL_MODE_RES       0x35

/*
 * Length : 1 + (item count * 6)
 * Data   :
 * - count    : 1byte
 * - mode     : 1byte
 * - operator : 5byte
 */
#define GSM_NETWORK_INQUIRY_OPERATOR_RES      0x36

/*
 * Length : 2
 * - Result    : 1 byte 
 * - Band value: 1 byte 
 */
#define GSM_NETWORK_GET_BAND_RES              0x37

/*
 * Length : 2
 * - Result    : 1 byte 
 * - Band value: 1 byte 
 */
#define GSM_NETWORK_SET_BAND_RES               0x38

/*
 * Length : 2
 * Data   :
 * - result : 0x00 (ok) or 0x01 (failed)
 * - mode : refer mzgsmnet.h
 */
#define GSM_NETWORK_GET_NETWORK_INFO_RES       0x39

// EMTRACE_I878 [
#define GSM_NETWORK_POWERONATTACH_RES			0x3A
#define GSM_NETWORK_SVCDOMAIN_RES				0x3B
// EMTRACE_I878, civily 2005.10.13 ]

#define GSM_NETWORK_PLMN_LIST		0x3C


#define GSM_NETWORK_CURRENT_PLMN_NOTI		0x3D

/* gsm_network_current_plmn_info, nitz info by cosmos in 20090213 */
#define GSM_NETWORK_CURRENT_PLMN_INFO          0x3E
#define GSM_NETWORK_CURRENT_NITZ_INFO          0x3F

/* --------------------------------------------------------------------------
   GSM_SUPS : Sups Service TX function
   --------------------------------------------------------------------------*/
/*
 * Length : MMI String length
 * Data   : 
 * - MMI String
 */
#define GSM_SUPS_MMI_REQ                      0x41

/*
 * Length : 1(fac value) + 1(class)
 * Data   :
 * - fac value (1byte) see mzgsmsups.h
 * - class     (1byte) see mzgsmmisc.h
 */
#define GSM_SUPS_GET_FAC_LOCK_STATUS_REQ      0x42

/*
 * Length : 1(fac value) + 1(class) + 1(mode) + 1(pw length) + VAR(passwd)
 * Data   :
 * - fac value (1byte) see mzgsmsups.h
 * - class     (1byte) see mzgsmmisc.h
 * - mode      (1byte) see mzgsmsups.h
 * - pw length (1byte)
 * - passwd    (pw length bytes)
 */
#define GSM_SUPS_SET_FAC_LOCK_STATUS_REQ      0x43

/*
 * Length : 1(the reason)
 * Data   :
 * - GSM_CF_Reason    (1byte) see mzgsmsups.h
 */
#define GSM_SUPS_GET_CCFC_REQ                 0x44

/*
 * Length : sizeof (struct GSM_CF_Data)
 * Data   :
 * - GSM_CF_Data    (1byte) see mzgsmsups.h
 */
#define GSM_SUPS_SET_CCFC_REQ                 0x45

/*
 * Length : 0
 */
#define GSM_SUPS_GET_CLIR_REQ                 0x46

/*
 * Length : 1 (CLIR Setting Value)
 * Data   :
 * - CLIR_Activation_Status : (1byte)
 */
#define GSM_SUPS_SET_CLIR_REQ                 0x47

/*
 * Length : 1(CW_Notify) + 1(GSM_CW_Mode) + 1(GSM_ClassX)
 * Data   :
 * - CW_Notify : (1byte) 
 * - CW_Mode   : (1byte)
 * - GSM_ClassX  (1byte)
 */
#define GSM_SUPS_GET_CW_REQ                   0x48

/*
 * Length : 1(CW_Notify) + 1(GSM_CW_Mode) + 1(GSM_ClassX)
 * Data   :
 * - CW_Notify : (1byte) 
 * - CW_Mode   : (1byte)
 * - GSM_ClassX  (1byte)
 */
#define GSM_SUPS_SET_CW_REQ                   0x49

/*
 * Length : 1(GSM_AOC_Value)
 * Data   :
 * - None
 */
#define GSM_SUPS_AOC_REQ                      0x4A

/*
 * Length : None
 * Data   :
 * - None
 */
#define GSM_SUPS_AOC_RESET_REQ                0x4B

/*
 * Length : 11
 * Data   :
 * - acm max value (6bytes) // Hexadecimal value
 * - pin2 length   (1byte)
 * - pin2          (4byte)
 */
#define GSM_SUPS_SET_ACM_MAX_REQ              0x4C

/*
 * Length : 
 * Data   :
 * - AOC_CURRENCY_Code (1byte)
 * - length            (1byte)
 * - price             (VAR)
 */
#define GSM_SUPS_AOC_SET_PPU_REQ              0x4D

/*
 * Length : USSD String length
 * Data   : 
 * - USSD String
 */
#define GSM_SUPS_USSD_SEND_REQ                0x4E		

/*
 * Length : 0
 * Data   : 
 * - None
 */
#define GSM_SUPS_USSD_CANCEL_REQ              0x4F
// sunnyfan, 2005.11.09 [
#define GSM_SUPS_PWDSET_CB_REQ				 0x50
#define GSM_SUPS_SET_CB_REQ					 0x51
#define GSM_SUPS_GET_CB_REQ					 0x52
#define GSM_SUPS_USSD_SEND_CONFIRM            0x53
#define GSM_SUPS_NOTI_FEEDBACK				 0x54
#define GSM_SUPS_MESSAGE_FEEDBACK			 0x55
// sunnyfan ]


/*
 * Length : 1
 * Data   : 
 * - Barring Facility
 */
#define GSM_SUPS_CB_STATUS_REQ                0x56

/*
 * Length : 0
 * Data   : 
 * - None
 */
#define GSM_SUPS_GET_PPU_REQ                  0x57

/* --------------------------------------------------------------------------
   GSM_SUPS : Notification or Response
   --------------------------------------------------------------------------*/
// sunnyfan, 2006.01.19 [
#define GSM_SUPS_NETWORK_POPUP_MESSAGE		0x40
// sunnyfan ]
/*
 * Length : 1(status) + 1(fac value) + 1(class)
 * Data   :
 * - status      (1byte) 0x00 : not active, 0x01 : active  
 * - fac   value (1byte) see mzgsmsups.h
 * - class value (1byte) see mzgsmsups.h
 */
#define GSM_SUPS_GET_FAC_LOCK_STATUS_RES      0x41

/*
 * Length : 1(fac value) + 1(class) + 1(mode)
 * Data   :
 * - result (1byte) 
 * - fac    (1byte) see mzgsmsups.h
 * - remain (1byte)
 */
#define GSM_SUPS_SET_FAC_LOCK_STATUS_RES      0x42

/*
 * Length : 1(status) + 1(class) + 1(number_length) + VAR(number) +
 *          1(time)
 * Data   :
 * - GSM_CF_Status   (1byte) see mzgsmsups.h
 * - GSM_ClassX      (1byte)
 * - length          (1byte)
 * - DN              (VAR)
 * - time            (1byte) only 'no reply' case
 */
#define GSM_SUPS_GET_CCFC_RES                 0x43

/*
 * Length : 1(Activation Status) + 1(Network Provisioning Status)
 * Data   :
 * - CLIR_Activation_Status (1byte) see mzgsmsups.h
 * - CLIR_Provision_Status  (1byte) see mzgsmsups.h
 */
#define GSM_SUPS_GET_CLIR_RES                 0x44

/*
 * Length : 1(status) + 1(class)
 * Data   :
 * - GSM_CW_Status   (1byte) mzgsmsups.h
 * - GSM_ClassX      (1byte) mzgsmsups.h
 */
#define GSM_SUPS_GET_CW_RES                   0x45

/*
 * Length : 1(notify code) + 2(USSD str length) + VAR(USSD string) + 1(dcs)
 * Data   :
 * - USSD_Notify        (1byte) mzgsmsups.h
 * - USSD string length (2byte)
 * - USSD String        (VAR)
 * - Decoding Scheme    (1byte)
 */
#define GSM_SUPS_USSD_NOTI                    0x46

#define GSM_SUPS_CW_NOTI                      0x47

/*
 * Length : 2(code) + 1(Call id)
 * Data
 * CSSU code 2 value (2bytes)
 * Call id           (1byte)
 */
#define GSM_SUPS_CSSU_NOTI                    0x48

/*
 * Length : 2(code)
 * Data
 * CSSI code 2 value (2bytes)
 */
#define GSM_SUPS_CSSI_NOTI                    0x49

/*
 * Length : 2(code)
 * Data
 * CSSI code 2 value (2bytes)
 */
#define GSM_SUPS_CSSI_NOTI                    0x49

/*
 * Length : 4(code)
 * Data
 * CCCM code 4 value (4bytes)
 */
#define GSM_SUPS_CCCM_NOTI                    0x4A

/*
 * Length : 18
 * Data
 * - CCM  : 6bytes  // String type hexdecimal e.g : "00001E" indicates decimal
 *                  // value 30
 * - CACM : 6bytes  // type same as CCM
 * - CAMM : 6bytes  // type same as CCM
 */
#define GSM_SUPS_AOC_RES                      0x4B

/*
 * Length : 0
 * Data   :
 * - None
 */
#define GSM_SUPS_AOC_RESET_RES                0x4C

/*
 * Length : 0
 * Data   :
 * - None
 */
#define GSM_SUPS_SET_ACM_MAX_RES              0x4D

/*
 * Length : 0
 * Data   :
 * - None
 */
#define GSM_SUPS_AOC_SET_PPU_RES              0x4E

/*
 * Length : 1
 * Data   :
 * - Result
 */
#define GSM_SUPS_MMI_RES                      0x4F

#define GSM_SUPS_GET_CB_RES					 0x50
#define GSM_SUPS_CB_NOTI						 0x51
#define GSM_SUPS_CB_PW_INDI					 0x52

/*
 * Length : 1
 * Data   :
 * - Result
 */
#define GSM_SUPS_GET_CCFC_FAILED_RES          0x53

/*
 * Length : 1
 * Data   :
 * - Result
 */
#define GSM_SUPS_CB_STATUS_RES                0x54

/*
 * Length : VAR
 * Data   :
 * - Result
 */
#define GSM_SUPS_GET_PPU_RES                  0x55

#define GSM_SUPS_STATE			0x56 
#define GSM_SUPS_CONFERENCE			0x57 
#define GSM_SUPS_HOLD			0x58 
#define GSM_SUPS_UNHOLD			0x59 
#define GSM_SUPS_AOC_SET        0x5a 
#define GSM_SUPS_AOC_GET        0x5b 
#define GSM_SUPS_CW_SET			0xe0 
#define GSM_SUPS_CW_GET			0xe1 
#define GSM_SUPS_CF_SET			0xe2 
#define GSM_SUPS_CF_GET			0xe3 
#define GSM_SUPS_CB_SET			0xe4 
#define GSM_SUPS_CB_GET			0xe5 
#define GSM_SUPS_USSD_GET       0xe6 
#define GSM_SUPS_USSD_SET       0xe7 

/* --------------------------------------------------------------------------
   GSM_MISC : misc Functions
   --------------------------------------------------------------------------*/

/*
 * Length : 2
 * - CMD ID       (2bytes) Debug command ID
 */
#define GSM_MISC_DEBUG_REQ                    0x61

/*
 * Length : 4
 * - CMD ID       (2bytes) Debug command ID
 * - param1       (1byte)  parameter1 for H/W setting
 * - param2       (1byte)  parameter2 for H/W setting
 */
#define GSM_MISC_DEBUG_HW_REQ                 0x62

/*
 * Length : 1
 * - RAMDUMP Trace (1byte)  The flag of RAMDUMP acceptance.
 */
#define GSM_MISC_RAMDUMP_REQ                  0x63

/*
 * Length : 0
 * - request IMEI
 */
#define GSM_MISC_IMEI_REQ                     0x64

/*
 * Length : 0
 * - request IMSI
 */
#define GSM_MISC_IMSI_REQ                     0x65

/*
 * Length : 0
 * - request the Phone Version
 */
#define GSM_MISC_VERSION_INFO_REQ             0x66

/*
 * Length : 0
 * - request SIM and network status.
 */
#define GSM_MISC_INTERNAL_MODE_REQ            0x67

/* --------------------------------------------------------------------------
   GSM_DISPLAY : Notification or Response
   --------------------------------------------------------------------------*/

/*
 * Length : (1) + VAR
 * - String Length 1byte
 * - String        VAR
 */
#define GSM_MISC_DEBUG_NOTI                   0x61

/*
 * Length : (1) + VAR
 * - String Length 1byte
 * - String        VAR
 */
#define GSM_MISC_RESET_NOTI                   0x62

/*
 * Length : (1) + (2)
 * - Error type : (1byte) see mzgsmerror.h
 * - CME or CMS : (2bytes) error code for CME or CMS
 */
#define GSM_MISC_ERROR_RESPONSE               0x63

/*
 * Length : (1) + VAR
 * - String Length 1byte
 * - String        VAR
 */
#define GSM_MISC_IMEI_RES                     0x64

/*
 * Length : (1) + VAR
 * - String Length 1byte
 * - String        VAR
 */
#define GSM_MISC_IMSI_RES                     0x65

/*
 * Length : (1) + VAR
 * - String Length 1byte
 * - String        VAR
 */
#define GSM_MISC_RESET_RES                    0x66

/*
 * Length : 32 + 32
 * - Phone Version
 */
#define GSM_MISC_VERSION_INFO_RES			0x67

// sunnyfan, 2006.02.07 [
#define GSM_MISC_SLEEP_BY_PHONE_NOTI			0x68
#define GSM_MISC_DIAG_KEYCODE_NOTI				0x69
#define GSM_MISC_BT_TEST_MODE_NOTI				0x70	// sunnyfanbt
// sunnyfan ]

/* --------------------------------------------------------------------------
   GSM_DISPLAY : Functions for Indicator
   --------------------------------------------------------------------------*/

/*
 * Declare Display Functions constant.
 */

/* --------------------------------------------------------------------------
   GSM_DISPLAY : Notification or Response
   --------------------------------------------------------------------------*/

/*
 * Length : 2bytes
 * Data   :
 * - Indicator (1byte) see mzgsmmisc.h
 * - Value     (1byte) 
 */
#define GSM_DISPLAY_INDICATOR_NOTI            0x71

/* --------------------------------------------------------------------------
   GSM_SIM : SIM card Functions
   --------------------------------------------------------------------------*/

/*
 * Length : VAR
 * Data   :
 * - PIN2 (VAR)
 */

//#define GSM_SIM_PIN2_REQ                      0x81
#define GSM_SIM_SELECT_PB_STORAGE_REQ         0x82
#define GSM_SIM_PB_READ_REQ                   0x83
#define GSM_SIM_PB_WRITE_REQ                  0x84
#define GSM_SIM_PB_ERASE_REQ                  0x85
#define GSM_SIM_PB_CLEAR_REQ                  0x86
#define GSM_SIM_WRITE_VOICE_MAIL_REQ          0x87
#define GSM_SIM_READ_VOICE_MAIL_REQ           0x88


#define GSM_SIM_PIN_REQ                       0x89  //GVSM does not uesd this
#define GSM_SIM_PUK_REQ                       0x8A //state
#define GSM_SIM_CHANGE_PW_REQ                 0x8B
#define GSM_SIM_CRSM_GET_REQ                  0x8C
#define GSM_SIM_PUK2_REQ                      0x8D
#define GSM_SIM_PB_READY_REQ                  0x8E
#define GSM_SIM_CRSM_READ_BIN_REQ             0x8F
#define GSM_SIM_CRSM_READ_RECORD_REQ          0x90
#define GSM_SIM_GET_ALS_REQ                   0x91
#define GSM_SIM_SET_ALS_REQ                   0x92
#define GSM_SIM_CRSM_UPDATE_RECORD_REQ        0x93
#define GSM_SIM_CRSM_UPDATE_BIN_REQ           0x94
#define GSM_SIM_SELECT_SM_STORAGE_REQ         0x95
#define GSM_SIM_SM_ERASE_REQ                  0x96
#define GSM_SIM_SEC_STATUS_REQ                0x97

#define GSM_SIM_PB_CAPACITY_REQ               0x98
#define GSM_PB_ACCESS_REQ					0x99
#define GSM_PB_STORAGE_REQ					0x9A
#define GSM_SIM_PHONELOCK_STATUS_REQ			0x9B
#define GSM_SIM_SET_PHONELOCK_REQ			0x9C
#define GSM_PB_RECORD_ADD_REQ				0x9D
#define GSM_PB_RECORD_DEL_REQ				0x9E
#define GSM_PB_RECORD_EDIT_REQ				0x9F
#define GSM_SIM_LOCK_INFO_REQ				0xA0
#define GSM_PB_SET_STORAGE_TYPE_REQ			0xA1
#define GSM_SIM_SECURITY_STATUS_REQ			0xA2
#define GSM_PB_ENTRY_INFO_REQ				0xA3
#define GSM_SIM_LOCK_PASSWORD_REQ             0xA4
#define GSM_SIM_CARD_READY_REQ                0xA5
#define GSM_SIM_PIN2_RES                      0x81
#define GSM_SIM_SECURITY_STATUS_RES           0x82
#define GSM_SIM_SELECT_PB_STORAGE_RES         0x83
#define GSM_SIM_PB_READ_RES                   0x84
#define GSM_SIM_PB_ACTION_RES                 0x85
#define GSM_SIM_AUTH_RES                      0x86
#define GSM_SIM_CHANGE_PW_RES                 0x87
#define GSM_SIM_PB_READY_RES                  0x88
#define GSM_SIM_CRSM_GET_RES                  0x89
#define GSM_SIM_CRSM_READ_BIN_RES             0x8A
#define GSM_SIM_CRSM_READ_RECORD_RES          0x8B
#define GSM_SIM_GET_ALS_RES                   0x8C
#define GSM_SIM_SET_ALS_RES                   0x8D
#define GSM_SIM_CRSM_UPDATE_RECORD_RES        0x8E
#define GSM_SIM_SELECT_SM_STORAGE_RES         0x8F
#define GSM_SIM_SM_ACTION_RES                 0x90
#define GSM_SIM_AUTH_PIN2_RES                 0x91
#define GSM_SIM_CDCI_NOTI                     0x92
#define GSM_SIM_SEC_STATUS_RES                0x93
#define GSM_SIM_PB_CAPACITY_RES               0x94
#define GSM_PB_ACCESS_RES					0x95
#define GSM_PB_STORAGE_RES					0x96
#define GSM_SIM_PHONELOCK_STATUS_RES			0x97
#define GSM_SIM_SET_PHONELOCK_RES			0x98
#define GSM_SIM_CHANGE_LOCKING_PW_RES		0x99
#define GSM_PB_ACCESS_ERROR_RES				0x9A
#define GSM_SIM_PUK_RES						0x9B
#define GSM_SIM_PUK2_RES						0x9C
#define GSM_SIM_LOCK_INFO_RES				0x9D
#define GSM_PB_ENTRY_INFO_RES				0x9E
#define GSM_SIM_LOCK_PASSWORD_RES             0x9F
// sunnyfan, 2005.12.16, for CPHS [
#define GSM_SIM_SET_CFL_REQ					0xB1
#define GSM_SIM_SET_CFL_RES					0xB2
#define GSM_SIM_SET_VMW_REQ					0xB3
#define GSM_SIM_SET_VMW_RES					0xB4
// 2006.01.02
#define GSM_SIM_EF_SELECT_REQ				0xB5
#define GSM_SIM_EF_SELECT_RES				0xB6
#define GSM_SIM_PIN_RES						0xB7
//
#define GSM_SIM_INIT_REQ						0xB8//state
#define GSM_SIM_FRIZEN						0xb9//state
#define GSM_SIM_PIN2_REQ						0xba
#define GSM_SIM_READY						0xbb//state
#define GSM_SIM_WRONG_STATUS_TYPE                        0xbc
#define GSM_SIM_STATUS_SET                  		          0xbd
#define GSM_SIM_CHANGE_PASS_SET                  		  0xbe
#define GSM_SIM_FACILITY_SET                  		         0xbf
#define GSM_SIM_NO_SIM                  		                 0xa0 //state
#define GSM_SIM_PIN_REQ_STATE				0xa1
#define GSM_SIM_FDN_STATE					0xa2
#define GSM_SIM_FACILITY_PIN					0xa3
#define GSM_SIM_FACILITY_FDN					0xa4
#define GSM_SIM_PIN2_OK						0xa5
#define GSM_SIM_FACILITY_RES					0xa6
#define GSM_SIM_GET_SIM_DB_REQ				0xa7
#define GSM_SIM_GET_SIM_DB_RES				0xa8
#define GSM_SIM_MAKE_PB_DB_REQ				0xa9
#define GSM_SIM_ADD_PB_DB_REQ				0xaa
#define GSM_SIM_GET_PB_DB_REQ				0xab
#define GSM_SIM_DELETE_PB_DB_REQ				0xac
#define GSM_SIM_GET_PB_ST_INFO_RES			0xad
#define GSM_SIM_SET_PB_STORAGE_REQ			0xae
#define GSM_SIM_GET_ALL_PB_DB_REQ                          0xaf
#define GSM_SIM_ADD_EVENT_PB_DB_REQ                    0xb0
#define GSM_SIM_DELETE_EVENT_PB_DB_REQ                          0xb1
#define GSM_SIM_PROTECT_SIM_DB                          0xc0
#define GSM_SIM_PUKSET_REQ                       0xc1 //set
#define GSM_SIM_PUK2SET_REQ                       0xc2 //set
   
#define	GSM_SIMINFO_REQ							0xD1	//get SIMINFO
#define	GSM_SIMDATA_REQ							0xD2	//get SIMDATA
#define	GSM_SIMINFO_RES							0xD3	//get SIMINFO
#define	GSM_SIMDATA_RES							0xD4	//get SIMDATA
#define GSM_SIMSAVE_REQ							0xD5	//get SIMSAVE
#define GSM_SIM_REINIT_REQ							0xD6	//get SIMSAVE
#define GSM_SIM_ERR_ADD_NOTI					0xD7
#define GSM_SIM_ERR_DEL_NOTI					0xD8

//
//SAT
#define GSM_SAT_CMD_MENU_SELECTION                     	0xd3

#define GSM_SAT_CMD_DISPLAY_TEXT                         	0x01
#define GSM_SAT_CMD_GET_INKEY                        	 	0x02
#define GSM_SAT_CMD_GET_INPUT                        	 	0x03
#define GSM_SAT_CMD_SET_UP_MENU                     	0x04
#define GSM_SAT_CMD_SELECT_ITEM	                     	0x05
#define GSM_SAT_CMD_SEND_SMS	                     	0x06
#define GSM_SAT_CMD_SET_UP_CALL	                     	0x07

// 090908 for FUNC_EDB
#define FUNC_GET_PB_DB_REQ	                     	0x01
#define FUNC_DEL_PB_DB_REQ	                     	0x02
#define FUNC_ADD_PB_DB_REQ	                     	0x03
#define FUNC_GET_ONE_PB_DB_REQ	                   	0x04













    
    
     
/* --------------------------------------------------------------------------
   GSM_DATA : Data Service Functions
   --------------------------------------------------------------------------*/
/*
 * Length : VAR
 * Data   :
 * - APN Name (VAR)
 */
#define GSM_DATA_GPRS_REQ                     0xA1

/*
 * Length : VAR
 * Data   :
 * - Dial Number (VAR)
 */
#define GSM_DATA_GSM_REQ                      0xA2

// EMTRACE_I878 [
/*
 * Length : VAR
 * Data   :
 * nsclass : for multiple PDP
 */
#define GSM_DATA_IP_ADDR_REQ						0xA3

/*
 * Length : VAR
 * Data   :
 * nsclass : for multiple PDP
 */
#define GSM_DATA_CLOSE_REQ						0xA4
// EMTRACE_I878, civily YK11 ]

// nsclass
#define GSM_DATA_EXTERNAL_HANGUP_REQ				0xA5

/*
 * Length : 0
 */
#define GSM_DATA_HANGUP_REQ                   0xA6

/* --------------------------------------------------------------------------
   GSM_DATA : Notification or Response
   --------------------------------------------------------------------------*/
/*
 * Length : 1
 * - State        (1byte) see mzgsmnet.h
 * GPRS state and Network Registration state are similar.
 */
#define GSM_DATA_GPRS_REG_NOTI                0xA1

/*
 * Length : NONE
 */
#define GSM_DATA_GPRS_RES                     0xA2

/*
 * Length : NONE
 */
#define GSM_DATA_GSM_RES                      0xA3

/*
 * Length : 1
 * dummy 1 byte (doesn't be used)
 */
#define GSM_DATA_CSD_CONNECT_NOTI             0xA4

/*
 * Length : 0
 */
#define GSM_DATA_CSD_FAILED_NOTI              0xA5

// nsclass : multiple PDP
#define GSM_DATA_IP_ADDR_RES					 	0xA6

// nsclass : multiple PDP status
#define GSM_DATA_STATUS_RES						0xA7

// nsclass : for data relase noti
#define GSM_DATA_PDP_CONNECT_NOTI				0xA8

// nsclass : for data relase noti
#define GSM_DATA_PDP_DISCONNECT_NOTI				0xA9

// nsclass : for data svc noti
#define GSM_DATA_EXTERNAL_SVC_NOTI				0xAA

/*
 * Length : 2
 * GSM_DataService 1 byte (mzgsmnet.h)
 * GSM_DataStatus  1 byte (mzgsmnet.h)
 */
#define GSM_DATA_SERVICE_STATUS_RES           0xAB

/*
 * Length : 1
 */
#define GSM_DATA_HANGUP_RES		0xAC

/*
 * Length : 4
 * rx : 2byte
 * tx : 2byte
 */
#define GSM_DATA_SMPCTR_NOTI		0xAD

/*
 * Length : 1
 * 0: stop, 1: start
 */
#define GSM_DATA_STATE				0xAE

/*
 * Length : 1
 * 0: stop, 1: start
 */
#define GSM_DATA_STATUS			0xAE

// add by mckim: for informing evt injector with the initial data restored from DB
#define GSM_DATA_GPRS_INIT_REQ	0xAF

/* --------------------------------------------------------------------------
   GSM_EXT : Gong Jung mode
   --------------------------------------------------------------------------*/
/*
 * Length : IMEI length
 * Data   : 
 * - IMEI Value
 */
#define GSM_EXT_SET_IMEI_REQ                  0xB1

/*
 * Length : BTMAC length
 * Data   : 
 * - BTMAC Value
 */
#define GSM_EXT_SET_BTMAC_REQ                 0xB2

/*
 * Length : 0
 * Data   : 
 * - None
 */
#define GSM_EXT_GET_BTMAC_REQ                 0xB3

/*
 * Length : 1
 * Data   : 
 * - mode : up Or down
 */
#define GSM_EXT_SET_VOICE_VOL_REQ             0xB4

/*
 * Length : 1
 * Data   : 
 * - path : Audio path
 */
#define GSM_EXT_SET_AUDIO_PATH_REQ            0xB5

/*
 * Length : 1
 * Data   : 
 * - path : Phone State
 */
#define GSM_EXT_SET_INTERNAL_STATE_REQ        0xB6
/* --------------------------------------------------------------------------
   GSM_EXT : Notification or Response
   --------------------------------------------------------------------------*/
/*
 * Length : 1
 * Data   : 
 * - result
 */
#define GSM_EXT_SET_IMEI_RES                  0xB1

/*
 * Length : 1
 * Data   : 
 * - result
 */
#define GSM_EXT_SET_BTMAC_RES                 0xB2

/*
 * Length : 1 + VAR
 * Data   : 
 * - mac length
 * - mac value
 */
#define GSM_EXT_GET_IMEI_RES                  0xB3

/*
 * Length : 1
 * Data   : 
 * - result
 */
#define GSM_EXT_BATT_INFO_NOTI                0xB4

/*
 * Length : 1
 * Data   : 
 * - result
 */
#define GSM_EXT_PHONE_POWER_ON_RES            0xB5

// sunnyfan, 2005.09.26 [
#define GSM_SUPS_GENRES_CW					 0xA0
#define GSM_SUPS_GENRES_CLI					 0xA1
#define GSM_SUPS_GENRES_CB					 0xA2
#define GSM_SUPS_GENRES_CBPW					 0xA3
#define GSM_SUPS_GENRES_CF					 0xA4
#define GSM_SUPS_GENRES_MANAGE				 0xA5
#define GSM_SUPS_GENRES_USSD					 0xA6
#define GSM_SUPS_GENRES_SSINFO				 0xA7
#define GSM_SUPS_GENRES_SSMAX				 0xA8
#define GSM_SUPS_GENRES_UNKNOWN				 0xAA
#define GSM_SUPS_INFORMATION					 0xB0
// sunnyfan ]


// sunnyfan, 2005.11.04 [ for SAT Response
#define GSM_GENRES_CALLOUTGOING					 0xB6
// sunnyfan ]


/* --------------------------------------------------------------------------
   GSM_SMS : SMS Functions
   --------------------------------------------------------------------------*/
#define GSM_SMS_SEND_MSG_REQ					0x01
#define GSM_SMS_READ_MSG_REQ					0x02
#define GSM_SMS_SAVE_MSG_REQ					0x03
#define GSM_SMS_DEL_MSG_REQ					0x04
#define GSM_SMS_SEND_ACK_REQ					0x05
#define GSM_SMS_GET_SEL_MEM_REQ				0x06
#define GSM_SMS_SET_SEL_MEM_REQ				0x07
#define GSM_SMS_STORED_MSG_CNT_REQ			0x08
#define GSM_SMS_GET_MSG_CONFIG_REQ			0x09
#define GSM_SMS_SET_MSG_CONFIG_REQ			0x0A
#define GSM_SMS_GET_SVC_OPTION_REQ			0x0B
#define GSM_SMS_SET_SVC_OPTION_REQ			0x0C
#define GSM_SMS_MEM_FULL_REQ					0x0D
// by luxmea, 2005.11.16 [
#define GSM_SMS_GET_CB_CONFIG_REQ        0x0E
#define GSM_SMS_SET_CB_CONFIG_REQ        0x0F
#define GSM_SMS_SET_MSG_STATUS_REQ       0x10
#define GSM_SMS_SET_BLK_LIST_REQ         0x11
// by luxmea ]
#define GSM_SMS_INCOMING_CB_MSG_REQ		0x12
  
/* --------------------------------------------------------------------------
   GSM_SMS : Notification or Response
   --------------------------------------------------------------------------*/
#define GSM_SMS_INCOMING_MSG_NOTI			0x01
#define GSM_SMS_READ_MSG_RES					0x02
#define GSM_SMS_RECEIVE_ACK_NOTI				0x03
#define GSM_SMS_SEL_MEM_RES					0x04
#define GSM_SMS_STORED_MSG_CNT_RES			0x05
#define GSM_SMS_MSG_CONFIG_RES				0x06
#define GSM_SMS_SVC_OPTION_RES				0x07
#define GSM_SMS_MEM_FULL_NOTI				0x08
#define GSM_SMS_ACTION_STATUS_NOTI			0x09
// by luxmea, 2005.11.16 [
#define GSM_SMS_CB_CONFIG_RES        0x0A
// by luxmea ]
#define GSM_SMS_STORED_MSG_CNT_NOTI           0x0B        // by luxmea
#define GSM_SMS_RECEIVE_RES		0x0C


/* --------------------------------------------------------------------------
   GSM_SAT : SAT functions
   --------------------------------------------------------------------------*/
#define GSM_SAT_PROFILE_DOWNLOAD_SET               0x01
#define GSM_SAT_ENVELOPE_COMMAND_REQ               0x02
#define GSM_SAT_PROACTIVE_COMMAND_GET              0x03
#define GSM_SAT_PROVIDE_LOCAL_INFOMATION_GET       0x04
#define GSM_SAT_POLLING_GET                        0x05
#define GSM_SAT_REFRESH_GET                        0x06
#define GSM_SAT_SETUP_EVENT_LIST_GET               0x07
#define GSM_SAT_TERMINATE_SESSION_SET              0x08
#define GSM_SAT_PROACTIVE_COMMAND_SEND              0x09
//////////////////////////////////////////////////////////////////////////
#define GSM_SAT_PROACTIVE_DISPLAY_TEXT              0xa0
#define GSM_SAT_PROACTIVE_GET_INKEY          	       0xa1
#define GSM_SAT_PROACTIVE_GET_INPUT       	       0xa2
#define GSM_SAT_PROACTIVE_SET_MENU       	       0xa3
#define GSM_SAT_PROACTIVE_SELECT_ITEM       	       0xa4
#define GSM_SAT_PROACTIVE_SEND_SMS       	       0xa5
#define GSM_SAT_PROACTIVE_SET_UP_CALL   	       0xa6



/* --------------------------------------------------------------------------
   GSM_SAT : Notification or Response
   --------------------------------------------------------------------------*/
#define GSM_SAT_PROACTIVE_COMMAND_INDICATION       0x01
#define GSM_SAT_PROACTIVE_COMMAND_RESPONSE         0x02
#define GSM_SAT_ENVELOPE_COMMAND_NOTIFICATION      0x03
#define GSM_SAT_PROVIDE_LOCAL_INFOMATION_RESPONSE  0x04
#define GSM_SAT_POLLING_RESPONSE                   0x05
#define GSM_SAT_REFRESH_RESPONSE                   0x06
#define GSM_SAT_REFRESH_NOTIFICATION               0x07
#define GSM_SAT_SETUP_EVENT_LIST_RESPONSE          0x08
///////////////////////////////////////////////////////////////////////////



/* --------------------------------------------------------------------------
   GSM_SOUND : Sound functions
   --------------------------------------------------------------------------*/

#define GSM_SND_KEY_TONE_REQ						0x01
#define GSM_SND_NOTI_TONE_REQ					0x02
#define GSM_SND_LED_CTRL_REQ						0x03
#define GSM_SND_VIB_CTRL_REQ						0x04
#define GSM_SND_GET_SPKR_VOLUME_CTRL_REQ			0x05
#define GSM_SND_SET_SPKR_VOLUME_CTRL_REQ			0x06
#define GSM_SND_GET_MIC_GAIN_CTRL_REQ			0x07
#define GSM_SND_SET_MIC_GAIN_CTRL_REQ			0x08
#define GSM_SND_GET_MIC_MUTE_CTRL_REQ			0x09
#define GSM_SND_SET_MIC_MUTE_CTRL_REQ			0x0A
#define GSM_SND_SPKR_PHONE_CTRL_REQ				0x0B
#define GSM_SND_HFK_AUDIO_STARTSTOP_REQ			0x0C
#define GSM_SND_VOICECALL_RECORD_REPORT_REQ		0x0D
#define GSM_SND_AUDIO_PATH_CTRL_REQ				0x0E
#define GSM_SND_SET_USER_CONFIG					0x0F
#define GSM_SND_GET_GAIN_CTRL_REQ				0x10
#define GSM_SND_SET_GAIN_CTRL_REQ				0x11

/* --------------------------------------------------------------------------
   GSM_SOUND : Notification or Response
   --------------------------------------------------------------------------*/
#define GSM_SND_SPKR_VOLUME_CTRL_RES				0x01
#define GSM_SND_MIC_GAIN_CTRL_RES				0x02
#define GSM_SND_SET_MIC_MUTE_CTRL_RES			0x03
#define GSM_SND_AUDIO_SOURCE_CTRL_NOTI			0x04
#define GSM_SND_GAIN_CTRL_RES					0x05
#define GSM_SND_GAIN_CTRL_NOTI					0x06

/* --------------------------------------------------------------------------
   GSM_SERVICEMODE : Service Mode functions
   --------------------------------------------------------------------------*/
#define GSM_SVCMODE_ENTER_REQ				0x01
#define GSM_SVCMODE_END_REQ					0x02
#define GSM_SVCMODE_PROCESS_KEY_REQ			0x03
//#define GSM_SVCMODE_GET_HW_SOUND_CONFIG_REQ	0x04
//#define GSM_SVCMODE_SET_HW_SOUND_CONFIG_REQ	0x05

/* --------------------------------------------------------------------------
   GSM_SERVICEMODE : Notifications or Responses
   --------------------------------------------------------------------------*/
#define GSM_SVCMODE_ENTER_NOTI				0x01
#define GSM_SVCMODE_END_NOTI					0x02
#define GSM_SVCMODE_SCREEN_CONFIG_NOTI		0x03
#define GSM_SVCMODE_DISPLAY_NOTI				0x04
//#define GSM_SVCMODE_HW_SOUND_CONFIG_RES		0x05
//#define GSM_SVCMODE_HW_SOUND_CONFIG_NOTI		0x06

/* --------------------------------------------------------------------------
   GSM_EMULATOR
   --------------------------------------------------------------------------*/

/*
 * Length : HDLC length
 * Data   : HDLC
 * - Caller ID
 */
#define GSM_EMULATOR_HDLC_REQ				0x01
#define GSM_EMULATOR_HDLC_MODE				0x02
#define GSM_EMULATOR_HDLC_CLEAR				0x03
#define GSM_EMULATOR_SET_CALL_GENERAL_RESPONSE_ERROR		0x04
#define GSM_EMULATOR_SET_CALL_STATUS_ERROR			0x05
#define GSM_EMULATOR_SET_SCA					0x06
#define GSM_EMULATOR_SET_SS_GENERAL_RESPONSE_ERROR		0x07
#define GSM_EMULATOR_CLIENT_RELEASE_NOTI	0X08
#define GSM_EMULATOR_SET_SS_HOLD_RESPONSE_ERROR 0x09
#define GSM_EMULATOR_SET_SS_RETRIEVE_RESPONSE_ERROR 0x0a
#define GSM_EMULATOR_SET_SS_JOIN_RESPONSE_ERROR 		0x0b
#define  GSM_EMULATOR_SET_SS_SPLIT_RESPONSE_ERROR		0x0c
#define  GSM_EMULATOR_SET_SS_TRANSFER_RESPONSE_ERROR	0x0d



/* --------------------------------------------------------------------------
   GSM_POWER
   --------------------------------------------------------------------------*/

#define GSM_POWER_MODEM_ON_REQ				0x01
#define GSM_POWER_MODEM_OFF_REQ				0x02
#define GSM_POWER_BATTERY_STATUS_NOTI				0x02


/* --------------------------------------------------------------------------
   GSM_GPRS
   --------------------------------------------------------------------------*/
/*
 * Length	    : 23 bytes
 * cid		    : 1 byte
 * field_flag	    : 2 bytes
 * ip_address	    : 4 bytes
 * primary_dns	    : 4 bytes
 * secondary_dns    : 4 bytes
 * default_gateway  : 4 bytes
 * subnet_mask	    : 4 bytes
 */
//#define GSM_GPRS_IP_CONFIGURATION	0x01

#endif // _vgsm_phone_h_
