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

#include "vgsm_sim.h"
#include "state.h"
#include "server_common.h"
#include "server_common_security.h"
#include "db_security.h"
#include "server_tx_security.h"
#include "oem_tx_security.h"

#include "misc.h"	// TAPIMessageInit()
#include "linuxtapi.h"
#include "vgsm_phone.h"
#include "phoneserver.h"

int server_tx_sim_init_start(void)
{
    int status;

    SIM_DEBUG("\n");
    //not used
    //server_tx_sim_sec_card_type_noti();

    //load from sim db
    db_sim_sec_restore(VGSM_SIMTypeLoad);

    status = server_sec_get_sec_status();

    SIM_DEBUG("boot time sim status =[%x]\n", status);

    if(status	==	GSM_SIM_READY) {
	server_tx_sim_sec_disable_facility();
    }
    else {
	server_tx_sim_sec_enable_facility(status);
    }

    return 1;
}


int server_tx_sim_sec_disable_facility(void)
{
    unsigned char *p = 0;
    LXT_MESSAGE packet;

    SIM_DEBUG("\n");
    SIM_DEBUG("pin lock disable mode - it means key not needed(ready) \n");

    TAPIMessageInit(&packet);

    packet.data	=	malloc(2);
    if(!packet.data)
	return -1;
    p			=	(unsigned char *)(packet.data);

    packet.group	=	GSM_SIM;
    packet.length	=	1 + 1 ;
    packet.action	=	GSM_SIM_FACILITY_RES;

    //server_sec_set_sec_status(GSM_SIM_READY);
    //set_current_state( STATE_SIM_READY, GSM_SEC_CMD, GSM_SEC_PIN_STATUS);

    server_tx_sim_sec_pin_status_noti(STATE_ON, STATE_SIM_READY);

    p[0]	=	0;
    p[1]	=	GSM_SIM_READY;

    /*	++ 2008-09-29
     */
    //FuncServer->Cast(&GlobalPS, LXT_ID_CLIENT_EVENT_INJECTOR, &packet);

    if(packet.data)
	free(packet.data);

    return 1;
}


int server_tx_sim_sec_enable_facility(int status)
{
    unsigned char *p = 0;
    LXT_MESSAGE packet;

    SIM_DEBUG("\n");
    SIM_DEBUG("sim status =[%d]\n", status);

    TAPIMessageInit(&packet);

    packet.data	=	malloc(2);
    p			=	(unsigned char *)(packet.data);

    packet.group	=	GSM_SIM;
    packet.length	=	1 + 1;
    packet.action	=	GSM_SIM_FACILITY_RES;

    p[0]	=	1;
    p[1]	=	status;

    switch (status)
    {
	/*
	   case GSM_SIM_READY :
	   set_current_state(STATE_SIM_READY, GSM_SEC_CMD, GSM_SEC_PIN_STATUS, 0 );
	   break;
	 */
	case GSM_SIM_PIN_REQ_STATE :
	    server_tx_sim_sec_pin_status_noti(STATE_ON, STATE_SIM_PIN);
	    break;

	case GSM_SIM_PUK_REQ :
	    server_tx_sim_sec_pin_status_noti(STATE_ON, STATE_SIM_PUK);
	    break;

	case GSM_SIM_FRIZEN :
	    server_tx_sim_sec_pin_status_noti(STATE_ON, STATE_SIM_FRIZEN);
	    break;

	case GSM_SIM_NO_SIM :
	    server_tx_sim_sec_pin_status_noti(STATE_ON, STATE_SIM_NO_SIM);
	    break;

	default :
	    SIM_DEBUG("No handled sim status=[%x]", status);
	    break;
    }

    //server_tx_sim_sec_pin_status_noti();

    FuncServer->Cast(&GlobalPS, LXT_ID_CLIENT_EVENT_INJECTOR, &packet);

    if(packet.data)
	free(packet.data);

    return 1;
}


////////////////////////// 090512 //////////////////////////////////////////
int server_tx_sim_data_get_db(unsigned char *sim_data, int len)
{
    int length;
    LXT_MESSAGE packet;

    SIM_DEBUG(" len_data :%d\n",len);

    TAPIMessageInit(&packet);

    //length = 9+9+9+9+4+4+4+4+4+4+4+4; //68
    length = len; //43

    packet.data	=	sim_data;
    packet.group	=	GSM_SIM;
    packet.length	=	length ;
    packet.action	=	GSM_SIMDATA_RES;

    FuncServer->Cast(&GlobalPS, LXT_ID_CLIENT_EVENT_INJECTOR, &packet);

    if(packet.data)
	free(packet.data);

    return  1;
}

int server_tx_sim_info_get_db(unsigned char *sim_info, int len)
{
    int length;
    LXT_MESSAGE packet;

    SIM_DEBUG(" len_info : %d\n",len);

    TAPIMessageInit(&packet);

    //length = 9+9+9+9+4+4+4+4+4+4+4+4; //68
    length = len; //43

    packet.data	=	sim_info;
    packet.group	=	GSM_SIM;
    packet.length	=	length ;
    packet.action	=	GSM_SIMINFO_RES;

    FuncServer->Cast(&GlobalPS, LXT_ID_CLIENT_EVENT_INJECTOR, &packet);

    if(packet.data)
	free(packet.data);

    return  1;
}
//////////////////////////////////////////////////////////////////////////////////

int server_tx_sim_sec_get_db(SimSecurity *sim_sec)
{
    int length;
    LXT_MESSAGE packet;

    SIM_DEBUG("\n");

    TAPIMessageInit(&packet);

    length = 9+9+9+9+4+4+4+4+4+4+4+4; //68

    packet.data	=	malloc(sizeof(SimSecurity));
    packet.data	=	sim_sec;
    packet.group	=	GSM_SIM;
    packet.length	=	length ;
    packet.action	=	GSM_SIM_GET_SIM_DB_RES;

    FuncServer->Cast(&GlobalPS, LXT_ID_CLIENT_EVENT_INJECTOR, &packet);

    if(packet.data)
	free(packet.data);

    return  1;
}


int server_tx_sim_sec_card_type_noti(void)
{
    int ret = 0;
    int data_size = 1;
    unsigned char *pdata = 0;

    SIM_DEBUG("\n");

    pdata = malloc(sizeof(unsigned char)*data_size);
    if(!pdata)
	return -1;

    pdata[0] = GSM_SEC_SIM_3G;

    ret = oem_tx_sim_sec_card_type_noti(pdata, data_size);

    if( pdata )
	free( pdata );

    return ret;
}


/* this API is for NOTIFICATION of SIM PIN STATUS	*/
int server_tx_sim_sec_pin_status_noti(STATE_TYPE before_state, STATE_TYPE current_state)
{
    int ret = 0;
    int data_size = 2;
    unsigned char *pdata = 0;

    int send_sim_init_msg	=	0;

    //	STATE current;
    //	STATE prev;

    SIM_DEBUG("\n");

    pdata = malloc(sizeof(unsigned char)*data_size);
    if(!pdata)
	return -1;

    //	get_current_state_machine( &current );
    //	get_prev_state_machine( &prev );

    //	SIM_DEBUG("STATE_TYPE(prev)=%d \n", STATE_TYPE(prev));

    switch(before_state)
    {
	//add STATE_STANDBY for test.
	case STATE_STANDBY :
	case STATE_ON:
	    {
		switch(current_state)
		{
		    case STATE_SIM_READY :
			{
			    /*	 The noti that modem(vgsm) should send to emulator when the lock disabled.
				 1) GSM_SEC_LOCK_TYPE_READY
				 2) GSM_SEC_SIM_INIT_COMPLETED
				 3) GSM_SEC_PB_INIT_COMPLETED - doesn't send this because the phonebook isn't implemented.
			     */
			    pdata[0]	=	GSM_SEC_LOCK_TYPE_READY;
			    pdata[1]	=	GSM_SEC_LOCK_KEY_UNLOCKED;
#if 0 //20110714 tmp 
			    oem_tx_sim_sec_pin_status_noti(pdata, data_size);

			    sleep(3);

			    pdata[0]	=	GSM_SEC_SIM_INIT_COMPLETED;
			    pdata[1]	=	GSM_SEC_LOCK_KEY_UNLOCKED;
			    send_sim_init_msg	=	1;

			    /*
			       oem_tx_sim_sec_pin_status_noti(pdata, data_size);

			       pdata[0]	=	GSM_SEC_PB_INIT_COMPLETED;
			       pdata[1]	=	GSM_SEC_LOCK_KEY_UNLOCKED;
			     */
#endif
			}
			break;

		    case STATE_SIM_PIN :
			{
			    pdata[0]	=	GSM_SEC_LOCK_TYPE_SC;
			    pdata[1]	=	GSM_SEC_LOCK_KEY_PIN;
			}
			break;

		    case STATE_SIM_PUK :
			{
			    pdata[0]	=	GSM_SEC_LOCK_TYPE_SC;
			    pdata[1]	=	GSM_SEC_LOCK_KEY_PUK;
			}
			break;

		    case STATE_SIM_FRIZEN :
			{
			    pdata[0]	=	GSM_SEC_LOCK_TYPE_SC;
			    pdata[1]	=	GSM_SEC_LOCK_KEY_PERM_BLOCKED;
			}
			break;

		    case STATE_SIM_NO_SIM :
			{
			    pdata[0]	=	GSM_SEC_LOCK_TYPE_NO_SIM;
			    pdata[1]	=	GSM_SEC_LOCK_KEY_UNLOCKED;	//to be considered...
			}
			break;

		    default :
			SIM_DEBUG("No handled current_state=[%d]\n",current_state);
			break;

		}
	    }
	    break;

	case STATE_SIM_PIN :
	case STATE_SIM_PUK :
	    {
		switch(current_state)
		{
		    case  STATE_SIM_PIN_OK :
		    case  STATE_SIM_PUK_OK:
			{
			    pdata[0]	=	GSM_SEC_SIM_INIT_COMPLETED;
			    pdata[1]	=	GSM_SEC_LOCK_KEY_UNLOCKED;
			    send_sim_init_msg	=	1;
			    /*
			       oem_tx_sim_sec_pin_status_noti(pdata, data_size);

			       pdata[0]	=	GSM_SEC_PB_INIT_COMPLETED;
			       pdata[1]	=	GSM_SEC_LOCK_KEY_UNLOCKED;
			     */
			}
			break;

		    default:
			SIM_DEBUG("No handled current_state=[%d]\n",current_state);
			break;
		}
	    }
	    break;

	default:
	    {
		SIM_DEBUG("No handled case in before_state = [%d]\n", before_state);
	    }
	    break;

    }

    ret	=	oem_tx_sim_sec_pin_status_noti(pdata, data_size);
    if( pdata )
	free( pdata );

    return ret;
}

