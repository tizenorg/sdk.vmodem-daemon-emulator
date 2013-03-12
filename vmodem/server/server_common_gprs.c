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

#include "db_gprs.h"
#include "linuxtapi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vgsm_phone.h"
#include "phoneserver.h"
#include "logmsg.h"
#include "state.h"


extern GprsConfList g_gprs_conflist;

void init_gprs_info(void)
{
	/* apn: "Tizen", interface: "lo", ip: 1.2.3.4, primary dns: 10.10.10.10
	   , second dns: 11.11.11.11, gateway: 1.2.3.1, subnet: 255.255.255.0 */
	GprsDbEntry entry = {"Tizen", "lo", {1, 0x001F, 
		{0x01, 0x02, 0x03, 0x04}, {0x0A, 0x0A, 0x0A, 0x0A},
		{0x0B, 0x0B, 0x0B, 0x0B}, {0x01, 0x02, 0x03, 0x01},
		{0xFF, 0xFF, 0xFF, 0x00}}};

	LXT_MESSAGE packet;
	char *buf, *pos;
	int len, i;
	GprsDbEntry *pentry;

	// load database
	vgsm_gprs_database_restore();

	log_msg(MSGL_VGSM_INFO,"num of current saved GPRS IP configuration: %i \n", g_gprs_conflist.num);

	if (g_gprs_conflist.num == 0) {
		// add default IP configuration to DB
		vgsm_gprs_database_add(&entry);
	}

	// load database
	vgsm_gprs_database_restore(); 

	len = sizeof(int) + (g_gprs_conflist.num * (255 + 10 + 23));

	// send noti to evt injector
	if ((buf = malloc(len)) == NULL) {
		log_msg(MSGL_VGSM_ERR,"init_gprs_info: memory alloc error\n");
		return;
	}

	memset(buf, 0, len);

	*((int *)buf) = g_gprs_conflist.num;

	pos = buf + sizeof(int);

	for (i = 0; i < g_gprs_conflist.num; i++ ) {
		pentry = &(g_gprs_conflist.pentry[i]);

		strcpy(pos, pentry->apn);

		strcpy(pos + 255, pentry->intf);

		pos[265] = pentry->ipconf.cid;

		memcpy(pos + 266, (char *)(&(pentry->ipconf.fieldflag)), 2);
		memcpy(pos + 268, (char *)(pentry->ipconf.ip_addr), 4);
		memcpy(pos + 272, (char *)(pentry->ipconf.primary_dns), 4);
		memcpy(pos + 276, (char *)(pentry->ipconf.secondary_dns), 4);
		memcpy(pos + 280, (char *)(pentry->ipconf.default_gateway), 4);
		memcpy(pos + 284, (char *)(pentry->ipconf.subnet_mask), 4);

		pos += 255 + 10 + 23;
	}

	packet.group  = GSM_DATA;
	packet.action = GSM_DATA_GPRS_INIT_REQ;
	packet.length = len;
	packet.data = (void *)buf;

	FuncServer->Cast(&GlobalPS, LXT_ID_CLIENT_EVENT_INJECTOR, &packet);

	if (buf)
		free(buf);
}
