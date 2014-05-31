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

/****************************************
 *****    server_common_call.c
 *****************************************/

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "logmsg.h"
#include "misc.h"
#include "phoneserver.h"
#include "state.h"

#include "server_tx_call.h"
#include "server_rx_call.h"
#include "server_common_call.h"
#include "state.h" // for _ENTER()
#include "vgsm_ss.h"

#define MAX_CONFERENCE_CALL_COUNT	5

static gsm_call_list_t g_call_list;
static int g_call_id = -1;
static int g_call_type = -1;
static int g_call_line_id = 0; // default line id is 0.
static ss_hold_act_state_e_type g_ss_hold_act_state =  SS_NONE;

void set_call_list( gsm_call_direction_e_type dir, gsm_call_status_e_type stat, gsm_call_type_e_type type, char *number, int num_len )
{
    _ENTER();

    int call_id = get_call_id();

    if( g_call_list.CallCount >= MAX_CALL_COUNT )
	return;

    assert(valid_call_type(type));

    g_call_list.CallCount++;
    log_msg(MSGL_VGSM_INFO, "CallCount: %d\n", g_call_list.CallCount);
    //g_call_list.CallCount = 1; // for single call

    g_call_list.CallInfo[call_id].idx = call_id;
    g_call_list.CallInfo[call_id].dir = dir;
    g_call_list.CallInfo[call_id].stat = stat;
    g_call_list.CallInfo[call_id].call_type = type;
    g_call_list.CallInfo[call_id].mpty = 0x00;
    memcpy( g_call_list.CallInfo[call_id].number, number, num_len );
    g_call_list.CallInfo[call_id].num_len = num_len;
    g_call_list.CallInfo[call_id].num_type = 0x01; //0x23;

    _LEAVE();
}

void get_call_list( gsm_call_list_t *list )
{
    memcpy( list, &g_call_list, sizeof( gsm_call_list_t ) );
}

void set_call_list_status( int call_id, int mpty, gsm_call_status_e_type stat )
{
    _ENTER();

    if( g_call_list.CallCount > MAX_CALL_COUNT )
	return;

    g_call_list.CallInfo[call_id].stat = stat;
    g_call_list.CallInfo[call_id].mpty = 0x00;

    _LEAVE();
}

int delete_call_list( int call_id )
{
    _ENTER();

    log_msg(MSGL_VGSM_INFO,"delete call_id : %x\n", call_id);
    if( call_id < 0 || call_id >= MAX_CALL_COUNT )
	return 0;

    memset( &(g_call_list.CallInfo[call_id]), 0, sizeof( gsm_call_list_info_t ) );

    g_call_list.CallCount--;

    if( g_call_list.CallCount < 0 )
	g_call_list.CallCount = 0;

    _LEAVE();

    return 1;
}

void clear_call_list(void)
{
    _ENTER();

    memset( &g_call_list, 0, sizeof( gsm_call_list_t ) );
    //g_call_list.CallCount = 0;

    _LEAVE();
}

// 090320
int get_callcount(void)
{
    return g_call_list.CallCount;
}

int get_active_call_count(void)
{
    int i = 0, active = 0;

    for( i=0; i<MAX_CALL_COUNT; ++i ) {
	switch( g_call_list.CallInfo[i].stat ) {
	    case GSM_CALL_STATUS_ACTIVE:
		active++;
		break;
	    default:
		break;
	}
    }

    return active;
}

int get_hold_call_count(void)
{
    int i = 0, hold = 0;

    for( i=0; i<MAX_CALL_COUNT; ++i ) {
	switch( g_call_list.CallInfo[i].stat ) {
	    case GSM_CALL_STATUS_HELD:
		hold++;
		break;
	    default:
		break;
	}
    }

    return hold;
}
int is_prev_ss_state(ss_hold_act_state_e_type prev_state)
{
    if( prev_state == g_ss_hold_act_state )
	return 1;
    return 0;
}

void set_ss_state(ss_hold_act_state_e_type state)
{
    g_ss_hold_act_state = state;
}

void set_rx_ss_state(void)
{
    int i;
    for( i=0; i<MAX_CALL_COUNT; ++i )
    {
	switch( g_call_list.CallInfo[i].stat )
	{
	    case GSM_CALL_STATUS_ACTIVE:
		set_ss_state(SS_RX_HOLD);
		break;
	    case GSM_CALL_STATUS_HELD:
		set_ss_state(SS_RX_ACTIVATE);
		break;

	    default:
		break;
	}
    }
}

int swap_call(void)
{
    _ENTER();

    int i = 0, active = 0, hold = 0;
    unsigned char *data = 0;
    LXT_MESSAGE packet;

    active = get_active_call_count();
    hold = get_hold_call_count();

    for( i=0; i<MAX_CALL_COUNT; ++i ) {
	switch( g_call_list.CallInfo[i].stat ) {
	    case GSM_CALL_STATUS_ACTIVE:
		{
		    g_call_list.CallInfo[i].stat = GSM_CALL_STATUS_HELD;
		    break;
		}
	    case GSM_CALL_STATUS_WAITING:
		{
		    //STATE next;
		    set_call_id( i );
		    server_rx_call_answer_exec();
		    /*
		       if( find_next_state( &next, STATE_FLAG_ALL ) ) {
		       set_state_machine( next );
		       send_msg();
		       }
		     */
		    g_call_list.CallInfo[i].stat = GSM_CALL_STATUS_ACTIVE;
		}
		break;
	    case GSM_CALL_STATUS_HELD:
		if( ( active > 0 ) || ( hold == g_call_list.CallCount ) )
		    g_call_list.CallInfo[i].stat = GSM_CALL_STATUS_ACTIVE;
		break;
	    case GSM_CALL_STATUS_DIALING:
		{
		    //STATE next;
		    set_call_id( i );
		    server_rx_call_answer_exec();
		    /*
		       if( find_next_state( &next, STATE_FLAG_ALL ) )
		       {
		       set_state_machine( next );
		       send_msg();
		       }
		     */
		    g_call_list.CallInfo[i].stat = GSM_CALL_STATUS_ACTIVE;
		}
		break;

	    default:
		break;
	}
    }

    TAPIMessageInit(&packet);

    data = malloc(sizeof(unsigned char)*1);
    if( active == g_call_list.CallCount )
	data[0] = SS_STATE_HOLD; /* hold */
    else if( hold == g_call_list.CallCount )
	data[0] = SS_STATE_UNHOLD; /* unhold */
    else if( active != 0 && hold != 0 )
	data[0] = SS_STATE_SWAP; /* swap */

    packet.data = data;
    packet.length = 1;

    packet.group  = GSM_SUPS;
    packet.action = GSM_SUPS_STATE;
    packet.length = 1;

    FuncServer->Cast(&GlobalPS, LXT_ID_CLIENT_EVENT_INJECTOR, &packet);

    free(data);

    _LEAVE();

    return 1;
}

int drop_call( int call_id )
{
    _ENTER();

    int i = 0;
    int active_call_id = -1;
    unsigned char *data = 0;
    LXT_MESSAGE packet;

    if( call_id < 0 || call_id >= MAX_CALL_COUNT )
	return 0;
    /*
       for( i=0; i<MAX_CALL_COUNT; ++i ) {
       switch( g_call_list.CallInfo[i].stat ) {
       case GSM_CALL_STATUS_ACTIVE:
       if( g_call_list.CallInfo[i].idx == call_id )
       server_tx_call_release_internal( g_call_list.CallInfo[i].idx );
       else
       {
       if( active_call_id == -1 )
       active_call_id = g_call_list.CallInfo[i].idx;		// 1 active call
       else if( active_call_id >= 0 )
       active_call_id = -2;						// 2 or more active calls
       }

       break;
       default: // TODO: held call release check!!
       break;
       }
       }
     */
    for( i=0; i<MAX_CALL_COUNT; ++i )
    {
	if( g_call_list.CallInfo[i].idx == call_id )
	{
	    server_tx_call_release_internal( g_call_list.CallInfo[i].idx );
	    active_call_id = call_id;
	    break;

	}
    }

    if( active_call_id >= 0 )
	g_call_list.CallInfo[active_call_id].mpty = 0x00;

    TAPIMessageInit(&packet);

    data = malloc(sizeof(unsigned char)*1);
    data[0] = SS_STATE_DROP; /* drop */

    packet.data = data;
    packet.length = 1;

    packet.group  = GSM_SUPS;
    packet.action = GSM_SUPS_STATE;
    packet.length = 1;

    FuncServer->Cast(&GlobalPS, LXT_ID_CLIENT_EVENT_INJECTOR, &packet);

    free(data);

    _LEAVE();

    return 1;
}

int split_call( int call_id )
{
    _ENTER();

    int i = 0, active = 0, hold = 0;
    unsigned char *data = 0;
    LXT_MESSAGE packet;

    hold = get_hold_call_count();
    if( hold > 0 )
	return 0;

    active = get_active_call_count();

    for( i=0; i<MAX_CALL_COUNT; ++i ) {
	switch( g_call_list.CallInfo[i].stat ) {
	    case GSM_CALL_STATUS_ACTIVE:
		if( g_call_list.CallInfo[i].idx != call_id )
		{
		    g_call_list.CallInfo[i].stat = GSM_CALL_STATUS_HELD;
		    if( active == 2 )
			g_call_list.CallInfo[i].mpty = 0x00;
		}
		else
		    g_call_list.CallInfo[i].mpty = 0x00;
		break;

	    default:
		break;
	}
    }

    TAPIMessageInit(&packet);

    data = (unsigned char *)malloc(sizeof(unsigned char)*1);
    data[0] = SS_STATE_SPLIT; /* split */

    packet.data = data;
    packet.length = 1;

    packet.group  = GSM_SUPS;
    packet.action = GSM_SUPS_STATE;
    packet.length = 1;

    FuncServer->Cast(&GlobalPS, LXT_ID_CLIENT_EVENT_INJECTOR, &packet);

    free(data);

    _LEAVE();

    return 1;
}

int join_call(void)
{
    _ENTER();

    int i = 0;
    unsigned char *data = 0;
    LXT_MESSAGE packet;

    log_msg(MSGL_VGSM_INFO, "%d, %d\n", get_active_call_count(), get_hold_call_count() );

    if( get_hold_call_count() >= MAX_CONFERENCE_CALL_COUNT )
	return 0;

    for( i=0; i<MAX_CALL_COUNT; ++i ) {
	switch( g_call_list.CallInfo[i].stat ) {
	    case GSM_CALL_STATUS_HELD:
		g_call_list.CallInfo[i].stat = GSM_CALL_STATUS_ACTIVE;
	    case GSM_CALL_STATUS_ACTIVE:
		g_call_list.CallInfo[i].mpty = 0x01;
		break;
	    default:
		break;
	}
    }

    TAPIMessageInit(&packet);

    data = (unsigned char *)malloc(sizeof(unsigned char)*1);
    data[0] = SS_STATE_CONFERENCE; /* conference */

    packet.data = data;
    packet.length = 1;

    packet.group  = GSM_SUPS;
    packet.action = GSM_SUPS_STATE;
    packet.length = 1;

    FuncServer->Cast(&GlobalPS, LXT_ID_CLIENT_EVENT_INJECTOR, &packet);

    free(data);

    _LEAVE();

    return 1;
}

int check_call(void)
{
    int i = 0, active = 0, hold = 0;

    for( i=0; i<MAX_CALL_COUNT; ++i ) {
	switch( g_call_list.CallInfo[i].stat ) {
	    case GSM_CALL_STATUS_HELD:
		hold++;
		break;
	    case GSM_CALL_STATUS_ACTIVE:
		active++;
		break;
	    default:
		break;
	}
    }

    if( active != 0 && hold != 0 )
	return 0;
    else
	return 1;
}

int active_waiting_or_held_call(bool onWaiting)
{
    int i = 0;

    for( i = 0; i<MAX_CALL_COUNT; ++i ) {
	switch( g_call_list.CallInfo[i].stat ) {
	    case GSM_CALL_STATUS_WAITING:
		{
		    set_call_id( i );
		    server_rx_call_answer_exec();
		    break;
		}
	    case GSM_CALL_STATUS_HELD:
		if(onWaiting == false)
		    g_call_list.CallInfo[i].stat = GSM_CALL_STATUS_ACTIVE;
		break;
	    default:
		break;
	}
    }

    return 1;
}

int release_all_active_call(void)
{
    int i = 0;
    bool onWaiting = false;

    for( i = 0; i<MAX_CALL_COUNT; ++i ) {
	switch( g_call_list.CallInfo[i].stat ) {
	    case GSM_CALL_STATUS_ACTIVE:
		server_tx_call_release_internal( g_call_list.CallInfo[i].idx );
		break;
	    case GSM_CALL_STATUS_WAITING:
		onWaiting = true;
		break;
	    default:
		break;
	}
    }

    return active_waiting_or_held_call(onWaiting);
}

int release_incoming_call(void)
{
    int i = 0, releaseCnt = 0;

    for( i=0; i<MAX_CALL_COUNT; ++i )
    {
	switch( g_call_list.CallInfo[i].stat )
	{
	    case GSM_CALL_STATUS_INCOMING :
	    case GSM_CALL_STATUS_WAITING :
		server_tx_call_release_internal( g_call_list.CallInfo[i].idx );
		releaseCnt++;
		break;
	    default:
		break;
	}
    }   

    return releaseCnt;
}

int release_all_held_call(void)
{
    int i = 0;

    for( i=0; i<MAX_CALL_COUNT; ++i )
    {
	switch( g_call_list.CallInfo[i].stat )
	{
	    case GSM_CALL_STATUS_HELD:
		server_tx_call_release_internal( g_call_list.CallInfo[i].idx );
		break;
	    default:
		break;
	}
    }

    return 1;
}

// End of Call.


int make_call_id(void)
{
    int i = 0;

    if( g_call_list.CallCount == 0 ) {
	g_call_id = 0;
	return g_call_id;
    }

    for( i=0; i<MAX_CALL_COUNT; ++i ) {
	if( g_call_list.CallInfo[i].stat == GSM_CALL_STATUS_NONE ) {
	    g_call_id = i;
	    break;
	}
    }
    return g_call_id;
}

void set_call_id( int call_id )
{
    if( call_id < 0 || call_id >= MAX_CALL_COUNT )
	return;
    g_call_id = call_id;
}

int get_call_id(void)
{
    return g_call_id;
}

void set_call_type( int type )
{
    assert(valid_call_type(type));
    g_call_type = type;
}

int get_call_type(void)
{
    assert(valid_call_type(g_call_type));
    return g_call_type;
}

void set_call_line_id( int line_id )
{
    g_call_line_id = line_id;
}

int get_call_line_id(void)
{
    return g_call_line_id;
}

int valid_call_type(int call_type)
{
    return 1;
}

/*   check general response error & call status error    */
int check_call_error(void)
{
    int gen_resp_err;
    int call_status_err;

    gen_resp_err = get_general_response_error();
    set_current_general_response_error( gen_resp_err );
    call_status_err = get_call_status_error();
    set_current_call_status_error( call_status_err);
    return gen_resp_err;
}

