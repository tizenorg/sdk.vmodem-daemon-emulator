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
//#include "../vgsm/db_network.h"


int vgsm_network_registration(LXT_HANDLE* handle, gsm_network_reg_t reg)
{
	if( handle == NULL )
		return -1;

	int nretn = 0;
	int	length = 9;

	unsigned char * pdata = (unsigned char *) malloc(length);

	if (!pdata)
		return (-1);

	memset(pdata, 0, length);

	pdata[0] = (unsigned char)reg.act;
	pdata[1] = (unsigned char)reg.service_domain;
	pdata[2] = (unsigned char)reg.reg_status;
	pdata[3] = (unsigned char)reg.edge_support;
	pdata[4] = (unsigned char)reg.lac[0];
	pdata[5] = (unsigned char)reg.lac[1];
	pdata[6] = (unsigned char)reg.cell_id[0];
	pdata[7] = (unsigned char)reg.cell_id[1];
	pdata[8] = (unsigned char)reg.rej_cause;

	nretn = lxt_msg_send_message(	handle->fd,
									GSM_NETWORK,
									GSM_NETWORK_REG_NOTI,
									length,
									pdata);

	if (pdata)
		free(pdata);

	return nretn;
}


/* vgsm network plmn information by cosmos in 20090212 */
int vgsm_network_plmn_information(LXT_HANDLE* handle, gsm_network_plmn_info_t reg)
{
	if( handle == NULL )
		return -1;

	int nretn = 0;
	int	length = 9;

	unsigned char * pdata = (unsigned char *) malloc(length);

	if (!pdata)
		return (-1);

	memset(pdata, 0, length);

	pdata[0] = (unsigned char)reg.act;
	pdata[1] = (unsigned char)reg.statusplmn.status;
	memcpy(pdata + 2, reg.statusplmn.plmn, 6);
	pdata[8] = (unsigned char)reg.mode;

	nretn = lxt_msg_send_message(	handle->fd,
									GSM_NETWORK,
									GSM_NETWORK_CURRENT_PLMN_INFO,
									length,
									pdata);

	if (pdata)
		free(pdata);

	return nretn;

}

/* vgsm network plmn information by cosmos in 20090212 */
int vgsm_network_nitz_information(LXT_HANDLE* handle, gsm_network_nitz_info_t reg)
{
	if( handle == NULL )
		return -1;

	int nretn = 0;
	int	length = 56;

	unsigned char * pdata = (unsigned char *) malloc(length);

	printf("size of gsm_network_nitz_info_t : %d\n",sizeof(reg));

	if (!pdata)
		return (-1);

	memset(pdata, 0, length);

	pdata[0] = (unsigned char)reg.mm_info_type;
	pdata[1] = (unsigned char)reg.mask;

	pdata[2] = (unsigned char)reg.shortname_ci;
	pdata[3] = (unsigned char)reg.shortname_len;
	pdata[4] = (unsigned char)reg.shortname_dcs;
	memcpy(pdata + 5, reg.shortname, 16);

	pdata[21] = (unsigned char)reg.longname_ci;
	pdata[22] = (unsigned char)reg.longname_len;
	pdata[23] = (unsigned char)reg.longname_dcs;
	memcpy(pdata + 24, reg.longname, 32);

	nretn = lxt_msg_send_message(	handle->fd,
									GSM_NETWORK,
									GSM_NETWORK_CURRENT_NITZ_INFO,
									length,
									pdata);

	if (pdata)
		free(pdata);

	return nretn;
}

int vgsm_network_plmn_list(LXT_HANDLE* handle, gsm_network_plmn_list_t plmn_list)
{
	if (handle == NULL)
		return -1;

	int nretn = 0, i = 0, j = 1;
	int length = 1+(plmn_list.num_record*8);

	unsigned char * pdata = (unsigned char *) malloc(length);

	if (!pdata)
		return -1;

	memset(pdata, 0, length);

	pdata[0] = (unsigned char)plmn_list.num_record;

	for (i = 0; i < pdata[0]; ++i) {
		pdata[j++] = plmn_list.precord[i].status;
		memcpy(&pdata[j], plmn_list.precord[i].plmn, 6);
		j += 6;
		pdata[j++] = plmn_list.precord[i].act;
	}

	nretn = lxt_msg_send_message( handle->fd, GSM_NETWORK,
								GSM_NETWORK_PLMN_LIST,	length, pdata);

	if (pdata)
		free(pdata);

	return nretn;
}


// added by mckim (2007.2.26)
int vgsm_network_current_plmn(LXT_HANDLE *handle, gsm_network_plmn_record_t plmn)
{
	if (handle == NULL)
		return -1;

	int length = 9;
	int nretn = 0;

	unsigned char *pdata = (unsigned char *)malloc(length);

	if (!pdata)
		return -1;

	memset(pdata, 0, length);

	pdata[0] = 2;
	pdata[1] = plmn.status;
	memcpy(pdata + 2, plmn.plmn, 6);
	pdata[8] = plmn.act;

	lxt_msg_send_message(handle->fd, GSM_NETWORK,
						  GSM_NETWORK_CURRENT_PLMN_NOTI, length, pdata);

	if (pdata)
		free(pdata);

	return nretn;
}

