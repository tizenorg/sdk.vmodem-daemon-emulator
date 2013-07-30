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

/*  server_tx_power.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "phoneserver.h"
#include "vgsm_phone.h"
#include "linuxtapi.h"
#include "gsm.h"
#include "oem_tx_power.h"
#include "server_tx_power.h"

#include "server_tx_security.h"
#include "at_func.h"
#include "at_send.h"
#include "flight.h"

#include "server_common_network.h"
#include "server_tx_network.h"
#include "server_tx_display.h"

#define INITIAL_RSSI 0x04

extern unsigned char g_battery_level;

void* change_level(void *data);

int server_tx_power_up_resp(void)
{
	char sndbuf[SEND_BUF_SIZE];
	memset(sndbuf, '\0', sizeof(sndbuf));

	LXT_MESSAGE packet;
	packet.group  = GSM_POWER;
	packet.action = GSM_POWER_MODEM_ON_REQ;
	packet.length =  0;

	TRACE(MSGL_VGSM_INFO, "\n");

	FuncServer->Cast( &GlobalPS, LXT_ID_CLIENT_EVENT_INJECTOR, &packet );	//&ServerHandle->server_cast

	sprintf(sndbuf, "%s%d%s", CPAS, AT_GSM_PWR_UP_READY, CRLF);
	oem_tx_power_up_resp(sndbuf, strlen(sndbuf));	// power up noti

	/*  sim init processing   */
	server_tx_sim_init_start();

	// sim init
	memset(sndbuf, '\0', sizeof(sndbuf));
        sprintf(sndbuf, "%s%s%s", CPIN, STINIT_COMPL, CRLF);
        at_msg_send(ACK_SEQ_NOTIFICATION, sndbuf, strlen(sndbuf));
        return server_common_after_sim_init();
}

/* To support Flight mode on/off */
int server_tx_online_noti(unsigned char status)
{
	int nrn_len = 11;
	int pon_len = 0;
	unsigned char pon_data[10];	/* power online noti */
	unsigned char nrn_data[11];	/* net reg noti */
	unsigned char rssi = INITIAL_RSSI;

	TRACE(MSGL_VGSM_INFO, "status = %d\n", status);

	memset( nrn_data, 0, sizeof(unsigned char)*nrn_len );

	gsm_network_reg_t *reg = get_network_registration();

	nrn_data[0] = reg->act;             /* ACT */
	nrn_data[1] = reg->service_domain;  /* SERVICE_DOMAIN */
	nrn_data[2] = reg->reg_status;      /* REG_STATUS */
	nrn_data[3] = reg->edge_support;    /* EDGE_SUPPORT */
	nrn_data[4] = reg->lac[0];          /* LAC */
	nrn_data[5] = reg->lac[1];          /* LAC */
	nrn_data[6] = reg->cell_id[0];      /* CELL-ID */
	nrn_data[7] = reg->cell_id[1];      /* CELL-ID */
	nrn_data[8] = reg->cell_id[2];      /* CELL-ID */
	nrn_data[9] = reg->cell_id[3];      /* CELL-ID */
	nrn_data[10] = reg->rej_cause;

	if(status == AT_GSM_PWR_PHONE_ONLINE_OFF){

		nrn_data[0] = GSM_NET_ACT_NOTSPECIFIED;	/* ACT */
		nrn_data[2] = GSM_NET_REG_STAT_NOT_REG;	/* reg_status */
		pon_data[pon_len++] = AT_GSM_PWR_PHONE_RSN_ONLINE_OFF;
		rssi = 0x00;

	}else if(status == AT_GSM_PWR_PHONE_ONLINE_ON){

		nrn_data[0] = GSM_NET_ACT_UTRAN;			/* ACT */
		nrn_data[2] = GSM_NET_REG_STAT_REG_HOME;	/* reg_status */
		pon_data[pon_len++] = AT_GSM_PWR_PHONE_RSN_ONLINE_ON;
		rssi = INITIAL_RSSI; /* initial value */

	}else if(status == AT_GSM_PWR_PHONE_ONLINE_MAX){
		pon_data[pon_len++] = AT_GSM_PWR_PHONE_RSN_ONLINE_MAX;
	}else{
		pon_data[pon_len++] = AT_GSM_PWR_PHONE_RSN_ONLINE_FAIL;
	}

	/* change act & reg status */
	set_network_registration( nrn_data, nrn_len ); /* change ACT & status */

	server_tx_net_reg_noti(NULL); /* return */

	oem_tx_power_online_noti(pon_data, pon_len);

	server_tx_display_icon_info_noti(AT_GSM_DISP_ICON_TYPE_ALL, rssi, g_battery_level, 0, 0);

	return 1;
}

int server_tx_battery_status_noti(unsigned char level)
{
	unsigned char data[1];
	int n = 0;

	TRACE(MSGL_VGSM_INFO, "level = %d\n", level);

	assert(level <= 5);

	data[n++] = level;

	return oem_tx_battery_status_noti(data, n);
}
