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

#include "at_send.h"
#include "at_tx_security.h"
#include "at_gen_resp.h"
#include "at_func.h"

int at_tx_sim_sec_get_imsi_res(void* ptr_data, int data_len)
{
    TRACE(MSGL_VGSM_SIM, "+CIMI res get\n");

    at_msg_send(ACK_SEQ_RESPONSE, ptr_data, data_len);
    return at_gen_resp_send(AT_GEN_ERR_NO_ERROR);
}


int at_tx_sim_sec_set_pin_status_res(int error)
{
    TRACE(MSGL_VGSM_SIM, "res set\n");

    /*	char data[4];

	data[0] = AT_GSM_SEC_CMD;
	data[1] = AT_GSM_SEC_PIN_STATUS;
	data[2] = (error & 0xFF);
	data[3] = ((error >> 8) & 0xFF);

	return at_msg_send(ACK_SEQ_RESPONSE, data, 4);
     */
    return 0;
}


int at_tx_sim_sec_get_pin_status_res(void* ptr_data, int data_len)
{
    TRACE(MSGL_VGSM_SIM, "res get\n");
    /*
       return at_msg_send(ACK_SEQ_RESPONSE, ptr_data, data_len);
     */
    return 0;
}


int at_tx_sim_sec_get_lock_info_res(void* ptr_data, int data_len)
{
    TRACE(MSGL_VGSM_SIM, "+CPINR res unimplemented\n");

    //return at_msg_send(ACK_SEQ_RESPONSE, ptr_data, data_len);
    return -1;
}

int at_tx_sim_sec_check_password_res(void* ptr_data)
{       
    TRACE(MSGL_VGSM_SIM, "res\n");

    return at_gen_resp_send((char*)ptr_data);
}

int at_tx_sim_sec_change_password_res(void* ptr_data)
{
    TRACE(MSGL_VGSM_SIM, "res\n");

    return at_gen_resp_send((char*)ptr_data);
}


int at_tx_sim_sec_set_phone_lock_res(int error)
{
    TRACE(MSGL_VGSM_SIM, "res set\n");
    /*
       char data[4];

       data[0] = AT_GSM_SEC_CMD;
       data[1] = AT_GSM_SEC_PHONE_LOCK;
       data[2] = (error & 0xFF);
       data[3] = ((error >> 8) & 0xFF);

       return at_msg_send(ACK_SEQ_RESPONSE, data, 4);
     */
    return 0;
}


int at_tx_sim_sec_get_phone_lock_res(void* ptr_data, int data_len)
{
    TRACE(MSGL_VGSM_SIM, "+CLCK res get\n");
    return 0;
}

int at_tx_sim_sec_set_rsim_access_res(int error)
{
    TRACE(MSGL_VGSM_SIM, "+CRSM res set\n");
    /*
       char data[4];

       data[0] = AT_GSM_SEC_CMD;
       data[1] = AT_GSM_SEC_RSIM_ACCESS;
       data[2] = (error & 0xFF);
       data[3] = ((error >> 8) & 0xFF);

       return at_msg_send(ACK_SEQ_RESPONSE, data, 4);
     */
    return 0;
}

int at_tx_sim_sec_get_rsim_access_res(void* ptr_data, int data_len)
{
    TRACE(MSGL_VGSM_SIM, "+CRSM resp\n");
    unsigned char* data = (unsigned char*)ptr_data;
    char sndbuf[SEND_BUF_SIZE];
    memset(sndbuf, '\0', sizeof(sndbuf));

    int snd_len = 0;

    sprintf(sndbuf, "%s%d,%d", CRSM, data[0], data[1]);

    if(data_len > 0)
    {
	strcat(sndbuf, TOKEN);
	snd_len = strlen(sndbuf);
	memcpy(&sndbuf[strlen(sndbuf)], &data[2], data_len);
	snd_len += data_len;
    }
    else
	snd_len = strlen(sndbuf);

    strcpy(&sndbuf[snd_len], CRLF);
    snd_len += strlen(CRLF); 

    TRACE(MSGL_VGSM_SIM, "%s , len:%d", sndbuf, snd_len);
    at_msg_send(ACK_SEQ_RESPONSE, sndbuf, snd_len);
    return at_gen_resp_send(AT_GEN_ERR_NO_ERROR);
}

int at_tx_sim_sec_card_type_noti(void* ptr_data, int data_len)
{
    TRACE(MSGL_VGSM_SIM, "card type noti\n");
    return 0;
}


int at_tx_sim_sec_pin_status_noti(void* ptr_data, int data_len)
{
    TRACE(MSGL_VGSM_SIM, "pin status noti\n");
    char* buf = (char*)ptr_data;
    char sndbuf[SEND_BUF_SIZE];
    memset(sndbuf, '\0', sizeof(sndbuf));

    gsm_sec_lock_e_type pindata = buf[0];
    TRACE(MSGL_VGSM_SIM, "TYPE: %d, KEY: %d\n", buf[0], buf[1]);
    sprintf(sndbuf, "%s", CPIN);

    switch(pindata)
    {
	case GSM_SEC_LOCK_TYPE_SC:
	    strcat(sndbuf, STSC);
	    break;
	case GSM_SEC_LOCK_TYPE_NO_SIM:
	    strcat(sndbuf, STNOSIM);
	    break;
	case GSM_SEC_SIM_INIT_COMPLETED:
	    strcat(sndbuf, STINIT_COMPL);
	    break;
	case GSM_SEC_LOCK_TYPE_READY:
	default:
	    strcat(sndbuf, STREADY);
	    break;
    }

    strcat(sndbuf, TOKEN);
    pindata = buf[1];
    switch(pindata)
    {
	case GSM_SEC_LOCK_KEY_PIN:
	    strcat(sndbuf, SKPIN);
	    break;
	case GSM_SEC_LOCK_KEY_PUK:
	    strcat(sndbuf, SKPUK);
	    break;
	case GSM_SEC_LOCK_KEY_PERM_BLOCKED:
	    strcat(sndbuf, SKPERM_BLOCK);
	    break;
	case GSM_SEC_LOCK_KEY_UNLOCKED:
	default:
	    strcat(sndbuf, SKUNLOCK);
	    break;
    }
    strcat(sndbuf, CRLF);

    TRACE(MSGL_VGSM_SIM, "%s", sndbuf);

    return at_msg_send(ACK_SEQ_NOTIFICATION, sndbuf, strlen(sndbuf));
}



int at_tx_sim_sec_phone_lock_resp(unsigned char lock_type)
{
    TRACE(MSGL_VGSM_SIM, "+CLCK resp\n");

    char sndbuf[SEND_BUF_SIZE];
    memset(sndbuf, '\0', sizeof(sndbuf));

    sprintf(sndbuf, "%s%d%s", CLCK, lock_type, CRLF);

    at_msg_send(ACK_SEQ_RESPONSE, sndbuf, strlen(sndbuf));
    return at_gen_resp_send(AT_GEN_ERR_NO_ERROR);
}


int at_tx_sim_sec_card_type_resp(int card_type)
{
    TRACE(MSGL_VGSM_SIM, "%%SCCT resp\n");
    char sndbuf[SEND_BUF_SIZE];
    memset(sndbuf, '\0', sizeof(sndbuf));

    sprintf(sndbuf, "%s%d%s", SCCT, card_type, CRLF);

    at_msg_send(ACK_SEQ_RESPONSE, sndbuf, strlen(sndbuf));
    return at_gen_resp_send(AT_GEN_ERR_NO_ERROR);
}

