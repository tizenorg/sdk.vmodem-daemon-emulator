/*
 *  telephony-emulator
 *
 * Copyright (c) 2000 - 2011 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact: 
 * SungMin Ha <sungmin82.ha@samsung.com>
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

#ifndef _vgsm_error_h_
#define _vgsm_error_h_

/*
 * 아래의 에러코드중 어플리케이션(폰어플리케이션)에서 체크하는 것은 
 * AT command result가 OK가 아닌 다음의 경우 
 * ERR_CME
 * ERR_CMS
 * ERR_UNKNOWN
 * ERR_UNKNOWNRESPONSE
 * 이렇게 네 개의 경우만 확인한다.
 */
typedef enum {
    ERR_NONE=1,
    ERR_CME,
    ERR_CMS,
    ERR_UNKNOWN,
    ERR_UNKNOWNRESPONSE,    // 정의되지 않은 response
    ERR_DEVICEOPENERROR,    // device open 에러
    ERR_DEVICELOCKED,       // device lock 에러
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
    ERR_TIMEOUT,            // at command timeout 에러
    ERR_FRAMENOTREQUESTED,
    ERR_UNKNOWNFRAME,       // 위의 경우와 유사
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
