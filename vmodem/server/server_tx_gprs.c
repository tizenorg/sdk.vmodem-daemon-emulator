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

#include "oem_tx_gprs.h"
#include "linuxtapi.h"
#include "at_msg.h"
#include "vgsm_gprs.h"
#include "db_gprs.h"
#include "vgsm_gprs.h"
#include "logmsg.h"
#include <string.h>
#include "server_tx_gprs.h"
#include "at_func.h"

extern GprsConfList g_gprs_conflist;
extern GSM_SetPDPContext g_pdpcontext;

int server_tx_gprs_ipconf_match(void)
{
	int i;

	for (i = 0; i < g_gprs_conflist.num; i++) {
		log_msg(MSGL_VGSM_INFO,"apn(%s) conflist[%d](%s) \n"
				, g_pdpcontext.apn, i, g_gprs_conflist.pentry[i].apn);
		if (strcmp(g_pdpcontext.apn, g_gprs_conflist.pentry[i].apn) != 0)
			return i;
	}

	return -1;
}

int server_tx_gprs_IPConfigurationNotify(LXT_MESSAGE const* packet)
{
	TRACE(MSGL_VGSM_INFO, "\n");

	char sndbuf[SEND_BUF_SIZE];
	memset(sndbuf, '\0', sizeof(sndbuf));

	char* pdp_type = NULL;

	switch(g_pdpcontext.pdp_type)
	{
	case GSM_GPRS_DEFINE_PDP_CONTEXT_P_TYPE_IP:
		pdp_type = IP;
		break;
	case GSM_GPRS_DEFINE_PDP_CONTEXT_P_TYPE_PPP:
		pdp_type = PPP;
		break;
	case GSM_GPRS_DEFINE_PDP_CONTEXT_P_TYPE_IPV6:
		pdp_type = IPV6;
		break;
	default:
		pdp_type = UNKNOWN;
		break;
	}

	// search database
	vgsm_gprs_database_restore();

	sprintf(sndbuf, "%s%d,%s,%s,%s,%d,%d%s", CGDCONT, g_pdpcontext.cid, pdp_type, g_pdpcontext.apn, g_pdpcontext.pdp_addr, g_pdpcontext.d_comp, g_pdpcontext.h_comp, CRLF);
	
	log_msg(MSGL_VGSM_INFO,"packet:%s, length:%d\n", sndbuf, strlen(sndbuf));

	return oem_tx_gprs_IPConfigurationNotify(sndbuf, strlen(sndbuf));
}


