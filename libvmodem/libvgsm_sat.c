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

/***************************************************************************************
 *
 * Copyright (c) 2008 SAMSUNG Co. Ltd,
 * All rights reserved.
 *
 * File: vgsm_sat.c
 *
 * Release:
 *
 * Description: libvgsm interface.
 *
 * Revision History
 *
****************************************************************************************/

#include <string.h>
#include <stdlib.h>

#include "lxtutil.h"
#include "vgsm.h"
#include "../vmodem/include/sat/sat_values.h"


int vgsm_proactive_cmd_send(LXT_HANDLE * handle,int command)
{
	printf("\[LIBVGSM-SIM] vgsm_proactive_cmd_send\n\n");

	if( handle == NULL )
		return -1;

	return lxt_msg_send_message(	handle->fd,
									GSM_SAT,
									GSM_SAT_PROACTIVE_COMMAND_SEND,
									1,
									(void *)command);
}


int vgsm_sat_display_text(LXT_HANDLE * handle,char priority,char  clear_msg,char* textstring,char coding_scheme,char icon_identifier_flag,char icon_qualifier,char* icon_identifier,char immediate_response,char duration,char duration_unit )
{
	printf("\n vgsm_sat_display_text\n");

	if( handle == NULL )
		return -1;

	DP_TEXT *dp_text;
	int ret;

	dp_text = malloc(sizeof(DP_TEXT));
	if(!dp_text)
		return -1;
	memset(dp_text,0,sizeof(DP_TEXT));

	dp_text->priority				=	priority;
	dp_text->clear_msg			=	clear_msg;
	memcpy(dp_text->text_string,textstring,strlen(textstring));
	dp_text->coding_scheme		=	coding_scheme;
	dp_text->icon_identifier_flag		=	icon_identifier_flag;
	dp_text->icon_qualifier			=	icon_qualifier;
	dp_text->icon_identifier			=	0;
	dp_text->immediate_response	=	immediate_response;
	dp_text->duration				=	duration;
	dp_text->duration_unit			=	duration_unit;

	//printf("IN libvgsm - text is %s and  netxt test is %s",textstring,dp_text->text_string);

	ret = lxt_msg_send_message
		(
			handle->fd,
			GSM_SAT,
			GSM_SAT_PROACTIVE_DISPLAY_TEXT,
			sizeof(DP_TEXT),
			dp_text
		);

	if(dp_text)
		free(dp_text);

	return ret;
}


int vgsm_sat_get_inkey(LXT_HANDLE * handle,char cmd_detail,char* textstring,char coding_scheme,char icon_identifier_flag,char icon_qualifier,char* icon_identifier,char immediate_response,char duration,char duration_unit )
{
	printf("\n vgsm_sat_get_inkey\n");

	if( handle == NULL )
		return -1;

	GET_INKEY *get_inkey;
	int ret;
	
	get_inkey = malloc(sizeof(GET_INKEY));
	if(!get_inkey)
		return -1;
	memset(get_inkey,0,sizeof(GET_INKEY));

	get_inkey->cmd_detail			=	cmd_detail;
	memcpy(get_inkey->text_string,textstring,strlen(textstring));
	get_inkey->coding_scheme		=	coding_scheme;
	get_inkey->icon_identifier_flag	=	icon_identifier_flag;
	get_inkey->icon_qualifier		=	icon_qualifier;
	get_inkey->icon_identifier		=	0;
	get_inkey->immediate_response	=	immediate_response;
	get_inkey->duration			=	duration;
	get_inkey->duration_unit		=	duration_unit;

	ret = lxt_msg_send_message
		(
			handle->fd,
			GSM_SAT,
			GSM_SAT_PROACTIVE_GET_INKEY,
			sizeof(GET_INKEY),
			get_inkey
		);

	if(get_inkey)
		free(get_inkey);

	return ret;
}


int vgsm_sat_get_input(LXT_HANDLE * handle,char cmd_detail,char* textstring,char coding_scheme,char icon_identifier_flag,char icon_qualifier,char* icon_identifier,char immediate_response,char duration,char duration_unit )
{
	printf("\n vgsm_sat_get_input\n");

	if( handle == NULL )
		return -1;

	GET_INPUT *get_input;
	int ret;
	get_input = malloc(sizeof(GET_INPUT));
	if(!get_input)
		return -1;
	memset(get_input,0,sizeof(GET_INPUT));

	get_input->cmd_detail			=	cmd_detail;
	memcpy(get_input->text_string,textstring,strlen(textstring));
	get_input->coding_scheme		=	coding_scheme;
	get_input->icon_identifier_flag	=	icon_identifier_flag;
	get_input->icon_qualifier		=	icon_qualifier;
	get_input->icon_identifier		=	0;
	get_input->immediate_response	=	immediate_response;
	get_input->duration			=	duration;
	get_input->duration_unit		=	duration_unit;

	ret = lxt_msg_send_message
		(
			handle->fd,
			GSM_SAT,
			GSM_SAT_PROACTIVE_GET_INPUT,
			sizeof(GET_INPUT),
			get_input
		);

	if(get_input)
		free(get_input);

	return ret;
}


int vgsm_sat_set_up_menu(LXT_HANDLE * handle,unsigned char cmd_detail,int num,void * set_up_item )
{
	printf("\n vgsm_sat_set_up_menu \n");

	if( handle == NULL )
		return -1;

	unsigned char *data;
//	int i=0;
	int ret=0;
	data = malloc(num * sizeof(SET_MENU) + 2);
	if(!data)
		return -1;
	memset(data,0,num *sizeof(SET_MENU) + 2);

	data[0] =(unsigned char)num;
	data[1] = cmd_detail;
       memcpy(&data[2],(unsigned char *)set_up_item,num *sizeof(SET_MENU));
	printf("data num is <%x>\n",data[0]);
	printf("data cmd_detail is <%x>\n",data[1]);

	ret = lxt_msg_send_message
		(
			handle->fd,
			GSM_SAT,
			GSM_SAT_PROACTIVE_SET_MENU,
			(num * sizeof(SET_MENU) + 2),
			data
		);

	if(data)
		free(data);

	return ret;
}


int vgsm_sat_select_item(LXT_HANDLE * handle, unsigned char cmd_detail, int num, void * set_up_item )
{
	printf("\n vgsm_sat_select_item \n");

	if( handle == NULL )
		return -1;

	unsigned char *data;
//	int i=0;
	int ret=0;
	data = malloc(num * sizeof(SET_MENU) + 2);
	if(!data)
		return -1;
	memset(data,0,num *sizeof(SET_MENU) + 2);

	data[0] =(unsigned char)num;
	data[1] = cmd_detail;
	memcpy(&data[2],(unsigned char *)set_up_item,num *sizeof(SET_MENU));

	printf("data num is <%x>\n",data[0]);
	printf("data cmd_detail is <%x>\n",data[1]);

	ret = lxt_msg_send_message
		(
			handle->fd,
			GSM_SAT,
			GSM_SAT_PROACTIVE_SELECT_ITEM,
			(num * sizeof(SET_MENU) + 2),
			data
		);

	if(data)
		free(data);

	return ret;
}


int vgsm_sat_send_sms(LXT_HANDLE * handle, unsigned char cmd_detail, unsigned char address, char *number, unsigned char* sms_tpdu   )
{
	printf("\n vgsm_sat_send_sms \n");

	if( handle == NULL )
		return -1;

	//unsigned char *data;
	SEND_SMS *send_sms_packet;
	int ret=0;
	send_sms_packet = malloc(sizeof(SEND_SMS));
	if(!send_sms_packet)
		return -1;
	memset(send_sms_packet,0,sizeof(SEND_SMS));

	send_sms_packet->cmd_detail=cmd_detail;
	send_sms_packet->address=address ;
	memcpy(send_sms_packet->dial_num,number,strlen(number));
	//080117 - remove compile warning msg due to sms_tpdu.
	memcpy(send_sms_packet->tpdu_sms,sms_tpdu,strlen((char*)sms_tpdu));

	printf("\n SMS DIAL NUM is %s",send_sms_packet->dial_num);

	ret = lxt_msg_send_message
       	(
			handle->fd,
			GSM_SAT,
			GSM_SAT_PROACTIVE_SEND_SMS,
			sizeof(SEND_SMS),
			send_sms_packet
		);

	if(send_sms_packet)
		free(send_sms_packet);

	return ret;
}


 int vgsm_sat_set_up_call(LXT_HANDLE * handle,unsigned char cmd_detail,char* dial_number,char *alpha_user,/*char *alpha_call ,*/unsigned char address,unsigned char options,char duration_unit)
{
	printf("\n vgsm_sat_send_sms \n");

	if( handle == NULL )
		return -1;

	unsigned char *data = NULL;
	unsigned char *pdata = NULL;
	char dial_len;
	char alpha_user_len;
//	char alpha_call_len;
	char size = 0;
	int ret=0;

	switch(options)
	{
		case 0x00 : // not options
			dial_len = strlen(dial_number);
			size = 1+1+1+dial_len+1;
			data = malloc(size); // option(1) +cmd_detail(1)+dial len(1)+dial size+ address(1)
			memset(data,0,size);

			pdata = data;
			*pdata++ = options;
			*pdata++ =cmd_detail ;
			*pdata++ = dial_len;
			memcpy(pdata,dial_number,dial_len);
			pdata += dial_len;
			*pdata++ = address;

			break;

		case 0x01: // alpha_
			dial_len = strlen(dial_number);
			alpha_user_len = strlen(alpha_user);
			//alpha_call_len = strlen(alpha_call);
			size = 1+1+1+dial_len+1+alpha_user_len+1+/*alpha_call_len+*/1;
			data = malloc(size); //option(1)+ cmd_detail(1)+dial len(1)+dial size+alpha_user len(1)alpha_user size++alpha_call len(1)+alpha_call size+ address(1)
			memset(data,0,size);

			pdata = data;
			*pdata++ = options;
			*pdata++ =cmd_detail ;

			*pdata++ = dial_len;
			memcpy(pdata,dial_number,dial_len);
			pdata += dial_len;

			*pdata++ = alpha_user_len;
			memcpy(pdata,alpha_user,alpha_user_len);
			pdata += alpha_user_len;

/*
			*pdata++ = alpha_call_len;
			memcpy(pdata,alpha_call,alpha_call_len);
			pdata += alpha_call_len;
*/
			*pdata++ = address;


			break;
		case 0x02: // DURATION
			dial_len = strlen(dial_number);
			dial_len = strlen(dial_number);
			size = 1+1+1+dial_len+1+1;
			data = malloc(size); //option(1)+ cmd_detail(1)+dial len(1)+dial size+ address(1)+unit(1)
			memset(data,0,size);

			pdata = data;
			*pdata++ = options;
			*pdata++ =cmd_detail ;
			*pdata++ = dial_len;
			memcpy(pdata,dial_number,dial_len);
			pdata += dial_len;
			*pdata++ = address;
			*pdata++ = duration_unit;


			break;
		case 0x03: //  alpha_ & DURATION
			dial_len = strlen(dial_number);
			alpha_user_len = strlen(alpha_user);
			//alpha_call_len = strlen(alpha_call);
			size = 1+1+1+dial_len+1+alpha_user_len+1+/*alpha_call_len+*/1+1;
			data = malloc(size); // option(1)+cmd_detail(1)+dial len(1)+dial size+alpha_user len(1)alpha_user size++alpha_call len(1)+alpha_call size+ address(1)+unit(1)
			memset(data,0,size);

			pdata = data;
			*pdata++ = options;
			*pdata++ =cmd_detail ;

			*pdata++ = dial_len;
			memcpy(pdata,dial_number,dial_len);
			pdata += dial_len;

			*pdata++ = alpha_user_len;
			memcpy(pdata,alpha_user,alpha_user_len);
			pdata += alpha_user_len;
/*
			*pdata++ = alpha_call_len;
			memcpy(pdata,alpha_call,alpha_call_len);
			pdata += alpha_call_len;
*/
			*pdata++ = address;
			*pdata++ = duration_unit;

			printf("\n @- dial_number is %s",dial_number);
			printf("\n @- alpha_user is %s",alpha_user);
		//	printf("\n @- alpha_call is %s",alpha_call);

			break;

	}

	printf("\n vgsm_sat_set_up_call \n");

	ret = lxt_msg_send_message
		(
			handle->fd,
			GSM_SAT,
			GSM_SAT_PROACTIVE_SET_UP_CALL,
			size,
			data
		);

	if(data)
		free(data);

	return ret;
}

