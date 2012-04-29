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

#ifndef _LINUXTAPI4GSM_TYPE_H_
#define _LINUXTAPI4GSM_TYPE_H_


#ifdef __cplusplus
extern "C"
{
#endif


// ++++++++++++++++++++++++++++++++++++++++++++++include about standard library

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++include user define

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++define area

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++global variable area

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++function prototype

// sups. Basic Service Group
#define BS_GROUP_ALL_TELESERVICES                                           "10"
#define BS_GROUP_TELEPHONY                                                  "11"
#define BS_GROUP_ALL_DATA_TELESERVICES                                      "12"
#define BS_GROUP_FACSIMILE_SERVICES                                         "13"
#define BS_GROUP_SHORT_MESSAGE_SERVICES                                     "16"
#define BS_GROUP_ALL_TELESERVICES_EXCEPT_SMS                                "19"
#define BS_GROUP_VOICE_GROUP_CALL_SERVICE                                   "17"
#define BS_GROUP_VOICE_BROADCAST_SERVICE                                    "18"
#define BS_GROUP_ALL_BEARER_SERVICES                                        "20"
#define BS_GROUP_ALL_ASYNC_SERVICES                                         "21"
#define BS_GROUP_ALL_SYNC_SERVICES                                          "22"
#define BS_GROUP_ALL_DATA_CIRCUIT_SYNC                                      "24"
#define BS_GROUP_ALL_DATA_CIRCUIT_ASYNC                                     "25"
#define BS_GROUP_ALL_DEDICATED_PACKET_ACCESS                                "26"
#define BS_GROUP_ALL_DEDICATED_PAD_ACCESS                                   "27"
#define BS_GROUP_ALL_GPRS_BEARER_SERVICES                                   "99"

// sups. Service Code
#define SS_CODE_02_67_EMLPP                                                 "75"
#define SS_CODE_02_72_CD                                                    "66"
#define SS_CODE_02_81_CLIP                                                  "30"
#define SS_CODE_02_81_CLIR                                                  "31"
#define SS_CODE_02_81_COLP                                                  "76"
#define SS_CODE_02_81_COLR                                                  "77"
#define SS_CODE_02_82_CFU                                                   "21"
#define SS_CODE_02_82_CF_BUSY                                               "67"
#define SS_CODE_02_82_CF_NO_REPLY                                           "61"
#define SS_CODE_02_82_CF_NOT_REACHABLE                                      "62"
#define SS_CODE_02_82_ALL_CF                                                "002"
#define SS_CODE_02_82_ALL_CONDITIONAL_CF                                    "004"
#define SS_CODE_02_83_WAIT                                                  "43"
#define SS_CODE_02_87_UUS_SERVICE_1                                         "361"
#define SS_CODE_02_87_UUS_SERVICE_2                                         "362"
#define SS_CODE_02_87_UUS_SERVICE_3                                         "363"
#define SS_CODE_02_87_ALL_UUS_SERVICES                                      "360"
#define SS_CODE_02_88_BAOC                                                  "33"
#define SS_CODE_02_88_BAOIC                                                 "331"
#define SS_CODE_02_88_BAOIC_EXC_HOME                                        "332"
#define SS_CODE_02_88_BAIC                                                  "35"
#define SS_CODE_02_88_BAIC_ROAMING                                          "351"
#define SS_CODE_02_88_ALL_BARRING_SERV                                      "330"
#define SS_CODE_02_88_OUTG_BARR_SERV                                        "333"
#define SS_CODE_02_88_INC_BARR_SERV                                         "353"
#define SS_CODE_02_91_ECT                                                   "96"
#define SS_CODE_02_93_CCBS                                                  "37"
#define SS_CODE_02_95_SPNP                                                  "07n"
#define SS_CODE_02_96_CNAP                                                  "300"
#define SS_CODE_02_97_MSP                                                   "59n"

// sups. param id(for request)
#define SUPS_PARAM_ID_SS                                                0x00000001
#define SUPS_PARAM_ID_DN                                                0x00000002
#define SUPS_PARAM_ID_PW                                                0x00000004
#define SUPS_PARAM_ID_BS                                                0x00000008
#define SUPS_PARAM_ID_T                                                 0x00000010
#define SUPS_PARAM_ID_USSD                                              0x00000020

// sups. user confirm id(for request)
#define SUPS_USER_CONFIRM_ID_USS_RES_TYPE                               0x00000001
#define SUPS_USER_CONFIRM_ID_USS_RES_MESG                               0x00000002
#define SUPS_USER_CONFIRM_ID_USS_GUIDANCE_INFO                          0x00000004
#define SUPS_USER_CONFIRM_ID_USS_PASSWORD_RES                           0x00000008

// sups. user confirm(for request)
#define SUPS_CONFIRM_ID_USS_NOTIFY_RES                                  0x00000001


// sups. confirm id(for response)
#define SUPS_CFM_ID_SS                                                  0x00000001
#define SUPS_CFM_ID_BS                                                  0x00000002
#define SUPS_CFM_ID_BSG_LIST                                            0x00000004
#define SUPS_CFM_ID_SS_STATUS                                           0x00000008
#define SUPS_CFM_ID_FWD_BS_CODE                                         0x00000010
#define SUPS_CFM_ID_FWD_STATUS                                          0x00000020
#define SUPS_CFM_ID_FWD_TO_NUM                                          0x00000040
#define SUPS_CFM_ID_FWD_OPTION                                          0x00000080
#define SUPS_CFM_ID_NR_TIMER                                            0x00000100
#define SUPS_CFM_ID_FWD_INFO_SS_CODE                                    0x00000200
#define SUPS_CFM_ID_CLIR_STATUS                                         0x00000400
#define SUPS_CFM_ID_CLIR_OPTION                                         0x00000800
#define SUPS_CFM_ID_CC_CAUSE                                            0x00001000
#define SUPS_CFM_ID_SS_ERROR                                            0x00002000
#define SUPS_CFM_ID_BLOCK_IDS                                           0x00004000
#define SUPS_CFM_ID_BAR_SS_CODE                                         0x00008000
#define SUPS_CFM_ID_BAR_BS_CODE                                         0x00010000
#define SUPS_CFM_ID_BAR_STATUS                                          0x00020000
#define SUPS_CFM_ID_NEW_PW                                              0x00040000
#define SUPS_CFM_ID_LL_CAUSE                                            0x00080000
#define SUPS_CFM_ID_USS_DATA                                            0x00100000


// sups. network originated indication(for indication)
#define SUPS_NETWORK_INDICATION_USS_INVOKE_ID                           0x00000001
#define SUPS_NETWORK_INDICATION_USS_IND_DATA                            0x00000002
#define SUPS_NETWORK_INDICATION_PWD_INVOKE_ID                           0x00000004
#define SUPS_NETWORK_INDICATION_GUIDANCE_INFO                           0x00000008


    // call. status notification
    typedef enum
    {
        GSM_CALL_STATUS_UNKNOWN                                             =0x00,
        GSM_CALL_STATUS_WAITING_CONNECTION                                  =0x01,
        GSM_CALL_STATUS_CONNECTED                                           =0x02,
        GSM_CALL_STATUS_WAITING_FOR_USER_ANSWER                             =0x03,
        GSM_CALL_STATUS_RECALL                                              =0x04,
        GSM_CALL_STATUS_UNAVAILABLE                                         =0x05,
        GSM_CALL_STATUS_CALL_CLEARED                                        =0x06,
        GSM_CALL_STATUS_RELEASED_OR_FAIL                                    =0x07,
        GSM_CALL_STATUS_PROGRESS		                                    =0x08
    }
    LXT_GSM_CALL_STATUS;

    // call. alert info
    typedef enum
    {
        GSM_CALL_ALERT_INFO_TYPE_DISPLAY                                    =0x01,
        GSM_CALL_ALERT_INFO_TYPE_CALLED_PARTY                               =0x02,
        GSM_CALL_ALERT_INFO_TYPE_CALLING_PARTY                              =0x03,
        GSM_CALL_ALERT_INFO_TYPE_CONNECTED_NUMBER                           =0x04,
        GSM_CALL_ALERT_INFO_TYPE_REPLACE_BY_SIGNALING_REPORT                =0x05,
        GSM_CALL_ALERT_INFO_TYPE_MESSAGE_WAITING                            =0x06,
        GSM_CALL_ALERT_INFO_TYPE_SERVICE_CONFIGURATION                      =0x07
    }
    LXT_GSM_CALL_ALERT_INFO_TYPE;

    // call. presentation indicator
    typedef enum
    {
        GSM_CALL_PI_ALLOWED                                                 =0x00,
        GSM_CALL_PI_RESTRICTED                                              =0x01,
        GSM_CALL_PI_NUMBER_NOT_AVAILABLE                                    =0x02,
        GSM_CALL_PI_RESERVED                                                =0x03
    }
    LXT_GSM_CALL_PI;

    // call. sups status
    typedef enum
    {
        GSM_CALL_SUPS_STATUS_CALL_BARRED                                    =0x01,
        GSM_CALL_SUPS_STATUS_CALL_IS_IN_WAITING                             =0x02,
        GSM_CALL_SUPS_STATUS_CALL_RESTRICTED                                =0x03,
        GSM_CALL_SUPS_STATUS_DEFLECTED_CALL                                 =0x04,
        GSM_CALL_SUPS_STATUS_TRANSFERRED_CALL                               =0x05,
        GSM_CALL_SUPS_STATUS_CUG_INFO_RECEIVED                              =0x06,
        GSM_CALL_SUPS_STATUS_ORIG_FWD_STATE                                 =0x07,
        GSM_CALL_SUPS_STATUS_CALL_FORWARDED                                 =0x08,
        GSM_CALL_SUPS_STATUS_CALL_BEING_FORWARDED                           =0x09
    }
    LXT_GSM_CALL_SUPS_STATUS;

    // mpty. operation
    typedef enum
    {
        ENTERING_NO_OPERATION                                               =0x00,
        ENTERING_0_SEND                                                     =0x01,
        ENTERING_1_SEND                                                     =0x02,
        ENTERING_1_X_SEND                                                   =0x03,
        ENTERING_2_SEND                                                     =0x04,
        ENTERING_2_X_SEND                                                   =0x05,
        ENTERING_3_SEND                                                     =0x06,
        ENTERING_4_SEND                                                     =0x07,
        ENTERING_4_DN_SEND                                                  =0x08
    }
    LXT_GSM_MPTY_OPERATION;

    // mpty. param id
    typedef enum
    {
        MPTY_PARAM_ID_NONE                                                  =0x00,
        MPTY_PARAM_ID_X                                                     =0x01,
        MPTY_PARAM_ID_DN                                                    =0x02
    }
    LXT_GSM_MPTY_PARAM_ID;

    // mpty. cfrm id
    typedef enum
    {
        GSM_MPTY_CFM_ID_MPTY_CFRM_DATA_ID_NONE                              =0x00,
        GSM_MPTY_CFM_ID_ACTIVE_CALL_LIST                                    =0x01,
        GSM_MPTY_CFM_ID_SS_ERROR                                            =0x02
    }
    LXT_GSM_MPTY_CFM_ID;

    // sups. operation
    typedef enum
    {
        NO_OPERATION                                                        =0x00,
        REGISTRATION                                                        =0x01,
        ACTIVATION                                                          =0x02,
        DEACTIVATION                                                        =0x03,
        ERASURE                                                             =0x04,
        INTERROGATION                                                       =0x05,
        USSD                                                                =0x06,
        USSD_RESPONSE                                                       =0x07,
        USSD_NOTIFY_RESPONSE                                                =0x08,
        PASSWORD_REGISTRATION                                               =0x09
    }
    LXT_GSM_SUPS_OPERATION;


    // sups. network originated indication ooperation
    typedef enum
    {
        USS_INDICATION                                                      =0x01,
        USS_NOTIFY_INDICATION                                               =0x02,
        PASSWORD_INDICATION                                                 =0x03
    }
    LXT_GSM_SUPS_NETWORK_INDICATION_OPERATION;


    // sups. user confirm operation
    typedef enum
    {
        USS_RES                                                             =0x01,
        USS_NOTIFY_RES                                                      =0x02,
        USS_EDIT                                                            =0x03,
        USS_ABORT                                                           =0x04,
        USS_DISPLAY_TIMEOUT                                                 =0x05,
        USS_TIMER                                                           =0x06,
        PASSWORD_RES                                                        =0x07
    }
    LXT_GSM_SUPS_USER_CONFIRM_OPERATION;

    // sups. user confirm -- uss res type
    typedef enum
    {
        USS_RES_TYPE_UNKNOWN_ALPHABET                                       =0x00,
        USS_RES_TYPE_DISPLAY_BUSY                                           =0x01,
        USS_RES_TYPE_OK                                                     =0x02,
        USS_RES_TYPE_NONE                                                   =0x03
    }
    LXT_GSM_SUPS_USER_CONFIRM_USS_RES_TYPE;

    // sups. user confirm -- uss res type
    typedef struct
    {
        char uss_msg_len;
        char uss_msg[64];
    }
    LXT_GSM_SUPS_USER_CONFIRM_USS_RES_MSG_TYPE;

    // sups. user confirm -- uss guidance info type
    typedef enum
    {
        USS_GUIDANCE_INFO_TYPE_ENTER_PW                                     =0x00,
        USS_GUIDANCE_INFO_TYPE_ENTER_NEW_PW                                 =0x01,
        USS_GUIDANCE_INFO_TYPE_ENTER_NEW_PW_AGAIN                           =0x02,
        USS_GUIDANCE_INFO_TYPE_BAD_PW_TRY_AGAIN                             =0x03,
        USS_GUIDANCE_INFO_TYPE_BAD_PW_FORMAT_TRY_AGAIN                      =0x04
    }
    LXT_GSM_SUPS_USER_CONFIRM_USS_GUIDANCE_INFO_TYPE;
    // sups. user confirm -- uss password res type
    typedef struct
    {
        char pw_len;
        char pw[8];
    }
    LXT_GSM_SUPS_USER_CONFIRM_USS_PASSWORD_RES_TYPE;


    // sups. result
    typedef enum
    {
        FAILURE                                                             =0x00,
        SUCCESS                                                             =0x01,
    }
    LXT_GSM_SUPS_RESULT;

    // sups. error class
    typedef enum
    {
        COMMAND_ERROR                                                       =0x01,
        USER_DEFINED_ERROR                                                  =0x02
    }
    LXT_GSM_SUPS_ERROR_CLASS;

    // sups. error reason(command error)
    typedef enum
    {
        LXT_GSM_SUPS_ERROR_REASON_1_NO_ERROR                                =0x00,
        LXT_GSM_SUPS_ERROR_REASON_1_INVALID_CLIENT_ID                       =0x01,
        LXT_GSM_SUPS_ERROR_REASON_1_NULL_PARAM                              =0x02,
        LXT_GSM_SUPS_ERROR_REASON_1_OFFLINE_STATE                           =0x03,
        LXT_GSM_SUPS_ERROR_REASON_1_SRV_STATE_INVALID                       =0x04,
        LXT_GSM_SUPS_ERROR_REASON_1_NO_BUFFER                               =0x05,
        LXT_GSM_SUPS_ERROR_REASON_1_OTHER                                   =0x06,
        LXT_GSM_SUPS_ERROR_REASON_1_UNDEFINED_FEATURE                       =0x07
    }
    LXT_GSM_SUPS_ERROR_REASON_1;


    // sups. error reason(user define error)
    typedef enum
    {
        LXT_GSM_SUPS_ERROR_REASON_2_NO_ERROR                                =0x00,
        LXT_GSM_SUPS_ERROR_REASON_2_UNKNOWN_ERROR                           =0x01,
        LXT_GSM_SUPS_ERROR_REASON_2_NULL_AT_COM_PTR                         =0x02,
        LXT_GSM_SUPS_ERROR_REASON_2_INVALID_PARAM                           =0x03,
        LXT_GSM_SUPS_ERROR_REASON_2_UNKNOWN_MSG                             =0x04,
        LXT_GSM_SUPS_ERROR_REASON_2_OPER_TIMEOUT                            =0x05,
        LXT_GSM_SUPS_ERROR_REASON_2_AUTO_NETSEL_ERROR                       =0x06,
    }
    LXT_GSM_SUPS_ERROR_REASON_2;


    // sups. structure
    typedef struct _lxt_gsm_sups_originated
    {
        LXT_GSM_SUPS_OPERATION operation;       // operation
        int mask;                               // mask
        char ss[4];                             // service code
        char dn[64];                            // direct number
        char pw[8];                             // password
        char bs[4];                             // base group
        int  t_nr;                              // no reply timer
        char ussd[256];                         // unstructured ss data
        int length;                             // data length (for expention)
        void* data;                             // data (for expention)
    }
    LXT_GSM_SUPS_ORIGINATED;


    // sups. user confirm
    typedef struct _lxt_gsm_sups_user_confirm
    {
        LXT_GSM_SUPS_USER_CONFIRM_OPERATION operation;
        int mask;
        LXT_GSM_SUPS_USER_CONFIRM_USS_RES_TYPE res_type;
        LXT_GSM_SUPS_USER_CONFIRM_USS_RES_MSG_TYPE res_mesg;
        LXT_GSM_SUPS_USER_CONFIRM_USS_GUIDANCE_INFO_TYPE guidance_info_type;
        LXT_GSM_SUPS_USER_CONFIRM_USS_PASSWORD_RES_TYPE password_res_type;

    }
    LXT_GSM_SUPS_USER_CONFIRM;

    // sups. originated notification
    typedef struct _lxt_gsm_sups_originated_rx_header_status
    {
        char status;                            // 1byte
        char error_class;                       // 1byte
        char error_reason;                      // 1byte
        char operation;                         // 1byte
    }
    LXT_GSM_SUPS_ORIGINATED_RX_HEADER_STATUS;

    typedef struct _lxt_gsm_sups_originated_rx_header_data
    {
        int id_cfrm_data;                       // 4byte
        char len_data;                          // 1byte
    }
    LXT_GSM_SUPS_ORIGINATED_RX_HEADER_DATA;


    typedef enum
    {
        LXT_SUPS_CALLERID_DEFAULT                                           =0x00,
        LXT_SUPS_CALLERID_HIDE                                              =0x01,
        LXT_SUPS_CALLERID_SEND                                              =0x02
    }
    LXT_CALLER_ID_OPTION;

    typedef enum
    {
        LXT_SUPS_CCM                                                        =0x01,
        LXT_SUPS_ACM                                                        =0x02,
        LXT_SUPS_ACMMAX                                                     =0x03
    }
    LXT_CALL_COST_OPTION;

    // for register sups ==================
    typedef enum
    {
        NUMBER_IDENTIFICATION                                               =0x01,
        CALL_OFFERING                                                       =0x02,
        CALL_COMPLETION                                                     =0x03,
        CALL_RESTRICTION                                                    =0x04,
    }
    LXT_GSM_SUPS_REG_PARAMS_TYPE;

    typedef enum
    {
        FWD_INFO                                                            =0x01,
        FWD_TO_NUM_INFO                                                     =0x02,
        FWD_FEATURE_LIST_INFO                                               =0x03,
        CALL_BARRING_INFO                                                   =0x04,
        CLI_RESTRICTION_INFO                                                =0x05,
        CALLED_PARTY_NUM_INFO                                               =0x06,
        CUG_INFO                                                            =0x07,
        BSG_LIST_INFO                                                       =0x08,
        USS_DATA_INFO                                                       =0x09,
        USSD_DATA_INFO                                                      =0x0A,
        NEW_PWD_INFO                                                        =0x0B,
        SS_DATA_INFO                                                        =0x0C,
        SS_STATUS_INFO                                                      =0x0D,
        SS_ERROR_INFO                                                       =0x0E,
        CC_CAUSE_INFO                                                       =0x0F,
        LL_CAUSE_INFO                                                       =0x10,
        UNKNOWN_CONF_INFO                                                   =0x11,
        NO_INFO                                                             =0x12,
    }
    LXT_GSM_SUPS_CONF_TYPE;


    typedef enum
    {
        PERMANENT                                                           =0x01,
        TEMP_DEFAULT_RESTRICTED                                             =0x02,
        TEMP_DEFAULT_ALLOWED                                                =0x03,
    }
    LXT_GSM_SUPS_CLI_RESTRICTION_OPTION;


    typedef enum
    {
        GSM_CONFIG_RAT_UNKNOWN                                              =0x00,
        GSM_CONFIG_RAT_GSM900                                               =0x01,
        GSM_CONFIG_RAT_DCS1800                                              =0x02,
        GSM_CONFIG_RAT_PCS1900                                              =0x03,
        GSM_CONFIG_RAT_GSM_SAT                                              =0x04,
        GSM_CONFIG_RAT_UMTS                                                 =0x05,
        GSM_CONFIG_RAT_LIMITED_SRVC                                         =0x06
    }
    LXT_GSM_CONFIG_RAT;


#ifdef __cplusplus
}
#endif

#endif // _LINUXTAPI4GSM_TYPE_H_


