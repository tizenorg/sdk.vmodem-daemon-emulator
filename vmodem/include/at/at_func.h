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

/*
/////////////////////////////////////////////////////////////////////
// at_func.h
 */

#ifndef __AT_H__
#define __AT_H__

#include <stdint.h>
#include "gsmstate.h"
#include "at_recv.h"
#include "state.h"

/************************************* Defines ****************************************/
/* common */
#define TOKEN ","			/* AT command token */
#define CRLF "\r\n"
#define SEND_BUF_SIZE 512

/* display */
#define CIEV "+CIEV:"                   /* DISP ICON INFO */
#define SIGNAL 10
#define BATTCHG 15

/* security */
#define STREADY "NO_LOCK"               /* PIN(sim) TYPE */
#define STSC "LOCK_SC"
#define STNOSIM "NO_SIM"
#define SKUNLOCK "UNLOCKED"             /* PIN(sim) KEY */
#define SKPIN "PIN"
#define SKPUK "PUK"
#define SKPERM_BLOCK "BLOCKED"
#define STINIT_COMPL "INIT_COMP"
#define CPIN "%SCSIM:"			/* PIN STATUS */
#define CRSM "+CRSM:"			/* SEC RSIM ACCESS */
#define SCCT "%SCCT:"
#define CLCK "+CLCK:"			/* PHONE LOCK */
#define IMSI_NUM "262073997619483"      /* International Mobile Station Identity */

/* call */
#define RING "RING"                     /* CALL INCOMING */
#define SCLCC "%SCLCC:"                 /* CALL LIST NOTI */
#define CCWA "+CCWA:"			/* CALL WAITING */
#define CCFC "+CCFC:"			/* CALL FORWARDING */
#define CLCC "+CLCC:"			/* CALL LIST */

/* network */
#define CREG "+CREG:"			/* NET REGIST */
#define CGREG "+CGREG:"
#define COPS "+COPS:"			/* NET PLMN */
#define CNUM "+CNUM:"			/* NET SUBSCRIBER NUM */
#define CNITZ "%SCNITZ:"		/* NET IDENTITY */
#define SUBSCRIBER_NUM "821010101010"

/* misc */
#define CGMR "+CGMR:"			/* MISC ME VERSION */
#define CTZR "+CTZR:"			/* MISC V4 TIME INFO */

/* gprs */
#define CGDCONT "+CGDCONT:"		/* GPRS IP CONFIGURATION */
#define EMUL_IP "10.0.2.16"		/* for gprs */
#define IP "\"IP\""
#define PPP "\"PPP\""
#define IPV6 "\"IPV6\""
#define UNKNOWN "\"UNKNOWN\""

/* power */
#define CPAS "+CPAS:"			/* PWR PHONE PWR UP */
#define SCFUN "%SCFUN:"			/* FLIGHT MODE */
#define FLIGHT_MODE_ON 0
#define FLIGHT_MODE_OFF 1

/* sms */
#define CDEV "%SCDEV:"			/* SMS DEVICE READY */
#define CMT "+CMT:"			/* SMS INCOMING */
#define ATCMGS "AT+CMGS="		/* SMS OUTGOING */
#define CMGS "+CMGS:"
#define CPMS "+CPMS:"			/* SMS STORED MSG COUNT */

/************************************************************************************/

/********************************* Type enum ****************************************/
/* CALL SS CLASS field */
typedef enum{
    AT_CALL_SS_CLASS_VOICE = 1,
    AT_CALL_SS_CLASS_DATA = 2,
    AT_CALL_SS_CLASS_FAX = 4,
    AT_CALL_SS_CLASS_ALL = 7,
    AT_CALL_SS_CLASS_VIDEO = 16
} at_call_ss_class_e_type;

/* CALL CLASS field */
typedef enum{
    AT_CALL_CLASS_VOICE = 1,
    AT_CALL_CLASS_FAX = 4,
    AT_CALL_CLASS_SMS = 8,
    AT_CALL_CLASS_DATA = 32
} at_call_class_e_type;

/* CALL DIRECTION field */
typedef enum{
    AT_CALL_DIR_MO = 0x00,     	/* 0x00 : Mobile Originated Call ( MO Call ) */
    AT_CALL_DIR_MT,     		/* 0x01 : Mobile Terminated Call ( MT Call ) */
} at_call_direction_e_type;

/* CALL STAT field */
typedef enum{
    AT_CALL_STAT_ACTIVE = 0x00,	/* 0x00 : ACTIVE */
    AT_CALL_STAT_HELD,
    AT_CALL_STAT_DIALING,
    AT_CALL_STAT_ALERTING,
    AT_CALL_STAT_INCOMING,
    AT_CALL_STAT_WAITING,
    AT_CALL_STAT_CONNECTED,
    AT_CALL_STAT_RELEASED,
    AT_CALL_STAT_NONE
} at_call_stat_e_type;

/* CALL MODE field */
typedef enum{
    AT_CALL_MODE_VOICE = 0x00,	/* 0x00 : VOICE */
    AT_CALL_MODE_DATA
} at_call_mode_e_type;

/* MODE  field */
typedef enum {
    GSM_SS_MODE_REG=0x01,         /* 0x01 : Registration */
    GSM_SS_MODE_DEREG,            /* 0x02 : De-registration( erase ) */
    GSM_SS_MODE_ACTIVATE,        	/* 0x03 : Activation */
    GSM_SS_MODE_DEACTIVATE,    	/* 0x04 : De-activation */
    GSM_SS_MODE_MAX,
    GSM_SS_MODE_QUERY
} gsm_ss_mode_e_type;

typedef enum{
    AT_CALL_FW_MODE_DISABLE = 0x00,
    AT_CALL_FW_MODE_ENABLE,
    AT_CALL_FW_MODE_QUERY,
    AT_CALL_FW_MODE_REG,
    AT_CALL_FW_MODE_ERASURE
} at_call_fw_mode_e_type;

typedef enum {
    AT_SS_STATUS_NOT_ACTIVE = 0,    /* Service is not Active */
    AT_SS_STATUS_ACTIVE = 1         /* Service is Active */
}ss_status_e_type;

/* SS CM_MODE field : Call Manage Mode */
typedef enum {
    AT_GSM_SS_CM_0_SEND = 0x00,	/* 0x00 : Releases all held calls or sets User Determined
				   User Busy (UDUB) for a waiting call*/
    AT_GSM_SS_CM_1_SEND,          /* 0x01 : Releases all active calls (if any exist) and
				     accepts the other (held or waiting) call.*/
    AT_GSM_SS_CM_2_SEND,          /* 0x02 : Places all active calls (if any exist) on hold
				     and accepts the other (held or waiting) call.*/
    AT_GSM_SS_CM_3_SEND,          /* 0x03 : Adds a held call to the conversation.*/  
    AT_GSM_SS_CM_4_SEND,          /* 0x04 : Connects the two calls and disconnects
				     the subscriber from both calls (ECT).*/
    AT_GSM_SS_CM_1X_SEND,         /* 0x05 : Releases a specific active call X.
				     (also <INDEX> present)*/
    AT_GSM_SS_CM_2X_SEND,         /* 0x06 : Places all active calls on hold except call X
				     with which communication shall be supported.
				     (also <INDEX> present)*/
    AT_GSM_SS_CM_4DN_SEND,        /* 0x07 : Redirect an incoming or a waiting call
				     to the specified followed by SEND directory
				     number.(also <NUMBER> present)*/
    AT_GSM_SS_CM_5_SEND,          /* 0x08 : Activates the Completion of Calls to
				     Busy Subscriber Request.*/
    AT_GSM_SS_CM_6_SEND,          /* 0x0a : Release a held conference call.
				     (This is a special-case to enable us to pass
				     FTA case GSM 51.010 31.4.4.1.2.4) */
    AT_GSM_SS_CM_MAX
} at_gsm_ss_cm_mode_e_type;

/* CALL MPTY field */
typedef enum{
    AT_CALL_MPTY_FALSE = 0x00,	/* 0x00 : FALSE */
    AT_CALL_MPTY_TRUE
} at_call_mpty_e_type;

/* ICC TYPE field */
typedef enum{
    GSM_SEC_UNKNOWN,		/* 0x00 : UNKNOWN */
    GSM_SEC_SIM,          
    GSM_SEC_USIM,         
    GSM_SEC_RSIM
} gsm_sec_sim_e_type;

typedef enum {
    AT_GSM_PWR_UP_READY = 0x00,
    AT_GSM_PWR_UP_UNAVAILABLE,
    AT_GSM_PWR_UP_UNKNOWN,
    AT_GSM_PWR_UP_RINGING,
    AT_GSM_PWR_UP_CALLINPROGRESS,
    AT_GSM_PWR_UP_ASLEEP
} at_gsm_pwr_up_e_type;

typedef enum {  
    AT_GSM_PWR_PHONE_ONLINE_OFF = 0x00,		/* ONLINE OFF*/
    AT_GSM_PWR_PHONE_ONLINE_ON,			/* ONLINE ON */
    AT_GSM_PWR_PHONE_ONLINE_MAX
} at_gsm_pwr_phone_online_e_type;

typedef enum {
    AT_GSM_PWR_PHONE_RSN_ONLINE_OFF = 0x00,	/* LPM IN*/
    AT_GSM_PWR_PHONE_RSN_ONLINE_ON,		/* LPM OUT,ONLINEl */
    AT_GSM_PWR_PHONE_RSN_ONLINE_MAX,
    AT_GSM_PWR_PHONE_RSN_ONLINE_FAIL,		/* EXCUTE FAIL*/
} at_gsm_pwr_phone_online_noti_e_type;

typedef enum{
    AT_GSM_DISP_ICON_TYPE_RSSI           = 0x01,	/* 0000 0001 : signal strength */
    AT_GSM_DISP_ICON_TYPE_BAT            = 0x02,	/* 0000 0010 : battery level   */
    AT_GSM_DISP_ICON_TYPE_ACT            = 0x04,	/* 0000 0100 : Protocol Standard: GSM,GPRS,UMTS   */
    AT_GSM_DISP_ICON_TYPE_REGIST_STATUS  = 0x08,	/* 0000 1000 : Registration status   */
    AT_GSM_DISP_ICON_TYPE_ALL	       = 0xFF,	/* 1111 1111 : ALL Values Request */
} at_gsm_disp_icon_e_type;
/************************************************************************************/

/********************************* Functions ****************************************/
char *readline(void);
void prv_byte_to_char(unsigned char aByte, char* hexaCharP);
void prv_bytes_to_hexadecimal_string(unsigned char* bytesP, int bytesSize, char* hexaStringP, int* hexaStringSize);

uint8_t hexCharToInt(char c);
int hexStringToBytes(char* d, char *s);

int GSM_ATDispatchDpramData(GSM_StateMachine* pstate);
int convert_hdlc_to_at(char* atmsg, char *data, int data_len);

/* a function used to deal with a non-trivial request */
typedef int  (*ResponseHandler)(char*  cmd, char* atmsg);
/**********************************************************************************/

/********************************* Structs ****************************************/
static const struct {
    char*      	     cmd;     				/* if first character is '!', then the rest is a prefix only */
    int 	     main_cmd;	
    ResponseHandler  handler; 				/* specific handler, ignored if 'answer' is not NULL,
							   NULL if OK is good enough */
} sDefaultResponses[] =
{
    { "!+CGDCONT=", GSM_GPRS_CMD, at_recv_gprs },
    { "+CGDCONT?", GSM_GPRS_CMD, at_recv_gprs },
    { "!+CGACT=", GSM_GPRS_CMD, at_recv_gprs },
    { "!D*99***1#", GSM_GPRS_CMD, at_recv_gprs },

    /* see requestSendSMS() */
    { "!+CMMS=", GSM_SMS_CMD, at_recv_sms },
    { "!+CMGS=", GSM_SMS_CMD, at_recv_sms },
    { "!+CPMS", GSM_SMS_CMD, at_recv_sms },
    { "!+CNMA=", GSM_SMS_CMD, at_recv_sms },

    { "!%SCMUT=", GSM_SND_CMD, at_recv_snd },		/* set SND MIC MUTE CTRL */
    { "%SCMUT?", GSM_SND_CMD, at_recv_snd },		/* get SND AUDIO PATH CTRL */
    { "!%SCAUD=", GSM_SND_CMD, at_recv_snd },		/* set SND AUDIO PATH CTRL */
    { "!%SCCLK=", GSM_SND_CMD, at_recv_snd },		/* exec SND CLOCK CTRL */

    /* see requestSIM_IO() */
    { "!+CRSM=", GSM_SEC_CMD, at_recv_security },
    { "!%SCCT", GSM_SEC_CMD, at_recv_security },        /* ICC type */
    { "!+CPIN=", GSM_SEC_CMD, at_recv_security },

    { "!+CLCK=", GSM_SS_CMD, at_recv_ss },		/* call barring */
    { "+CCWA=?", GSM_SS_CMD, at_recv_ss },		/* call waiting get */
    { "!+CCWA=", GSM_SS_CMD, at_recv_ss },		/* call waiting set */
    { "!+CHLD=", GSM_SS_CMD, at_recv_ss },		/* call hold */
    { "!+CCFC=", GSM_SS_CMD, at_recv_ss },		/* call fowarding */

    /* see requestQueryNetworkSelectionMode() */
    { "+COPS?", GSM_NET_CMD, at_recv_network },
    { "+COPS=?", GSM_NET_CMD, at_recv_network },
    { "!+COPS=", GSM_NET_CMD, at_recv_network },
    { "+CNUM", GSM_NET_CMD, at_recv_network },		/* get subscriber num */
    { "+CREG?", GSM_NET_CMD, at_recv_network },		/* get network registration */
    { "!+CREG=", GSM_NET_CMD, at_recv_network },	/* set unsolicited result */

    { "+CGMR", GSM_MISC_CMD, at_recv_misc },		/* get me version */
    { "+CGSN", GSM_MISC_CMD, at_recv_misc },   		/* request model version */
    { "+CIMI", GSM_MISC_CMD, at_recv_misc },  	 	/* request internation subscriber identification number */

    { "+CLCC", GSM_CALL_CMD, at_recv_call },		/* get current call list */
    { "A", GSM_CALL_CMD, at_recv_call },		/* answer */
    { "!D", GSM_CALL_CMD, at_recv_call },		/* outgoing call */
    { "!H", GSM_CALL_CMD, at_recv_call },		/* release the first call */

    /* see requestRadioPower() */
    { "!+CFUN=", GSM_PWR_CMD, at_recv_power }, 		/* power online */
    { "+CPAS", GSM_PWR_CMD, at_recv_power },   		/* power up noti */

    /* end of list */
    {NULL, 0, NULL}
};

static const struct {
    const char*      fac;     
    ss_cb_e_type     cb_e_type;
} sDefaultFac[] =
{
    { "AO", SS_CB_TYPE_BAOC},		/* Barr All Outgoing Calls */	
    { "OI", SS_CB_TYPE_BOIC},		/* Barr Outgoing International Calls */
    { "OX", SS_CB_TYPE_BOIC_NOT_HC},	/* Barr Outgoing International Calls except to Home Country */
    { "AI", SS_CB_TYPE_BAIC},		/* Barr All Incoming Calls */
    { "IR", SS_CB_TYPE_BIC_ROAM},	/* Barr Incoming Calls when Roaming outside the home country */
    { "AB", SS_CB_TYPE_AB},		/* All Barring services */
    { "AG", SS_CB_TYPE_AOB},		/* All outGoing barring services */
    { "AC", SS_CB_TYPE_AIB},		/* All inComing barring services */
    { "NA", SS_CB_TYPE_BIC_NOT_SIM},	/* barr incoming calls from numbers Not stored in Any memory */

    /* end of list */
    {NULL, 0x00}
};

static const struct {
    const char*      		code;
    gsm_sec_lock_e_type     	lock_e_type;
} sDefaultLock[] =
{
    { "READY", GSM_SEC_LOCK_TYPE_READY},	/* 0x00 : ME is not locked */
    { "SIM PIN", GSM_SEC_LOCK_TYPE_SC},		/* 0x03 : Lock SIM/UICC card 
						   ( SIM asks password in ME power-up and when this command is issued ) */
    { "SIM PUK", GSM_SEC_LOCL_TYPE_PUK},	/* 0x0C : Lock PUK (... ) */
    { "SIM PIN2", GSM_SEC_LOCK_TYPE_SC2},	/* 0x09 : Lock PIN2 ( ... ) */
    { "SIM PUK2", GSM_SEC_LOCL_TYPE_PUK2},	/* 0x0A : Lock PUK2 (... ) */

    /* end of list */
    {NULL, 0x00}
};
/****************************************************************************************************************/

#endif /* __AT_H__ */

