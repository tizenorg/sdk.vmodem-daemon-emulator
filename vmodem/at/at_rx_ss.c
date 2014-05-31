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

/* at_rx_ss.c */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "oem_rx_ss.h"
#include "at_rx_ss.h"
#include "at_msg.h"
#include "at_recv.h"
#include "at_func.h"

#include "state.h"
#include "at_tx_func.h"
#include "at_tx_ss.h"

#include "db_ss.h"
#include "vgsm_phone.h"
#include "server_rx_ss.h"
#include "server_common_ss.h"

#define AO 

int at_rx_ss_default(char* atmsg)
{
    TRACE(MSGL_VGSM_INFO, "\n");
    return 1;
}

int at_rx_ss_manage_call_set(char* atmsg)
{
    return oem_rx_ss_manage_call_set(atmsg, strlen(atmsg));
}

int at_rx_ss_cw_get(char* atmsg)
{
    int tel_class = 1;	// voice

    TRACE(MSGL_VGSM_INFO, "tel_class = %d \n", tel_class);

    oem_rx_ss_cw_get(tel_class);
    return 1;
}

int at_rx_ss_cw_set(char* atmsg)
{
    char* ret = NULL;
    char* tdata = strchr(atmsg, '=');
    int tel_class = 0, ss_mode = 0;

    TRACE(MSGL_VGSM_INFO, "%s\n", atmsg);
    ret = strtok(tdata+1, TOKEN);
    if(ret)
    {
	// first param is ignored.
    }

    ret = strtok(NULL, TOKEN);
    if(ret)
    {
	if(atoi(ret) == AT_SS_STATUS_ACTIVE)
	    ss_mode = SS_MODE_ACT;
	else
	    ss_mode = SS_MODE_DEACT;
    }

    tel_class = AT_CALL_CLASS_VOICE;	// vmodem support only voice call

    return oem_rx_ss_cw_set(tel_class, ss_mode);
}

// Call Forwarding
int at_rx_ss_cf_get(char* atmsg)
{
    TRACE(MSGL_VGSM_INFO, "\n");
    int tel_class = 1;
    int cf_type = 1;
    char* tdata = strchr(atmsg, '=');
    char* ret = NULL;
    ret = strtok(tdata+1, TOKEN);
    if(ret)
	cf_type = atoi(ret) + 1;	// for at spec

    ret = strtok(NULL, TOKEN);
    TRACE(MSGL_VGSM_INFO, "%s\n", ret);
    ret = strtok(NULL, TOKEN);
    TRACE(MSGL_VGSM_INFO, "%s\n", ret);
    if(ret)
	tel_class = atoi(ret);	

    TRACE(MSGL_VGSM_INFO, "CCFC tel_class:%d\n", tel_class);
    oem_rx_ss_cf_get(tel_class, cf_type);
    return 1;
}

int at_rx_ss_cf_set(char* atmsg)
{
    int	ss_mode, tel_class, type, replyTime, num_len, num_type, satype;
    char number[MAX_GSM_DIALED_DIGITS_NUMBER], subaddr[MAX_GSM_DIALED_DIGITS_NUMBER];
    memset(subaddr, '\0', MAX_GSM_DIALED_DIGITS_NUMBER);
    char* tdata = strchr(atmsg, '=');
    char token[] = "\"";
    char* ret = NULL;
    ret = strtok(tdata+1, TOKEN);
    if(ret)
    {
	TRACE(MSGL_VGSM_INFO, "%s\n", ret);
	type = atoi(ret) + 1;
    }
    else
	type = 1;

    ret = strtok(NULL, TOKEN);
    if(ret)
    {
	TRACE(MSGL_VGSM_INFO, "%s\n", ret);
	switch(atoi(ret))
	{
	    case AT_CALL_FW_MODE_DISABLE:
		ss_mode = SS_MODE_DEACT;
		break; 
	    case AT_CALL_FW_MODE_ENABLE:
		ss_mode = SS_MODE_ACT;
		break;
	    case AT_CALL_FW_MODE_QUERY:
		ss_mode = GSM_SS_MODE_QUERY;
		break;
	    case AT_CALL_FW_MODE_REG:
		ss_mode = SS_MODE_REG;
		break;
	    case AT_CALL_FW_MODE_ERASURE:
		ss_mode = SS_MODE_DEREG;
		break;
	    default:
		ss_mode = GSM_SS_MODE_DEACTIVATE;
		break;
	}
    }
    else
	ss_mode = GSM_SS_MODE_DEACTIVATE;

    ret = strtok(NULL, token);
    if(ret)
    {
	TRACE(MSGL_VGSM_INFO, "%s\n", ret);
	strcpy(number, ret);
    }

    ret = strtok(NULL, TOKEN);
    if(ret)
    {
	TRACE(MSGL_VGSM_INFO, "%s\n", ret);
	num_type = atoi(ret);
    }
    else
	num_type = 129;	// default

    ret = strtok(NULL, TOKEN);
    if(ret)
    {
	TRACE(MSGL_VGSM_INFO, "%s\n", ret);
	strcpy(subaddr, ret);
    }

    ret = strtok(NULL, TOKEN);
    if(ret)
    {
	TRACE(MSGL_VGSM_INFO, "%s\n", ret);
	satype = atoi(ret);
    }
    else
	satype = 128;	// default

    ret = strtok(NULL, TOKEN);
    if(ret)
    {
	TRACE(MSGL_VGSM_INFO, "%s\n", ret);
	tel_class = atoi(ret);
    }
    else
	tel_class = AT_CALL_SS_CLASS_VOICE;	// default

    ret = strtok(NULL, TOKEN);
    if(ret)
    {
	TRACE(MSGL_VGSM_INFO, "%s\n", ret);
	replyTime = atoi(ret);
    }
    else
	replyTime = 20;	// default

    num_len = strlen(number);

    oem_rx_ss_cf_set(ss_mode, type, tel_class, replyTime, num_len, number, num_type);
    //	oem_rx_ss_cf_set(ss_mode, type, tel_class, replyTime, num_len, number, num_type, satype, subaddr);
    return 1;
}

int at_rx_ss_cb_get(char* atmsg)
{
    TRACE(MSGL_VGSM_INFO, "\n");
    int	tel_class = AT_CALL_SS_CLASS_ALL;	// all
    const char* scmd;
    int nn = 0, found = 0;
    int cb_type = 0;

    char* tdata = strchr(atmsg, '=');
    char token[] = "\"";
    char* ret = NULL;

    ret = strtok(tdata+1, token);

    if(ret)
    {
	for (nn = 0; ; nn++) {
	    scmd = sDefaultFac[nn].fac;
	    if (!scmd) // end of list
		break;

	    if ( !strcmp( scmd, ret ) ) {
		found = 1;
		break;
	    }
	}

	if(found)
	    cb_type = sDefaultFac[nn].cb_e_type;
	else 
	    cb_type = SS_CB_TYPE_NONE;
    }

    ret = strtok(NULL, TOKEN);
    ret = strtok(NULL, TOKEN);
    if(ret)
    {
	tel_class = atoi(ret);
    }

    oem_rx_ss_cb_get(tel_class, cb_type);
    return 1;
}

int at_rx_ss_cb_set(char* atmsg)
{
    int cb_type = 0, ss_mode = 0, tel_class;
    char passwd[32];
    memset(passwd, '\0', sizeof(passwd));
    const char* scmd;
    int nn = 0, found = 0;

    TRACE(MSGL_VGSM_INFO, "\n");

    char* tdata = strchr(atmsg, '=');
    char token[] = "\"";
    char* ret = NULL;

    ret = strtok(tdata+1, token);
    TRACE(MSGL_VGSM_INFO, "%s\n", ret);
    if(ret)
    {
	for (nn = 0; ; nn++) {
	    scmd = sDefaultFac[nn].fac;
	    if (!scmd) // end of list
		break;

	    if ( !strcmp( scmd, ret ) ) {
		found = 1;
		break;
	    }
	}

	if(found)
	    cb_type = sDefaultFac[nn].cb_e_type;
	else
	    cb_type = SS_CB_TYPE_NONE;
    }

    ret = strtok(NULL, TOKEN);
    TRACE(MSGL_VGSM_INFO, "%s\n", ret);
    if(ret)
    {
	switch(atoi(ret))
	{
	    case 0:
		ss_mode = SS_MODE_DEACT;
		break;
	    case 1:
		ss_mode = SS_MODE_ACT;
		break;
	}
    }
    TRACE(MSGL_VGSM_INFO, "set ss_mode:%d\n", ss_mode);

    ret = strtok(NULL, token);
    TRACE(MSGL_VGSM_INFO, "%s\n", ret);
    if(ret)
	strcpy(passwd, ret);	

    ret = strtok(NULL, TOKEN);
    TRACE(MSGL_VGSM_INFO, "%s\n", ret);
    if(ret)
	tel_class = atoi(ret);
    else
	tel_class = 17;	// suport voice only 	

    return oem_rx_ss_cb_set(cb_type, ss_mode, tel_class, passwd);
}

int at_rx_ss_cb_passwd_set(char* atmsg)
{
    TRACE(MSGL_VGSM_INFO, "unimplemented\n");
    /*	
	char curr_passwd[4];
	char new_passwd[4];

	TRACE(MSGL_VGSM_INFO, "\n");

	memcpy(curr_passwd, &atmsg[0], 4);
	memcpy(new_passwd, &atmsg[1], 4);

	oem_rx_ss_cb_passwd_set(curr_passwd, new_passwd);
     */
    return 1;
}

int at_rx_ss_aoc_get(char* atmsg)
{
    TRACE(MSGL_VGSM_INFO, "unimplemented\n");
    /*	TelSsAocType_t *aoc_type ;
	_AOC_t current_aoc ;
	_AOC_t *pcurrent_aoc ;
	ss_aoc_res_data_type res_type;

	aoc_type =( TelSsAocType_t*) atmsg[0] ;

	pcurrent_aoc = &current_aoc;

	get_aoc_data(  &current_aoc);

	switch ( *aoc_type )
	{
	case TAPI_SS_AOC_TYPE_ACM :
	res_type.type = TAPI_SS_AOC_TYPE_ACM ;
	memcpy(&res_type.acm, &current_aoc.acm, sizeof(res_type.acm));
	at_tx_ss_aoc_resp(&res_type,sizeof (ss_aoc_res_data_type ));
	break;
	case TAPI_SS_AOC_TYPE_CCM:
	res_type.type = TAPI_SS_AOC_TYPE_CCM ;
	memcpy(&res_type.ccm, &current_aoc.ccm, sizeof(res_type.ccm));

	at_tx_ss_aoc_resp(&res_type,sizeof (ss_aoc_res_data_type ));
	break;
	case TAPI_SS_AOC_TYPE_MAXACM:
	res_type.type = TAPI_SS_AOC_TYPE_MAXACM ;
	memcpy(&res_type.maxacm, &current_aoc.maxacm, sizeof(res_type.maxacm));

	at_tx_ss_aoc_resp(&res_type,sizeof (ss_aoc_res_data_type ));
	break;
	case TAPI_SS_AOC_TYPE_PUC:
	res_type.type = TAPI_SS_AOC_TYPE_PUC ;
	memcpy(&res_type.ppu, &current_aoc.ppu, sizeof(res_type.ppu));
	memcpy(&res_type.chartype, &current_aoc.chartype, sizeof(res_type.chartype));
	memcpy(&res_type.currency, &current_aoc.currency, sizeof(res_type.currency));

	at_tx_ss_aoc_resp(&res_type,sizeof (ss_aoc_res_data_type ));
	break;
	case TAPI_SS_AOC_TYPE_RESET:
	res_type.type = TAPI_SS_AOC_TYPE_RESET ;
	memcpy(&res_type.acm, &current_aoc.acm, sizeof(res_type.acm));
	memcpy(&res_type.ccm, &current_aoc.ccm, sizeof(res_type.ccm));

	at_tx_ss_aoc_resp(&res_type,sizeof (ss_aoc_res_data_type ));
	break;
	default:
	TRACE(MSGL_WARN, "Unknown AOC TYPE from telephony server( %x )\n", *aoc_type);
	break;
	}
     */
    return 0;
}

int at_rx_ss_aoc_set(char* atmsg)
{
    TRACE(MSGL_VGSM_INFO, "unimplemented\n");
    /*	TelSsAocType_t *aoc_type ;
	aoc_type = (TelSsAocType_t*) atmsg[0] ;

	_AOC_t current_aoc ;
	_AOC_t *aoc_set_data ;

	get_aoc_data( &current_aoc);

	aoc_set_data = (_AOC_t*)atmsg ;

	switch (*aoc_type )
	{
	case TAPI_SS_AOC_TYPE_ACM :
	memcpy(&current_aoc.acm, &aoc_set_data->acm, sizeof(current_aoc.acm));
	break;
	case TAPI_SS_AOC_TYPE_CCM:
	memcpy( &current_aoc.ccm, &aoc_set_data->ccm , sizeof(current_aoc.ccm));
	break;
	case TAPI_SS_AOC_TYPE_MAXACM:
	memcpy( &current_aoc.maxacm, &aoc_set_data->maxacm , sizeof(current_aoc.maxacm));
	break;
	case TAPI_SS_AOC_TYPE_PUC:
	memcpy( &current_aoc.ppu, &aoc_set_data->ppu, sizeof(current_aoc.ppu));
	memcpy( &current_aoc.chartype, &aoc_set_data->chartype, sizeof(current_aoc.chartype));
	memcpy( &current_aoc.currency, aoc_set_data->currency , sizeof(current_aoc.currency));
	break;
	case TAPI_SS_AOC_TYPE_RESET:
	memcpy( &current_aoc.acm, &aoc_set_data->acm,  sizeof(current_aoc.acm));
	memcpy(&current_aoc.ccm, &aoc_set_data->ccm ,sizeof(current_aoc.ccm));
	break;
	default:
	TRACE(MSGL_WARN, "Unknown AOC TYPE from telephony server %x\n", *aoc_type );
	break;
	}
     */
    return 0;
}

int at_rx_ss_cli(void)
{
    oem_rx_ss_cli("TelTapiSsQueryCliServiceStatus");
    return 0;
}

