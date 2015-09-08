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


#ifndef __vgsm_gprs_h__
#define __vgsm_gprs_h__

typedef enum{
    GSM_GPRS_DEFINE_PDP_MODE_ADD=0x01,  	/* 0x01 : Add PDP information */
    GSM_GPRS_DEFINE_PDP_MODE_DELETE,  		/* 0x02 : Delete PDP information */
    GSM_GPRS_DEFINE_PDP_MODE_MAX
} gsm_gprs_define_pdp_mode_e_type;

typedef enum{
    GSM_GPRS_DEFINE_PDP_CONTEXT_P_TYPE_UNKNOWN=0x00,  	/* 0x00 : Unknown */
    GSM_GPRS_DEFINE_PDP_CONTEXT_P_TYPE_X25,  			/* 0x01 : ITU-T/CCITT X.25 Layer 4 */
    GSM_GPRS_DEFINE_PDP_CONTEXT_P_TYPE_IP,  			/* 0x02 : Internet Protocol (IETF STD 5) */
    GSM_GPRS_DEFINE_PDP_CONTEXT_P_TYPE_IHOST,   		/* 0x03 : Internet Hosted Octet Stream Protocol */
    GSM_GPRS_DEFINE_PDP_CONTEXT_P_TYPE_PPP,  			/* 0x04 : Point-to-Point Protocol */
    GSM_GPRS_DEFINE_PDP_CONTEXT_P_TYPE_IPV6,   		/* 0x05 : Internet Protocol, version 6(IETF RFC 2460)-Maybe not use */
    GSM_GPRS_DEFINE_PDP_CONTEXT_P_TYPE_MAX
} gsm_gprs_define_pdp_context_p_e_type;

typedef enum{
    GSM_GPRS_DEFINE_PDP_CONTEXT_D_COMP_OFF=0x00,  	/* 0x00 : Compression off (Default) */
    GSM_GPRS_DEFINE_PDP_CONTEXT_D_COMP_ON,  			/* 0x01 : Compression on */
    GSM_GPRS_DEFINE_PDP_CONTEXT_D_COMP_V42BIS, 		/* 0x02 : V42BIS (maybe not use) */
    GSM_GPRS_DEFINE_PDP_CONTEXT_D_COMP_V44,   		/* 0x03 : V44 (maybe not use) */
    GSM_GPRS_DEFINE_PDP_CONTEXT_D_COMP_MAX
} gsm_gprs_define_pdp_context_d_comp_e_type;

/* PDP Header Compression Type (H_COMP) */
typedef enum{
    GSM_GPRS_DEFINE_PDP_CONTEXT_H_COMP_OFF=0x00,  	/* 0x00 : Compression off (Default) */
    GSM_GPRS_DEFINE_PDP_CONTEXT_H_COMP_ON,  			/* 0x01 : Compression on */
    GSM_GPRS_DEFINE_PDP_CONTEXT_H_COMP_RFC1144, 		/* 0x02 : RFC1144 (maybe not use) */
    GSM_GPRS_DEFINE_PDP_CONTEXT_H_COMP_RFC2507,   	/* 0x03 : RFC2507 (maybe not use) */
    GSM_GPRS_DEFINE_PDP_CONTEXT_H_COMP_RFC3095,		/* 0x04 : RFC3095 (maybe not use) */
    GSM_GPRS_DEFINE_PDP_CONTEXT_H_COMP_MAX
} gsm_gprs_define_pdp_context_h_comp_e_type;

/* PDP Authentication Type */
typedef enum{
    GSM_GPRS_DEFINE_PDP_CONTEXT_AUTHTYPE_NONE=0x00,  	/* 0x00 : NONE */
    GSM_GPRS_DEFINE_PDP_CONTEXT_AUTHTYPE_PAP=0x01,  	/* 0x01 : PAP */
    GSM_GPRS_DEFINE_PDP_CONTEXT_AUTHTYPE_CHAP=0x02,  	/* 0x02 : CHAP */
}gsm_gprs_define_pdp_context_authtype_e_type;

typedef enum {
    GSM_GPRS_IP_CONFIGURATION_FIELD_FLAG_IP_ADDR = 0x0001,		/* 0x0001 : IP Address */
    GSM_GPRS_IP_CONFIGURATION_FIELD_FLAG_PRIMARY_DNS = 0x0002,	/* 0x0002 : Primary DNS */
    GSM_GPRS_IP_CONFIGURATION_FIELD_FLAG_SECONDARY_DNS = 0x0004,	/* 0x0004 : Secondary DNS */
    GSM_GPRS_IP_CONFIGURATION_FIELD_FLAG_DEFAULT_GATEWAY = 0x0008,	/* 0x0004 : Default Gateway */
    GSM_GPRS_IP_CONFIGURATION_FIELD_FLAG_SUBNET_MASK = 0x0010,	/* 0x0010 : Subnet Mask */
    GSM_GPRS_IP_CONFIGURATION_FIELD_FLAG_PCSCF=0x0020,                /* 0x0020 : P-CSCF Address */
    GSM_GPRS_IP_CONFIGURATION_FIELD_FLAG_MAX = 0xFFFF
} gsm_gprs_ip_configuration_field_flag_e_type;

typedef struct tagIPConfiguration {
    unsigned char cid;
    gsm_gprs_ip_configuration_field_flag_e_type fieldflag;
    unsigned char ip_addr[4];
    unsigned char primary_dns[4];
    unsigned char secondary_dns[4];
    unsigned char default_gateway[4];
    unsigned char subnet_mask[4];
} GSM_GprsIPConfiguration;

typedef enum tagGprsHSDPAState {
    GSM_GPRS_HSDPA_STATE_INACTIVE = 0x00,
    GSM_GPRS_HSDPA_STATE_ACTIVE = 0x01
} GSM_GprsHSDPAState;

typedef struct tagSetPDPContext {
    gsm_gprs_define_pdp_mode_e_type mode;
    char cid;
    gsm_gprs_define_pdp_context_p_e_type pdp_type;
    char apn[102];
    char pdp_addr[20];
    gsm_gprs_define_pdp_context_d_comp_e_type d_comp;
    gsm_gprs_define_pdp_context_h_comp_e_type h_comp;
    gsm_gprs_define_pdp_context_authtype_e_type auth;
} GSM_SetPDPContext;

typedef struct tagPDPContextInfo {
    char cid;
    char apn[102];
    char pdp_addr[20];
    char d_comp;
    char h_comp;
} GSM_PDPContextInfo;

typedef struct tagResPDPContext {
    char num_pdp; // max 3
    GSM_PDPContextInfo info[3];
} GSM_ResPDPContext;

typedef enum{
    GSM_GPRS_QOS_TYPE_REQ=0x01,   	/* 0x01 : Quality of Service Profile Type Request (+CGQREQ) */
    GSM_GPRS_QOS_TYPE_MIN,  			/* 0x02 : Quality of Service Profile Type Minimum Acceptable (+CGQMIN) */
    GSM_GPRS_QOS_TYPE_MAX
} gsm_gprs_qos_e_type;

typedef enum{
    GSM_GPRS_QOS_PREC_SUBSCRIBED=0x00,  	/* 0x00 : Subscribed by the Network / default if value is omitted */
    GSM_GPRS_QOS_PREC_HIGH,  				/* 0x01 : Service commitments shall be maintained ahead of precedence classes 2 and 3 */
    GSM_GPRS_QOS_PREC_NORMAL,  			/* 0x02 : Service commitments shall be maintained ahead of precedence class 3 */
    GSM_GPRS_QOS_PREC_LOW,   				/* 0x03 : Service commitments shall be maintained ahead of precedence classes 1 and 2 */
    GSM_GPRS_QOS_PREC_MAX
} gsm_gprs_qos_prec_e_type;

typedef enum{
    GSM_GPRS_QOS_DELAY_SUBSCRIBED=0x00,  	/* 0x00 : Subscribed by the Network / default if value is omitted */
    GSM_GPRS_QOS_DELAY_PRED1,  			/* 0x01 : Predictive 1 */
    GSM_GPRS_QOS_DELAY_PRED2,  			/* 0x02 : Predictive 2 */
    GSM_GPRS_QOS_DELAY_PRED3,   			/* 0x03 : Predictive 3 */
    GSM_GPRS_QOS_DELAY_PRED4, 			/* 0x04 : Predictive 4 */
    GSM_GPRS_QOS_DELAY_MAX
} gsm_gprs_qos_delay_e_type;

typedef enum{
    GSM_GPRS_QOS_RELI_SUBSCRIBED=0x00,  	/* 0x00 : Subscribed by the Network / default if value is omitted */
    GSM_GPRS_QOS_RELI_CLASS1,  			/* 0x01 : Reliability class 1 */
    GSM_GPRS_QOS_RELI_CLASS2,  			/* 0x02 : Reliability class 2 */
    GSM_GPRS_QOS_RELI_CLASS3,   			/* 0x03 : Reliability class 3 */
    GSM_GPRS_QOS_RELI_CLASS4, 			/* 0x04 : Reliability class 4 */
    GSM_GPRS_QOS_RELI_CLASS5, 			/* 0x05 : Reliability class 5 */
    GSM_GPRS_QOS_RELI_MAX
} gsm_gprs_qos_reli_e_type;

typedef enum{
    GSM_GPRS_QOS_PEAK_SUBSCRIBED=0x00,  	/* 0x00 : Subscribed by the Network / default if value is omitted */
    GSM_GPRS_QOS_PEAK_8K,  				/* 0x01 : 8,000 bit per second */
    GSM_GPRS_QOS_PEAK_16K,  				/* 0x02 : 16,000 bit per second */
    GSM_GPRS_QOS_PEAK_32K,   				/* 0x03 : 32,000 bit per second */
    GSM_GPRS_QOS_PEAK_64K, 				/* 0x04 : 64,000 bit per second */
    GSM_GPRS_QOS_PEAK_128K, 				/* 0x05 : 128,000 bit per second */
    GSM_GPRS_QOS_PEAK_256K, 				/* 0x06 : 256,000 bit per second */
    GSM_GPRS_QOS_PEAK_512K, 				/* 0x07 : 512,000 bit per second */
    GSM_GPRS_QOS_PEAK_1024K, 				/* 0x08 : 1,024,000 bit per second */
    GSM_GPRS_QOS_PEAK_2048K, 				/* 0x09 : 2,048,000 bit per second */
    GSM_GPRS_QOS_PEAK_MAX
} gsm_gprs_qos_peak_e_type;

/* Quality of Service Mean (MEAN) */
typedef enum{
    GSM_GPRS_QOS_MEAN_SUBSCRIBED=0x00,  	/* 0x00 : Subscribed by the Network / default if value is omitted */
    GSM_GPRS_QOS_MEAN_1,  				/* 0x01 : 0.22 bits per second */
    GSM_GPRS_QOS_MEAN_2,  				/* 0x02 : 0.44 bit per second */
    GSM_GPRS_QOS_MEAN_3,   				/* 0x03 : 1.11 bit per second */
    GSM_GPRS_QOS_MEAN_4, 					/* 0x04 : 2.2 bit per second */
    GSM_GPRS_QOS_MEAN_5, 					/* 0x05 : 4.4 bit per second */
    GSM_GPRS_QOS_MEAN_6, 					/* 0x06 : 11.1 bit per second */
    GSM_GPRS_QOS_MEAN_7, 					/* 0x07 : 22 bit per second */
    GSM_GPRS_QOS_MEAN_8, 					/* 0x08 : 44 bit per second */
    GSM_GPRS_QOS_MEAN_9, 					/* 0x09 : 111 bit per second */
    GSM_GPRS_QOS_MEAN_10,					/* 0x10 : 220 bit per second */
    GSM_GPRS_QOS_MEAN_11,					/* 0x11 : 440 bit per second */
    GSM_GPRS_QOS_MEAN_12,					/* 0x12 : 1,110 bit per second */
    GSM_GPRS_QOS_MEAN_13,					/* 0x13 : 2,200 bit per second */
    GSM_GPRS_QOS_MEAN_14,					/* 0x14 : 4,400 bit per second */
    GSM_GPRS_QOS_MEAN_15,					/* 0x15 : 11,100 bit per second */
    GSM_GPRS_QOS_MEAN_16,					/* 0x16 : 22,000 bit per second */
    GSM_GPRS_QOS_MEAN_17,					/* 0x17 : 44,000 bit per second */
    GSM_GPRS_QOS_MEAN_18,					/* 0x18 : 111,000 bit per second */
    GSM_GPRS_QOS_MEAN_31=0x31,					/* 0x31 : Best effort */
    GSM_GPRS_QOS_MEAN_MAX
} gsm_gprs_qos_mean_e_type;

typedef struct tagSetQOSType {
    gsm_gprs_define_pdp_mode_e_type mode;
    gsm_gprs_qos_e_type qos_type;
    char cid;
    gsm_gprs_qos_prec_e_type qos_precedence;
    gsm_gprs_qos_delay_e_type qos_delay;
    gsm_gprs_qos_reli_e_type qos_reliability;
    gsm_gprs_qos_peak_e_type qos_peak;
    gsm_gprs_qos_mean_e_type qos_mean;
} GSM_SetQOSType;

typedef struct tagQOSTypeInfo {
    char cid;
    gsm_gprs_qos_prec_e_type qos_prece;
    gsm_gprs_qos_delay_e_type qos_delay;
    gsm_gprs_qos_reli_e_type qos_reli;
    gsm_gprs_qos_peak_e_type qos_peak;
    gsm_gprs_qos_mean_e_type qos_mean;
} GSM_QOSTypeInfo;

typedef struct tagResQOSType {
    gsm_gprs_qos_e_type qos_type;
    char num_qos;
    GSM_QOSTypeInfo	info[3]; // max 3
} GSM_ResQOSType;

typedef enum{
    GSM_GPRS_PS_STATE_DETACHED=0x00,   	/* 0x00 : Detached */
    GSM_GPRS_PS_STATE_ATTACHED,  			/* 0x01 : Attached */
    GSM_GPRS_PS_STATE_MAX
} gsm_gprs_ps_state_e_type;

typedef enum{
    GSM_GPRS_PS_SUSPEND =0x00,   	/* 0x00 : Packet Data Transfer Status is Suspend */
    GSM_GPRS_PS_ACTIVE,  			/* 0x01 : Packet Data Transfer Status is Active(Resume) */
    GSM_GPRS_PS_TRANSFER_STATUS_MAX
} gsm_gprs_ps_transfer_e_type;

typedef enum tagGPRS_Attachment_State {
    GSM_GPRS_ATTACHMENT_STATE_DEACTIVATED = 0x00,
    GSM_GPRS_ATTACHMENT_STATE_ACTIVATED = 0x01,
} GSM_GPRS_Attachment_Status;

typedef enum {
    GSM_GPRS_PDP_CONTEXT_STATE_DETACHED=0x00,   	/* 0x00 : Detached */
    GSM_GPRS_PDP_CONTEXT_STATE_ATTACHED,  		/* 0x01 : Attached */
    GSM_GPRS_PDP_CONTEXT_STATE_CANCEL,            /* 0x02 : Cancel   */
    GSM_GPRS_PDP_CONTEXT_STATE_MAX
} gsm_gprs_pdp_context_state_e_type;

typedef struct tagPDPStateInfo {
    char cid;
    gsm_gprs_pdp_context_state_e_type state;
} GSM_PDPStateInfo;

typedef struct tagResPDPState {
    char num_act; // max 3
    GSM_PDPStateInfo info[3];
} GSM_ResPDPState;

typedef struct tagResPDPAddress {
    char address[20];
} GSM_ResPDPAddress;

typedef enum {
    GSM_GPRS_MS_CLASS_GSMANDGPRS=0x00,  				/* 0x00 : "A" : Simultaneous voice and GPRS data */
    GSM_GPRS_MS_CLASS_GSMORGPRS,  				/* 0x01 : "B" : Simultaneous voice and GPRS traffic channel, one or other data */
    GSM_GPRS_MS_CLASS_GSMORGPRS_EXCLUSIVE,  		/* 0x02 : "C" : Either all voice or all GPRS, both traffic channels unmonitored */
    GSM_GPRS_MS_CLASS_GPRSONLY,  					/* 0x03 : "CC" : Only GPRS */
    GSM_GPRS_MS_CLASS_GSMONLY,  					/* 0x04 : "CG" : Only circuit switched voice and data */
    GSM_GPRS_MS_CLASS_MAX
} gsm_gprs_ms_class_e_type;

typedef enum tagGPRS_MO_SMS_Service {
    GSM_GPRS_MO_SMS_SVC_CIRCUIT = 0x00,
    GSM_GPRS_MO_SMS_SVC_GPRS = 0x01,
    GSM_GPRS_MO_SMS_SVC_CIRCUITPREFERRED = 0x02,
    GSM_GPRS_MO_SMS_SVC_GPRSPREFERRED = 0x03,
} GSM_GPRS_MO_SMS_Service;

typedef enum {
    GSM_GPRS_3G_QOS_TYPE_REQUESTED=0x01,   		/* 0x00 : Requested(+CGEQREQ) */
    GSM_GPRS_3G_QOS_TYPE_MINIMUM_ACCEPTABLE,  	/* 0x01 : Minimum Acceptable(+CGEQMIN) */
    GSM_GPRS_3G_QOS_TYPE_NEGOTIATED,  			/* 0x02 : Negotiated(+CGEQNEG) */
    GSM_GPRS_3G_QOS_TYPE_MAX
} gsm_gprs_3g_qos_type_e_type;

typedef enum {
    GSM_GPRS_3G_QOS_TRAFFIC_CONVERSATIONAL=0x00, 	/* 0x00 : Conversational */
    GSM_GPRS_3G_QOS_TRAFFIC_STREAMING,  			/* 0x01 : Streaming */
    GSM_GPRS_3G_QOS_TRAFFIC_INTERACTIVE,  		/* 0x02 : Interactive */
    GSM_GPRS_3G_QOS_TRAFFIC_BACKGROUND,  			/* 0x03 : Background */
    GSM_GPRS_3G_QOS_TRAFFIC_SUBSCRIVED_VALUE,  	/* 0x04 : Subscrived_value */
    GSM_GPRS_3G_QOS_TRAFFIC_MAX
} gsm_gprs_3g_qos_traffic_e_type;

/* 3G_QOS_MAX_BITRATE_UL / 3G_QOS_MAX_BITRATE_DL / 3G_QOS_GUARANTEED_BITRATE_UL / 3G_QOS_GUARANTEED_BITRATE_DL */
typedef enum {
    GSM_GPRS_3G_QOS_BITRATE_SUBSCRIBED=0x00, 	/* 0x00 : Subscribed by the Network / default if value is omitted */
    GSM_GPRS_3G_QOS_BITRATE_8K,  				/* 0x01 : 8,000 bits per second (bps) */
    GSM_GPRS_3G_QOS_BITRATE_16K,  			/* 0x02 : 16,000 bps */
    GSM_GPRS_3G_QOS_BITRATE_32K,  			/* 0x03 : 32,000 bps */
    GSM_GPRS_3G_QOS_BITRATE_64K,  			/* 0x04 : 64,000 bps */
    GSM_GPRS_3G_QOS_BITRATE_128K,  			/* 0x05 : 128,000 bps */
    GSM_GPRS_3G_QOS_BITRATE_256K,  			/* 0x06 : 256,000 bps */
    GSM_GPRS_3G_QOS_BITRATE_512K,  			/* 0x07 : 512,000 bps */
    GSM_GPRS_3G_QOS_BITRATE_1024K,  			/* 0x08 : 1,024,000 bps */
    GSM_GPRS_3G_QOS_BITRATE_2048K,  			/* 0x09 : 2,048,000 bps */
    GSM_GPRS_3G_QOS_BITRATE_MAX
} gsm_gprs_3g_qos_bitrate_e_type;

/* 3G_QOS_SDU_ERROR */
typedef enum {
    GSM_GPRS_3G_QOS_SDU_ERROR=0x00, 	/* 0x00 : Subscribed by the Network / default if value is omitted */
    GSM_GPRS_3G_QOS_SDU_ERROR_1E1,  	/* 0x01 : 10^(-1) */
    GSM_GPRS_3G_QOS_SDU_ERROR_1E2,  	/* 0x02 : 10^(-2) */
    GSM_GPRS_3G_QOS_SDU_ERROR_7E3,  	/* 0x03 : 7*(10^(-3)) */
    GSM_GPRS_3G_QOS_SDU_ERROR_1E3,  	/* 0x04 : 10^(-3) */
    GSM_GPRS_3G_QOS_SDU_ERROR_1E4,  	/* 0x05 : 10^(-4) */
    GSM_GPRS_3G_QOS_SDU_ERROR_1E5,  	/* 0x06 : 10^(-5) */
    GSM_GPRS_3G_QOS_SDU_ERROR_1E6,  	/* 0x07 : 10^(-6) */
    GSM_GPRS_3G_QOS_SDU_ERROR_MAX
} gsm_gprs_3g_qos_sdu_error_e_type;

/* 3G_QOS_RESIDUAL_BIT_ERROR */
typedef enum {
    GSM_GPRS_3G_QOS_RESIDUAL_BIT_ERROR=0x00, 	/* 0x00 : Subscribed by the Network / default if value is omitted */
    GSM_GPRS_3G_QOS_RESIDUAL_BIT_ERROR_5E2,  	/* 0x01 : 5*(10^(-2)) */
    GSM_GPRS_3G_QOS_RESIDUAL_BIT_ERROR_1E2,  	/* 0x02 : 10^(-2) */
    GSM_GPRS_3G_QOS_RESIDUAL_BIT_ERROR_5E3,  	/* 0x03 : 5*(10^(-3)) */
    GSM_GPRS_3G_QOS_RESIDUAL_BIT_ERROR_4E3,  	/* 0x04 : 4*(10^(-3)) */
    GSM_GPRS_3G_QOS_RESIDUAL_BIT_ERROR_1E4,  	/* 0x05 : 10^(-4) */
    GSM_GPRS_3G_QOS_RESIDUAL_BIT_ERROR_1E5,  	/* 0x06 : 10^(-5) */
    GSM_GPRS_3G_QOS_RESIDUAL_BIT_ERROR_1E6,  	/* 0x07 : 10^(-6) */
    GSM_GPRS_3G_QOS_RESIDUAL_BIT_ERROR_6E8,  	/* 0x08 : 6*(10^(-8)) */
    GSM_GPRS_3G_QOS_RESIDUAL_BIT_ERROR_MAX
} gsm_gprs_3g_qos_residual_bit_error_e_type;

typedef enum{
    GSM_GPRS_3G_QOS_MODE_ADD=0x01,  		/* 0x01 : Add Quality of Service Profile */
    GSM_GPRS_3G_QOS_MODE_DELETE,   		/* 0x02 : Delete Quality of Service Profile */
    GSM_GPRS_3G_QOS_MODE_MAX
} gsm_gprs_3g_qos_mode_e_type;

typedef enum tagGprs3GQosDeliError {
    GSM_GPRS_3G_QOS_DELI_ERROR_NO = 0x00,
    GSM_GPRS_3G_QOS_DELI_ERROR_YES = 0x01,
    GSM_GPRS_3G_QOS_DELI_ERROR_NO_DETECT = 0x02,
    GSM_GPRS_3G_QOS_DELI_ERROR_SUBSCRIBE = 0x03,
} GSM_Gprs3GQosDeliError;

typedef enum tagGprs3GQosDeliOrder {
    GSM_GPRS_3G_QOS_DELI_ORDER_NO = 0x00,
    GSM_GPRS_3G_QOS_DELI_ORDER_YES = 0x01,
    GSM_GPRS_3G_QOS_DELI_ORDER_SUBSCRIBE = 0x02,
} GSM_Gprs3GQosDeliOrder;

typedef struct tagSet3GQOSType {
    gsm_gprs_define_pdp_mode_e_type mode;
    gsm_gprs_qos_e_type type;
    char cid;
    gsm_gprs_3g_qos_traffic_e_type traffic;
    char max_bitrate_ul;
    char max_bitrate_dl;
    char guarante_bitrate_ul;
    char guarante_bitrate_dl;
    GSM_Gprs3GQosDeliOrder delivery_order;
    short max_sdu_size;
    gsm_gprs_3g_qos_sdu_error_e_type sdu_error;
    gsm_gprs_3g_qos_residual_bit_error_e_type residual_bit_error;
    GSM_Gprs3GQosDeliError delivery_erroreous_sdu;
    char transfer_delay;
    char traffic_handling_priority;
    gsm_gprs_define_pdp_context_p_e_type pdp_type;
} GSM_Set3GQOSType;


typedef struct tag3GQOSTypeInfo {
    gsm_gprs_3g_qos_traffic_e_type traffic;
    char max_bitrate_ul;
    char max_bitrate_dl;
    char guarante_bitrate_ul;
    char guarante_bitrate_dl;
    GSM_Gprs3GQosDeliOrder delivery_order;
    short max_sdu_size;
    gsm_gprs_3g_qos_sdu_error_e_type sdu_error;
    gsm_gprs_3g_qos_residual_bit_error_e_type residual_bit_error;
    GSM_Gprs3GQosDeliError delivery_errorous_sdu;
    char transfer_delay;
    char traffic_handling_priority;
    gsm_gprs_define_pdp_context_p_e_type pdp_type;
} GSM_3GQOSTypeInfo;

typedef struct tagRes3GQOSType {
    gsm_gprs_qos_e_type type;
    char num_type;
    GSM_3GQOSTypeInfo info[3]; // max 3
} GSM_Res3GQOSType;


typedef enum {
    DSTATE_OK = 0x00,
    DSTATE_CONNECT,
    DSTATE_RING,
    DSTATE_NO_CARRIER,
    DSTATE_ERROR,
    DSTATE_NO_DIALTONE,
    DSTATE_BUSY,
    DSTATE_NO_ANSWER,
    DSTATE_MAX,
} GSM_GprsDataStatus;

typedef enum {
    DATA_REASON_NORMAL = 0x00,
    DATA_REASON_REL_BY_USER,
    DATA_REASON_REL_BY_NET,
    DATA_REASON_REL_NET_BUSY,
    DATA_REASON_SIGNAL_LOSS,
    DATA_REASON_ORIG_FAIL_SIGNAL_LOSS,
    DATA_REASON_ORIG_REJECTED_NO_FUNDS,
    DATA_REASON_ORIG_SILENT_REDIA,
    DATA_REASON_CALL_REASON_NO_SVC,
    DATA_REASON_OTHERS,
    DATA_REASON_BARRED,
    DATA_REASON_MAX,
} GSM_GprsDStatusReason;


////////////////////////////////////////////////////////////////////
typedef struct {
    unsigned char cid;
    unsigned char ip_addr[4];
    unsigned char gateway_addr[4];
    unsigned char dns_first[4];
    unsigned char dns_second[4];
    char eth_name[16];
} GSM_GprsConnectionInfo;

typedef struct {
    unsigned char cid;
    GSM_GprsDataStatus status;
    GSM_GprsDStatusReason reason;
    unsigned char external;
} GSM_GprsPdpStatus;

typedef enum{
    GSM_GPRS_DEFINE_PDP_CONTEXT=0x01, 	/* 0x01 : Define PDP Context */
    GSM_GPRS_QOS,  						/* 0x02 : Quality of Service Profile */
    GSM_GPRS_PS,  						/* 0x03 : PS attach or detach */
    GSM_GPRS_PDP_CONTEXT,   				/* 0x04 : PDP context activate or deactivate */
    GSM_GPRS_ENTER_DATA,  				/* 0x05 : Enter data */
    GSM_GPRS_SHOW_PDP_ADDR,   			/* 0x06 : Show PDP address*/
    GSM_GPRS_MS_CLASS,  					/* 0x07 : GPRS mobile station class*/
    GSM_GPRS_3G_QUAL_SRVC_PROFILE,		/* 0x08 : 3G Quality of service profile */
    GSM_GPRS_IP_CONFIGURATION,			/* 0x09 : Multiple PDP context IP configuration */
    GSM_GPRS_DEFINE_SEC_PDP_CONTEXT,	/*0X0A : AT+CGDSCONT 	05.10.07 ky.doo EDIT */
    GSM_GPRS_TFT,						/*0X0B : AT+CGTFT 	05.10.07 ky.doo EDIT */
    GSM_GPRS_HSDPA_STATUS,			/* 0x0C : HSDPA status */
    GSM_GPRS_CURRENT_SESSION_DATA_COUNTER,             /* 0x0D : Current data session tx/rx total bytes */
    GSM_GPRS_DATA_DORMANT,                     /* 0x0E : Force to set Dormant */
    GSM_GPRS_DUN_PIN_CTRL ,                /* 0x0F : Dial up Networking Pin Control Message */
    GSM_GPRS_CALL_STATUS,   			     /* 0x10 : DS TE2 Data Call result(Multiple PDP) */
    GSM_GPRS_MAX
} gsm_sub_gprs_cmd_type;


////////////////

typedef struct gsm_GprsDbEntry {
    char apn[255];
    char intf[10];
    GSM_GprsIPConfiguration ipconf;
} gsm_GprsDbEntry;

typedef struct gsm_GprsConfList {
    int num;
    gsm_GprsDbEntry *pentry;
} gsm_GprsConfList;

#endif // __vgsm_gprs_h__



