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

#include "vgsm.h"

int vgsm_ss_conference(LXT_HANDLE* handle)  // this is not used, and is not needed.
{
	if( handle == NULL )
		return -1;

    return lxt_msg_send_message(	handle->fd,
									GSM_SUPS,
									GSM_SUPS_CONFERENCE,
									0x00,
									NULL);
}

int vgsm_ss_hold(LXT_HANDLE* handle)
{
	if( handle == NULL )
		return -1;

	return lxt_msg_send_message(	handle->fd,
									GSM_SUPS,
									GSM_SUPS_HOLD,
									0x00,
									NULL);
}

int vgsm_ss_unhold(LXT_HANDLE* handle)
{
	if( handle == NULL )
		return -1;

	return lxt_msg_send_message(	handle->fd,
									GSM_SUPS,
									GSM_SUPS_UNHOLD,
									0x00,
									NULL);
}

// 090215
int vgsm_ss_getAOC(LXT_HANDLE* handle)
{
	if( handle == NULL )
		return -1;

	return lxt_msg_send_message(	handle->fd,
									GSM_SUPS,
									GSM_SUPS_AOC_GET,
									0x00,
									NULL);
}

// 090326
int vgsm_ss_restoreEI(LXT_HANDLE* handle)
{
	if( handle == NULL )
		return -1;

	return lxt_msg_send_message(	handle->fd,
									0x11,
									GSM_SUPS_AOC_GET,
									0x00,
									NULL);
}

// 090330, 090403
int vgsm_ss_setCW(LXT_HANDLE* handle, _setCW_t *setCW, int size)
{
	if( handle == NULL )
		return -1;

	return lxt_msg_send_message(	handle->fd,
									GSM_SUPS,
									GSM_SUPS_CW_SET,
									size,
									setCW);
}

int vgsm_ss_setCF(LXT_HANDLE* handle, _setCF_t *setCF, int size)
{
	if( handle == NULL )
		return -1;

	return lxt_msg_send_message(	handle->fd,
									GSM_SUPS,
									GSM_SUPS_CF_SET,
									size,
									setCF);
}

int vgsm_ss_setCB(LXT_HANDLE* handle, _setCB_t *setCB, int size)
{
	if( handle == NULL )
		return -1;

	return lxt_msg_send_message(	handle->fd,
									GSM_SUPS,
									GSM_SUPS_CB_SET,
									size,
									setCB);
}

// 090213
int vgsm_ss_getUSSD(LXT_HANDLE* handle)
{
	if( handle == NULL )
		return -1;

	return lxt_msg_send_message(    handle->fd,
									GSM_SUPS,
									GSM_SUPS_USSD_GET,
									0x00,
									NULL);
}

int vgsm_ss_setUSSD(LXT_HANDLE* handle, _USSD_data_t *ussd, int size)
{
	if( handle == NULL )
		return -1;

#if 1
	printf("************in libvgsm_ss***************\n");
	printf("time: %s, weather: %s\n",
	ussd->time, ussd->weather);

	printf("data length : %d\n",sizeof(*ussd));
	printf("time length : %d\n",sizeof(ussd->time));
	printf("weather length : %d\n",sizeof(ussd->weather));
	printf("size : %d\n", size);
#endif
 
	return lxt_msg_send_message(    handle->fd,
									GSM_SUPS,
									GSM_SUPS_USSD_SET,
									size,
									ussd);
}

int vgsm_ss_setAOC(LXT_HANDLE* handle, _AOC_t *aoc, int size)
{
	if( handle == NULL )
		return -1;

#if 1
	printf("************in libvgsm_ss***************\n");
	printf("acm: %ld, ccm: %ld, maxacm: %ld, ppu: %ld, chartype: %c\n",
	aoc->acm, aoc->ccm, aoc->maxacm, aoc->ppu,
	aoc->chartype);

	printf("data length : %d\n",sizeof(*aoc));
	printf("acm length : %d\n",sizeof(aoc->acm));
	printf("type length : %d\n",sizeof(aoc->ccm));
	printf("type length : %d\n",sizeof(aoc->maxacm));
	printf("type length : %d\n",sizeof(aoc->ppu));
	printf("type length : %d\n",sizeof(aoc->chartype));
	printf("size : %d\n", size);
#endif
 
	return lxt_msg_send_message(    handle->fd,
									GSM_SUPS,
									GSM_SUPS_AOC_SET,
									size,
									aoc);
}
