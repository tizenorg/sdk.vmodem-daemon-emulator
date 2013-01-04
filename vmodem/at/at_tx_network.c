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

/*    at_tx_network.c      */

#include <string.h>
#include "at_send.h"
#include "at_gen_resp.h"
#include "at_tx_network.h"
#include "at_func.h"

int at_tx_net_plmn_list_noti(void *data, int len)
{
	TRACE(MSGL_VGSM_INFO, "noti\n");
	return 0;
//	return at_msg_send(ACK_SEQ_NOTIFICATION, data, len);
}

int at_tx_net_plmn_list_resp(void *data, int len)
{
	TRACE(MSGL_VGSM_INFO, "+COPS list resp\n");
	TRACE(MSGL_VGSM_INFO, "%s", (char*)data);
	at_msg_send(ACK_SEQ_RESPONSE, data, len);
	return at_gen_resp_send(AT_GEN_ERR_NO_ERROR);
}

int at_tx_net_reg_noti(void *data, int len)
{
	TRACE(MSGL_VGSM_INFO, "+CREG noti\n");
	unsigned char* buf = (unsigned char*)data;
        char sndbuf[SEND_BUF_SIZE];
        memset(sndbuf, '\0', sizeof(sndbuf));

        sprintf(sndbuf, "%s%d,%x%x,%x%x%x%x,%d%s", CREG, buf[2], buf[4], buf[5], buf[6], buf[7], buf[8], buf[9], buf[0], CRLF);
        TRACE(MSGL_VGSM_INFO, "%s", sndbuf);
        at_msg_send(ACK_SEQ_NOTIFICATION, sndbuf, strlen(sndbuf));
        
        memset(sndbuf, '\0', sizeof(sndbuf));
        sprintf(sndbuf, "%s%d,%x%x,%x%x%x%x,%d%s", CGREG, buf[2], buf[4], buf[5], buf[6], buf[7], buf[8], buf[9], buf[0], CRLF);
        TRACE(MSGL_VGSM_INFO, "%s", sndbuf);

	return at_msg_send(ACK_SEQ_NOTIFICATION, sndbuf, strlen(sndbuf));
}

int at_tx_net_reg_resp(void *data, int len)
{
	TRACE(MSGL_VGSM_INFO, "+CREG resp\n");
	unsigned char* buf = (unsigned char*)data;
        char sndbuf[len];
        sprintf(sndbuf, "%s%d,%x%x,%x%x%x%x,%d%s", CREG, buf[2], buf[4], buf[5], buf[6], buf[7], buf[8], buf[9], buf[0], CRLF);
        TRACE(MSGL_VGSM_INFO, "%s", sndbuf);

	at_msg_send(ACK_SEQ_RESPONSE, sndbuf, strlen(sndbuf));
        return at_gen_resp_send(AT_GEN_ERR_NO_ERROR);
}

int at_tx_net_current_plmn_noti(void *data, int len)
{
	TRACE(MSGL_VGSM_INFO, "noti\n");
        return 0;
//	return at_msg_send(ACK_SEQ_NOTIFICATION, data, len);
}

int at_tx_net_current_plmn_resp(void *data, int len)
{
	TRACE(MSGL_VGSM_INFO, "+COPS? resp\n");

        at_msg_send(ACK_SEQ_RESPONSE, data, len);
	return at_gen_resp_send(AT_GEN_ERR_NO_ERROR);
}

int at_tx_gen_resp(void *data, int len)
{
	TRACE(MSGL_VGSM_INFO, "+CFUN resp\n");
	return at_gen_resp_send(AT_GEN_ERR_NO_ERROR);
}

int at_tx_net_plmn_selection_resp(void *data, int len)
{
	TRACE(MSGL_VGSM_INFO, "resp\n");
        return 0;
//	return at_msg_send(ACK_SEQ_RESPONSE, data, len);
}

int at_tx_net_svc_domain_resp(void *data, int len)
{
	TRACE(MSGL_VGSM_INFO, "resp\n");
        return 0;
//	return at_msg_send(ACK_SEQ_RESPONSE, data, len);
}

int at_tx_net_current_nitz_noti(void *data, int len)
{
	TRACE(MSGL_VGSM_INFO, "current nitz noti\n");
	unsigned char* buf = (unsigned char*)data;
	char sndbuf[SEND_BUF_SIZE];
	memset(sndbuf, '\0', sizeof(sndbuf));
	char formatbuf[8];
	int data_index = 0;
	
	sprintf(sndbuf, "%s", CNITZ);
	for(data_index = 0; data_index < len; data_index++)
	{
		sprintf(formatbuf, "%x", buf[data_index]);
		strcat(sndbuf, formatbuf);
		if(data_index + 1 == len)
			break;

		strcat(sndbuf, TOKEN);
	}
	strcat(sndbuf, CRLF);

	TRACE(MSGL_VGSM_INFO, "%s", sndbuf);
	return at_msg_send(ACK_SEQ_NOTIFICATION, sndbuf, strlen(sndbuf));
}

int at_tx_net_band_get_resp(void *data, int len)
{
	TRACE(MSGL_VGSM_INFO, "resp\n");
        return 0;
//	return at_msg_send(ACK_SEQ_RESPONSE, data, len);
}
