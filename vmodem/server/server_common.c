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

#include "server_common_network.h"
#include "server_common_security.h"
#include "server_common_ss.h"
#include "db_gprs.h"
#include "db_ss.h"
#include "db_network.h"

#include "server_tx_sms.h"
#include "server_tx_display.h"
#include "server_tx_network.h"
#include "server_common.h"
#include "at_func.h"
#include "vgsm_ss.h"
#include "oem_rx_ss.h"

//080227 - after editting gprs part in state.c, will be removed.
#include "state.h"


unsigned char g_rssi_value = 0x05;
unsigned char g_hdr_rssi_value = 0x00;
unsigned char g_battery_level = 0x04;

void init_vgsm(void)
{
	// initialize network db
	vgsm_network_database_init();
	init_plmn_list();
	init_network_registration();

	// initialize sim security & phonebook db
	server_sim_db_init();

	// initialize sim data - 090502
	server_simdata_init();

	// initialize gprs db
	vgsm_gprs_database_init();
	init_gprs_info();

	// initialize ss db
	vgsm_ss_database_init();
	init_ss_info();
	oem_rx_ss_cw_set(AT_CALL_CLASS_VOICE, SS_MODE_ACT);
}


int server_common_after_sim_init(void)
{
	/* request device ready for SMS */
	server_tx_sms_deviceReady(NULL);

	/* network reg init */
	server_tx_net_reg_noti(NULL);

	/* display info noti */
	server_tx_display_icon_info_noti(0xff, g_rssi_value, g_battery_level, 0, 0);

	return 1;
}
