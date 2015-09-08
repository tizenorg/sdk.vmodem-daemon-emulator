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

#ifndef _vgsm_call_h
#define _vgsm_call_h

#define MAX_GSM_CALL_LIST_NUMBER 6
#define MAX_GSM_DIALED_DIGITS_NUMBER 32
#define MAX_BUF_LEN 256

/* DIRECTION field */
typedef enum{
    GSM_CALL_DIR_MO = 0x01,     /* 0x01 : Mobile Originated Call ( MO Call ) */
    GSM_CALL_DIR_MT,     /* 0x02 : Mobile Terminated Call ( MT Call ) */
    GSM_CALL_DIR_MAX
} gsm_call_direction_e_type;


// !! CAUTION: CallState differs from CallStatus !!
// CallState: from CALL STATUS NOTI
// CallStatus: from CALL LIST

/* CALL_STATE  field */
/* Call Processing Status ( or call state ) */
typedef enum{
    GSM_CALL_STATE_NONE,                  /* 0x00 : Idle State ( Non-call state ) */
    GSM_CALL_STATE_OUTGOING,          /* 0x01 : Originating Attempt state */
    GSM_CALL_STATE_INCOMING,           /* 0x02 : Incoming alert state */
    GSM_CALL_STATE_CONNECTED,        /* 0x03 : connected state */
    GSM_CALL_STATE_RELEASED,           /* 0x04 : Release State */
    GSM_CALL_STATE_MAX
} gsm_call_state_e_type;

/* CALL_STATUS field */
typedef enum{
    GSM_CALL_STATUS_NONE,              /* 0x00 : None Call status */
    GSM_CALL_STATUS_ACTIVE,           /* 0x01 : current call is active */
    GSM_CALL_STATUS_HELD,              /* 0x02 : current call is on hold */
    GSM_CALL_STATUS_DIALING,         /* 0x03 : current call is dialing(MO call) */
    GSM_CALL_STATUS_ALERT,             /* 0x04 : terminated party is ringing ( MO call ) */
    GSM_CALL_STATUS_INCOMING,      /* 0x05 : incoming call(MT call) */
    GSM_CALL_STATUS_WAITING,        /* 0x06 : Another incoming call ( call waiting ) is alerting to the user in conversation ( MT call ) */
    /* Others are reserved */
    GSM_CALL_STATUS_MAX
}  gsm_call_status_e_type;


/* CALL_TYPE  field */
typedef enum {
    GSM_CALL_TYPE_NONE,     /* 0x00 : None */
    GSM_CALL_TYPE_VOICE,    /* 0x01 : Voice Call Type */
    GSM_CALL_TYPE_DATA,     /* 0x02 : Data Call Type */
    GSM_CALL_TYPE_VIDEO,    /* 0x03 : Video Call Type */
    GSM_CALL_TYPE_MAX        /* 0x04 : Max */
} gsm_call_type_e_type;
#define GSM_CALL_TYPE_FORWARDED 0x33

/* NUMBER_TYPE  field */
/* NUMBER_TYPE = ( (NUM_TYPE << 4 ) | NUM_PLAN )*/
typedef enum{
    GSM_NUM_TYPE_UNKNOWN,                 /* 0x00 : unknown */
    GSM_NUM_TYPE_INTERNATIONAL,       /* 0x01 : international number */
    GSM_NUM_TYPE_NATIONAL,                 /* 0x02 : national number */
    GSM_NUM_TYPE_NETWORK,                  /* 0x03 : network specific number */
    GSM_NUM_TYPE_DEDICATE,                 /* 0x04 : dedicated access, short code */
    GSM_NUM_TYPE_MAX
} gsm_num_type_e_type;

/* CLIR_STATUS  field */
/* override the CLIR supplementary service subscription default value for this call */
typedef enum {
    GSM_CALL_CLIR_STATUS_NOT_CHANGED,     /* 0x00 : previous CLI presentation */
    GSM_CALL_CLIR_STATUS_INVOCATION,        /* 0x01 : restrict CLI presentation */
    GSM_CALL_CLIR_STATUS_SUPPRESSION,      /* 0x02 : allow CLI presentation */
    GSM_CALL_CLIR_STATUS_MAX
} gsm_clir_status_e_type;


typedef struct {
    gsm_call_type_e_type calltype;
    gsm_clir_status_e_type clirstatus;
    unsigned short numlen;
    gsm_num_type_e_type numtype;
    unsigned char number[MAX_GSM_DIALED_DIGITS_NUMBER];

#if 0
    char uus_type[MAX_BUF_LEN];
    char protocol_type[MAX_BUF_LEN];
    char user_type[MAX_BUF_LEN];
    char alerting_pattern[MAX_BUF_LEN];
#endif

    unsigned char present_ind;
    unsigned char redirected_num;
    unsigned char no_cli_cause;
    unsigned char forwardedstatus;

} gsm_call_info_t;

typedef struct
{
    unsigned int      idx;				// call id.
    gsm_call_direction_e_type       dir;
    gsm_call_status_e_type      stat;
    gsm_call_type_e_type      call_type;
    unsigned int      mpty;			//multy party or not
    char               number[MAX_GSM_DIALED_DIGITS_NUMBER];
    int                type;  				// not used
    int					num_len;
    gsm_num_type_e_type	num_type;
    char               alpha[32];			// not used
} gsm_call_list_info_t;


typedef struct
{
    int CallCount;
    gsm_call_list_info_t CallInfo[MAX_GSM_CALL_LIST_NUMBER];
} gsm_call_list_t;


/*/ 090314
  typedef struct
  {
  gsm_call_list_info_t    *call_list_info;
  int                     isIng;              // 01 : Ing
  } new_call_list_info_t
 *////////////////////////////////////////////
#endif
