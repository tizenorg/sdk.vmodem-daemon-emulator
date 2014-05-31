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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "at_rx_func.h"
#include "at_tx_func.h"
#include "logmsg.h"
#include "state.h"
#include "misc.h"
#include "phoneserver.h"
//#include "db_network.h"
#include "db_gprs.h"
//#include "db_ss.h"

#include "server_tx_call.h"
#include "server_rx_call.h"
//#include "server_common_ss.h" // for init_ss_info()
//#include "server_common_network.h"

//#include "server_tx_security.h"
//#include "server_common_security.h"

#define MAX_STATE_COUNT			100


int g_hdlc_mode = 0;
int g_general_response_error = 0x8000;

int g_ss_hold_response_error = 0x8000;
int g_ss_retrieve_response_error = 0x8000;
int g_ss_join_response_error = 0x8000;
int g_ss_split_response_error = 0x8000;
int g_ss_transfer_response_error = 0x8000;

int g_call_status_error = 0x00;
int g_current_general_response_error = 0x8000;
int g_current_call_status_error = 0x00;

SmsAddressInfo g_sca = { 0x01, 0x01, 10, "8210911111" };

STATE_MACHINE state_machine_call =
{ {STATE_ON, 0, 0, 0}, {STATE_STANDBY, 0, 0, 0} };

STATE_MACHINE state_machine_sim =
{ {STATE_ON, 0, 0, 0}, {STATE_ON, 0, 0, 0} };

/* Call - end */

/* Supplementary Service - begin */
int g_ss_general_response_error = 0x8000;
int g_ss_current_general_response_error = 0x8000;
/* Supplementary Service - end */

STATE_MACHINE *state_machine = &state_machine_sim;
//STATE_MACHINE *state_machine = &state_machine_call;

/* GPRS - begin */
GprsDbEntry g_gprs_ipconf = {"", "", {0xFF, 0, {0,}, {0,}, {0,}, {0,}, {0,}}};
GprsConfList g_gprs_conflist = {0, 0};
/* GPRS - end */

STATE_TABLE state_table[MAX_STATE_COUNT] = {
    /* Call - begin */
    { {STATE_STANDBY, GSM_CALL_CMD, GSM_CALL_OUTGOING, 0},	// set by server_rx_call_originate_exec()
	{STATE_CALL_WAITING_OUTGOING, GSM_CALL_CMD, GSM_CALL_STATUS, &server_tx_call_msg} },

    { {STATE_CALL_WAITING_OUTGOING, GSM_CALL_CMD, GSM_CALL_ANSWER, 0},	// set by server_tx_call_answer_exec()
	{STATE_CALL_CONVERSATION, GSM_CALL_CMD, GSM_CALL_STATUS, &server_tx_call_msg} },

    /*    In call status msg, incoming msg do not exist.  Incoming noti msg is replaced.
	  { {STATE_STANDBY, GSM_CALL_CMD, GSM_CALL_INCOMING, 0},
	  {STATE_CALL_WAITING_INCOMING, GSM_CALL_CMD, GSM_CALL_STATUS, &TxCall_Msg} },
     */
    { {STATE_CALL_WAITING_INCOMING, GSM_CALL_CMD, GSM_CALL_ANSWER, 0}, //set by server_rx_call_answer_exec()
	{STATE_CALL_CONVERSATION, GSM_CALL_CMD, GSM_CALL_STATUS, &server_tx_call_msg} },

    { {STATE_CALL_CONVERSATION, GSM_CALL_CMD, GSM_CALL_RELEASE, 0},
	{STATE_CALL_RELEASED, GSM_CALL_CMD, GSM_CALL_STATUS, &server_tx_call_msg} },

    { {STATE_CALL_RELEASED, GSM_CALL_CMD, GSM_CALL_RELEASE, 0},	// set by server_tx_call_release_internal()
	{STATE_STANDBY, GSM_CALL_CMD, GSM_CALL_STATUS, &server_tx_call_msg} },
    /* Call - end */

    /* SIM - start */

    /* SIM - end */
    { {0,0,0},
	{0,0,0} }
};

#define TABLE_CURRENT_STATE_TYPE(x)	state_table[x].current_state.state_type
#define TABLE_CURRENT_STATE_MAIN_CMD(x)	state_table[x].current_state.main_cmd
#define TABLE_CURRENT_STATE_SUB_CMD(x)	state_table[x].current_state.sub_cmd
#define TABLE_CURRENT_STATE_FUNCTION(x)	state_table[x].current_state.state_function

#define TABLE_NEXT_STATE_TYPE(x)	state_table[x].next_state.state_type
#define TABLE_NEXT_STATE_MAIN_CMD(x)	state_table[x].next_state.main_cmd
#define TABLE_NEXT_STATE_SUB_CMD(x)	state_table[x].next_state.sub_cmd
#define TABLE_NEXT_STATE_FUNCTION(x)	state_table[x].next_state.state_function

//080227 - move to server_common.c
#if 0
void init_vgsm()
{
    // initialize network db
    vgsm_network_database_init();
    init_plmn_list();
    init_network_registration();

    // initialize sim security & phonebook db
    server_sim_db_init();


    // initialize gprs db
    vgsm_gprs_database_init();
    init_gprs_info();

    // initialize ss db
    vgsm_ss_database_init();
    init_ss_info();
}
#endif

#ifdef _DEBUG
void print_state_table(void)
{
    int i = 0;

    for( i=0; TABLE_CURRENT_STATE_TYPE(i)!=0; ++i ) {
	log_msg(MSGL_VGSM_INFO, "CURRENT_STATE: %d, %d, %d, %p ",
		TABLE_CURRENT_STATE_TYPE(i),
		TABLE_CURRENT_STATE_MAIN_CMD(i),
		TABLE_CURRENT_STATE_SUB_CMD(i),
		TABLE_CURRENT_STATE_FUNCTION(i) );
	log_msg(MSGL_VGSM_INFO, "NEXT_STATE: %d, %d, %d, %p\n",
		TABLE_NEXT_STATE_TYPE(i),
		TABLE_NEXT_STATE_MAIN_CMD(i),
		TABLE_NEXT_STATE_SUB_CMD(i),
		TABLE_NEXT_STATE_FUNCTION(i) );
    }
}
char *string_state[]={
    "STATE_NONE",
    "STATE_OFF",
    "STATE_ON",
    "STATE_STANDBY",
    /* Call - begin */
    "STATE_CALL_WAITING_INCOMING",
    "STATE_CALL_WAITING_OUTGOING",
    "STATE_CALL_CONVERSATION",
    "STATE_CALL_RELEASED",
    /* Call - end */

    /* SIM -start */

    "STATE_SIM_READY",
    "STATE_SIM_PIN",
    "STATE_SIM_FDN", // 0x10
    "STATE_SIM_PUK",

    "STATE_SIM_FRIZEN",
    "STATE_SIM_PUK2",
    "STATE_SIM_PIN2",

    "STATE_SIM_PIN_WRONG",
    "STATE_SIM_PUK_WRONG",
    "STATE_SIM_PUK2_WRONG",
    "STATE_SIM_PIN2_WRONG",

    "STATE_SIM_PIN_OK",
    "STATE_SIM_PUK_OK", //0x20
    "STATE_SIM_PUK2_OK",
    "STATE_SIM_PIN2_OK",

    "STATE_SIM_PIN_WRONG_ALL",
    "STATE_SIM_PUK_WRONG_ALL",
    "STATE_SIM_PUK2_WRONG_ALL",
    "STATE_SIM_PIN2_WRONG_ALL",

    //STATE_SIM_PIN_LOCK_DISABLE,
    //0215
    "STATE_SIM_NO_SIM",
    "STATE_ANY"
};

void print_state( STATE state )
{
    log_msg(MSGL_VGSM_INFO, "STATE: %d, %d, %d\n",
	    STATE_TYPE(state),
	    STATE_MAIN_CMD(state),
	    STATE_SUB_CMD(state) );
}

void print_state_machine2(void)
{
    log_msg(MSGL_VGSM_INFO,"===========================================\n");
    log_msg(MSGL_VGSM_INFO, "PREV_STATE: %d, %d, %d, %p \n",
	    STATE_TYPE(state_machine->prev_state),
	    STATE_MAIN_CMD(state_machine->prev_state),
	    STATE_SUB_CMD(state_machine->prev_state),
	    STATE_FUNCTION(state_machine->prev_state) );
    log_msg(MSGL_VGSM_INFO, "CURRENT_STATE: %d, %d, %d, %p\n",
	    STATE_TYPE(state_machine->current_state),
	    STATE_MAIN_CMD(state_machine->current_state),
	    STATE_SUB_CMD(state_machine->current_state),
	    STATE_FUNCTION(state_machine->current_state) );
    log_msg(MSGL_VGSM_INFO,"=============================================\n");
}

void print_state_machine(void)
{
    log_msg(MSGL_VGSM_INFO,"==========================\n");
    log_msg(MSGL_VGSM_INFO, "PREV_STATE: %s, %d, %d, %p \n",
	    string_state[STATE_TYPE(state_machine->prev_state)],
	    STATE_MAIN_CMD(state_machine->prev_state),
	    STATE_SUB_CMD(state_machine->prev_state),
	    STATE_FUNCTION(state_machine->prev_state) );
    log_msg(MSGL_VGSM_INFO, "CURRENT_STATE: %s, %d, %d, %p\n",
	    string_state[STATE_TYPE(state_machine->current_state)],
	    STATE_MAIN_CMD(state_machine->current_state),
	    STATE_SUB_CMD(state_machine->current_state),
	    STATE_FUNCTION(state_machine->current_state) );
    log_msg(MSGL_VGSM_INFO,"==========================\n");
}
#endif

int find_next_state_for_state( STATE current, STATE *next, int flag )
{
    int i = 0;

    for( i=0; TABLE_CURRENT_STATE_TYPE(i)!=0; ++i ) {
	if( flag == STATE_FLAG_ALL ) {
	    if( ( ( STATE_TYPE(current) == TABLE_CURRENT_STATE_TYPE(i) ) ||
			( TABLE_CURRENT_STATE_TYPE(i) == STATE_ANY ) ) &&
		    ( STATE_MAIN_CMD(current) == TABLE_CURRENT_STATE_MAIN_CMD(i) ) &&
		    ( STATE_SUB_CMD(current) == TABLE_CURRENT_STATE_SUB_CMD(i) ) ) {
		memcpy( next, &state_table[i].next_state, sizeof( STATE ) );

		return 1;
	    }
	}
	else if( ( flag & STATE_FLAG_TYPE ) &&
		( ( STATE_TYPE(current) == TABLE_CURRENT_STATE_TYPE(i) ) ||
		  ( TABLE_CURRENT_STATE_TYPE(i) == STATE_ANY )
		) &&
		( flag & STATE_FLAG_MAIN_CMD ) &&
		( STATE_MAIN_CMD(current) == TABLE_CURRENT_STATE_MAIN_CMD(i) )
	       ) {
	    memcpy( next, &state_table[i].next_state, sizeof( STATE ) );

	    return 1;
	}
	else if( ( ( flag == STATE_FLAG_TYPE ) &&
		    ( ( STATE_TYPE(current) == TABLE_CURRENT_STATE_TYPE(i) ) ||
		      ( TABLE_CURRENT_STATE_TYPE(i) == STATE_ANY )
		    )
		 )
	       ) {
	    memcpy( next, &state_table[i].next_state, sizeof( STATE ) );

	    return 1;
	}
	else if( ( flag == STATE_FLAG_MAIN_CMD ) &&
		( STATE_MAIN_CMD(current) == TABLE_CURRENT_STATE_MAIN_CMD(i) )
	       ) {
	    memcpy( next, &state_table[i].next_state, sizeof( STATE ) );

	    return 1;
	}
	else if( ( flag == STATE_FLAG_SUB_CMD ) &&
		( STATE_SUB_CMD(current) == TABLE_CURRENT_STATE_SUB_CMD(i) )
	       ) {
	    memcpy( next, &state_table[i].next_state, sizeof( STATE ) );

	    return 1;
	}
    }

    return 0;
}

int find_next_state( STATE *next, int flag )
{
    STATE state;
    int ret = 0;

    _ENTER();

    get_current_state_machine( &state );
    ret = find_next_state_for_state( state, next, flag );

#ifdef	_DEBUG
    if( ret )
	print_state(*next);
#endif /* _DEBUG */

    _LEAVE();

    return ret;
}

void set_state_machine( STATE state )
{
    int type = STATE_TYPE(state_machine->current_state);

    _ENTER();

    memcpy( &state_machine->prev_state, &state_machine->current_state, sizeof( STATE ) );
    memcpy( &state_machine->current_state, &state, sizeof( STATE ) );

    if( ( STATE_TYPE(state) == STATE_NONE ) || ( STATE_TYPE(state) == STATE_ANY ) )
	STATE_TYPE(state_machine->current_state) = type;

#ifdef	_DEBUG
    print_state_machine();
#endif /* _DEBUG */

    _LEAVE();
}

int change_state_machine( int main_cmd )
{
    _ENTER();

    switch( main_cmd ) {
	case GSM_CALL_CMD:  /* 0x02 : Call Control Commands */
	case GSM_SS_CMD:    /* 0x0C : Supplementary Service Control Command */
	    state_machine = &state_machine_call;
	    break;
	case GSM_SEC_CMD:   /* 0x05 : Security - SIM control Commands */
	    state_machine = &state_machine_sim;
	    break;

	case GSM_PWR_CMD:   /* 0x01 : Power Control Commands */
	    break;
	case GSM_DATA_CMD:  /* 0x03 : Data Control Commands */
	    break;
	case GSM_SMS_CMD:   /* 0x04 : Short Message Service Commands */
	    break;
	case GSM_PB_CMD:    /* 0x06 : Phonebook Control Commands */
	case GSM_DISP_CMD:  /* 0x07 : Display Control Commands */
	case GSM_NET_CMD:   /* 0x08 : Network Commands */
	case GSM_SND_CMD:   /* 0x09 : Sound Control Commands */
	case GSM_MISC_CMD:  /* 0x0A : Miscellaneous Control Commands */
	case GSM_SVC_CMD:   /* 0x0B : Service Mode Control Commands - Factory Test or Debug Screen Control */
	case GSM_GPRS_CMD:  /* 0x0D : GPRS Commands */
	case GSM_SAT_CMD:   /* 0x0E : SIM Toolkit Commands */
	case GSM_CFG_CMD:   /* 0x0F : Configuration Commands */
	case GSM_GEN_CMD:   /* 0x80 : General Response Commands */
	default:
	    break;
    }

    _LEAVE();

    return 1;
}

void send_msg( void)
{
    STATE state;

    _ENTER();

    get_current_state_machine( &state );
    log_msg(MSGL_VGSM_INFO," exe state_type %d, state main_cmd=%d sub_cmd =%d\n", state.state_type,state.main_cmd,state.sub_cmd);

    if( state.state_function )
    {
	log_msg(MSGL_VGSM_CALL,"state.function exe !! \n");

	state.state_function();
    }

    _LEAVE();
}

void get_current_state_machine( STATE *state )
{
    _ENTER();

    memcpy( state, &state_machine->current_state, sizeof( STATE ) );

    _LEAVE();
}

void get_prev_state_machine( STATE *state )
{
    _ENTER();

    memcpy( state, &state_machine->prev_state, sizeof( STATE ) );

    _LEAVE();
}

void set_current_state( int state, int main_cmd, int sub_cmd)
{
    _ENTER();

    STATE st;
    STATE_TYPE(st) = state;
    STATE_MAIN_CMD(st) = main_cmd;
    STATE_SUB_CMD(st) = sub_cmd;
    //STATE_FUNCTION(st) = func;
    set_state_machine( st );
    _LEAVE();
}

void set_hdlc_mode( int mode )
{
    g_hdlc_mode = mode;
}

int get_hdlc_mode()
{
    return g_hdlc_mode;
}

void set_ss_transfer_response_error( int error ,int ss_error_type)
{

    log_msg(MSGL_VGSM_INFO,"---<set_ss_response_error>--- error : %x type : %d\n", error,ss_error_type);
    switch ( ss_error_type)
    {
	case GSM_EMULATOR_SET_SS_HOLD_RESPONSE_ERROR :
	    g_ss_hold_response_error = error;
	    break;
	case GSM_EMULATOR_SET_SS_RETRIEVE_RESPONSE_ERROR:
	    g_ss_retrieve_response_error = error;
	    break;
	case GSM_EMULATOR_SET_SS_JOIN_RESPONSE_ERROR:
	    g_ss_join_response_error = error;
	    break;
	case GSM_EMULATOR_SET_SS_SPLIT_RESPONSE_ERROR:
	    g_ss_split_response_error = error;
	    break;
	case GSM_EMULATOR_SET_SS_TRANSFER_RESPONSE_ERROR :
	    g_ss_transfer_response_error = error;
	    break;
	case GSM_EMULATOR_SET_SS_GENERAL_RESPONSE_ERROR:
	    g_ss_general_response_error = error;
	    break;
    }
}

void set_general_response_error( int error )
{
    log_msg(MSGL_VGSM_INFO,"---<set_general_response_error>---general response error : %x \n", error);
    g_general_response_error = error;
}

int get_general_response_error()
{
    log_msg(MSGL_VGSM_INFO,"---<get_general_response_error>---general response error : %x \n", g_general_response_error);
    return g_general_response_error;
}

void set_ss_general_response_error( int error )
{
    g_ss_general_response_error = error;
}

int get_ss_general_response_error()
{
    return g_ss_general_response_error;
}

void set_call_status_error( int error )
{
    g_call_status_error = error;
}

int get_call_status_error()
{
    return g_call_status_error;
}

void set_current_general_response_error( int error )
{
    g_current_general_response_error = error;
}

int get_current_general_response_error()
{
    return g_current_general_response_error;
}

void set_ss_current_general_response_error( int error )
{
    g_ss_current_general_response_error = error;
}

int get_ss_current_general_response_error()
{
    return g_ss_current_general_response_error;
}

void set_current_call_status_error( int error )
{
    g_current_call_status_error = error;
}

int get_current_call_status_error()
{
    return g_current_call_status_error;
}

#if 0
void set_sca( SmsAddressInfo SCA )
{
    memcpy( &g_sca, &SCA, sizeof( SmsAddressInfo ) );
}

int get_sca( SmsAddressInfo *SCA )
{
    memcpy( SCA, &g_sca, sizeof( SmsAddressInfo ) );

    return 1;
}
#endif

static void int_to_string(char *res, unsigned int s_value, int s_radix)
{
    static const char c_base_ascii[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    static char s_buffer[64] = {0, };
    size_t s_offset = sizeof(s_buffer) - 1;

    do {
	s_buffer[--s_offset] = c_base_ascii[ s_value % s_radix ];
	s_value /= s_radix;
    } while (s_value != 0u);

    strcpy(res, (char *)(&s_buffer[s_offset]));

    return;
}

/*UNUSED! */ int change_ip_to_ipstring(unsigned char *ip_addr, char* message, int msg_len);
int change_ip_to_ipstring(unsigned char *ip_addr, char* message, int msg_len)
{
    int i = 0;					//, j = 0;
    char temp[20];			//, ret;

    memset(message, 0, msg_len);

    for (i = 0; i < 4; i++) {
	int_to_string(temp, ip_addr[i], 10);

	strcat(message, temp);

	if (i != 3)
	    strcat(message, ".");
    }

    return 0;
}
