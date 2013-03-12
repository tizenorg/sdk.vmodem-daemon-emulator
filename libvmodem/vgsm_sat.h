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

/////////////////////////////////////////////////////////////////////
// 

#ifndef __vgsm_sat_h__
#define __vgsm_sat_h__

#define PROVIDE_LOCAL_INFO_DATA_LEN 53
#define POLLING_GET_TYPE_LEN  4
#define POLLING_RESP_TYPE_LEN 5


typedef struct tagSatProfileDown {
    unsigned char result_code;
    unsigned char usat_profile[20];
} GSM_SatProfileDown;

typedef struct tagSatEnvelopeCmd {
    unsigned char length;
    unsigned char *cmd;
} GSM_SatEnvelopeCmd;

typedef struct tagSatProactiveCmd {
	unsigned char length;
	unsigned char *cmd;
} GSM_SatProactiveCmd;

typedef struct tagSatEventDown {
	unsigned char length;
	unsigned char *event_resp;
} GSM_SatEventDown;

typedef struct tagSatProvideLocalInfo {
	unsigned char infoType;
	unsigned char infoData[PROVIDE_LOCAL_INFO_DATA_LEN];
} GSM_SatProvideLocalInfo;

typedef struct tagSatPollingGetType {
	unsigned char pollReqType;
	unsigned char present;
	unsigned char units;
	unsigned char value;
} GSM_SatPollingGetType;

typedef struct tagSatPollingRespType {
	unsigned char pollConfType;
	unsigned char pollingResult;
	unsigned char present;
	unsigned char units;
	unsigned char value;
} GSM_SatPollingRespType;

typedef struct tagSatRefreshGetType {
	unsigned char refreshMode;
	unsigned char numOfFile;
	unsigned char filePathLength;
	unsigned char fullFilePath[256];
	unsigned char aidPresent;
	unsigned char aid[16];
} GSM_SatRefreshGetType;

typedef struct tagSatSetupEventListGetType {
	unsigned char length;
	unsigned char eventList[17];
} GSM_SatEventListGetType;

typedef struct tagSatSetupInternalType {
	unsigned char length;
	unsigned char isSet;
	unsigned char title[256];
} GSM_SatSetupInternalType;

#endif // __vgsm_sat_h__
