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

#ifndef __vgsm_display_h__
#define __vgsm_display_h__

#include "vgsm_network.h"

/* ICON_TYPE  field */
/* Sets the request value */
typedef enum {
	DISP_TYPE_RSSI	    		= 0x01, 	/* 0000 0001 : signal strength */
	DISP_TYPE_BATTERY	    	= 0x02, 	/* 0000 0010 : battery level   */
	DISP_TYPE_ACT	    		= 0x04, 	/* 0000 0100 : Protocol Standard: GSM,GPRS,UMTS   */
	DISP_TYPE_REG_STATUS	= 0x08, 	/* 0000 1000 : Registration status   */
	DISP_TYPE_ALL	    		= 0xFF  	/* 1111 1111 : ALL Values Request */
} disp_icon_type_e;

/* RSSI  field */
/* Radio signal strength */
typedef enum {
	DISP_RSSI_0	= 0x00,
	DISP_RSSI_1	= 0x01,
	DISP_RSSI_2	= 0x02,
	DISP_RSSI_3	= 0x03,
	DISP_RSSI_4	= 0x04,
	DISP_RSSI_5	= 0x05,
	DISP_RSSI_MAX
} disp_icon_rssi_type_e;

/* BATTERY  field */
/* Determines a battery Icon. */
typedef enum {
	DISP_BATTERY_LEVEL0	= 0x00,	/* 0x00 : blank */
	DISP_BATTERY_LEVEL1	= 0x01,	/* 0x01 */
	DISP_BATTERY_LEVEL2	= 0x02,	/* 0x02 */
	DISP_BATTERY_LEVEL3	= 0x03,	/* 0x03 : full */
	DISP_BATTERY_LEVEL4	= 0x04,	/* 0x04 : full for smart phone*/
	DISP_BATTERY_MAX
} disp_icon_batt_type_e;

/* DISPLAY ICON INFO */
typedef struct {
    disp_icon_type_e icon_type;
    disp_icon_rssi_type_e rssi;
    disp_icon_batt_type_e battery;
    gsm_net_act_e_type act;
    gsm_net_regist_status_e_type reg_status;
} disp_icon_info_t;

#endif // __v_gsm_display_h__
