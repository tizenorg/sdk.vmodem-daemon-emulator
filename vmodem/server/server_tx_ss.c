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
// server_tx_ss.c

#include <stdlib.h>
#include <string.h>

#include "vgsm_ss.h"  // for ss_state_e_type
#include "misc.h"	// LXT_MESSAGE(), TAPIMessageInit()
#include "vgsm_phone.h"
#include "server_tx_call.h"
#include "server_common_call.h"
#include "server_common_ss.h"
#include "server_tx_ss.h"
#include "server_rx_ss.h"
#include "oem_tx_ss.h"
#include "vgsm_debug.h"
#include "logmsg.h"
#include "db_ss.h"

// 090406 /////////////////////////////////////////////////////////////////////////////////////////
void server_tx_ss_cw_set_req ( LXT_MESSAGE *packet)
{
    int *a;

    call_waiting_entry_t * entry = malloc(sizeof(call_waiting_entry_t));
    if(!entry)
	return;
    memset(entry, 0, sizeof(call_waiting_entry_t));

    TRACE(MSGL_VGSM_SS,"\n");
    a = (int*)packet->data;
    TRACE(MSGL_VGSM_SS,"class: %d\n", a[0]);
    TRACE(MSGL_VGSM_SS,"status: %d\n", a[1]);

    if (a[1] == 0)				// Activation
    {
	entry->ss_mode = SS_MODE_ACT;

	switch (a[0])
	{
	    case 0:				// All
		entry->tel_class = -1;

		break;
	    case 1:				// Voice
		entry->tel_class = 1;

		break;
	    case 2:				// Fax
		entry->tel_class = 4;

		break;
	    case 3:				// SMS
		entry->tel_class = 8;

		break;
	    case 4:				// async
		entry->tel_class = 32;

		break;
	}
    }
    else if(a[1] == 1)			// Deactivation
    {
	entry->ss_mode = SS_MODE_DEACT;

	switch (a[0])
	{
	    case 0:				// All
		entry->tel_class = -1;

		break;
	    case 1:				// Voice
		entry->tel_class = 1;

		break;
	    case 2:				// Fax
		entry->tel_class = 4;

		break;
	    case 3:				// SMS
		entry->tel_class = 8;

		break;
	    case 4:				// async
		entry->tel_class = 32;

		break;
	}
    }
    else
    {
	TRACE(MSGL_VGSM_SS,"status isn't 0 or 1 : %d\n", a[1]);
    }

    vgsm_ss_database_add(SS_CMD_CW, entry);

    init_ss_info_re();						// restore data list in EI

    if(entry)
	free(entry);
}

void server_tx_ss_cf_set_req ( LXT_MESSAGE *packet)
{
    // insert cfdata here
    int *a;

    call_forwarding_entry_t * entry = malloc(sizeof(call_forwarding_entry_t));
    if(!entry)
	return;
    memset(entry, 0, sizeof(call_forwarding_entry_t));

    TRACE(MSGL_VGSM_SS,"\n");
    a = (int*)packet->data;
    TRACE(MSGL_VGSM_SS,"code: %d, number: %d\n", a[0], a[1]);
    TRACE(MSGL_VGSM_SS,"BC: %d, Timer: %d\n", a[2], a[3]);
    TRACE(MSGL_VGSM_SS,"status: %d\n", a[4]);

    if (a[4] == 0)				// Registeration
    {
	entry->ss_mode = SS_MODE_REG;

	switch (a[0])
	{
	    case 0:				// unconditional
		{
		    entry->type = 0x01;
		    switch(a[2])
		    {
			case 0:
			    entry->tel_class = -1;
			    break;
			case 1:
			    entry->tel_class = 0x11;
			    break;
			case 2:
			    entry->tel_class = 0x13;
			    break;
			case 3:
			    entry->tel_class = 0x16;
			    break;
			case 4:
			    entry->tel_class = 0x21;
			    break;
		    }
		}
		break;
	    case 1:				// when busy
		{
		    entry->type = 0x02;
		    switch(a[2])
		    {
			case 0:
			    entry->tel_class = -1;
			    break;
			case 1:
			    entry->tel_class = 0x11;
			    break;
			case 2:
			    entry->tel_class = 0x13;
			    break;
			case 3:
			    entry->tel_class = 0x16;
			    break;
			case 4:
			    entry->tel_class = 0x21;
			    break;
		    }
		}

		break;
	    case 2:				// no rely
		{
		    entry->type = 0x03;
		    switch(a[2])
		    {
			case 0:
			    entry->tel_class = -1;
			    break;
			case 1:
			    entry->tel_class = 0x11;
			    break;
			case 2:
			    entry->tel_class = 0x13;
			    break;
			case 3:
			    entry->tel_class = 0x16;
			    break;
			case 4:
			    entry->tel_class = 0x21;
			    break;
		    }
		}

		break;
	    case 3:				// Not reachable
		{
		    entry->type = 0x04;
		    switch(a[2])
		    {
			case 0:
			    entry->tel_class = -1;
			    break;
			case 1:
			    entry->tel_class = 0x11;
			    break;
			case 2:
			    entry->tel_class = 0x13;
			    break;
			case 3:
			    entry->tel_class = 0x16;
			    break;
			case 4:
			    entry->tel_class = 0x21;
			    break;
		    }
		}

		break;
	}
    }
    else if(a[4] == 1)			// Deregisteration
    {
	entry->ss_mode = SS_MODE_DEREG;

	switch (a[0])
	{
	    case 0:				// unconditional
		{
		    entry->type = 0x01;
		    switch(a[2])
		    {
			case 0:
			    entry->tel_class = -1;
			    break;
			case 1:
			    entry->tel_class = 0x11;
			    break;
			case 2:
			    entry->tel_class = 0x13;
			    break;
			case 3:
			    entry->tel_class = 0x16;
			    break;
			case 4:
			    entry->tel_class = 0x21;
			    break;
		    }
		    log_msg(MSGL_VGSM_INFO,"uncon[ %d : %d] \n",entry->type, entry->tel_class );
		}

		break;
	    case 1:				// when busy
		{
		    entry->type = 0x02;
		    switch(a[2])
		    {
			case 0:
			    entry->tel_class = -1;
			    break;
			case 1:
			    entry->tel_class = 0x11;
			    break;
			case 2:
			    entry->tel_class = 0x13;
			    break;
			case 3:
			    entry->tel_class = 0x16;
			    break;
			case 4:
			    entry->tel_class = 0x21;
			    break;
		    }
		    log_msg(MSGL_VGSM_INFO,"when[ %d : %d] \n",entry->type, entry->tel_class );
		}

		break;
	    case 2:				// no rely
		{
		    entry->type = 0x03;
		    switch(a[2])
		    {
			case 0:
			    entry->tel_class = -1;
			    break;
			case 1:
			    entry->tel_class = 0x11;
			    break;
			case 2:
			    entry->tel_class = 0x13;
			    break;
			case 3:
			    entry->tel_class = 0x16;
			    break;
			case 4:
			    entry->tel_class = 0x21;
			    break;
		    }
		    log_msg(MSGL_VGSM_INFO,"norely[ %d : %d] \n",entry->type, entry->tel_class );
		}

		break;
	    case 3:				// Not reachable
		{
		    entry->type = 0x04;
		    switch(a[2])
		    {
			case 0:
			    entry->tel_class = -1;
			    break;
			case 1:
			    entry->tel_class = 0x11;
			    break;
			case 2:
			    entry->tel_class = 0x13;
			    break;
			case 3:
			    entry->tel_class = 0x16;
			    break;
			case 4:
			    entry->tel_class = 0x21;
			    break;
		    }
		    log_msg(MSGL_VGSM_INFO,"notreach[ %d : %d] \n",entry->type, entry->tel_class );
		}

		break;
	}
    }
    else
    {
	TRACE(MSGL_VGSM_SS,"status isn't 0 or 1 : %d\n", a[4]);
    }

    sprintf(entry->number,"%d", a[1]);
    entry->replyTime=a[3];

    vgsm_ss_database_add(SS_CMD_CF, entry);

    init_ss_info_re();						// restore data list in EI

    if(entry)
	free(entry);
}

void server_tx_ss_cb_set_req ( LXT_MESSAGE *packet)
{
    // insert cwdata here
    int *a;

    call_barring_entry_t * entry = malloc(sizeof(call_barring_entry_t));
    if(!entry)
	return;
    memset(entry, 0, sizeof(call_barring_entry_t));

    TRACE(MSGL_VGSM_SS,"\n");
    a = (int*)packet->data;
    TRACE(MSGL_VGSM_SS,"code: %d\n", a[0]);
    TRACE(MSGL_VGSM_SS,"passwd: %d\n", a[1]);
    TRACE(MSGL_VGSM_SS,"status: %d\n", a[2]);
    entry->tel_class = -1;

    if (a[2] == 0)				// Activation
    {
	entry->ss_mode = SS_MODE_ACT;

	switch (a[0])
	{
	    entry->tel_class = -1;
	    case 0:
	    entry->type = 0x01;				// BAOC
	    break;
	    case 1:
	    entry->type = 0x02;				// BOIC
	    break;
	    case 2:
	    entry->type = 0x03;				// BAOIC
	    break;
	    case 3:
	    entry->type = 0x04;				// BAIC
	    break;
	    case 4:
	    entry->type = 0x05;				// BAIC when roam
	    break;
	}
    }
    else if(a[2] == 1)			// Deactivation
    {
	entry->ss_mode = SS_MODE_DEACT;
	switch (a[0])
	{
	    entry->tel_class = -1;
	    case 0:
	    entry->type = 0x01;				// BAOC
	    break;
	    case 1:
	    entry->type = 0x02;				// BOIC
	    break;
	    case 2:
	    entry->type = 0x03;				// BAOIC
	    break;
	    case 3:
	    entry->type = 0x04;				// BAIC
	    break;
	    case 4:
	    entry->type = 0x05;				// BAIC when roam
	    break;
	}
    }
    else
    {
	TRACE(MSGL_VGSM_SS,"status isn't 0 or 1 : %d\n", a[2]);
    }

    vgsm_ss_database_add(SS_CMD_CB, entry);

    init_ss_info_re();						// restore data list in EI

    if(entry)
	free(entry);
}


///////////////////////////////////////////////////////////////////////////
void server_tx_ss_ccfc_set_req ( LXT_MESSAGE *packet)
{
}

void server_tx_ss_aoc_get_req ( LXT_MESSAGE *packet) // 090215
{
    TRACE(MSGL_VGSM_SS,"\n");

    // a = (int*)packet->data;

    // TRACE(MSGL_VGSM_SS,"data: %d\n", a[0]);
    server_rx_ss_aoc_get();								// checkpoint 090215
}

void server_tx_ss_aoc_set_req ( LXT_MESSAGE *packet) // bychang
{
    int *a;

    TRACE(MSGL_VGSM_SS,"\n");
    a = (int*)packet->data;
    TRACE(MSGL_VGSM_SS,"acm: %d\n", a[0]);
    TRACE(MSGL_VGSM_SS,"ccm: %d\n", a[1]);
    TRACE(MSGL_VGSM_SS,"maxacm: %d\n", a[2]);
    TRACE(MSGL_VGSM_SS,"ppu: %d\n", a[3]);
    TRACE(MSGL_VGSM_SS,"chartype: %d\n", a[4]);
    TRACE(MSGL_VGSM_SS,"length: %d\n", packet->length);


    set_aoc_data(packet->data);

    //	TRACE(MSGL_VGSM_SS,"hdr: %d\n", a[7]);
}

void server_tx_ss_ussd_get_req ( LXT_MESSAGE *packet) // 090215
{
    TRACE(MSGL_VGSM_SS,"\n");

    // a = (int*)packet->data;

    // TRACE(MSGL_VGSM_SS,"data: %d\n", a[0]);
    server_rx_ss_ussd_get();								// checkpoint 090215
}

void server_tx_ss_ussd_set_req ( LXT_MESSAGE *packet)
{
    TRACE(MSGL_VGSM_SS,"\n");

    set_ussd_data(packet->data);

    //	TRACE(MSGL_VGSM_SS,"hdr: %d\n", a[7]);
}

int server_tx_ss_state( ss_state_e_type type )
{
    /*
       data[0] : ss_info
       data[1] : PI
       data[2] : Cause of No CLI
       data[3] : CUG_INDEX
       data[4] : NUMBER_LEN
       data[5] : NUMBER_TYPE
       data[6] : NUMBER[]
       data[7+] : NAME_LEN // 0
       data[7+] : DCS   // 0
     */
    TRACE(MSGL_VGSM_INFO,"\n");

    unsigned char *data = 0;
    //LXT_MESSAGE packet;
    gsm_call_list_t list;
    //int CallCount = 0;
    int length = 0, call_id = 0;

    get_call_list( &list );
    call_id = get_call_id();
    length = 8 + list.CallInfo[call_id].num_len;

    if( list.CallCount <= 0 )
	return 0;

    if( type == SS_STATE_HOLD || type == SS_STATE_UNHOLD )
    {
	if( list.CallCount > 1 )
	{
	    VGSM_DEBUG("Call Count is not 1. \n");
	    return 0;
	}
    }
    else if( type == SS_STATE_CONFERENCE )
    {
	if( list.CallCount == 1 )
	    return 0;
    }

    if( is_prev_ss_state(SS_RX_HOLD) )
    {
	VGSM_DEBUG("prev ss state is SS_RX_HOLD\n");
	return 0;
    }

    data = malloc(sizeof(unsigned char)*length);
    if(!data)
	return -1;
    memset( data, 0, sizeof(unsigned char)*length );

    switch( type ) {
	case SS_STATE_HOLD:
	    data[0] = 0x61; // GSM_SS_INFO_CALL_ON_HOLD
	    if( is_prev_ss_state(SS_TX_ACTIVATE) ||is_prev_ss_state(SS_NONE) || is_prev_ss_state(SS_RX_ACTIVATE) )
	    {
		swap_call();
		set_ss_state(SS_TX_HOLD);
	    }
	    else
	    {
		VGSM_DEBUG("\n");
		free(data);
		return 0;
	    }
	    break;

	case SS_STATE_UNHOLD:
	    data[0] = 0x62; // GSM_SS_INFO_CALL_RETRIEVED
	    if( is_prev_ss_state(SS_TX_HOLD) )
	    {
		swap_call();
		set_ss_state(SS_TX_ACTIVATE);
	    }
	    else
	    {
		VGSM_DEBUG("\n");
		free(data);
		return 0;
	    }
	    break;

	case SS_STATE_CONFERENCE: // not used.
	    data[0] = 0x63; // GSM_SS_INFO_MPTY_ENTERED
	    join_call();
	    break;

	default:
	    free(data);
	    return 0;
    }

    data[1] = 0x00; 	 // PI : Allowed
    data[2] = 0x00; 	// no cli cause
    data[3] = 0;		 // cug index
    data[4] = list.CallInfo[call_id].num_len;
    data[5] = 0x01; 	//GSM_NUM_TYPE_INTERNATIONAL
    memcpy(&data[6], list.CallInfo[call_id].number, list.CallInfo[call_id].num_len);
    //data[6+list.CallInfo[call_id].num_len] = 0x01;
    //data[6+list.CallInfo[call_id].num_len+1] = 'a';

    oem_tx_ss_ssinfo_noti(data,length);

    free(data);

    server_tx_call_list_noti();

    return 1;
}

int server_tx_ss_cli(unsigned char ss_call_type,unsigned char ss_present_indi, unsigned char ss_no_cli_cause)
{
    TRACE(MSGL_VGSM_INFO, "\n");

    gsm_call_list_t list;
    int CallCount = 0;
    int length = 0, call_id = 0;
    unsigned char *data = NULL;

    get_call_list( &list );
    CallCount = list.CallCount;
    if( CallCount == 0 )
	return 0;

    call_id = get_call_id();
    length = 8 + list.CallInfo[call_id].num_len;
    data = malloc(sizeof(unsigned char)*length);
    memset( data, 0, sizeof(unsigned char)*length );

    //data[0] = 0x01;
    data[0] = ss_call_type; 	// GSM_SS_INFO_CLI; //0x31;
    data[1] = ss_present_indi; 	 // PI : Allowed  // check...!!
    data[2] = ss_no_cli_cause; 	// cause of no cli
    data[3] = 0;		 // cug index
    data[4] = list.CallInfo[call_id].num_len;
    data[5] = 0x01; //GSM_NUM_TYPE_INTERNATIONAL
    //list.CallInfo[call_id].number[0] = '0'+call_id;
    memcpy(&data[6], list.CallInfo[call_id].number, list.CallInfo[call_id].num_len);
    //data[6+list.CallInfo[call_id].num_len] = 0x01;
    //data[6+list.CallInfo[call_id].num_len+1] = 'a';

    //log_msg(MSGL_VGSM_INFO,"call id %d\n", call_id);

    oem_tx_ss_ssinfo_noti(data,length);

    free(data);

    return 1;
}

