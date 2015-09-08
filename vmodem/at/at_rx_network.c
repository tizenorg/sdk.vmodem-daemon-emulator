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
// at_rx_network.c
 */

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "vgsm_phone.h"
#include "vgsm_network.h"
#include "oem_rx_network.h"
#include "at_rx_network.h"
#include "linuxtapi.h"
#include "phoneserver.h"
#include "misc.h"
#include "server_common_network.h"
#include "at_send.h"
#include "at_gen_resp.h"
#include "at_func.h"

int at_rx_net_default(char* atmsg)
{
    TRACE(MSGL_VGSM_INFO, "\n");
    return 1;
}

int at_rx_net_plmn_list_get(void)
{
    TRACE(MSGL_VGSM_INFO, "\n");
    return oem_rx_net_plmn_list_get();
}

int at_rx_net_current_plmn_get(void)
{
    TRACE(MSGL_VGSM_INFO, "\n");
    return oem_rx_net_current_plmn_get( );
}

int at_rx_net_reg_get(void)
{
    TRACE(MSGL_VGSM_INFO, "\n");
    return oem_rx_net_reg_get();
}

int at_rx_net_plmn_selection_get(void)
{
    TRACE(MSGL_VGSM_INFO, "\n");
    return oem_rx_net_plmn_selection_get();
}

int at_rx_net_plmn_selection_set(char* atmsg)
{
    TRACE(MSGL_VGSM_INFO, "\n");

    //	return oem_rx_net_plmn_selection_set(atmsg, strlen(atmsg));
    return 0;
}

void at_cast_network_band_set(  unsigned char band_mode,  int net_band)
{
    LXT_MESSAGE packet;

    TAPIMessageInit(&packet);

    unsigned char data[5];
    packet.data = data ;
    packet.group  = GSM_NETWORK;
    packet.action  = GSM_NETWORK_SET_BAND_REQ;
    packet.length  = sizeof data;
    data[0] = band_mode; //gsm_net_band_mode_e_type
    data[1] = (net_band & 0xFF);
    data[2] = ((net_band >> 8) & 0xFF);
    data[3] = ((net_band >> 16) & 0xFF);
    data[4] = ((net_band >> 24) & 0xFF);

    FuncServer->Cast(&GlobalPS, LXT_ID_CLIENT_EVENT_INJECTOR, &packet);
}

int at_rx_net_band_set(char* atmsg)
{
    TRACE(MSGL_VGSM_INFO, "\n");

    //	return oem_rx_net_band_set(atmsg, strlen(atmsg));
    return 0;
}

int at_rx_net_service_domain_set(char* atmsg)
{
    TRACE(MSGL_VGSM_INFO, "\n");

    //	set_network_service_domain(*(gsm_net_svc_domain_e_type*)atmsg[0] );
    return 1;
}

int at_rx_net_mode_sel_set(char* atmsg)
{
    return oem_rx_net_mode_sel_set("TelTapiNwSetNetworkMode");
}

int at_rx_net_mode_sel_get(char* atmsg)
{
    return oem_rx_net_mode_sel_get("TelTapiNwGetNetworkMode");
}

int at_rx_net_pref_plmn_set(char* atmsg)
{
    return oem_rx_net_pref_plmn_set("TelTapiNwSetPreferredPlmn");
}

int at_rx_net_pref_plmn_get(char* atmsg)
{
    return oem_rx_net_pref_plmn_get("TelTapiNwGetPreferredPlmn");
}

int at_rx_net_service_domain_get(char* atmsg)
{
    TRACE(MSGL_VGSM_INFO, "\n");

    return oem_rx_net_service_domain_get();
}

int at_rx_net_band_get(char* atmsg)
{
    TRACE(MSGL_VGSM_INFO, "\n");

    return oem_rx_net_band_get( );
}

static const int max_net_subs_num_len = 40;
static const int max_net_subs_alpha_len = 16;

static int at_rx_add_subscriber_number( unsigned char *data, const char *number, const char *alpha)
{
    int len;
    int n = 0;

    len = strlen(number);
    assert(len <= max_net_subs_num_len);
    data[n++] = len;		// NUMBER_LEN
    data[n++] = 0;			// NUMBER_TYPE
    memset(&data[n], 0, max_net_subs_num_len);
    memcpy(&data[n], number, len);
    n += max_net_subs_num_len;

    len = strlen(alpha);
    assert(len <= max_net_subs_alpha_len);
    data[n++] = len;			// ALPHA_LEN
    memset(&data[n], 0, max_net_subs_alpha_len);
    memcpy(&data[n], alpha, len);
    n += max_net_subs_alpha_len;

    return n;
}

int at_rx_net_subscriber_num_get(char* atmsg)
{
    const int number_count = 1;
    unsigned char data[1 + (3 + max_net_subs_num_len + max_net_subs_alpha_len)*number_count];
    unsigned char *p = data;
    char sndbuf[SEND_BUF_SIZE];
    memset(sndbuf, '\0', sizeof(sndbuf));
    char* number = SUBSCRIBER_NUM;
    int n = 0;

    TRACE(MSGL_VGSM_INFO, "\n");

    p[n++] = 1;	// one number
    n += at_rx_add_subscriber_number( &p[n], number, "");

    sprintf(sndbuf, "%s,\"%s\",%d,,4%s", CNUM, number, p[2], CRLF);	// p[2] = number type, 4 = voice service
    assert(n <= sizeof data);

    at_msg_send(ACK_SEQ_RESPONSE, sndbuf, strlen(sndbuf));
    return at_gen_resp_send(AT_GEN_ERR_NO_ERROR);
}
