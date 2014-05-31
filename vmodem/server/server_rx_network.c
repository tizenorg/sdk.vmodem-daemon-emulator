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

/* server_rx_network.c */

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

#include "oem_tx_network.h"
#include "server_common_network.h"
#include "vgsm_network.h"
#include "vgsm_debug.h"
#include "server_tx_network.h"
#include "at_rx_network.h"
#include "server_rx_network.h"
#include "at_func.h"
#include "at_gen_resp.h"

int server_rx_net_plmn_list_get(void)
{
    unsigned char *data = 0;
    int ret = 0, len = 0;
    int i = 0, n = 0;
    char sndbuf[SEND_BUF_SIZE];
    memset(sndbuf, '\0', sizeof(sndbuf));
    char tmpbuf[128];
    char plmn_data[6];

    VGSM_DEBUG("\n");

    gsm_network_plmn_list_t *plmn_list = get_plmn_list();

    VGSM_DEBUG("plmn_list->num_record:[%d]\n", plmn_list->num_record);

    len = 1+(plmn_list->num_record*10);
    data = malloc(len);
    if(!data)
	return 0;

    data[n++] = plmn_list->num_record; /* NUM_RECORD */
    sprintf(sndbuf, "%s", COPS);

    for (i=0; i<plmn_list->num_record; ++i)
    {
	data[n++] = plmn_list->precord[i].status;
	memcpy(&data[n], plmn_list->precord[i].plmn, 6);
	plmn_data[0] = plmn_list->precord[i].plmn[0];
	plmn_data[1] = plmn_list->precord[i].plmn[1];
	plmn_data[2] = plmn_list->precord[i].plmn[2];
	plmn_data[3] = plmn_list->precord[i].plmn[3];
	plmn_data[4] = plmn_list->precord[i].plmn[4];
	plmn_data[5] = plmn_list->precord[i].plmn[5];

	n += 6;
	data[n++] = plmn_list->precord[i].act;
	data[n++] =0x11; /* LAC  2bytes*/
	memset(tmpbuf, '\0', sizeof(tmpbuf));
	VGSM_DEBUG("plmn: %s\n", plmn_data);
	sprintf(tmpbuf, "(%d,\"%c%c%c%c%c%c\",\"%c%c%c%c%c%c\",\"%c%c%c%c%c%c\")", plmn_list->precord[i].status, 
		plmn_data[0], plmn_data[1], plmn_data[2], plmn_data[3], plmn_data[4], plmn_data[5],
		plmn_data[0], plmn_data[1], plmn_data[2], plmn_data[3], plmn_data[4], plmn_data[5],
		plmn_data[0], plmn_data[1], plmn_data[2], plmn_data[3], plmn_data[4], plmn_data[5]);
	strcat(sndbuf, tmpbuf);
	if(i + 1 == plmn_list->num_record)
	    strcat(sndbuf, CRLF);
	else
	    strcat(sndbuf, TOKEN);
    }
    //sleep(3);
    ret = oem_tx_net_plmn_list_resp(sndbuf, strlen(sndbuf));

    if (data)
	free(data);

    return ret;
}

int server_rx_net_reg_get(void)
{
    unsigned char data[13];
    int n = 0;

    VGSM_DEBUG("\n");

    gsm_network_reg_t *reg = get_network_registration();

    data[n++] = reg->act; /* ACT */
    data[n++] = reg->service_domain; /* SERVICE_DOMAIN */
    data[n++] = reg->reg_status; /* REG_STATUS */
    data[n++] = reg->edge_support; /* EDGE_SUPPORT */
    data[n++] = reg->lac[0]; /* LAC */
    data[n++] = reg->lac[1]; /* LAC */
    data[n++] = reg->cell_id[0]; /* CELL-ID */
    data[n++] = reg->cell_id[1]; /* CELL-ID */
    data[n++] = reg->cell_id[2]; /* CELL-ID */
    data[n++] = reg->cell_id[3]; /* CELL-ID */
    // FIXME: where do the other two CELL-ID bytes come from?
    data[n++] = 0; /* CELL-ID */
    data[n++] = 0; /* CELL-ID */
    data[n++] = 0; /* REJ_CAUSE */

    return oem_tx_net_reg_resp(data, n);
}

#define PLMN_SIZE 6
/* Update for public open
   45001# => 11111#
 */
static const unsigned char default_plmn[PLMN_SIZE] = { '1','1','1','1','1','#' };

static void find_plmn( unsigned char *plmn, unsigned char *act)
{
    gsm_network_plmn_list_t *plmn_list = get_plmn_list();
    int i;

    for (i = 0; i < plmn_list->num_record; ++i)
    {
	if (plmn_list->precord[i].status == GSM_NET_PLMN_STATUS_CURRENT)
	{
	    memcpy(plmn, plmn_list->precord[i].plmn, sizeof default_plmn);
	    *act = plmn_list->precord[i].act;
	    return;
	}
    }

    // set defaults
    *act = GSM_NET_ACT_UTRAN;
    memcpy(plmn, default_plmn, sizeof default_plmn);
}

int server_rx_net_current_plmn_get(void)
{
    gsm_network_reg_t *reg = get_network_registration();
    //	unsigned char *plmn, *act;
    unsigned char data[16];
    int n = 0;
    int i =0;
    char sndbuf[SEND_BUF_SIZE];
    memset(sndbuf, '\0', sizeof(sndbuf));
    VGSM_DEBUG("\n");

    data[n++] = GSM_NET_SEL_AUTO; // GSM_AUTO. this mode is different from  get_network_sel_mode().
    data[n++] = reg->reg_status;

#if 1
    data[n++] = reg->act;
    for(i=0;i<sizeof(default_plmn);i++)
    {
	data[n++] = default_plmn[i];
    }
    //LAC
    data[n++] = reg->lac[0];			/* LAC */
    data[n++] = reg->lac[1];			/* LAC */

#else
    act = &data[n++];
    plmn = &data[n];
    n += sizeof default_plmn;
    data[n++] = 0x02; //GSM_FIRST

    assert(n <= sizeof data);

    find_plmn(plmn, act);
#endif
    sprintf(sndbuf, "%s%d,%d,%s,%d%s", COPS, data[0], GSM_NET_PLMN_FORMAT_NUMERIC, default_plmn, data[2], CRLF); 
    return oem_tx_net_current_plmn_resp(sndbuf, strlen(sndbuf));
}


int server_rx_net_plmn_selection_get(void)
{
    unsigned char data[1];
    int n = 0;

    VGSM_DEBUG("\n");

    data[n++] = get_network_sel_mode(); // MODE_SELECT

    return oem_tx_net_plmn_selection_resp(data, n);
}


int server_rx_net_plmn_selection_set(void *ptr_data, int data_len )
{
    unsigned char* ptr = (unsigned char *)ptr_data;
    gsm_net_act_e_type act;
    unsigned char plmn[7];
    gsm_net_sel_e_type mode;

    VGSM_DEBUG("\n");

    mode = *ptr++;
    memcpy( plmn, ptr, 6 );
    plmn[6] = 0;
    ptr += 6;
    act = *ptr;

    VGSM_DEBUG("mode=%d, plmn=%s, act=%d \n ", mode, plmn, act);

    assert(mode <= 4);

    set_plmn_selection(mode, plmn, act);

    return server_tx_net_reg_noti(NULL);
}

int server_rx_net_band_set(void *ptr_data, int data_len )
{
    unsigned char* ptr = ptr_data;
    gsm_net_band_mode_e_type net_band_mode;
    int net_band;
    char data[5];
    int n = 0;

    net_band_mode = ptr[0];
    net_band  = (ptr[1] & 0xFF);
    net_band  = net_band | ((ptr[2] & 0xFF) << 8);
    net_band  = net_band | ((ptr[3] & 0xFF) << 16);
    net_band  = net_band | ((ptr[4] & 0xFF) << 24);

    VGSM_DEBUG("net_band_mode=%d net_band=0x%08x \n", net_band_mode, net_band);

    set_network_band_mode( net_band_mode, net_band );
    at_cast_network_band_set( net_band_mode, net_band );
    assert(n <= sizeof data);

    return oem_tx_net_gen_phone_resp(data, n);
}

int server_rx_net_band_get(void)
{
    gsm_net_band_mode_e_type net_band_mode ;
    int net_band ;
    int n = 0;
    unsigned char data[5];

    net_band_mode = get_network_band_mode();
    net_band = get_network_band();

    VGSM_DEBUG("net_band_mod e= %d, band = 0x%08x\n", net_band_mode, net_band);

    data[n++] = net_band_mode;
    data[n++] =(net_band & 0xFF);
    data[n++] =( (net_band >> 8 ) & 0xFF);
    data[n++] =( (net_band >> 16 ) & 0xFF);
    data[n++] =( (net_band >> 24 ) & 0xFF);

    return oem_tx_net_band_get_resp(data, n);
}

int server_rx_net_service_domain_get(void)
{
    VGSM_DEBUG("\n");

    int ret =0;

    gsm_net_svc_domain_e_type net_svc_domain ;

    int  len = 1;
    unsigned char data[len];

    memset( data, 0, sizeof(unsigned char)*len );

    net_svc_domain = get_network_service_domain( );

    data[0] = net_svc_domain; // MODE_SELECT

    VGSM_DEBUG("net_svc_domain=%d,   \n ", net_svc_domain );

    //ret = server_tx_net_net_svc_domain_resp(data,len);

    return ret;
}

int server_rx_net_mode_sel_get(char * api)
{
    at_unimplemented(api);
    return 1;
}
int server_rx_net_mode_sel_set(char * api)
{
    at_unimplemented(api);
    return 1;
}

int server_rx_net_pref_plmn_get(char * api)
{
    at_unimplemented(api);
    return 1;
}

int server_rx_net_pref_plmn_set(char * api)
{
    at_unimplemented(api);
    return 1;
}

#if 0
int server_rx_net_plmn_list(char * api)
{
    //unimplemented(api);
    return 1;
}
#endif
