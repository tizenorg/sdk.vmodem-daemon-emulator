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
#include <stdlib.h>
#include <string.h>

#include "vgsm_network.h"
#include "db_network.h"
#include "vgsm_debug.h"

#include "linuxtapi.h"
#include "phoneserver.h"
#include "vgsm_phone.h"
#include "server_common_network.h"

gsm_network_plmn_list_t g_plmn_list;
gsm_network_reg_t g_network_reg;
gsm_net_sel_e_type g_network_sel_mode = GSM_NET_SEL_AUTO;
gsm_net_band_mode_e_type  g_network_band_mode = 0x01;
gsm_net_svc_domain_e_type g_service_domain = 0x01;
int  g_network_band = 0x40000000;
gsm_network_nitz_info_t   g_network_identity ;
#if 0
	 = {
	GSM_NET_NITZ_MMINFO_CS,
	GSM_NET_NITZ_SHORT_NAME_MASK,
	1,
	4,
	{0x41,0x42,0x43,0x44,0,},//"ABCD",
	0,
	0,
	{0x33,0x34,0x35,0x36,0,}//"ABCD",
};  // need to initial
#endif


/*
 * load plmn list from network db.
 * if not exising plmn, then load default plmn and backup the plmn (mckim)
 */
void init_plmn_list(void)
{
	NetworkEntry entry = {0};
	/* Update for public open
	   0x34, 0x35, 0x30, 0x30, 0x31, 0x23 // 45001#
	   =>
	   0x31, 0x31, 0x31, 0x31, 0x31, 0x23 // 11111#
	 */
	//char plmn[6] = {0x31, 0x31, 0x31, 0x31, 0x31, 0x23}; // "111111#"
	char plmn[6]; // "111111#"
	memset(plmn, '\0', sizeof(plmn));
	plmn[0] = 0x31;
	plmn[1] = 0x31;
	plmn[2] = 0x31;
	plmn[3] = 0x31;
	plmn[4] = 0x31;
	plmn[5] = 0x23;

	LXT_MESSAGE packet;
	char *buf;
	int i;

	// load 
	vgsm_network_database_restore();

	VGSM_DEBUG("num of current saved PLMN: %i \n", g_plmn_list.num_record);

	if (g_plmn_list.num_record == 0) {
		g_plmn_list.num_record = 1;
		g_plmn_list.precord = malloc(sizeof(gsm_network_plmn_record_t)*g_plmn_list.num_record); // it needs free().
		memset(g_plmn_list.precord, '\0', sizeof(gsm_network_plmn_record_t));
		g_plmn_list.precord[0].status = GSM_NET_PLMN_STATUS_AVAIL; // PLMN_STATUS
		
		//memcpy(g_plmn_list.precord[0].plmn, plmn, 6);
		g_plmn_list.precord[0].plmn[0] = plmn[0];
		g_plmn_list.precord[0].plmn[1] = plmn[1];
		g_plmn_list.precord[0].plmn[2] = plmn[2];
		g_plmn_list.precord[0].plmn[3] = plmn[3];
		g_plmn_list.precord[0].plmn[4] = plmn[4];
		g_plmn_list.precord[0].plmn[5] = plmn[5];
		
		g_plmn_list.precord[0].act = GSM_NET_ACT_UTRAN; // ACT

		// add to db
		VGSM_DEBUG("precord[0].plmn:%c %c %c %c %c %c\n", g_plmn_list.precord[0].plmn[0], g_plmn_list.precord[0].plmn[1], g_plmn_list.precord[0].plmn[2], g_plmn_list.precord[0].plmn[3], g_plmn_list.precord[0].plmn[4], g_plmn_list.precord[0].plmn[5]);
		memcpy(entry.number_PLMN, g_plmn_list.precord[0].plmn, 6);
		VGSM_DEBUG("number_PLMN:%s\n", entry.number_PLMN);
		entry.PLMN_status = g_plmn_list.precord[0].status;
		entry.ACT = g_plmn_list.precord[0].act;

		vgsm_network_database_add(&entry);
	}

	// send noti to evt injector
	if ((buf = malloc(1 + g_plmn_list.num_record * 8)) == NULL) { 
		log_msg(MSGL_VGSM_ERR,"init_plmn_list: memory alloc error\n");
		return;
	}

	buf[0] = g_plmn_list.num_record;

	for (i = 0; i < g_plmn_list.num_record; i++) {
		int idx = 1 + 8 * i;
		buf[idx] = g_plmn_list.precord[i].status;
		memcpy((buf + idx + 1), g_plmn_list.precord[i].plmn, 6);
		buf[idx + 7] = g_plmn_list.precord[i].act;
	}

	packet.group  = GSM_NETWORK;
	packet.action = GSM_NETWORK_PLMN_LIST;
	packet.length = 1 + g_plmn_list.num_record * 8;
	packet.data = (void *)buf;

	FuncServer->Cast(&GlobalPS, LXT_ID_CLIENT_EVENT_INJECTOR, &packet);	// &ServerHandle->server_cast

	if(buf)
		free(buf);
}


void set_plmn_list( unsigned char *data, int len )
{
	int i = 0, j = 1;
	char tmp = 0x00;
	NetworkEntry entry = {0};
	VGSM_DEBUG("\n");

	if (g_plmn_list.num_record != 0) {

		// clean network db (mckim)
		for (i = 0; i < g_plmn_list.num_record; i++) {
			memcpy(entry.number_PLMN, g_plmn_list.precord[i].plmn, 6);
			vgsm_network_database_remove(&entry);
		}

		g_plmn_list.num_record = 0;

		free( g_plmn_list.precord );
		g_plmn_list.precord = 0;
	}

	tmp = (char)data[0];
	if(tmp < 0x00 || tmp > 0x7f){
		VGSM_DEBUG("ERROR!! Invalid length value.\n");
		return;
	}
	g_plmn_list.num_record = tmp;

	g_plmn_list.precord = (gsm_network_plmn_record_t *)malloc(sizeof(gsm_network_plmn_record_t)*g_plmn_list.num_record);

	for (i = 0; i < g_plmn_list.num_record; ++i) {

		g_plmn_list.precord[i].status = data[j++];
		memcpy(g_plmn_list.precord[i].plmn, &data[j], 6);
		j += 6;
		g_plmn_list.precord[i].act = data[j++];

		// update network db (mckim)
		memcpy(entry.number_PLMN, g_plmn_list.precord[i].plmn, 6);
		entry.PLMN_status = g_plmn_list.precord[i].status;
		entry.ACT = g_plmn_list.precord[i].act;

		vgsm_network_database_add(&entry);
	}

}


gsm_network_plmn_list_t *get_plmn_list()
{
    return &g_plmn_list;
}

// TODO:  this function will be based on network db.
void init_network_registration( void)
{
	VGSM_DEBUG("\n");
	g_network_reg.act = GSM_NET_ACT_UTRAN;
	g_network_reg.service_domain = GSM_NET_SERVICE_DOMAIN_COMBINED; //GSM_NET_SERVICE_DOMAIN_PACKET;
	g_network_reg.reg_status = GSM_NET_REG_STAT_REG_HOME;
	g_network_reg.edge_support = GSM_NET_EDGE_NOTSUPPORT;
	g_network_reg.lac[0] = 0x11;
	g_network_reg.lac[1] = 0x20;
	g_network_reg.cell_id[0] = 0x00;
	g_network_reg.cell_id[1] = 0x00;
	g_network_reg.cell_id[2] = 0x00;
	g_network_reg.cell_id[3] = 0x0A;
	g_network_reg.rej_cause = 0x00;
}

void set_network_registration( unsigned char *data, int len )
{
	VGSM_DEBUG("\n");
	g_network_reg.act = data[0];
	g_network_reg.service_domain = data[1];
	g_network_reg.reg_status = data[2];
	g_network_reg.edge_support = data[3];
	g_network_reg.lac[0] = data[4];
	g_network_reg.lac[1] = data[5];
	g_network_reg.cell_id[0] = data[6];
	g_network_reg.cell_id[1] = data[7];
	g_network_reg.cell_id[2] = data[8];
	g_network_reg.cell_id[3] = data[9];
	g_network_reg.rej_cause = data[10];
}

gsm_network_reg_t *get_network_registration()
{
    return &g_network_reg;
}

void set_plmn_selection( gsm_net_sel_e_type mode, unsigned char plmn[], gsm_net_act_e_type act )
{
	int i = 0, find = 0;

	g_network_sel_mode = mode;

	/* manual */
	if( mode == GSM_NET_SEL_MANU )
	{
		for(i=0; i<g_plmn_list.num_record; ++i)
		{
			if( memcmp( g_plmn_list.precord[i].plmn, plmn, 6 ) == 0 )
			{
				g_plmn_list.precord[i].status = GSM_NET_PLMN_STATUS_CURRENT;
				find = 1;
			}
			else if( g_plmn_list.precord[i].status == GSM_NET_PLMN_STATUS_CURRENT )
				g_plmn_list.precord[i].status = GSM_NET_PLMN_STATUS_AVAIL;
		}
	}

	if( mode == GSM_NET_SEL_AUTO)// || ( mode == GSM_NET_SEL_MANU_AUTO && find == 0 ) )
	{
		/* automatic */
		for(i=0; i<g_plmn_list.num_record; ++i)
		{
			if( i == 0 )
				g_plmn_list.precord[i].status = GSM_NET_PLMN_STATUS_CURRENT;
			else if( g_plmn_list.precord[i].status == GSM_NET_PLMN_STATUS_CURRENT )
				g_plmn_list.precord[i].status = GSM_NET_PLMN_STATUS_AVAIL;
		}
	}
}

void  set_network_band_mode(gsm_net_band_mode_e_type band_mode, int band)
{
	g_network_band_mode = band_mode ;
	g_network_band = band ;
}

gsm_net_sel_e_type get_network_sel_mode()
{
    return g_network_sel_mode;
}

void  set_network_service_domain(gsm_net_svc_domain_e_type  sevice_domain)
{
	g_service_domain = sevice_domain ;

}

gsm_net_svc_domain_e_type get_network_service_domain()
{
	return g_service_domain;

}


gsm_net_band_mode_e_type  get_network_band_mode(  )
{
    return  g_network_band_mode ;
}

int  get_network_band( )
{
    return  g_network_band ;
}

void set_network_identity( gsm_network_nitz_info_t net_identity)
{
	memcpy( &g_network_identity, &net_identity, sizeof( g_network_identity));
}

void get_network_identity( gsm_network_nitz_info_t *net_identity)
{
	memcpy( net_identity, &g_network_identity, sizeof(g_network_identity));
}
