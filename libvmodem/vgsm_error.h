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

#ifndef _vgsm_error_h_
#define _vgsm_error_h_

/*
 * The phone app checks,
 * when the AT command result is not OK,
 *  ERR_CME
 *  ERR_CMS
 *  ERR_UNKNOWN
 *  ERR_UNKNOWNRESPONSE
 * only this four cases.
 */
typedef enum {
    ERR_NONE=1,
    ERR_CME,
    ERR_CMS,
    ERR_UNKNOWN,
    ERR_UNKNOWNRESPONSE,
    ERR_DEVICEOPENERROR,
    ERR_DEVICELOCKED,
    ERR_DEVICENOTEXIST,
    ERR_DEVICEBUSY,
    ERR_DEVICENOPERMISSION,
    ERR_DEVICENODRIVER,
    ERR_DEVICENOTWORK,
    ERR_DEVICEDTRRTSERROR,
    ERR_DEVICECHANGESPEEDERROR,
    ERR_DEVICEWRITEERROR,
    ERR_DEVICEREADERROR,
    ERR_DEVICEPARITYERROR,
    ERR_TIMEOUT,
    ERR_FRAMENOTREQUESTED,
    ERR_UNKNOWNFRAME,
    ERR_UNKNOWNCONNECTIONTYPESTRING,
    ERR_UNKNOWNMODELSTRING,
    ERR_SOURCENOTAVAILABLE,
    ERR_NOTSUPPORTED,
    ERR_EMPTY,
    ERR_SECURITYERROR,
    ERR_INVALIDLOCATION,
    ERR_NOTIMPLEMENTED,
    ERR_FULL,
    ERR_CANTOPENFILE,
    ERR_MOREMEMORY,
    ERR_PERMISSION,
    ERR_EMPTYSMSC,
    ERR_INSIDEPHONEMENU,
    ERR_NOTCONNECTED,
    ERR_WORKINPROGRESS,
    ERR_PHONEOFF,
    ERR_FILENOTSUPPORTED,
    ERR_BUG,
    ERR_CANCELED,
    ERR_NEEDANOTHERANSWER,
    ERR_OTHERCONNECTIONREQUIRED,
    ERR_WRONGCRC,
    ERR_INVALIDDATETIME,
    ERR_MEMORY,
    ERR_INVALIDDATA,
    ERR_FILEALREADYEXIST,
    ERR_FILENOTEXIST
} GSM_Error;

#define CME_SIM_NOT_INSERTED            10
#define CME_SIM_PIN_REQUIRED            11
#define CME_SIM_PUK_REQUIRED            12
#define CME_SIM_BUSY                    14
#define CME_INCORRECT_PASSWORD          16
#define CME_SIM_PUK2_REQUIRED           18
// nsclass {
#define CME_SIM_PIN2_REQUIRED           19
#define CME_SIM_PIN1_BLOCKED			20
#define CME_SIM_PIN2_BLOCKED			21
// }

#endif
