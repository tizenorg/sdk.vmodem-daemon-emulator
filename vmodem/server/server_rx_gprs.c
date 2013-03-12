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

#include <assert.h>

#include "server_tx_data.h"
#include "vgsm_phone.h"
#include "linuxtapi.h"
#include "state.h"
#include "phoneserver.h"
#include "server_tx_gprs.h"
#include "server_rx_gprs.h"
#include "vgsm_gprs.h"
#include "misc.h"
#include "logmsg.h"
#include "at_func.h"

GSM_SetPDPContext g_pdpcontext;

int server_rx_gprs_SetPDPContext(void *ptr_data, int data_len )
{
	TRACE(MSGL_VGSM_INFO, "data_len(%d) >= 128 \n", data_len);

	if (ptr_data && (data_len > 0)) {
		char *ptr = (char *)ptr_data;
		char cdata[SEND_BUF_SIZE];
		memset(cdata, '\0', sizeof(cdata));
		strcpy(cdata, ptr);

		memset(&g_pdpcontext, 0, sizeof(g_pdpcontext));

		char* data = strchr(cdata, '=');
		char* ret = NULL;
		
		ret = strtok(data+1, TOKEN);
		if(ret)
			g_pdpcontext.cid = atoi(ret);

		g_pdpcontext.mode = GSM_GPRS_DEFINE_PDP_MODE_ADD;
		
		ret = strtok(NULL, TOKEN);
		if(ret)
		{
			if(!strcmp(ret, IP))
				g_pdpcontext.pdp_type = GSM_GPRS_DEFINE_PDP_CONTEXT_P_TYPE_IP;
			else if(!strcmp(ret, PPP))
				g_pdpcontext.pdp_type = GSM_GPRS_DEFINE_PDP_CONTEXT_P_TYPE_PPP;
			else if(!strcmp(ret, IPV6))
				g_pdpcontext.pdp_type = GSM_GPRS_DEFINE_PDP_CONTEXT_P_TYPE_IPV6;
			else
				g_pdpcontext.pdp_type = GSM_GPRS_DEFINE_PDP_CONTEXT_P_TYPE_UNKNOWN;
		}

		ret = strtok(NULL, TOKEN);
		if(ret)
			memcpy(g_pdpcontext.apn, ret, strlen(ret));

		ret = strtok(NULL, TOKEN);
		if(ret)
			memcpy(g_pdpcontext.pdp_addr, ret, strlen(ret));
		else
			strcpy(g_pdpcontext.pdp_addr, EMUL_IP);

		ret = strtok(NULL, TOKEN);
		if(ret)
			g_pdpcontext.d_comp = atoi(ret);

		ret = strtok(NULL, TOKEN);
		if(ret)
			g_pdpcontext.h_comp = atoi(ret);

		log_msg(MSGL_VGSM_INFO,"MODE[%x] CID[%x] PDP_TYPE[%x] APN[%s] \n", 
				g_pdpcontext.mode, g_pdpcontext.cid, g_pdpcontext.pdp_type, g_pdpcontext.apn);
		log_msg(MSGL_VGSM_INFO,"PDP_ADDRESS[%s] D_COMP[%x] H_COMP[%x] \n",
				g_pdpcontext.pdp_addr, g_pdpcontext.d_comp, g_pdpcontext.h_comp);
	}

	return 1;
}

int server_rx_gprs_SetPSActDeact(void *ptr_data, int data_len )
{
	TRACE(MSGL_VGSM_INFO, "\n");

	if (ptr_data && (data_len > 0)) {
		char *data = 0, *ptr = (char *)ptr_data;
		char cdata[SEND_BUF_SIZE];
		memset(cdata, '\0', sizeof(cdata));
                strcpy(cdata, ptr);
		
		char* tdata = strchr(cdata, '=');
                char* ret = NULL;

		LXT_MESSAGE packet;

		TAPIMessageInit(&packet);

		data = malloc(sizeof(char)*1);
		if(!data)
			return 0;
		ret = strtok(tdata+1, TOKEN);
		if(ret)
                        data[0] = atoi(ret);  /* 0: stop, 1: start */
                else {
			free(data);
                        return 0;
		}

		ret = strtok(NULL, TOKEN);
		if(ret)
			log_msg(MSGL_VGSM_INFO,"STATE[%x, 0:stop, 1:start] CID[%d] \n", data[0], atoi(ret));

		packet.data = data;
		packet.length = 1;

		packet.group  = GSM_DATA;
		packet.action = GSM_DATA_STATE;
		packet.length = 1;

		FuncServer->Cast(&GlobalPS, LXT_ID_CLIENT_EVENT_INJECTOR, &packet);

		free(data);

		return 1;
	}

	return 0;
}

