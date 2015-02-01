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
 *     vgsm_ss.h
 *********************************************************************/
#ifndef _vgsm_ss_h_
#define _vgsm_ss_h_

#include <vgsm_call.h> // for MAX_GSM_DIALED_DIGITS_NUMBER



typedef enum {
    SS_NONE = 0x01,
    SS_TX_HOLD = 0X02,			/* requested HOLD by EI */
    SS_TX_ACTIVATE = 0x03,		/* requested ACTIVATE by EI */
    SS_RX_HOLD = 0x04,			/* requested HOLD by Simulator */
    SS_RX_ACTIVATE = 0X05		/* requested ACTIVATE by Simulator */
} ss_hold_act_state_e_type;

typedef enum {
    SS_STATE_HOLD = 0x01,
    SS_STATE_UNHOLD = 0x02,
    SS_STATE_CONFERENCE = 0x03,
    SS_STATE_SWAP = 0x04,
    SS_STATE_SPLIT = 0x05,
    SS_STATE_DROP = 0x06
} ss_state_e_type; //SS_State_Type;

typedef enum {
    SS_MODE_REG = 1,		/**< SS Mode registration. */
    SS_MODE_DEREG = 2,		/**< SS Mode Deregistration. */
    SS_MODE_ACT = 3,			/**< SS Mode Activation. */
    SS_MODE_DEACT = 4		/**< SS Mode Deactivation. */
}ss_mode_e_type;

/* CF_TYPE  field */
typedef enum{
    SS_CF_TYPE_CFU = 0x01,               /* 0x01 : Call Forwarding Unconditional */
    SS_CF_TYPE_CFB,               /* 0x02 : Call Forwarding Mobile Busy */
    SS_CF_TYPE_CFNRy,          /* 0x03 : Call Forwarding No Reply */
    SS_CF_TYPE_CFNRc,          /* 0x04 : Call Forwarding Not Reachable */
    SS_CF_TYPE_CF_ALL,        /* 0x05 : All Call Forwarding */
    SS_CF_TYPE_CFC,              /* 0x06 : All Conditional Call Forwarding */
    SS_CF_TYPE_MAX               /* 0x07 : Max */
} ss_cf_e_type;

/* CB_TYPE field */
typedef enum {
    SS_CB_TYPE_NONE,
    SS_CB_TYPE_BAOC,                /* 0x01 : Barring All Outgoing Calls */
    SS_CB_TYPE_BOIC,                /* 0x02 : Barring Outgoing International Calls */
    SS_CB_TYPE_BOIC_NOT_HC, /* 0x03 : Barring Outgoing International Calls except to Home Country */
    SS_CB_TYPE_BAIC,                /* 0x04 : Barring All Incoming Calls */
    SS_CB_TYPE_BIC_ROAM,       /* 0x05 : Barring Incoming Calls when roam, outside of the Home Country */
    SS_CB_TYPE_AB,                   /* 0x06 : All Barring Services */
    SS_CB_TYPE_AOB,                 /* 0x07 : All Outgoing Barring Services */
    SS_CB_TYPE_AIB,                  /* 0x08 : All Incoming Barring Services */
    SS_CB_TYPE_BIC_NOT_SIM, /* 0x09 : Barring Incoming Calls which is not stored in the SIM memory */
    SS_CB_TYPE_MAX
} ss_cb_e_type;

// actually, this data structures is used at db.
typedef enum {
    SS_CMD_CW = 1,
    SS_CMD_CB,
    SS_CMD_CF
} ss_cmd_e_type;

typedef struct {
    int 		tel_class;
    int 		ss_mode; // reg(1) / dereg / act / deact
    int		count;
    char 		number[MAX_GSM_DIALED_DIGITS_NUMBER];
} call_waiting_entry_t;

typedef struct {
    int 		tel_class;
    int 		type; //cb_type
    int 		ss_mode;
    int		count;
    //	char		passwd[4];
} call_barring_entry_t;

typedef struct {
    int 		tel_class;  // gsm_ss_class_e_type
    int 		type; // gsm_ss_cf_e_type
    char 	number[MAX_GSM_DIALED_DIGITS_NUMBER];
    char	subaddr[MAX_GSM_DIALED_DIGITS_NUMBER];
    int 		number_type;
    int 		replyTime;
    int 		ss_mode; // gsm_ss_mode_e_type
    int		satype;
    int		count;
} call_forwarding_entry_t;

typedef enum
{
    TAPI_SS_AOC_TYPE_RESET  = 0x00,	/**< Specifies the Reset MAXACM Value. */
    TAPI_SS_AOC_TYPE_ACM    = 0x01,	/**< Specifies the Accumulated call meter. */
    TAPI_SS_AOC_TYPE_CCM    = 0x02,	/**< Specifies theCurrent call meter. */
    TAPI_SS_AOC_TYPE_MAXACM = 0x04,	/**< Specifies theMax accumulated call meter. */
    TAPI_SS_AOC_TYPE_PUC    = 0x08	/**< Specifies thePrice per unit and currency.*/
}TelSsAocType_t;

#if 0
typedef struct
{
    TelSsAocType_t AocType;		/**< Specifies the AOC type. */
    UINT32	ACM; 					/**< Specifies the accumulated call meter value. */
    UINT32	MaxAcm; 				/**< Specifies the maximum value of acm . */
    float 	CCM;					/**< Specifies the Current call meter value. */
    float 	PPM;					/**< Specifies the Price per unit value. */
    unsigned char CharTypeOfCurrency; 			/**< Specifies the Char type of currency. */
    unsigned char szCurrency[TAPI_SS_AOC_CURRENCY_LEN_MAX];	/**< Specifies the Currency characters. */
} TelCallAocInfo_t;
#endif
// 090213 090330//////////////////////////////////////////////
typedef struct
{
    long acm;
    long ccm;
    long maxacm;
    long ppu;
    char   chartype;
    char currency[3];
    //	char *hdr;
    //gsm_hdr_type *hdr;
} _AOC_t;

typedef struct
{
    int cwclass;
    int status;
} _setCW_t;

typedef struct
{
    int code;
    int number;
    int bc;
    int timer;
    int status;
} _setCF_t;

typedef struct
{
    int code;
    int passwd;
    int status;
} _setCB_t;

typedef struct
{
    char time[50];
    char weather[50];
} _USSD_data_t;

// 090213 090330//////////////////////////////////////////////

#endif // _vgsm_ss_h_

