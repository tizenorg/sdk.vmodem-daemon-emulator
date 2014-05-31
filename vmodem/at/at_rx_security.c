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

#include <assert.h>

#include "oem_rx_security.h"
#include "oem_tx_security.h"
#include "at_rx_security.h"
#include "at_tx_security.h"
#include "server_rx_security.h"
#include "at_gen_resp.h"
#include "vgsm_sim.h"
#include "at_func.h"
#include "server_common.h"

int at_rx_sim_sec_imsi(char* atmsg)
{
    TRACE(MSGL_VGSM_SIM, "\n");

    return oem_rx_sim_sec_get_imsi_req(atmsg, strlen(atmsg));
}


int at_rx_sim_sec_pin_status(char* atmsg)
{
    TRACE(MSGL_VGSM_SIM, "\n");
    return -1;
}


int at_rx_sim_sec_lock_info(char* atmsg)
{
    TRACE(MSGL_VGSM_SIM, "\n");

    return oem_rx_sim_sec_get_lock_info_req(atmsg, strlen(atmsg));
}

int at_rx_sim_sec_check_password(char* atmsg)
{
    TRACE(MSGL_VGSM_SIM, "\n");

    return oem_rx_sim_sec_check_password_req(atmsg, strlen(atmsg));
} 

int at_rx_sim_sec_change_password(char* atmsg)
{
    TRACE(MSGL_VGSM_SIM, "\n");

    return oem_rx_sim_sec_change_password_req(atmsg, strlen(atmsg));
}

static int at_rx_sim_sec_set_phone_lock_req(void* ptr_data, int data_len)
{
    unsigned char *p = ptr_data;
    int pb_mode = p[0];

    TRACE(MSGL_VGSM_INFO, "%d\n", pb_mode);
    switch (pb_mode)
    {
	case 0x00:	//TAPI_SIM_LOCK_MODE_UNLOCK:
	    TRACE(MSGL_WARN, "TAPI_SIM_LOCK_MODE_UNLOCK not implemented\n");
	    assert(0);
	    break;

	case 0x01:	//TAPI_SIM_LOCK_MODE_LOCK:
	    TRACE(MSGL_WARN, "TAPI_SIM_LOCK_MODE_LOCK not implemented\n");
	    assert(0);
	    break;

	default:
	    return server_rx_sim_sec_set_phone_lock_req(ptr_data, data_len);
    }

    return -1;
}


int at_rx_sim_sec_phone_lock(char* atmsg)
{
    TRACE(MSGL_VGSM_SIM, "\n");
    /*
    // get
    oem_rx_sim_sec_get_phone_lock_req(atmsg, strlen(atmsg));
    // set
    at_rx_sim_sec_set_phone_lock_req(atmsg, strlen(atmsg));
     */
    return -1;
}

int at_rx_sim_sec_phone_lock_get(char* atmsg)
{
    TRACE(MSGL_VGSM_SIM, "\n");
    unsigned char lock_type;
    char* origPwd = get_callbarring_pwd();
    lock_type = 0;	// 0: not active, 1: active

    char* data = strchr(atmsg, '=');
    char* ret = NULL;

    ret = strtok(data+1, TOKEN);

    char token[] = "\"";
    ret = strtok(NULL, TOKEN);
    ret = strtok(NULL, TOKEN);
    TRACE(MSGL_VGSM_SIM, "input pw:%s, orig pw:%s\n", ret, origPwd);
    if(ret)
    {
	if( !strcmp(ret, origPwd) )
	    return at_gen_resp_send(AT_GEN_ERR_NO_ERROR);
	else 
	    return at_gen_resp_send(AT_CME_ERR_INCORRECT_PWD);
    }
    else
	return at_tx_sim_sec_phone_lock_resp(lock_type);
}

const unsigned char at_g_gsm_efile_usim_li_info[] = {
    //0x90, 0x00, 0x1e,     // SW1, SW2, LENGTH
    0x62, 0x1c, 0x82, 0x02, 0x41, 0x21, 0x83, 0x02,
    0x6f, 0x05, 0xa5, 0x03, 0xda, 0x01, 0x06, 0x8a,
    0x01, 0x05, 0x8b, 0x03, 0x6f, 0x06, 0x05, 0x80,
    0x02, 0x00, 0x08, 0x88, 0x01, 0x10,
};

const unsigned char at_g_gsm_efile_ecc_info[] = {
    //0x90, 0x00, 0x21,     //SW1, SW2, LENGTH
    0x62, 0x1f, 0x82, 0x05, 0x42, 0x21, 0x00, 0x14,
    0x05, 0x83, 0x02, 0x6f, 0xb7, 0xa5, 0x03, 0xda,
    0x01, 0x04, 0x8a, 0x01, 0x05, 0x8b, 0x03, 0x6f,
    0x06, 0x0d, 0x80, 0x02, 0x00, 0x64, 0x88, 0x01,
    0x08,
};

const unsigned char at_g_gsm_efile_sst_info[] = {
    //0x90, 0x00, 0x1e,
    0x62, 0x1c, 0x82, 0x02, 0x41, 0x21, 0x83, 0x02,
    0x6f, 0x38, 0xa5, 0x03, 0xda, 0x01, 0x04, 0x8a,
    0x01, 0x05, 0x8b, 0x03, 0x6f, 0x06, 0x08, 0x80,
    0x02, 0x00, 0x07, 0x88, 0x01, 0x20,
};

const unsigned char at_g_gsm_efile_est_info[] = {
    //0x90, 0x00, 0x1e,
    0x62, 0x1c, 0x82, 0x02, 0x41, 0x21, 0x83, 0x02,
    0x6f, 0x56, 0xa5, 0x03, 0xda, 0x01, 0x04, 0x8a,
    0x01, 0x05, 0x8b, 0x03, 0x6f, 0x06, 0x07, 0x80,
    0x02, 0x00, 0x01, 0x88, 0x01, 0x28,
};

const unsigned char at_g_gsm_efile_spn_info[] = {
    //0x90, 0x00, 0x27,
    0x62, 0x25, 0x82, 0x02, 0x41, 0x21, 0x83, 0x02,
    0x6f, 0x46, 0xa5, 0x06, 0x80, 0x01, 0x31, 0xc0,
    0x01, 0x00, 0x8a, 0x01, 0x05, 0x8b, 0x06, 0x6f,
    0x06, 0x01, 0x06, 0x00, 0x06, 0x80, 0x02, 0x00, 
    0x11, 0x81, 0x02, 0x00, 0x25, 0x88, 0x00 
};

const unsigned char at_g_gsm_efile_imsi_info[] = {
    //0x90, 0x00, 0x28,
    0x62, 0x26, 0x82, 0x02, 0x41, 0x21, 0x83, 0x02,
    0x6f, 0x07, 0xa5, 0x06, 0x80, 0x01, 0x31, 0xc0,
    0x01, 0x00, 0x8a, 0x01, 0x05, 0x8b, 0x06, 0x6f,
    0x06, 0x01, 0x04, 0x00, 0x04, 0x80, 0x02, 0x00,
    0x09, 0x81, 0x02, 0x00, 0x1d, 0x88, 0x01, 0x38,
};

const unsigned char at_g_gsm_efile_spdi_info[] = {
    0x62, 0x26, 0x82, 0x02, 0x01, 0x21, 0x83, 0x02,
    0x6f, 0xcd, 0xa5, 0x06, 0x80, 0x01, 0x31, 0xc0,
    0x01, 0x00, 0x8a, 0x01, 0x05, 0x8b, 0x06, 0x6f,
    0x06, 0x01, 0x04, 0x00, 0x04, 0x80, 0x02, 0x00,
    0x40, 0x81, 0x02, 0x00, 0x54, 0x88, 0x01, 0xd8,
};

const unsigned char at_g_gsm_efile_opl_info[] = {
    0x62, 0x29, 0x82, 0x05, 0x02, 0x21, 0x00, 0x08,
    0x14, 0x83, 0x02, 0x6f, 0xc6, 0xa5, 0x06, 0x80,
    0x01, 0x31, 0xc0, 0x01, 0x00, 0x8a, 0x01, 0x05,
    0x8b, 0x06, 0x6f, 0x06, 0x01, 0x06, 0x00, 0x06,
    0x80, 0x02, 0x00, 0xa0, 0x81, 0x02, 0x00, 0xb4,
    0x88, 0x01, 0xd0,
};

const unsigned char at_g_gsm_efile_cfis_info[] = {
    0x62, 0x28, 0x82, 0x05, 0x02, 0x21, 0x00, 0x10,
    0x01, 0x83, 0x02, 0x6f, 0xcb, 0xa5, 0x06, 0x80,
    0x01, 0x31, 0xc0, 0x01, 0x00, 0x8a, 0x01, 0x05,
    0x8b, 0x06, 0x6f, 0x06, 0x01, 0x03, 0x00, 0x03,
    0x80, 0x02, 0x00, 0x10, 0x81, 0x02, 0x00, 0x24,
    0x88, 0x00,
};

const unsigned char at_g_gsm_efile_pnn_info[] = {
    0x62, 0x29, 0x82, 0x05, 0x02, 0x21, 0x00, 0x22,
    0x02, 0x83, 0x02, 0x6f, 0xc5, 0xa5, 0x06, 0x80,
    0x01, 0x31, 0xc0, 0x01, 0x00, 0x8a, 0x01, 0x05,
    0x8b, 0x06, 0x6f, 0x06, 0x01, 0x06, 0x00, 0x06,
    0x80, 0x02, 0x00, 0x44, 0x81, 0x02, 0x00, 0x58,
    0x88, 0x01, 0xc8,
};

const unsigned char at_g_gsm_efile_smsp_info[] = {
    0x62, 0x19, 0x82, 0x05, 0x42, 0x21, 0x00, 0x28,
    0x02, 0x83, 0x02, 0x6f, 0x42, 0x8a, 0x01, 0x05,
    0x8b, 0x03, 0x6f, 0x06, 0x03, 0x80, 0x02, 0x00,
    0x50, 0x88, 0x00,
};

static const char* get_sim_filename(gsm_sec_sim_file_id_t file_id)
{
    switch (file_id)
    {
#define X(id) case GSM_SEC_EFILE_##id: return #id;
	X(USIM_LI)
	    X(ECC)
	    X(SST)
	    X(EST)
	    X(CPHS_CPHS_INFO)
	    X(SPN)
	    X(IMSI)
	    X(ICCID)
	    X(SPDI)
	    X(CPHS_CFIS)
	    X(OPL)
	    X(PNN)
#undef X
	default:
	    return "unknown";
    }
}

static const unsigned char* sim_get_file_info(gsm_sec_sim_file_id_t file_id, unsigned char *file_info_len)
{
    const unsigned char *data;
    unsigned char len;

    TRACE(MSGL_VGSM_INFO, "\n");

    switch (file_id)
    {
	case GSM_SEC_EFILE_USIM_LI :
	    data = at_g_gsm_efile_usim_li_info;
	    len = sizeof at_g_gsm_efile_usim_li_info;
	    break;

	case GSM_SEC_EFILE_ECC :
	    data = at_g_gsm_efile_ecc_info;
	    len = sizeof at_g_gsm_efile_ecc_info;
	    break;

	case GSM_SEC_EFILE_SST :
	    data = at_g_gsm_efile_sst_info;
	    len = sizeof at_g_gsm_efile_sst_info;
	    break;

	case GSM_SEC_EFILE_EST :
	    data = at_g_gsm_efile_est_info;
	    len = sizeof at_g_gsm_efile_est_info;
	    break;

	case GSM_SEC_EFILE_SPN :
	    data = at_g_gsm_efile_spn_info;
	    len = sizeof at_g_gsm_efile_spn_info;
	    break;

	case GSM_SEC_EFILE_IMSI :
	    data = at_g_gsm_efile_imsi_info;
	    len = sizeof at_g_gsm_efile_imsi_info;
	    break;

	case GSM_SEC_EFILE_SPDI :
	    data = at_g_gsm_efile_spdi_info;
	    len = sizeof at_g_gsm_efile_spdi_info;
	    break;

	case GSM_SEC_EFILE_OPL :
	    data = at_g_gsm_efile_opl_info;
	    len = sizeof at_g_gsm_efile_opl_info;
	    break;

	case GSM_SEC_EFILE_CPHS_CFIS :
	    data = at_g_gsm_efile_cfis_info;
	    len = sizeof at_g_gsm_efile_cfis_info;
	    break;

	case GSM_SEC_EFILE_PNN :
	    data = at_g_gsm_efile_pnn_info;
	    len = sizeof at_g_gsm_efile_pnn_info;
	    break;

	case GSM_SEC_EFILE_SMSP :
	    data = at_g_gsm_efile_smsp_info;
	    len = sizeof at_g_gsm_efile_smsp_info;
	    break;

	case GSM_SEC_EFILE_CPHS_CPHS_INFO :
	case GSM_SEC_EFILE_ICCID :
	default:
	    TRACE(MSGL_VGSM_INFO, "Not handled file id =[%x] \n", file_id);
	    len = 0;
	    return 0;
    }

    *file_info_len = len;

    return data;
}

const unsigned char at_g_gsm_efile_icd_data[] = {
    //0x90, 0x00, 0x0a,
    0x98, 0x94, 0x22, 0x06, 0x45, 0x61, 0x53, 0x49,
    0x38, 0xf6,
};

const unsigned char at_g_gsm_efile_usim_li_data[] = {
    //0x90, 0x00, 0x08,
    0x65, 0x6e, 0x65, 0x6e, 0x65, 0x6e, 0x64, 0x65
};

const unsigned char at_g_gsm_efile_ecc_data[] = {
    //0x90, 0x00, 0x14,
    0x11, 0xf2, 0xff, 0x4e, 0x6f, 0x74, 0x72, 0x75,
    0x66, 0x20, 0x46, 0x65, 0x75, 0x65, 0x72, 0x77,
    0x65, 0x68, 0x72, 0x00
};

const unsigned char at_g_gsm_efile_sst_data[] = {
    //0x90, 0x00, 0x07,
    0x9e, 0xdf, 0xb7, 0x1d, 0xe7, 0xfe, 0x07
};

const unsigned char at_g_gsm_efile_est_data[] = {
    //0x90, 0x00, 0x01,
    0x00
};

const unsigned char at_g_gsm_efile_spn_data[] = {
    //0x90, 0x00, 0x11,
    0x01, 0x54, 0x49, 0x5a, 0x45, 0x4e, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff
};
/*
   const unsigned char at_g_gsm_efile_imsi_data[] = {
//0x90, 0x00, 0x09,
0x08, 0x19, 0x11, 0x11, 0x30, 0x00, 0x61, 0x56,
0x26
};
 */

const unsigned char at_g_gsm_efile_imsi_data[] = {
    //0x90, 0x00, 0x09,
    0x08, 0x19, 0x11, 0x11, 0x30, 0x01, 0x61, 0x56,
    0x26
};

const unsigned char at_g_gsm_efile_spdi_data[] = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
};

const unsigned char at_g_gsm_efile_opl_data[] = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
};

const unsigned char at_g_gsm_efile_cfis_data[] = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
};

const unsigned char at_g_gsm_efile_pnn_data[] = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff,
};

const unsigned char at_g_gsm_efile_smsp1_data[] = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xe9, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0x07, 0x91, 0x44, 0x97, 0x37, 0x01, 0x90,
    0x37, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff,
};

const unsigned char at_g_gsm_efile_smsp2_data[] = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
};

static const unsigned char* sim_get_file_data(gsm_sec_sim_file_id_t file_id, unsigned char* file_data_len)
{
    const unsigned char *data;
    unsigned char len;

    TRACE(MSGL_VGSM_INFO, "\n");

    switch (file_id)
    {
	case GSM_SEC_EFILE_ICCID :
	    data = at_g_gsm_efile_icd_data;
	    len = sizeof at_g_gsm_efile_icd_data;
	    break;

	case GSM_SEC_EFILE_USIM_LI :
	    data = at_g_gsm_efile_usim_li_data;
	    len = sizeof at_g_gsm_efile_usim_li_data;
	    break;

	case GSM_SEC_EFILE_ECC :
	    data = at_g_gsm_efile_ecc_data;
	    len = sizeof at_g_gsm_efile_ecc_data;
	    break;

	case GSM_SEC_EFILE_SST :
	    data = at_g_gsm_efile_sst_data;
	    len = sizeof at_g_gsm_efile_sst_data;
	    break;

	case GSM_SEC_EFILE_EST :
	    data = at_g_gsm_efile_est_data;
	    len = sizeof at_g_gsm_efile_est_data;
	    break;

	case GSM_SEC_EFILE_SPN :
	    data = at_g_gsm_efile_spn_data;
	    len = sizeof at_g_gsm_efile_spn_data;
	    break;

	case GSM_SEC_EFILE_IMSI :
	    data = at_g_gsm_efile_imsi_data;
	    len = sizeof at_g_gsm_efile_imsi_data;
	    break;

	case GSM_SEC_EFILE_SPDI :
	    data = at_g_gsm_efile_spdi_data;
	    len = sizeof at_g_gsm_efile_spdi_data;
	    break;

	case GSM_SEC_EFILE_OPL :
	    data = at_g_gsm_efile_opl_data;
	    len = sizeof at_g_gsm_efile_opl_data;
	    break;

	case GSM_SEC_EFILE_CPHS_CFIS :
	    data = at_g_gsm_efile_cfis_data;
	    len = sizeof at_g_gsm_efile_cfis_data;
	    break;

	case GSM_SEC_EFILE_PNN :
	    data = at_g_gsm_efile_pnn_data;
	    len = sizeof at_g_gsm_efile_pnn_data;
	    break;

	case GSM_SEC_EFILE_CPHS_CPHS_INFO :
	default:
	    TRACE(MSGL_VGSM_INFO, "unhandled file id =[%x] \n", file_id);
	    len = 0;
	    return 0;
    }

    *file_data_len = len;

    return data;
}

static int at_rx_sim_sec_get_rsim_access_req(char* atmsg)
{
    unsigned char len = 0;
    unsigned char access_cmd = 0x00;
    unsigned short file_id = 0;
    unsigned char data[0x103];
    char cdata[SEND_BUF_SIZE];
    memset(cdata, '\0', sizeof(cdata));
    const unsigned char *ptr = 0;
    strcpy(cdata, atmsg);
    char* tdata = strchr(cdata, '=');
    char* ret = NULL;
    int n = 0, p1 = 0;

    ret = strtok(tdata+1, TOKEN);
    if(ret)
	access_cmd = atoi(ret);

    ret = strtok(NULL, TOKEN);
    if(ret)
	file_id = atoi(ret);

    ret = strtok(NULL, TOKEN);
    if(ret)
    {
	p1 = atoi(ret);
	if(p1 == 1)
	{
	    ptr = at_rx_sms_param_get(&len);
	    TRACE(MSGL_VGSM_INFO, "len:%02x\n", len);
	}
	else if(p1 == 2)
	{
	    ptr = at_g_gsm_efile_smsp2_data;
	    len = sizeof at_g_gsm_efile_smsp2_data; 
	}
    }
    else 
    {
	TRACE(MSGL_VGSM_INFO, "access type [%02x] file id [%x] (%s)\n", access_cmd, file_id, get_sim_filename(file_id));

	switch (access_cmd)
	{
	    case GSM_SEC_SIM_ACC_GET_RESPONSE:
		ptr = sim_get_file_info(file_id, &len);
		break;

	    case GSM_SEC_SIM_ACC_READ_BINARY:
	    case GSM_SEC_SIM_ACC_READ_RECORD:
		ptr = sim_get_file_data(file_id, &len);
		break;

	    case GSM_SEC_SIM_ACC_UPDATE_RECORD:
	    case GSM_SEC_SIM_ACC_STATUS:
		return at_gen_resp_send( AT_GEN_ERR_OPER_NOT_SUPPORTED);

	    default:
		TRACE(MSGL_VGSM_INFO, "Not handled access_type [%02x]\n", access_cmd);
		assert(0);
		break;
	}
    }
    memset(data, '\0', sizeof(data));

    if (ptr)
    {
	// found
	data[n++] = 0x90;
	data[n++] = 0;

	int hexaStringSize = (len * 2) + 1;	
	char* hexaStringP = malloc(hexaStringSize);
	if(!hexaStringP)
	    return -1;

	TRACE(MSGL_VGSM_INFO, "hexaStringSize:%d\n", hexaStringSize);

	memset(hexaStringP, '\0', hexaStringSize);

	prv_bytes_to_hexadecimal_string(ptr, len, hexaStringP, &hexaStringSize);

	strcat((char*)&data[n], hexaStringP);

	len = hexaStringSize;
	n += len;

	if(hexaStringP)
	    free(hexaStringP);
    }
    else
    {
	// not found
	data[n++] = 0x6a;
	data[n++] = 0x82;
	data[n++] = len;

	TRACE(MSGL_WARN, "file not found [%04x]\n", file_id);
    }

    assert(n <= sizeof data);

    at_tx_sim_sec_get_rsim_access_res(data, len);

    if ( file_id == GSM_SEC_EFILE_USIM_LI &&
	    (access_cmd == GSM_SEC_SIM_ACC_READ_BINARY || access_cmd == GSM_SEC_SIM_ACC_READ_RECORD) )
    {
	TRACE(MSGL_VGSM_INFO, "LI read case!\n");

	char pdata[2];
	pdata[0]	=	GSM_SEC_SIM_INIT_COMPLETED;
	pdata[1]	=	GSM_SEC_LOCK_KEY_UNLOCKED;

	oem_tx_sim_sec_pin_status_noti(pdata, 2);

	server_common_after_sim_init();
    }

    return 1;
}

int at_rx_sim_sec_rsim_access(char* atmsg)
{
    TRACE(MSGL_VGSM_SIM, "\n");
    return at_rx_sim_sec_get_rsim_access_req(atmsg);
}

int at_rx_sim_sec_card_type(char* atmsg)
{
    TRACE(MSGL_VGSM_SIM, "\n");
    return at_tx_sim_sec_card_type_resp(GSM_SEC_USIM);
}

