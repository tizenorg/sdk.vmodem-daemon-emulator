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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vgsm_phone.h"
#include "vgsm_pb.h"
#include "oem_tx_phonebook.h"
#include "server_tx_phonebook.h"
#include "db_phonebook.h"

#include "misc.h"	// TAPIMessageInit()
#include "linuxtapi.h"
#include "vgsm_phone.h"
#include "phoneserver.h"

/*
*	flag : 0 -> EI UI + emulator
*		1,2 -> EI UI
*/

void callback_pb_action(PB *pb)
{
	LXT_MESSAGE packet;

	TAPIMessageInit(&packet);
	packet.data = malloc(sizeof(PB));
	packet.group = GSM_SIM;
	packet.length = sizeof(PB);
	packet.action = GSM_SIM_ERR_ADD_NOTI;

	memcpy(packet.data,pb,packet.length);

	FuncServer->Cast(&GlobalPS, LXT_ID_CLIENT_EVENT_INJECTOR, &packet);

	if(packet.data)
		free(packet.data);
}

void server_tx_sim_pb_get_access_RX_UI(PB *pb,int flag)
{
	LXT_MESSAGE packet;
	TAPIMessageInit(&packet);

	//RX Eventenjector UI
	packet.data	=	(unsigned char *)malloc(sizeof(PB));
	packet.group	=	GSM_SIM;
	packet.length	=	sizeof(PB);

	switch(flag)
	{
		case 0:
			packet.action = GSM_SIM_GET_PB_DB_REQ;
		break;

		case 1:
			packet.action = GSM_SIM_ADD_EVENT_PB_DB_REQ;
		break;

		case 2:
			packet.action = GSM_SIM_DELETE_EVENT_PB_DB_REQ;
		break;
	}

	memcpy(packet.data,pb, packet.length);

	FuncServer->Cast(&GlobalPS, LXT_ID_CLIENT_EVENT_INJECTOR, &packet);

	if(packet.data)
		free(packet.data);

}


int server_tx_sim_pb_get_all_db(PB *pb,int cnt)
{
	LXT_MESSAGE packet;
	TAPIMessageInit(&packet);
	int length;

	//length = cnt * 62;
	length = cnt * sizeof(PB);

	//packet.data = (PB *) malloc(sizeof(PB)*cnt);

	packet.data	= (PB *)pb;
//	packet.data = 	pb;
	packet.group	= GSM_SIM;
       packet.length	= length ;
	packet.action	= GSM_SIM_GET_ALL_PB_DB_REQ;

    	FuncServer->Cast(&GlobalPS, LXT_ID_CLIENT_EVENT_INJECTOR, &packet);
/*
	if(packet.data)
		free(packet.data);
*/
	return  1;

}
