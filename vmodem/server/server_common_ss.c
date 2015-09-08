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

#include "vgsm_ss.h"
#include "db_ss.h"
#include "oem_tx_ss.h"
#include "misc.h"

#include "phoneserver.h"
#include "vgsm_phone.h"
#include "vgsm_debug.h"
#include "server_common_ss.h"
#include "at_func.h"

_AOC_t  g_aoc_data ;
_USSD_data_t	g_ussd_data;
int iset=0;

static call_waiting_entry_t * setinitDB(call_waiting_entry_t *entry, int class, int mode, int count)
{
    entry->tel_class = class;
    entry->ss_mode = mode;
    entry->count = count;
    strcpy(entry->number, SUBSCRIBER_NUM);
    vgsm_ss_database_add(SS_CMD_CW, entry);
    return entry;
}

int init_ss_info_re(void)
{
    call_waiting_entry_t * entry = malloc(sizeof(call_waiting_entry_t));
    if(!entry)
	return -1;
    memset(entry, 0, sizeof(call_waiting_entry_t));


    VGSM_DEBUG("\n");

    LXT_MESSAGE cw_packet, cf_packet, cb_packet, cb_pwd_packet;
    TAPIMessageInit(&cw_packet);
    TAPIMessageInit(&cf_packet);
    TAPIMessageInit(&cb_packet);
    TAPIMessageInit(&cb_pwd_packet);

    /* CW DB RESTORE */
    vgsm_ss_database_restore(SS_CMD_CW);

    call_waiting_entry_t * cw_entry = get_call_waiting_entry();
    cw_packet.data = cw_entry;
    cw_packet.group  = GSM_SUPS;
    cw_packet.action = GSM_SUPS_SET_CW_REQ;
    cw_packet.length = (sizeof(call_waiting_entry_t) * cw_entry[0].count);
    if(cw_packet.length)
	FuncServer->Cast(&GlobalPS, LXT_ID_CLIENT_EVENT_INJECTOR, &cw_packet);	// &ServerHandle->server_cast

    /* CF DB RESTORE */
    vgsm_ss_database_restore(SS_CMD_CF);

    call_forwarding_entry_t * cf_entry = get_call_forwarding_entry();
    cf_packet.data = cf_entry;
    cf_packet.group  = GSM_SUPS;
    cf_packet.action = GSM_SUPS_SET_CCFC_REQ;
    cf_packet.length = (sizeof(call_forwarding_entry_t) * cf_entry[0].count);

    if(cf_packet.length)
	FuncServer->Cast(&GlobalPS, LXT_ID_CLIENT_EVENT_INJECTOR, &cf_packet);	// &ServerHandle->server_cast

    /* CB DB RESTORE */
    vgsm_ss_database_restore(SS_CMD_CB);

    call_barring_entry_t * cb_entry  = get_call_barring_entry();
    cb_packet.data = cb_entry;
    cb_packet.group  = GSM_SUPS;
    cb_packet.action = GSM_SUPS_SET_CB_REQ;
    cb_packet.length = (sizeof(call_barring_entry_t) * cb_entry[0].count);

    if(cb_packet.length)
	FuncServer->Cast(&GlobalPS, LXT_ID_CLIENT_EVENT_INJECTOR, &cb_packet);	// &ServerHandle->server_cast

    /* CB PASSWORD NOTIFY */
    char* cb_pwd = get_callbarring_pwd();
    cb_pwd_packet.data = cb_pwd;
    cb_pwd_packet.group  = GSM_SUPS;
    cb_pwd_packet.action = GSM_SUPS_PWDSET_CB_REQ;
    cb_pwd_packet.length = sizeof(cb_pwd);
    VGSM_DEBUG("callbarring password : %c%c%c%c  \n", cb_pwd[0], cb_pwd[1], cb_pwd[2], cb_pwd[3]);

    if(cb_pwd_packet.length)
	FuncServer->Cast(&GlobalPS, LXT_ID_CLIENT_EVENT_INJECTOR, &cb_pwd_packet);	// &ServerHandle->server_cast

    if(entry)
	free(entry);
    return 1;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int init_ss_info(void)
{
    call_waiting_entry_t * entry = (call_waiting_entry_t *)malloc(sizeof(call_waiting_entry_t));
    if(!entry)
	return -1;
    memset(entry, 0, sizeof(call_waiting_entry_t));


    VGSM_DEBUG("\n");

    LXT_MESSAGE cw_packet, cf_packet, cb_packet, cb_pwd_packet;
    TAPIMessageInit(&cw_packet);
    TAPIMessageInit(&cf_packet);
    TAPIMessageInit(&cb_packet);
    TAPIMessageInit(&cb_pwd_packet);

#if	1		// 090224
    //	vgsm_ss_sqlite_cw_check_exist(17);
    //	if(cw_count !=0)							// 1 : Registration, 2 : Deregistration, 3 : Activation, 4 : Deactivation
    setinitDB(entry,1,4,0);				// 1 : voice
    //	vgsm_ss_sqlite_cw_check_exist(19);
    //	if(cw_count !=0)
    setinitDB(entry,4,4,1);				// 4 : fax
    //	vgsm_ss_sqlite_cw_check_exist(22);
    //	if(cw_count !=0)
    setinitDB(entry,8,4,2);				// 8 : SMS
    setinitDB(entry,32,4,3);			// 32 : async
#endif

    /* CW DB RESTORE */
    vgsm_ss_database_restore(SS_CMD_CW);

    call_waiting_entry_t * cw_entry = get_call_waiting_entry();
    cw_packet.data = cw_entry;
    cw_packet.group  = GSM_SUPS;
    cw_packet.action = GSM_SUPS_SET_CW_REQ;
    cw_packet.length = (sizeof(call_waiting_entry_t) * cw_entry[0].count);
    if(cw_packet.length)
	FuncServer->Cast(&GlobalPS, LXT_ID_CLIENT_EVENT_INJECTOR, &cw_packet);	// &ServerHandle->server_cast

    /* CF DB RESTORE */
    vgsm_ss_database_restore(SS_CMD_CF);

    call_forwarding_entry_t * cf_entry = get_call_forwarding_entry();
    cf_packet.data = cf_entry;
    cf_packet.group  = GSM_SUPS;
    cf_packet.action = GSM_SUPS_SET_CCFC_REQ;
    cf_packet.length = (sizeof(call_forwarding_entry_t) * cf_entry[0].count);

    if(cf_packet.length)
	FuncServer->Cast(&GlobalPS, LXT_ID_CLIENT_EVENT_INJECTOR, &cf_packet);	// &ServerHandle->server_cast

    /* CB DB RESTORE */
    vgsm_ss_database_restore(SS_CMD_CB);

    call_barring_entry_t * cb_entry  = get_call_barring_entry();
    cb_packet.data = cb_entry;
    cb_packet.group  = GSM_SUPS;
    cb_packet.action = GSM_SUPS_SET_CB_REQ;
    cb_packet.length = (sizeof(call_barring_entry_t) * cb_entry[0].count);

    if(cb_packet.length)
	FuncServer->Cast(&GlobalPS, LXT_ID_CLIENT_EVENT_INJECTOR, &cb_packet);	// &ServerHandle->server_cast

    /* CB PASSWORD NOTIFY */
    char* cb_pwd = get_callbarring_pwd();
    cb_pwd_packet.data = cb_pwd;
    cb_pwd_packet.group  = GSM_SUPS;
    cb_pwd_packet.action = GSM_SUPS_PWDSET_CB_REQ;
    cb_pwd_packet.length = sizeof(cb_pwd);
    VGSM_DEBUG("callbarring password : %c%c%c%c  \n", cb_pwd[0], cb_pwd[1], cb_pwd[2], cb_pwd[3]);

    if(cb_pwd_packet.length)
	FuncServer->Cast(&GlobalPS, LXT_ID_CLIENT_EVENT_INJECTOR, &cb_pwd_packet);	// &ServerHandle->server_cast

    if(entry)
	free(entry);
    return 1;
}

void set_aoc_data (char *at)
{
    VGSM_DEBUG("\n");
    int len=0;

    len = sizeof(g_aoc_data);

    memcpy ( &g_aoc_data, at, len-3); // currency is not set
    // 090414
    oem_tx_ss_aoc_noti(at, len-3);	// currency is not set
}

void get_aoc_data ( _AOC_t * data)
{
    VGSM_DEBUG("\n");
    memcpy ( data, &g_aoc_data, sizeof(g_aoc_data));

}

void set_ussd_data (char *at)
{
    VGSM_DEBUG("\n");
    int len=0;

    len = sizeof(g_ussd_data);
    iset = 1;

    memcpy ( &g_ussd_data, at, len-3); // currency is not set
}

void get_ussd_data ( _USSD_data_t * data)
{
    VGSM_DEBUG("\n");
    if(iset != 0)
    {
	memcpy ( data, &g_ussd_data, sizeof(g_ussd_data));
    }
    else
    {
	strcpy(g_ussd_data.time, "Not set yet!!");
	strcpy(g_ussd_data.weather, "Not set yet!!");
	memcpy ( data, &g_ussd_data, sizeof(g_ussd_data));
    }
}
