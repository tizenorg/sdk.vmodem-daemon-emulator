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

#ifndef __vgsm_net_h
#define __vgsm_net_h
/* MODE_SELECT field : Network Selection Mode */
typedef enum{
    GSM_NET_SEL_AUTO = 0x00,                /* 0x00 */
    GSM_NET_SEL_MANU = 0x01,                /* 0x01 : Automatic */
    GSM_NET_SEL_MANU_AUTO = 0x04,     	/* 0x03 : Manual-Automatic */
} gsm_net_sel_e_type;
#if 0
typedef enum{
    GSM_NET_SEL_NONE = 0x00,		/* 0x00 */
    GSM_NET_SEL_GLOBAL,                /* 0x01 : Automatic */  	
    GSM_NET_SEL_GSM_AUTO,		/* 0x02 : GSM Automatic selection*/
    GSM_NET_SEL_GSM_MANU, 		/* 0x03 : GSM Manual selection*/
    GSM_NET_SEL_CDMA ,     		/* 0x04 : CDMA selection*/
    GSM_NET_SEL_MAX = 0x04
} gsm_net_sel_e_type;
#endif
/* MODE_SELECT field : NITZ Information Mode by cosmos in 20090213 */
typedef enum{
    GSM_TEXT_ENC_ASCII = 0x01,			/* 0x01 : ASCII   */
    GSM_TEXT_ENC_GSM7BIT,					/* 0x02 : GSM7BIT */
    GSM_TEXT_ENC_UCS2, 					/* 0x03 : UCS2 */
    GSM_TEXT_ENC_HEX,     				 /* 0x04 : HEX */
    GSM_TEXT_ENC_MAX
} gsm_text_enc_e_type;

/* ACT field */
/* Access technology selected */
typedef enum{
    GSM_NET_ACT_GSM = 0x00,            /* 0x00 : GSM */
    GSM_NET_ACT_GSM_COMPACT,      /* 0x01 : GSM Compact */
    GSM_NET_ACT_UTRAN,                   /* 0x02 : UMTS */
    GSM_NET_ACT_EGPRS,                 /* 0x03 : EGPRS */
    GSM_NET_ACT_GPRS,                     /* 0x04 : GPRS */
    GSM_NET_ACT_NOTSPECIFIED=0xFF,     /* 0xFF : Not Specified*/
    GSM_NET_ACT_MAX
} gsm_net_act_e_type;

/* SERVICE_DOMAIN field */
typedef enum {
    GSM_NET_SERVICE_DOMAIN_COMBINED =0x01,     /* 0x01 : Combined(CS + PS) Mode */
    GSM_NET_SERVICE_DOMAIN_CIRCUIT,            /* 0x02 : Circuit Only Mode */
    GSM_NET_SERVICE_DOMAIN_PACKET,           	 /* 0x03 : Packet service Only Mode */
    GSM_NET_SERVICE_DOMAIN_MODE_MAX
} gsm_net_svc_domain_e_type;

/* BAND_MODE field */
typedef enum {
    GSM_NET_BAND_MODE_PREF =0x01,     /* 0x01 : Band Preferred Mode */
    GSM_NET_BAND_MODE_ONLY,               /* 0x02 : Band Only Mode */
    GSM_NET_BAND_MODE_MAX
} gsm_net_band_mode_e_type;

/* REGIST_STATUS  field */
/* Registration status */
typedef enum{
    GSM_NET_REG_STAT_NOT_REG=0x00,   /* 0x00 : not registered, ME is not currently searching a new
					PLMN to register to */
    GSM_NET_REG_STAT_REG_HOME,          /* 0x01 : Registered, home network */
    GSM_NET_REG_STAT_SEARCHING,         /* 0x02 : not registered, but ME is currently searching a new
					   SPLMN to register to */
    GSM_NET_REG_STAT_REG_DENIED,       /* 0x03 : registration denied */
    GSM_NET_REG_STAT_UNKNOWN,           /* 0x04 : unknown */
    GSM_NET_REG_STAT_REG_ROAM,          /* 0x05 : registered, roaming */
    GSM_NET_REG_STAT_MAX
} gsm_net_regist_status_e_type;

/* EDGE_SUPPORT filed */
typedef enum{
    GSM_NET_EDGE_NOTSUPPORT=0x00,
    GSM_NET_EDGE_SUPPORT=0x01
}gsm_net_edge_support_e_type;

/* PLMN_STATUS  field */
typedef enum{
    GSM_NET_PLMN_STATUS_UNKNOWN = 0x00,        /* 0x00 : Unknown or unavailable */
    GSM_NET_PLMN_STATUS_AVAIL,               /* 0x01 : Available */
    GSM_NET_PLMN_STATUS_CURRENT,          /* 0x02 : Current */
    GSM_NET_PLMN_STATUS_FORBIDDEN,      /* 0x03 : Forbidden */
    GSM_NET_PLMN_STATUS_MAX
} gsm_net_plmn_status_e_type;

typedef enum{
    GSM_NET_PLMN_FORMAT_LONGALPHANUMERIC = 0x00,
    GSM_NET_PLMN_FORMAT_SHORTALPHANUMERIC,
    GSM_NET_PLMN_FORMAT_NUMERIC
} gsm_net_plmn_format_e_type;
typedef enum{
    GSM_NET_NITZ_MMINFO_CS = 0x00,        /* 0x00 : CS*/
    GSM_NET_NITZ_MMINFO_PS = 0x01,        /* 0x01 : PS*/
    GSM_NET_NITZ_MMINFO_MAX
}gsm_net_identity_mm_info_e_type;

typedef enum{
    GSM_NET_NITZ_SHORT_NAME_MASK	= 0x01,        /* 0x01: short name active mask*/
    GSM_NET_NITZ_FULL_NAME_MASK	= 0x02,        /* 0x02: full name active mask*/
    GSM_NET_NITZ_ALL_NAME_MASK	= 0xff,        /* 0xff All names active mask*/
    GSM_NET_NITZ_NAME_MAX
}gsm_net_identity_active_mask_e_type;

typedef enum{
    GSM_NET_NITZ_DCS_ASCII       = 0x01,
    GSM_NET_NITZ_DCS_GSM7BIT  = 0x02,
    GSM_NET_NITZ_DCS_UCS2       = 0x03,
    GSM_NET_NITZ_DCS_HEX         = 0x04,
    GSM_NET_NITZ_DCS_MAX
}gsm_net_identity_data_coding_scheme;
typedef struct {
    gsm_net_plmn_status_e_type status;
    char plmn[6]; /* PLMN: MCC(3byte)+MNC(3byte). if the MNC is 2 digit, make it 6 digit by filling '#' in last blank.*/
    gsm_net_act_e_type act;
} gsm_network_plmn_record_t;// GSM_NetPLMNRecord;

typedef struct {
    char num_record;
    gsm_network_plmn_record_t* precord;
} gsm_network_plmn_list_t; // GSM_NetResPLMNList;

typedef struct {
    gsm_net_act_e_type act;
    gsm_net_svc_domain_e_type service_domain;
    gsm_net_regist_status_e_type reg_status;
    gsm_net_edge_support_e_type edge_support;
    unsigned char lac[2];
    unsigned char cell_id[4];
    unsigned char rej_cause;
} gsm_network_reg_t; //GSM_NetworkRegistration

/* GSM_PLMN Information by cosmos in 20090212 */
typedef struct {
    gsm_net_act_e_type act;
    gsm_network_plmn_record_t statusplmn;
    gsm_net_sel_e_type mode;
} gsm_network_plmn_info_t;

/* GSM_NITZ Information by cosmos in 20090212 */
typedef struct {
    gsm_net_identity_mm_info_e_type mm_info_type;
    gsm_net_identity_active_mask_e_type mask;
    char	shortname_ci ; // 00 : disable, 01 : enable
    char	shortname_len ; // max 16
    gsm_net_identity_data_coding_scheme	shortname_dcs ; //
    char shortname[16];
    char longname_ci ; // 00 : disable, 01 : enable
    char	longname_len ; // max 32
    gsm_net_identity_data_coding_scheme	longname_dcs ; //
    char longname[32];
} gsm_network_nitz_info_t;

#endif
