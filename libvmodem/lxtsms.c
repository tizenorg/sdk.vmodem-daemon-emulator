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

// ++++++++++++++++++++++++++++++++++++++++++++++include about standard library
#include <string.h>
// ++++++++++++++++++++++++++++++++++++++++++++++include user define
#include "lxtsmstypedef.h"
#include "lxtsms.h"
// ++++++++++++++++++++++++++++++++++++++++++++++define area

// ++++++++++++++++++++++++++++++++++++++++++++++global variable area

// ++++++++++++++++++++++++++++++++++++++++++++++define public function


void lxt_sms_init_message(LXT_SMS *s)
{
    //    // initalize sms header
    //    s->header.teleservice_id = 0;
    //    s->header.message_type = 0;
    //    s->header.message_id = 0;
    //    s->header.language_indicator = 0;
    //    s->header.destination_number[0] = 0;
    //    s->header.call_back_number[0] = 0;
    //    s->header.size = 0;
    //
    //    // initalize sms body
    //    memset(s->body.content, 0, 256);

    memset(s,0,sizeof(LXT_SMS));
}
