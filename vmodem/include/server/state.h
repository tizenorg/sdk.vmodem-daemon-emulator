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

#ifndef	_STATE_H_
#define _STATE_H_

#include <linuxtapi.h>
#include <at_msg.h>
#include <vgsm.h>
#include <vgsm_phone.h>
//#include "db_security.h"
#include "db_ss.h"

// #define _DEBUG

#ifdef  _DEBUG
#define _ENTER()    log_msg(MSGL_VGSM_INFO,"ENTER\n")
#define _LEAVE()	log_msg(MSGL_VGSM_INFO,"LEAVE\n" )
#else
#define _ENTER()
#define _LEAVE()
#endif  /* _DEBUG */

#define STATE_TYPE(x)		    (x).state_type
#define STATE_MAIN_CMD(x)	    (x).main_cmd
#define STATE_SUB_CMD(x)	    (x).sub_cmd
#define STATE_FUNCTION(x)	    (x).state_function

#define STATE_FLAG_TYPE		    0x01
#define STATE_FLAG_MAIN_CMD	    0x02
#define STATE_FLAG_SUB_CMD	    0x04
#define STATE_FLAG_FUNCTION	    0x08
#define STATE_FLAG_ALL		    0xFF

#define MAX_CALL_COUNT		    6

/*
* For State Machine
*/
typedef enum {
	GSM_PWR_CMD=0x01,		/* 0x01 : Power Control Commands */
	GSM_CALL_CMD,			/* 0x02 : Call Control Commands */
	GSM_DATA_CMD,			/* 0x03 : Data Control Commands */
	GSM_SMS_CMD,			/* 0x04 : Short Message Service Commands */
	GSM_SEC_CMD,			/* 0x05 : Security - SIM control Commands */
	GSM_PB_CMD,			/* 0x06 : Phonebook Control Commands */
	GSM_DISP_CMD,			/* 0x07 : Display Control Commands */
	GSM_NET_CMD,			/* 0x08 : Network Commands */
	GSM_SND_CMD,			/* 0x09 : Sound Control Commands */
	GSM_MISC_CMD,			/* 0x0A : Miscellaneous Control Commands */
	GSM_SVC_CMD,			/* 0x0B : Service Mode Control Commands - Factory Test or Debug Screen Control */
	GSM_SS_CMD,			/* 0x0C : Supplementary Service Control Command */
	GSM_GPRS_CMD,			/* 0x0D : GPRS Commands */  // joonook 20041011 create
	GSM_SAT_CMD,			/* 0x0E : SIM Toolkit Commands */
	GSM_CFG_CMD,			/* 0x0F : Configuration Commands */
	GSM_IMEI_CMD,			/* 0x10 : IMEI Tool Commands */
	GSM_GPS_CMD,			/* 0x11 : GPSl Commands */
	GSM_SAP_CMD,			/* 0x12 : SIM Access Profile Commands */
	GSM_GEN_CMD=0x80,		/* 0x80 : General Response Command */
	GSM_CMD_MAX
}gsm_main_cmd_type;

typedef enum{
	GSM_SEC_PIN_STATUS			=0x01,    /* 0x01 : PIN Status Message */
	GSM_SEC_PHONE_LOCK,					/* 0x02 : Phone Lock Message */
	GSM_SEC_CHANGE_LOCKING_PW,			/* 0x03 : Change Locking Password Message */
	GSM_SEC_SIM_LANG,						/* 0x04 : SIM Language Message */
	GSM_SEC_RSIM_ACCESS,					/* 0x05 : +CRSM, Restricted SIM Access Message */
	GSM_SEC_GSIM_ACCESS,					/* 0x06 : +CSIM, General SIM Access Message */             
	GSM_SEC_SIM_ICC_TYPE,					/* 0x07 : SIM ICC Type Message*/
	GSM_SEC_LOCK_INFO,					/* 0x08 : Lock Information Message*/
	GSM_SEC_ISIM_AUTH,					/* 0x9 : ISIM Authentication Message */
	GSM_SEC_MAX
} gsm_sec_sub_cmd_type; 

typedef enum{
  GSM_CALL_OUTGOING=0x01,           /* 0x01 : Call Outgoing Message */
  GSM_CALL_INCOMING,                     /* 0x02 : Call Incoming Message */
  GSM_CALL_RELEASE,                        /* 0x03 : Call Release Message */
  GSM_CALL_ANSWER,                        /* 0x04 : Call Answer Message */
  GSM_CALL_STATUS,                         /* 0x05 : Current Call Status Message */
  GSM_CALL_LIST,                              /* 0x06 : Current Call List Message */
  GSM_CALL_BURST_DTMF,                /* 0x07 : Burst DTMF Message */
  GSM_CALL_CONT_DTMF,                  /* 0x08 : Continuous DTMF Message */
  GSM_CALL_WAITING,                      /* 0x09 : Call Waiting Message */
  GSM_CALL_LINE_ID,                      /* 0x0A : Call Line id Message */
  GSM_CALL_MAX                                /* 0x0B : Max */
} gsm_call_sub_cmd_type;

typedef enum _STATE_TYPE {
    STATE_NONE=0x00,
    STATE_OFF=0x01,
    STATE_ON,
    STATE_STANDBY,
/* Call - begin */
    STATE_CALL_WAITING_INCOMING,
    STATE_CALL_WAITING_OUTGOING,
    STATE_CALL_CONVERSATION,
    STATE_CALL_RELEASED,
/* Call - end */

/* SIM -start */

    STATE_SIM_READY,
    STATE_SIM_PIN,
    STATE_SIM_FDN, // 0x10
    STATE_SIM_PUK,
    STATE_SIM_FRIZEN,
    STATE_SIM_PUK2,
    STATE_SIM_PIN2,

    STATE_SIM_PIN_WRONG,
    STATE_SIM_PUK_WRONG,
    STATE_SIM_PUK2_WRONG,
    STATE_SIM_PIN2_WRONG,

    STATE_SIM_PIN_OK,
    STATE_SIM_PUK_OK, //0x20
    STATE_SIM_PUK2_OK,
    STATE_SIM_PIN2_OK,
   	
    STATE_SIM_PIN_WRONG_ALL,
    STATE_SIM_PUK_WRONG_ALL,
    STATE_SIM_PUK2_WRONG_ALL,
    STATE_SIM_PIN2_WRONG_ALL,

	//STATE_SIM_PIN_LOCK_DISABLE,
	//0215
	STATE_SIM_NO_SIM,
/* SIM -end */
	
    STATE_ANY
} STATE_TYPE;

typedef struct _STATE {
    int state_type;
    int main_cmd;
    int sub_cmd;
    int (*state_function)(void);
} STATE;

#if 0 /* start-move to server_common_security.h*/
typedef enum {
   SIM_STATE_READY=	0,
   SIM_STATE_PIN_REQ=	1,
   SIM_STATE_PUK_REQ	,	
   SIM_STATE_PIN2_REQ	,	
   SIM_STATE_PUK2_REQ	,	
   SIM_STATE_FRIZEN	,		
   SIM_STATE_NO_SIM	,		
  SIM_STATE_FDN_STATE,
  SIM_STATE_PIN2_OK
} SIM_STATE;
#endif  /* end-move to server_common_security.h*/

typedef struct _STATE_TABLE {
    STATE current_state;
    STATE next_state;
} STATE_TABLE;

typedef struct _STATE_MACHINE {
    STATE prev_state;
    STATE current_state;
} STATE_MACHINE;

#if 0  // moved to vgsm_ss.h
typedef enum {
    SS_STATE_HOLD = 0x01,
    SS_STATE_UNHOLD = 0x02,
    SS_STATE_CONFERENCE = 0x03,
    SS_STATE_SWAP = 0x04,
    SS_STATE_SPLIT = 0x05,
    SS_STATE_DROP = 0x06
} SS_State_Type;
#endif
//080227 - move to server_common.h
//void init_vgsm();
void print_state_table(void);
void print_state( STATE state );
void print_state_machine(void);
int find_next_state_for_state( STATE current, STATE *next, int flag );
int find_next_state( STATE *next, int flag );
void set_state_machine( STATE state );
int change_state_machine( int main_cmd );
void get_current_state_machine( STATE *state );
void get_prev_state_machine( STATE *state );
void send_msg( void);
void set_current_state( int state, int main_cmd, int sub_cmd);
#if 0
void set_call_list( gsm_call_direction_e_type dir, gsm_call_status_e_type stat, gsm_call_type_e_type type, char *number, int num_len );
void set_call_list_status( int call_id, int mpty, gsm_call_status_e_type stat );
int delete_call_list( int call_id );
void clear_call_list();
void get_call_list( gsm_call_list_t *list );
int release_all_held_call();
int release_all_active_call();
int drop_call( int call_id );
int swap_call();
int split_call( int call_id );
int join_call();
int check_call();
//int release_call( int call_id );
//void release_all_call();
int get_active_call_count();
int get_hold_call_count();
#endif

void set_hdlc_mode( int mode );
int get_hdlc_mode(void);
void set_general_response_error( int error );
int get_general_response_error(void);
void set_ss_general_response_error( int error );
int get_ss_general_response_error(void);
void set_call_status_error( int error );
int get_call_status_error(void);
void set_current_general_response_error( int error );
int get_current_general_response_error(void);
void set_ss_current_general_response_error( int error );
int get_ss_current_general_response_error(void);
void set_current_call_status_error( int error );
int get_current_call_status_error(void);

#if 0
void make_call_id();
void set_call_id( int call_id );
int get_call_id();
void set_call_type( int type );
int get_call_type();
#endif

void set_sca( SmsAddressInfo SCA );
int get_sca( SmsAddressInfo *SCA );

#if 0 //moved to server_common_network.h
void init_plmn_list();
void set_plmn_list( unsigned char *data, int len );
gsm_network_plmn_list_t *get_plmn_list();
void init_network_registration();
void set_network_registration( unsigned char *data, int len );
gsm_network_reg_t *get_network_registration();
void set_plmn_selection( gsm_net_sel_e_type mode, unsigned char plmn[], gsm_net_act_e_type act );
gsm_net_sel_e_type get_network_sel_mode();
#endif

#if 0
/*SS*/
void set_call_waiting_entry(call_waiting_entry_t * entry, int num);
call_waiting_entry_t * get_call_waiting_entry();
call_waiting_entry_t * find_call_waiting_entry(int tel_class);

void set_call_barring_entry(call_barring_entry_t * entry, int num);
call_barring_entry_t * get_call_barring_entry();
char find_call_barring_entry(int tel_class, int type, int * count);

void set_call_forwarding_entry(call_forwarding_entry_t * entry, int num);
call_forwarding_entry_t * get_call_forwarding_entry();
call_forwarding_entry_t * find_call_forwarding_entry(int tel_class, int type, int * count);

void set_outgoing_call_barring_state(int state);
BOOL get_outgoing_call_barring_state();
#endif

/* SIM */
/* - do not use
void set_sim_sec_general_response_error( int error );
int get_sim_sec_general_response_error();
*/

/* GPRS */
void init_gprs_info(void);
GSM_GprsIPConfiguration * get_gprs_ipconfiguration_list(void);
void set_gprs_ipconfiguration_list(char *ptr, int len);

void set_ss_transfer_response_error(int error, int ss_error_type);

extern gsm_network_plmn_list_t g_plmn_list;

#endif /* _STATE_H_ */

