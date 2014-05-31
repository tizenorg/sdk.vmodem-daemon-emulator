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

/////////////////////////////////////////////////////////////////////



#ifndef __vgsm_data_h__
#define __vgsm_data_h__

typedef enum tagDataSignal {
    GSM_DATA_SIGNAL_NONE,
    GSM_DATA_SIGNAL_DCD,                /* 0x01 */
    GSM_DATA_SIGNAL_RX,                   /* 0x02 */
    GSM_DATA_SIGNAL_TX,                   /* 0x03 */
    GSM_DATA_SIGNAL_DTR,                /* 0x04 */
    GSM_DATA_SIGNAL_GND,                /* 0x05 */
    GSM_DATA_SIGNAL_DSR,                /* 0x06 */
    GSM_DATA_SIGNAL_RTS,                /* 0x07 */
    GSM_DATA_SIGNAL_CTS,                /* 0x08 */
    GSM_DATA_SIGNAL_RI,                  /* 0x09 */
    GSM_DATA_SIGNAL_MAX                /* 0x0A */
} gsm_data_pin_signal_e_type;

typedef enum tagDataSignalStatus {
    GSM_DATA_SIGNAL_STATUS_DETACH = 0x00,
    GSM_DATA_SIGNAL_STATUS_ATTACH = 0x01,
    GSM_DATA_SIGNAL_STATUS_ON = 0x01,
    GSM_DATA_SIGNAL_STATUS_OFF = 0x00,
} GSM_DataSignalStatus;

typedef enum{
    GSM_DATA_TE2_TYPE_UART,   /* 0x00 : UART Interface */
    GSM_DATA_TE2_TYPE_USB,     /* 0x01 : USB Interface */
    GSM_DATA_TE2_TYPE_BT,       /* 0x02 : Bluetooth Interface */
    GSM_DATA_TE2_TYPE_MAX     /* 0x03 : Max */
} gsm_data_te2_e_type;

typedef enum{
    GSM_DATA_TE2_STATUS_DETACHED,
    GSM_DATA_TE2_STATUS_ATTACHED
} gsm_data_te2_status_e_type; 

typedef struct tagDataBytesCounter {
    int cur_rx_bytes;
    int cur_tx_bytes;
    char total_rx_bytes[8]; // todo : nsclass
    char total_tx_bytes[8]; // todo : nsclass
} GSM_DataBytesCounter;

typedef struct tagDataDSTE2DialInfo {
    unsigned char length;
    char dial_digit[32];
} GSM_DataDSTE2DialInfo;

typedef struct tagDataDSTE2RateInfo {
    int tx_bytes;
    int rx_bytes;
    unsigned char report_period;
    unsigned char num_channel;
    unsigned char tx_bar_level;
    unsigned char rx_bar_level;
} GSM_DataDSTE2RateInfo;

typedef enum {
    GSM_DATA_TE2_DATA_CALL_RESULT_OK=0x00,   		/* 0x00 : OK (maybe not use) */
    GSM_DATA_TE2_DATA_CALL_RESULT_CONNECT,  		/* 0x01 : CONNECT */
    GSM_DATA_TE2_DATA_CALL_RESULT_RING,  			/* 0x02 : RING (maybe not use) */
    GSM_DATA_TE2_DATA_CALL_RESULT_NO_CARRIER,  	/* 0x03 : NO CARRIER */
    GSM_DATA_TE2_DATA_CALL_RESULT_ERROR,  		/* 0x04 : ERROR */
    GSM_DATA_TE2_DATA_CALL_RESULT_NO_DIALTONE,  	/* 0x05 : NO DIALTONE (maybe not use) */
    GSM_DATA_TE2_DATA_CALL_RESULT_BUSY,  			/* 0x06 : BUSY (maybe not use)*/
    GSM_DATA_TE2_DATA_CALL_RESULT_NO_ANSWER,   	/* 0x07 : NO ANSWER */
    GSM_DATA_TE2_DATA_CALL_RESULT_FDN,		/* 0x08 : FDN */ 
    GSM_DATA_TE2_DATA_CALL_RESULT_MAX
} gsm_data_te2_data_call_result_e_type;

typedef enum {
    GSM_DATA_CALL_REASON_NORMAL=0x00,
    GSM_DATA_CALL_REASON_REL_BY_USER,
    GSM_DATA_CALL_REASON_REGULAR_DEACTIVATION,
    GSM_DATA_CALL_REASON_LLC_SNDCP_FAILURE,
    GSM_DATA_CALL_REASON_INSUFFICIENT_RESOURCES,
    GSM_DATA_CALL_REASON_MISSING_OR_UNKNOWN_APN,          
    GSM_DATA_CALL_REASON_UNKNOWN_PDP_ADDRESS_OR_TYPE,
    GSM_DATA_CALL_REASON_USER_AUTHENTICATION_FAILED,
    GSM_DATA_CALL_REASON_ACTIVATION_REJECTED_BY_GGSN,
    GSM_DATA_CALL_REASON_ACTIVATION_REJECTED_UNSPECIFIED,
    GSM_DATA_CALL_REASON_SERVICE_OPTION_NOT_SUPPORTED, 				
    GSM_DATA_CALL_REASON_SERVICE_NOT_SUBSCRIBED, 
    GSM_DATA_CALL_REASON_SERVICE_OUT_OR_ORDER,
    GSM_DATA_CALL_REASON_NSAPI_ALREADY_USED,
    GSM_DATA_CALL_REASON_QOS_NOT_ACCEPTED,
    GSM_DATA_CALL_REASON_NETWORK_FAILURE,
    GSM_DATA_CALL_REASON_REACTIVATION_REQUIRED,
    GSM_DATA_CALL_REASON_FEATURE_NOT_SUPPORTED,
    GSM_DATA_CALL_REASON_TFT_OR_FILTER_ERR,
    GSM_DATA_CALL_REASON_UNKNOWN_PDP_CONTEXT,
    GSM_DATA_CALL_REASON_INVALID_MSG,
    GSM_DATA_CALL_REASON_PROTOCOL_ERR,
    GSM_DATA_CALL_REASON_MOBILE_FAILURE_ERR,
    GSM_DATA_CALL_REASON_TIMEOUT_ERR,
    GSM_DATA_CALL_REASON_UNKNOWN_ERR
}gsm_data_call_reason_e_type;

typedef enum tagDataCallExternalPSCall {
    GSM_DATA_CALL_INTERNAL_PS_CALL = 0x00,
    GSM_DATA_CALL_EXTERNAL_PS_CALL = 0x01
} GSM_DataCallExternalPSCall;

typedef enum {
    GSM_DATA_SERVICE_STATE_EXTERNAL_START = 0x01,	/* 0x01 : external call start */
    GSM_DATA_SERVICE_STATE_EXTERNAL_CONNECTED,		/* 0x02 : external call connect */
    GSM_DATA_SERVICE_STATE_EXTERNAL_RELEASE,		/* 0x03 : external call release */
    GSM_DATA_SERVICE_STATE_GPRS_SUSPEND,			/* 0x04 : GPRS Suspended (NOT implemented, PDA implemnted dormant status) */
    GSM_DATA_SERVICE_STATE_GPRS_RESUME,				/* 0x05 : GRPS RESUMED (NOT implemented, PDA implemnted dormant status) */
    GSM_DATA_SERVICE_STATE_MAX
} GSM_DataSvcStateType;

typedef struct tagDataCallStatus {
    unsigned char cid;
    gsm_data_call_reason_e_type data_call_state;
    gsm_data_call_reason_e_type data_call_state_rsn;
    GSM_DataCallExternalPSCall external_ps_call;
} GSM_DataCallStatus;

#endif // __vgsm_data_h__

