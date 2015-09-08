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

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "vgsm_sim.h"
#include "state.h"
#include "server_common_security.h"
#include "db_security.h"
#include "server_tx_security.h"
#include "server_rx_security.h"
#include "oem_tx_security.h"

#include "misc.h"	// TAPIMessageInit()
#include "linuxtapi.h"
#include "vgsm_phone.h"
#include "phoneserver.h"
#include "at_recv.h"
#include "at_func.h"
#include "at_gen_resp.h"

int server_rx_sim_sec_get_imsi_req(void* ptr_data, int data_len)
{
    char pdata[16];

    //SIM_DEBUG("\n");
    strcpy(pdata, IMSI_NUM);

    return oem_tx_sim_sec_get_imsi_res( pdata, strlen(pdata));
}


int server_rx_sim_sec_set_pin_status_req(void* ptr_data, int data_len)
{
    int result = -1;
    int length_p;
    int new_length_p = 0;
    int current_status;
    int lock_type;
    char *password		=	0;
    char *new_password	=	0;

    //int cnt_pin2;
    //int cnt_puk2;
    //char *t_password;

    unsigned char *p = 0;
    LXT_MESSAGE packet;

    SIM_DEBUG("\n");

    TAPIMessageInit(&packet);

    if (ptr_data && data_len > 0)
    {
	packet.data = malloc(data_len+3 /*p[19~21]*/);
	if(!packet.data)
	    return -1;
	memcpy(packet.data, ptr_data, data_len);
	p = (unsigned char *)(packet.data);
    }
    else {
	SIM_DEBUG("server_rx_sim_sec_set_pin_status_req: invalid parameter, ptr_data\n");
	return -1;
    }


    packet.group	=	GSM_SIM;
    /*pin->pin_len + pin->puk_len + status + remain count;*/
    packet.length	=	1 + 1 + 1 + 8 + 8 + 1 + 1 + 1;

    /****************************************************
     *				CHV1 , SC verification					*
     *****************************************************/
    if(p[0] == GSM_SEC_LOCK_TYPE_SC)
    {
	packet.action = GSM_SIM_PIN_REQ_STATE;

	/* PUK CODE VERIFY CASE */
	if(p[2] > 0)	//puk  length exist. -> PUK case.
	{
	    lock_type			=	GSM_SIM_PUK_REQ;

	    length_p			=	p[2];	//puk
	    password			=	malloc(length_p+1);
	    if(password == NULL){
		free(packet.data);
		return -1;
	    }
	    memcpy(password,&p[11],length_p);
	    password[length_p]	=	'\0';

	    new_length_p		= p[1];	//new pin
	    new_password	= malloc(new_length_p+1);
	    if(new_password == NULL){
		free(packet.data);
		free(password);
		return -1;
	    }
	    memcpy(new_password,&p[3],new_length_p);
	    new_password[new_length_p]	=	'\0';
	}
	/* PIN CODE VERIFY CASE */
	else
	{
	    lock_type		=	GSM_SIM_PIN_REQ_STATE;
	    length_p		=	p[1];   //pin
	    password		=	malloc(length_p+1);
	    if(password == NULL){
		free(packet.data);
		return -1;
	    }
	    memcpy(password,&p[3],length_p);
	    password[length_p]	=	'\0';
	}

	current_status = server_sec_get_sec_status();

	/* PIN check enable case from here */
	if(current_status  == lock_type)
	{
	    result	=	server_sec_verify_password(password,lock_type,length_p);
	}
	else
	{
	    SIM_DEBUG("Not matched lock type with current_status!! current_status,lock_type=[%x],[%x]",current_status, lock_type);
	}

	switch(result)
	{
	    /* INCORRECT PASSWORD	*/
	    case SIM_FAIL:
		{
		    SIM_DEBUG("wrong password\n");

		    p[19]	=	server_sec_get_sec_status();

		    // decrease the remain count of relevant lock_type
		    server_sec_status_mgr(lock_type);

		    p[20]	=	server_sec_get_remain_cnt(p[19]);
		    p[21]	=	0;

		    /* incorrect pin input 3 times	*/
		    if(p[19] ==GSM_SIM_PIN_REQ_STATE && p[20] == 0)
		    {
			// TAPI_OP_GEN_ERR_SIM_PUK_REQU	12	0x000c
			oem_tx_sim_sec_set_pin_status_res(0x000c /*error code*/);
		    }
		    /* incorrect puk input 10 times	*/
		    else if(p[19] ==GSM_SIM_PUK_REQ && p[20] == 0)
		    {
			// TAPI_OP_GEN_ERR_SIM_PERM_BLOCKED		0x800b
			oem_tx_sim_sec_set_pin_status_res(0x800B /*error code*/);
		    }
		    else
		    {
			// TAPI_OP_GEN_ERR_INCORRECT_PW	16	0x0010
			oem_tx_sim_sec_set_pin_status_res(0x0010 /*error code*/);
		    }
		}
		break;

		/* CORRECT PASSWORD	*/
	    case SIM_SUCCESS:
		{
		    SIM_DEBUG("passwd OK\n");

		    p[19]	=	server_sec_get_sec_status();
		    p[20]	=	server_sec_get_remain_cnt(p[19]);
		    p[21]	=	1;

		    oem_tx_sim_sec_set_pin_status_res(0x8000 /*error code*/);

		    if(lock_type == GSM_SIM_PUK_REQ)
		    {
			server_sec_change_password(new_password,GSM_SIM_PIN_REQ_STATE,new_length_p);

			server_tx_sim_sec_pin_status_noti(STATE_SIM_PUK, STATE_SIM_PUK_OK);
		    }
		    else
		    {
			server_tx_sim_sec_pin_status_noti(STATE_SIM_PIN, STATE_SIM_PIN_OK);
		    }
		}
		break;

	    default:
		SIM_DEBUG("Unhandled Verify Result=[%x]\n", result);
		break;

	}

    }

    /****************************************************
     *				CHV2 , SC2 verification					*
     *****************************************************/
    else if(p[0] == GSM_SEC_LOCK_TYPE_SC2)
    {
	packet.action = GSM_SIM_PIN2_REQ;

	if(p[2] > 0) //puk 2
	{
	    lock_type		=	GSM_SIM_PUK2_REQ;
	    length_p		=	p[2];	//puk2
	    password		=	malloc(length_p+1);
	    if(password == NULL){
		free(packet.data);
		return -1;
	    }
	    memcpy(password,&p[11],length_p);
	    password[length_p] = '\0';

	    new_length_p		= p[1];	//new pin2
	    new_password	= malloc(new_length_p+1);
	    if(new_password == NULL){
		free(packet.data);
		free(password);
		return -1;
	    }
	    memcpy(new_password,&p[3],new_length_p);
	    new_password[new_length_p] = '\0';
	}
	else
	{
	    lock_type		=	GSM_SIM_PIN2_REQ;
	    length_p		=	p[1];   //pin2
	    password		=	malloc(length_p+1);
	    if(password == NULL){
		free(packet.data);
		return -1;
	    }
	    memcpy(password,&p[3],length_p);
	    password[length_p] = '\0';
	}

	// On first booting period, this sends the pin_status_noti(this is based on current_status) to pda.
	current_status	=	server_sec_get_sec2_status();

	/* PIN check enable case from here */
	if(current_status  == lock_type)
	{
	    result	=	server_sec_verify_password(password,lock_type,length_p);
	}
	else
	{
	    SIM_DEBUG("Not matched lock type with current_status!! current_status,lock_type=[%x],[%x]",current_status, lock_type);
	}

	switch(result)
	{
	    /* INCORRECT PASSWORD	*/
	    case SIM_FAIL:
		{
		    SIM_DEBUG("wrong password\n");

		    p[19]	=	server_sec_get_sec2_status();

		    // reduce retry count.
		    server_sec_status_mgr(lock_type);

		    p[20]	=	server_sec_get_remain_cnt(p[19]);
		    p[21]	=	0;

		    /* incorrect pin input 3 times	*/
		    if(p[19] ==GSM_SIM_PIN2_REQ && p[20] == 0)
		    {
			// TAPI_OP_GEN_ERR_SIM_PUK_REQU	12	0x000c
			oem_tx_sim_sec_set_pin_status_res(0x000c /*error code*/);
		    }
		    /* incorrect puk input 10 times	*/
		    else if(p[19] ==GSM_SIM_PUK2_REQ && p[20] == 0)
		    {
			// TAPI_OP_GEN_ERR_SIM_PERM_BLOCKED		0x800b
			oem_tx_sim_sec_set_pin_status_res(0x800B /*error code*/);
		    }
		    else
		    {
			// TAPI_OP_GEN_ERR_INCORRECT_PW	16	0x0010
			oem_tx_sim_sec_set_pin_status_res(0x0010 /*error code*/);
		    }
		}
		break;

		/* CORRECT PASSWORD	*/
	    case SIM_SUCCESS:
		{
		    SIM_DEBUG("correct password\n");

		    p[19]	=	server_sec_get_sec2_status();
		    p[20]	=	server_sec_get_remain_cnt(p[19]);
		    p[21]	=	1;

		    oem_tx_sim_sec_set_pin_status_res(0x8000 /*error code*/);

		    if(lock_type == GSM_SIM_PUK2_REQ)
		    {
			server_sec_change_password(new_password,GSM_SIM_PIN2_REQ,new_length_p);
			server_tx_sim_sec_pin_status_noti(STATE_SIM_PUK2, STATE_SIM_PUK2_OK);
		    }
		    else
		    {
			server_tx_sim_sec_pin_status_noti(STATE_SIM_PIN2, STATE_SIM_PIN2_OK);
		    }
		}
		break;

	    default:
		break;

	}

    }

    if(password)
	free(password);

    if(new_password)
	free(new_password);

    //FuncServer->Cast(&GlobalPS, LXT_ID_CLIENT_EVENT_INJECTOR, &packet);

    if(packet.data)
	free(packet.data);

    return 1;
}
/*
   do not use in XOcean.
 */
int server_rx_sim_sec_get_pin_status_req(void* ptr_data, int data_len)
{
    int ret = 0;
    int data_size = 2;
    unsigned char pdata[2];

    SIM_DEBUG("\n");

    switch( server_sec_get_sec_status())
    {
	case   GSM_SIM_PIN_REQ_STATE :
	    {
		pdata[0]	=	GSM_SEC_LOCK_TYPE_SC;
		pdata[1]	=	GSM_SEC_LOCK_KEY_PIN;
	    }
	    break;

	case  GSM_SIM_PIN2_REQ :
	    {
		pdata[0]	=	GSM_SEC_LOCK_TYPE_SC2;
		pdata[1]	=	GSM_SEC_LOCK_KEY_PIN2;
	    }
	    break;

	case  GSM_SIM_PUK_REQ :
	    {
		pdata[0]	=	GSM_SEC_LOCK_TYPE_SC;
		pdata[1]	=	GSM_SEC_LOCK_KEY_PUK;
	    }
	    break;

	case  GSM_SIM_PUK2_REQ :
	    {
		pdata[0]	=	GSM_SEC_LOCL_TYPE_PUK2;
		pdata[1]	=	GSM_SEC_LOCK_KEY_PUK2;
	    }
	    break;

	case  GSM_SIM_FRIZEN :
	    {
		pdata[0]	=	GSM_SEC_LOCK_TYPE_UNAVAIL;
		pdata[1]	=	GSM_SEC_LOCK_KEY_PERM_BLOCKED;
	    }
	    break;

	case  GSM_SIM_NO_SIM :
	    {
		pdata[0]	=	GSM_SEC_LOCK_TYPE_NO_SIM;
		pdata[1]	=	GSM_SEC_LOCK_KEY_UNLOCKED;
	    }
	    break;

	case  GSM_SIM_FDN_STATE :
	    {
		pdata[0]	=	GSM_SEC_LOCK_TYPE_FD;
		pdata[1]	=	GSM_SEC_LOCK_KEY_PIN2;
	    }
	    break;

	case GSM_SIM_READY:
	    {
		pdata[0]	=	GSM_SEC_LOCK_TYPE_READY;
		pdata[1]	=	GSM_SEC_LOCK_KEY_UNLOCKED;
	    }
	    break;

	default:
	    break;

    }

    ret	=	oem_tx_sim_sec_get_pin_status_res( pdata, data_size);

    return ret;
}


int server_rx_sim_sec_get_lock_info_req(void* ptr_data, int data_len)
{
    SIM_DEBUG("\n");

    int lock_type;
    unsigned char pdata[4];
    unsigned char *p = ptr_data;

    lock_type=	p[1];

    SIM_DEBUG("lock_type(3:SC, 9:SC2) =[%x]\n",lock_type);

    switch(lock_type)
    {
	case GSM_SEC_LOCK_TYPE_SC:
	    {
		switch( server_sec_get_sec_status())
		{
		    case   GSM_SIM_PIN_REQ_STATE :
			{
			    pdata[0]	=	1;
			    pdata[1]	=	GSM_SEC_LOCK_TYPE_SC;
			    pdata[2]	=	GSM_SEC_LOCK_KEY_PIN;
			    pdata[3]	=	server_sec_get_remain_cnt(GSM_SIM_PIN_REQ_STATE);
			}
			break;

		    case  GSM_SIM_PUK_REQ :
			{
			    pdata[0]	=	1;
			    pdata[1]	=	GSM_SEC_LOCK_TYPE_SC;
			    pdata[2]	=	GSM_SEC_LOCK_KEY_PUK;
			    pdata[3]	=	server_sec_get_remain_cnt(GSM_SIM_PUK_REQ);
			}
			break;

		    case  GSM_SIM_FRIZEN :
			{
			    pdata[0]	=	1;
			    pdata[1]	=	GSM_SEC_LOCK_TYPE_UNAVAIL;
			    pdata[2]	=	GSM_SEC_LOCK_KEY_PERM_BLOCKED;
			    pdata[3]	=	0;
			}
			break;

		    case  GSM_SIM_NO_SIM :
			{
			    pdata[0]	=	1;
			    pdata[1]	=	GSM_SEC_LOCK_TYPE_NO_SIM;
			    pdata[2]	=	GSM_SEC_LOCK_KEY_UNLOCKED;
			    pdata[3]	=	0;
			}
			break;

		    case GSM_SIM_READY:
			{
			    pdata[0]	=	1;
			    pdata[1]	=	GSM_SEC_SIM_INIT_COMPLETED;
			    pdata[2]	=	GSM_SEC_LOCK_KEY_UNLOCKED;
			    pdata[3]	=	0;
			}
			break;

		    default:
			SIM_DEBUG("Not handled security status =[%x]\n", server_sec_get_sec_status());
			break;
		}
	    }
	    break;

	case GSM_SEC_LOCK_TYPE_SC2 :
	    {
		switch( server_sec_get_sec2_status())
		{
		    case   GSM_SIM_PIN2_REQ :
			{
			    pdata[0]	=	1;
			    pdata[1]	=	GSM_SEC_LOCK_TYPE_SC2;
			    pdata[2]	=	GSM_SEC_LOCK_KEY_PIN2;
			    pdata[3]	=	server_sec_get_remain_cnt(GSM_SIM_PIN2_REQ);
			}
			break;

		    case  GSM_SIM_PUK2_REQ :
			{
			    pdata[0]	=	1;
			    pdata[1]	=	GSM_SEC_LOCK_TYPE_SC2;
			    pdata[2]	=	GSM_SEC_LOCK_KEY_PUK2;
			    pdata[3]	=	server_sec_get_remain_cnt(GSM_SIM_PUK2_REQ);
			}
			break;

		    case  GSM_SIM_FRIZEN :
			{
			    pdata[0]	=	1;
			    pdata[1]	=	GSM_SEC_LOCK_TYPE_UNAVAIL;
			    pdata[2]	=	GSM_SEC_LOCK_KEY_PERM_BLOCKED;
			    pdata[3]	=	0;
			}
			break;

		    case  GSM_SIM_NO_SIM :
			{
			    pdata[0]	=	1;
			    pdata[1]	=	GSM_SEC_LOCK_TYPE_NO_SIM;
			    pdata[2]	=	GSM_SEC_LOCK_KEY_UNLOCKED;
			    pdata[3]	=	0;
			}
			break;

		    case GSM_SIM_PIN2_OK:
			{
			    pdata[0]	=	1;
			    pdata[1]	=	GSM_SEC_LOCK_TYPE_SC2;
			    pdata[2]	=	GSM_SEC_LOCK_KEY_UNLOCKED;	// or GSM_SEC_LOCK_KEY_PIN2_DISABLE
			    pdata[3]	=	0;
			}
			break;

		    default:
			SIM_DEBUG("Not handled security status =[%x]\n", server_sec_get_sec2_status());
			break;
		}
	    }
	    break;

	default:
	    SIM_DEBUG("Unhandled Lock_type =[%x]\n",lock_type);
	    break;
    }

    return oem_tx_sim_sec_get_lock_info_res(pdata, sizeof pdata);
}

int server_rx_sim_sec_check_password_req(void* ptr_data, int data_len)
{
    int result = -1;
    int length_p;
    int lock_type;
    char *password=0;
    int recv_type = 0;
    char tmpmsg[SEND_BUF_SIZE];
    memset(tmpmsg, '\0', sizeof(tmpmsg));
    strcpy(tmpmsg, (char*)ptr_data);
    char* tdata = strchr(tmpmsg, '=');
    char token[] = "\"";
    char* ret = NULL;

    ret = strtok(tdata+1, token);
    if(ret)
    {
	length_p = strlen(ret);
	password = malloc(length_p+1);
	if(!password)
	    return -1;
	strcpy(password, ret);
	strcat(password, "\0");
    }
    else {
	SIM_DEBUG("server_rx_sim_sec_check_password_req: invalid password error\n");
	return -1;
    }

    recv_type = GSM_SEC_LOCK_TYPE_SC;
    lock_type = GSM_SIM_PIN2_REQ;

    result = server_sec_verify_password(password,lock_type,length_p);
    /* fail case    */
    if(result       ==      SIM_FAIL)
    {
	// reduce retry count.
	server_sec_status_mgr(lock_type);
	oem_tx_sim_sec_check_password_res(AT_CME_ERR_INCORRECT_PWD);
    }
    /* success case */
    else if(result  ==      SIM_SUCCESS)
    {
	oem_tx_sim_sec_check_password_res(AT_GEN_ERR_NO_ERROR);
    }

    if(password)
	free(password);

    return 1;
}

int server_rx_sim_sec_change_password_req(void* ptr_data, int data_len)
{
    int result = -1;
    int length_p = 0;
    int new_length_p = 0;
    int lock_type;
    char *password=0;
    char *new_password=0;
    const char* scmd;
    int nn = 0, found = 0;
    int recv_type = 0;
    char tmpmsg[SEND_BUF_SIZE];
    memset(tmpmsg, '\0', sizeof(tmpmsg));
    strcpy(tmpmsg, (char*)ptr_data);
    char* tdata = strchr(tmpmsg, '=');
    char token[] = "\"";
    char* ret = NULL;

    ret = strtok(tdata+1, token);
    if(ret)
    {
	length_p = strlen(ret);
	password = malloc(length_p+1);
	if(!password)
	    return -1;
	strcpy(password, ret);
	strcat(password, "\0");
    }
    else {
	SIM_DEBUG("server_rx_sim_sec_change_password_req: invalid password error\n");
	return -1;
    }

    ret = strtok(NULL, token);
    ret = strtok(NULL, token);
    if(ret)
    {
	new_length_p = strlen(ret);
	new_password = malloc(new_length_p+1);
	if(!new_password){
	    free(password);
	    return -1;
	}
	strcpy(new_password, ret);
	strcat(new_password, "\0");
    }
    else {
	SIM_DEBUG("server_rx_sim_sec_change_password_req: invalid new_password error\n");
	free(password);
	return -1;
    }

    ret = strtok(NULL, TOKEN);
    if(ret)
    {
	for (nn = 0; ; nn++) {
	    scmd = sDefaultLock[nn].code;
	    if (!scmd) // end of list
		break;

	    if ( !strcmp( scmd, ret ) ) {
		found = 1;
		break;
	    }
	}

	if(found)
	    recv_type = sDefaultLock[nn].lock_e_type;
	else
	    recv_type = GSM_SEC_LOCK_TYPE_UNAVAIL;
    }
    else
	recv_type = GSM_SEC_LOCK_TYPE_SC;

    SIM_DEBUG("%02x\n", recv_type);

    if(recv_type == GSM_SEC_LOCK_TYPE_SC)
    {
	lock_type = GSM_SIM_PIN_REQ_STATE;
    }
    else if(recv_type == GSM_SEC_LOCK_TYPE_SC2 /*add more...*/)
    {
	lock_type = GSM_SIM_PIN2_REQ;
    }
    else //  Note temp...!!!
    {
	SIM_DEBUG("no matching lock type. ERROR!!!\n");
	lock_type = GSM_SIM_PIN2_REQ;
    }

    result = server_sec_verify_password(password,lock_type,length_p);

    /* fail case	*/
    if(result	==	SIM_FAIL)
    {
	// reduce retry count.
	server_sec_status_mgr(lock_type);
	oem_tx_sim_sec_change_password_res(AT_CME_ERR_INCORRECT_PWD);
    }
    /* success case	*/
    else if(result	==	SIM_SUCCESS)
    {
	server_sec_change_password(new_password,lock_type,new_length_p);
	oem_tx_sim_sec_change_password_res(AT_GEN_ERR_NO_ERROR);
    }

    if(password)
	free(password);

    if(new_password)
	free(new_password);

    return 1;
}


int server_rx_sim_sec_set_phone_lock_req(void* ptr_data, int data_len)
{
    int length_p;
    int lock_type;
    char *password=0;
    int result = -1;
    int facility_type = 0;
    int mode;
    unsigned char *p = 0;
    LXT_MESSAGE packet;

    SIM_DEBUG("\n");

    TAPIMessageInit(&packet);

    if (ptr_data && data_len > 0)
    {
	packet.data = (unsigned char *) malloc(data_len);
	if(!packet.data)
	    return -1;
	memcpy(packet.data, ptr_data, data_len);
    }

    p = (unsigned char *)(packet.data);

    packet.group	= GSM_SIM;
    packet.length	= 1 + 1 +1+  8 +  1 + 1 ;/*lock_type + lock_mode +pw_len +  pw + result(true,false);*/
    packet.action	= GSM_SIM_SET_PHONELOCK_REQ;

    length_p = p[2]; //current len
    password = malloc(length_p+1);
    if(!password){
	free(packet.data);
	return -1;
    }
    memcpy(password,&p[3],length_p);
    password[length_p] = '\0';

    if(p[0] == GSM_SEC_LOCK_TYPE_SC)
    {
	lock_type		=	GSM_SIM_PIN_REQ_STATE;//pin
	facility_type	=	GSM_SIM_FACILITY_PIN;
	p[12]		=	0x00;
    }
    else if(p[0] == GSM_SEC_LOCK_TYPE_FD)
    {
	lock_type		=	GSM_SIM_PIN2_REQ; //fdn
	facility_type	=	GSM_SIM_FACILITY_FDN;
	p[12]		=	0x01;
    }
    else
	assert(0);

    result = server_sec_verify_password(password,lock_type,length_p);

    if(result	==	SIM_FAIL)
    {
	//gen err
	p[11]=0x00;

	// reduce retry count.
	server_sec_status_mgr(lock_type);

	oem_tx_sim_sec_set_phone_lock_res(0x0010);
    }
    else if(result	==	SIM_SUCCESS)
    {
	p[11]=0x01;
	mode = p[1];

	server_sec_set_facility(facility_type,mode);

	oem_tx_sim_sec_set_phone_lock_res(0x8000);
    }
    else
	assert(0);

    if(password)
	free(password);

    FuncServer->Cast(&GlobalPS, LXT_ID_CLIENT_EVENT_INJECTOR, &packet);

    if(packet.data)
	free(packet.data);

    return  1;
}
/*
   do not use in XOcean.
 */
int server_rx_sim_sec_get_phone_lock_req(void* ptr_data, int data_len)
{
    unsigned char pdata[2];
    unsigned char *p = ptr_data;

    SIM_DEBUG("\n");

    assert( data_len >= 2);

    switch( p[0])
    {
	case GSM_SEC_LOCK_TYPE_SC :
	    pdata[0]	=	GSM_SEC_LOCK_TYPE_SC;
	    pdata[1]	=	server_sec_get_facility(GSM_SIM_FACILITY_PIN) ;
	    break;

	case  GSM_SEC_LOCK_TYPE_FD :
	    pdata[0]	=	GSM_SEC_LOCK_TYPE_FD;
	    pdata[1]	=	server_sec_get_facility(GSM_SIM_FACILITY_FDN);
	    break;

	default:
	    assert(0);
	    break;
    }

    return oem_tx_sim_sec_get_phone_lock_res(pdata, sizeof pdata);
}

