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

#ifndef	_SERVER_COMMON_NETWORK_H_
#define _SERVER_COMMON_NETWORK_H_

#include "vgsm_network.h"

void init_plmn_list(void);
void set_plmn_list( unsigned char *data, int len );
gsm_network_plmn_list_t *get_plmn_list(void);
void init_network_registration(void );
void set_network_registration( unsigned char *data, int len );
gsm_network_reg_t *get_network_registration(void);
void set_plmn_selection( gsm_net_sel_e_type mode, unsigned char plmn[], gsm_net_act_e_type act );
gsm_net_sel_e_type get_network_sel_mode(void);
void	set_network_identity( gsm_network_nitz_info_t net_identity);
void get_network_identity( gsm_network_nitz_info_t *net_identity);

void set_network_service_domain(gsm_net_svc_domain_e_type service_domain ) ;
gsm_net_svc_domain_e_type get_network_service_domain(void);

void  set_network_band_mode(gsm_net_band_mode_e_type band_mode, int band );
gsm_net_band_mode_e_type  get_network_band_mode(void);
int  get_network_band(void);

#endif


