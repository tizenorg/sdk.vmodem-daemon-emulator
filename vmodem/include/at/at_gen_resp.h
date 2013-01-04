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

#ifndef _AT_GEN_RESP_
#define _At_GEN_RESP_

#define AT_GEN_ERR_NO_ERROR 		"OK"
#define AT_GEN_ERR_OPER_NOT_SUPPORTED 	"NOT SUPPORTED"
#define AT_GEN_ERR_CONNECT		"CONNECT"
#define AT_GEN_ERR_RELEASE		"NO CARRIER"

#define AT_CME_ERR_INCORRECT_PWD	"+CME ERROR: 16"
#define AT_CME_ERR_OPER_NOT_ALLOWED 	"+CME ERROR: 3"
#define AT_CMS_ERR_UNKNOWN_ERR		"+CMS ERROR: 500"

#include "logmsg.h"

void at_set_recv_cmd(char* atmsg);
int get_at_msgseq(void);
int at_gen_resp_send(char* call_gen_resp_err);

#endif
