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

#include <stdarg.h>
#include <string.h>
#include <errno.h>

#include "gsmstate.h"
#include "logmsg.h"

#define DEBUG_TITLE	"VGSM"

GSM_Error GSM_InitConnection(GSM_StateMachine *s, int ReplyNum)
{
    GSM_Error   error;

    s->ReplyNum           = ReplyNum;
    s->Phone.Data.CMECode		= 0;
    s->Phone.Data.CMSCode		= 0;
    s->Phone.Data.PhonePower	= false;
    s->Phone.Data.RamDump		= false;
    memset(s->Phone.Data.dumpaddr, 0x00, sizeof(s->Phone.Data.dumpaddr));
    s->Phone.Data.ExternalMode	= false;	
    s->Phone.Data.PBReady		= false;
    s->Phone.Data.PBMirroringDone	= false;
    s->Phone.Data.PIN_retry		= 3;
    s->Phone.Data.ALS			= 0;
    s->Phone.Data.Band			= 3;
    s->opened					= false;

    s->Device.Functions			= NULL;
    s->Device.Functions			= &DPRAMDevice;

    if(!s->opened)
    {
	error=s->Device.Functions->OpenDevice(s);
	if(error != ERR_NONE) return error;

	s->opened = true;
    }

    TRACE(MSGL_VGSM_INFO, "OK\n");

    return ERR_NONE;
}


GSM_Error GSM_TerminateConnection(GSM_StateMachine *s)
{
    GSM_Error error;

    if (!s->opened) return ERR_UNKNOWN;

    error = s->Device.Functions->CloseDevice(s);
    if (error!=ERR_NONE) return error;

    s->opened = false;

    return ERR_NONE;
}

void GSM_OSErrorInfo(GSM_StateMachine *s, char const* description)
{
    TRACE(MSGL_VGSM_ERR,"[%s] System Error: %s\n", DEBUG_TITLE, description);
}

