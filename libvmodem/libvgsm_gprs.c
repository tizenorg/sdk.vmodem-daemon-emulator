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

#include "vgsm.h"
#include "vgsm_gprs.h"

int vgsm_gprs_ip_configuration(LXT_HANDLE* handle, gsm_GprsConfList list)
{
    int nretn = 0;
    int length, i;
    unsigned char *pdata, *pos;
    gsm_GprsDbEntry *pentry;

    if (handle == NULL)
	return -1;

    length = sizeof(int) + (list.num * (255 + 10 + 23));

    pdata = (unsigned char *) malloc(length);
    if (!pdata)
	return -1;

    memset(pdata, 0, length);

    *((int *)pdata) = list.num;

    pos = pdata + sizeof(int);

    for (i = 0; i < list.num; i++ ) {
	pentry = &(list.pentry[i]);

	strcpy((char *)pos, pentry->apn);

	strcpy((char *)(pos + 255), pentry->intf);

	pos[265] = pentry->ipconf.cid;

	memcpy(pos + 266, (char *)(&(pentry->ipconf.fieldflag)), 2);
	memcpy(pos + 268, (char *)(pentry->ipconf.ip_addr), 4);
	memcpy(pos + 272, (char *)(pentry->ipconf.primary_dns), 4);
	memcpy(pos + 276, (char *)(pentry->ipconf.secondary_dns), 4);
	memcpy(pos + 280, (char *)(pentry->ipconf.default_gateway), 4);
	memcpy(pos + 284, (char *)(pentry->ipconf.subnet_mask), 4);

	pos += 255 + 10 + 23;
    }

    nretn = lxt_msg_send_message(handle->fd,
	    GSM_GPRS, GSM_GPRS_IP_CONFIGURATION,
	    length, pdata);

    if (pdata)
	free(pdata);

    return 0;
}
