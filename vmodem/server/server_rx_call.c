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

/*	server_rx_call.c	*/

#include <assert.h>

#include "server_tx_call.h"
#include "server_rx_call.h"
#include "linuxtapi.h"
#include "phoneserver.h"
#include "vgsm_call.h"
#include "state.h"
#include "misc.h"
#include "oem_tx_call.h"
#include "server_common_call.h"
#include "db_ss.h"
#include "logmsg.h"
#include "at_recv.h"
#include "at_gen_resp.h"
#include "at_func.h"

#define MAX_GSM_DIALED_DIGITS_NUM           82
// caution: MAX_GSM_DIALED_DIGITS_NUMBER and MAX_GSM_DIALED_DIGITS_NUM are confusingly similar

int server_rx_call_list_get(void)
{
    TRACE(MSGL_VGSM_INFO, "\n");
    server_tx_call_list_resp();
    return 1;
}

int server_rx_call_answer_exec(void)
{
    STATE next;

    TRACE(MSGL_VGSM_INFO, "\n");

    unsigned char data[1];
    LXT_MESSAGE packet;
    TAPIMessageInit(&packet);
    int call_gen_resp_err=-1;

    /*   check general response error & call status error    */
    call_gen_resp_err = check_call_error();

    data[0] = 0x02; 		//The object is simulator,   0x01 : Outgoing Call, 0x02 : Answer Call, 0x03 : Release Call

    packet.data = data;
    packet.length = sizeof data;
    packet.group  = GSM_CALL;
    packet.action = GSM_CALL_STATE;

    FuncServer->Cast(&GlobalPS, LXT_ID_CLIENT_EVENT_INJECTOR, &packet);

    set_current_state( STATE_CALL_WAITING_INCOMING, GSM_CALL_CMD, GSM_CALL_ANSWER);

    if( find_next_state( &next, STATE_FLAG_ALL ) ) {
	set_state_machine( next );
	send_msg();
    }

    return 1;
}

int server_rx_call_originate_exec(void *ptr_data, int data_len )
{
    const unsigned char *p = ptr_data;
    unsigned short call_type;
    unsigned char clir_status;
    unsigned char num_len;
    unsigned char num_type;
    const unsigned char *number;
    unsigned char call_id;
    STATE next;

    number = malloc(MAX_GSM_DIALED_DIGITS_NUM+1);
    if(!number)
	return -1;
    TRACE(MSGL_VGSM_INFO, "\n");
    char* call_data = strchr((char*)p, 'D');
    char token[] = ";";
    char* ret = NULL;

    ret = strtok(call_data+1, token);
    if(ret)
	strcpy((char*)number, ret);

    // DOCUMENTATION ERROR
    //
    // Section 1.1.1 Call Outgoing Message (SUB_CMD = 0x01) claims:
    //
    // NUMBER[ ]     variable     Destination number in ASCII.
    //                             : Maximum length is 82 bytes
    //
    // but the NUMBER is not variable, it is fixed at 82 bytes

    call_type = GSM_CALL_TYPE_VOICE;

    assert(valid_call_type(call_type));

    clir_status = GSM_CALL_STATUS_DIALING;
    num_len = strlen((char*)number);
    num_type = GSM_NUM_TYPE_INTERNATIONAL;

    assert(num_len <= MAX_GSM_DIALED_DIGITS_NUM);


    if(get_outgoing_voice_call_barring_state())
    {
	TRACE(MSGL_VGSM_INFO, "call is barred!\n");

	oem_tx_call_gen_resp(AT_CME_ERR_OPER_NOT_ALLOWED);

	if(number)
	    free(number);
	return 1;
    }
    oem_tx_call_gen_resp(AT_GEN_ERR_NO_ERROR);

    call_id = make_call_id();
    set_call_type( call_type );

    unsigned char data[MAX_GSM_DIALED_DIGITS_NUMBER+4];

    if (num_len >= MAX_GSM_DIALED_DIGITS_NUMBER)
	num_len = MAX_GSM_DIALED_DIGITS_NUMBER - 1;

    data[0] = call_id;
    data[1] = call_type;

    // On this path, the condition "clir_status == GSM_CALL_CLIR_STATUS_INVOCATION" cannot be true.
    // Because clir_status = GSM_CALL_STATUS_DIALING;
    //if(clir_status == GSM_CALL_CLIR_STATUS_INVOCATION)
    //	num_len = 0;

    data[2] = num_len;
    data[3] = clir_status;

    memset(&data[4], 0, MAX_GSM_DIALED_DIGITS_NUMBER);
    memcpy(&data[4], number, num_len);
    data[4+num_len] = '\0';

    LXT_MESSAGE packet;
    TAPIMessageInit(&packet);
    packet.data = data;
    packet.group  = GSM_CALL;
    packet.action = GSM_CALL_MAKE_REQ;
    packet.length = num_len + 4;

    FuncServer->Cast(&GlobalPS, LXT_ID_CLIENT_EVENT_INJECTOR, &packet);

    set_call_list( GSM_CALL_DIR_MO, GSM_CALL_STATUS_DIALING, call_type, (char*)number, num_len );

    callback_callist();

    data[0] = 0x01; //The object is simulator,   0x01 : Outgoing Call, 0x02 : Answer Call, 0x03 : Release Call

    packet.data = data;
    packet.length = 1;
    packet.group  = GSM_CALL;
    packet.action = GSM_CALL_STATE;
    packet.length = 1;

    FuncServer->Cast(&GlobalPS, LXT_ID_CLIENT_EVENT_INJECTOR, &packet);

    set_current_state( STATE_STANDBY, GSM_CALL_CMD, GSM_CALL_OUTGOING);

    if( find_next_state( &next, STATE_FLAG_ALL ) ) {
	set_state_machine( next );
	send_msg();
    }
    if(number)
	free(number);
    return 1;
}

int server_rx_call_release_exec(void) // for all call release case. one call release would be received by SS MANAGE CALL msg.
{
    int call_gen_resp_err;

    /* check general response error & call status error */
    call_gen_resp_err = check_call_error();
    server_tx_call_all_release();
    return 1;
}

int server_rx_call_burst_dtmf_exec(void)
{
    int success = 1;

    server_tx_call_burst_dtmf_noti(success);
    return 1;
}

int server_rx_call_line_id_set(void *ptr_data, int data_len )
{
    const unsigned char *p = ptr_data;
    unsigned char line_id;
    int n = 0;

    TRACE(MSGL_VGSM_INFO, "\n");

    line_id = p[n++];

    if (n != data_len)
	TRACE(MSGL_WARN, "data_len = %d, expected = %d\n", data_len, n);

    set_call_line_id(line_id);

    return 1;
}


int server_rx_call_line_id_get(void)
{
    TRACE(MSGL_VGSM_INFO, "\n");
    server_tx_call_line_id_resp();
    return 1;
}

int server_rx_call_release_incoming(void)
{       
    TRACE(MSGL_VGSM_INFO, "\n");
    int releaseCnt = 0;

    releaseCnt = release_incoming_call();
    if(releaseCnt > 0)
	server_tx_call_list_noti();

    return releaseCnt;
}

/*
 *	0_SEND=0x01,
 *	Releases all held calls or sets User Determined User Busy (UDUB) for a waiting call
 */
int server_rx_call_release_all_held(void)
{
    TRACE(MSGL_VGSM_INFO, "\n");

    release_all_held_call();
    server_tx_call_list_noti();
    return 1;
}


int server_rx_call_release_all_active(void)
{
    TRACE(MSGL_VGSM_INFO, "\n");

    release_all_active_call();
    server_tx_call_list_noti();

    return 1;
}

int server_rx_call_release_single(int call_id)
{
    TRACE(MSGL_VGSM_INFO, "\n");

    drop_call(call_id);
    server_tx_call_list_noti();

    return 1;
}

int server_rx_call_swap(void)
{
    TRACE(MSGL_VGSM_INFO, "\n");

    set_rx_ss_state();
    swap_call();
    server_tx_call_list_noti();

    return 1;
}

int server_rx_call_split_conf(int call_id)
{
    TRACE(MSGL_VGSM_INFO, "\n");

    split_call(call_id);
    server_tx_call_list_noti();

    return 1;
}

int server_rx_call_join_conf(void)
{
    TRACE(MSGL_VGSM_INFO, "\n");

    join_call();
    server_tx_call_list_noti();

    return 1;
}



