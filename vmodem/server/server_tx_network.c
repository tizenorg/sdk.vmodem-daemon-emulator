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

/*
/////////////////////////////////////////////////////////////////////
// at_tx_network.c
 */

#include <stdlib.h>
#include <string.h>

#include "server_common_network.h"
#include "oem_tx_network.h"
#include "server_tx_network.h"
#include "vgsm_network.h"
#include "vgsm_debug.h"
#include "linuxtapi.h"

//int TxNET_PLMNList(LXT_MESSAGE const* packet, int cmd_type)
int server_tx_net_plmn_list_noti(LXT_MESSAGE const* packet)
{
    VGSM_DEBUG("\n");

    unsigned char *data = 0;
    int ret = 0, len = 0, tmp;

    unsigned char *ptr = (unsigned char *)packet->data;
    int i = 0;

    tmp = (int)ptr[0];
    if(tmp < 0 || tmp > 254){
	TRACE(MSGL_VGSM_INFO, "ERROR!! Invalid value of packet.data.\n");
	return -1;
    }

    len = 1 + (tmp * 8);
    data = malloc(sizeof(unsigned char)*len);
    if(data == NULL)
	return -1;

    for(i=0; i<len; ++i)
	data[i] = ptr[i];

    set_plmn_list( data, len ); //network.db clean and update

    ret = oem_tx_net_plmn_list_noti(data, len);

    if(data)
	free( data );

    return ret;

}

int server_tx_net_reg_noti(LXT_MESSAGE const* packet)
{
    VGSM_DEBUG("\n");

    int ret = 0, len = 11;
    unsigned char data[len ];

    memset( data, 0, sizeof(unsigned char)*len );

    if (packet && packet->data)
    {
	VGSM_DEBUG("\n");
	unsigned char *ptr = (unsigned char *)packet->data;
	int i = 0;

	for(i=0; i<len; ++i)
	    data[i] = ptr[i];

	set_network_registration( data, len );
    }
    else  // it is the cast that  net reg noti is sended after net selection set.
    {
	VGSM_DEBUG("\n");
	gsm_network_reg_t *reg = get_network_registration();

	data[0] = reg->act;				/* ACT */
	data[1] = reg->service_domain;	/* SERVICE_DOMAIN */
	data[2] = reg->reg_status;		/* REG_STATUS */
	data[3] = reg->edge_support;	/* EDGE_SUPPORT */
	data[4] = reg->lac[0];			/* LAC */
	data[5] = reg->lac[1];			/* LAC */
	data[6] = reg->cell_id[0];		/* CELL-ID */
	data[7] = reg->cell_id[1];		/* CELL-ID */
	data[8] = reg->cell_id[2];		/* CELL-ID */
	data[9] = reg->cell_id[3];		/* CELL-ID */

	data[10] = reg->rej_cause;
    }

    ret = oem_tx_net_reg_noti(data, len);

    return ret;
}

//int TxNET_CurrentPLMN( LXT_MESSAGE const* packet)
int server_tx_net_current_plmn_noti( LXT_MESSAGE const* packet)
{
    VGSM_DEBUG("\n");
    int ret = 0, len = 9;
    unsigned char data[len];
    unsigned char *p = (unsigned char *)(packet->data);

    memset(data, 0, sizeof(data));

    memcpy(data, p, len);

    ret = oem_tx_net_current_plmn_noti(data, len);

    return ret;
}

int server_tx_net_current_nitz_noti(LXT_MESSAGE const* packet)
{
    VGSM_DEBUG("\n");
    int ret = 0, len;
    unsigned char *p = (unsigned char *)(packet->data);
    gsm_network_nitz_info_t	net_identity_info;

    len = sizeof(gsm_network_nitz_info_t);

    memcpy(&net_identity_info, p, sizeof(net_identity_info));

    set_network_identity( net_identity_info);

    ret = oem_tx_net_current_nitz_noti(p, len);

    return ret;
}
