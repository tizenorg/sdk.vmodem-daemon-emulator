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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "misc.h"
#include "state.h"
#include "server_common_security.h"
#include "db_security.h"
#include "db_phonebook.h"

#include "phoneserver.h"

static char g_pin_value[9];
static char g_puk_value[9];
static char g_pin2_value[9];
static char g_puk2_value[9];
static int g_pin_cnt;
static int g_pin2_cnt;
static int g_puk_cnt;
static int g_puk2_cnt;
static int g_sim_state;
static int g_facility_pin;
static int g_facility_fdn;
static int g_lock_sc2_state;

//static int g_sim_sec_general_response_error = 0x8000;

int server_simdata_init()
{
	int ret_info =	0;
	int ret_data =	0;
	int i;

	SIM_DEBUG("\n");

	/* SIM SECURITY DATA	*/
	ret_info	=	db_sim_info_create();
	ret_data	=	db_sim_data_create();

	// return value '1' means that sim db already exists. so, we don`t need to put new record.
	if(ret_info != 1)
	{
		for(i=0;i<7;i++)
			//090602
			db_sim_info_add(i,NULL);
	}
	if(ret_data != 1)
	{
		for(i=0;i<7;i++)
			//090602
			db_sim_data_add(i,NULL);
	}

//	db_sim_info_restore(VGSM_SIMTypeLoad);
//	db_sim_data_restore(VGSM_SIMTypeLoad);

	return 1;
}

//init
int server_sim_db_init()
{
	int ret =	0;

	SIM_DEBUG("\n");

	/* SIM SECURITY DATA	*/
	ret	=	db_sim_sec_create();

	// return value '1' means that sim db already exists. so, we don`t need to put new record.
	if(ret != 1)
	{
		db_sim_sec_add();
	}

	db_sim_sec_restore(VGSM_SIMTypeLoad);

	/* SIM PHONEBOOK DATA	*/
	db_sim_pb_create();

	return 1;
}


//080226 -for mem free in mem alloc func. void -> int.
int server_sec_set_sec_db_info(SimSecurity *sim_sec,int  ncol)
{
	SIM_DEBUG("server_sec_set_sec_db_info\n");
	if(sim_sec == NULL)
		return -1;
//init
	memset(g_pin_value,0,9);
	memset(g_puk_value,0,9);
	memset(g_pin2_value,0,9);
	memset(g_puk2_value,0,9);

	g_pin_cnt		=	0;
	g_pin2_cnt		=	0;
	g_puk_cnt		=	0;
	g_puk2_cnt		=	0;
	g_sim_state		=	0;
	g_facility_pin	=	0;
	g_facility_fdn	=	0;
	g_lock_sc2_state	=	0;

//load
	memcpy(g_pin_value,sim_sec->pin_value, strlen(sim_sec->pin_value));
	memcpy(g_puk_value,sim_sec->puk_value, strlen(sim_sec->puk_value));
	memcpy(g_pin2_value,sim_sec->pin2_value,strlen(sim_sec->puk_value));
	memcpy(g_puk2_value,sim_sec->puk2_value, strlen(sim_sec->puk2_value));

	g_pin_cnt		=sim_sec->pin_cnt;
	g_pin2_cnt		=sim_sec->pin2_cnt;
	g_puk_cnt		=sim_sec->puk_cnt;
	g_puk2_cnt		=sim_sec->puk2_cnt;
	g_sim_state		=_trans_db_sim_state_type(sim_sec->sim_state);
	g_facility_pin	=sim_sec->facility_pin;
	g_facility_fdn	=sim_sec->facility_fdn;
	g_lock_sc2_state	= _trans_db_sim_state_type(sim_sec->lock_sc2_state);


	if(sim_sec)
		free(sim_sec);

	return 1;

}


int _trans_db_sim_state_type(int type)
{
	SIM_DEBUG("\n");

	switch(type)
	{
		case SIM_STATE_READY:
			return GSM_SIM_READY;
		case SIM_STATE_PIN_REQ:
			return GSM_SIM_PIN_REQ_STATE ;
		case SIM_STATE_PUK_REQ:
			return GSM_SIM_PUK_REQ;
		case SIM_STATE_PIN2_REQ:
			return GSM_SIM_PIN2_REQ;
		case SIM_STATE_PUK2_REQ:
			return GSM_SIM_PUK2_REQ;
		case SIM_STATE_FRIZEN:
			return GSM_SIM_FRIZEN;
		case SIM_STATE_NO_SIM:
			return GSM_SIM_NO_SIM;
		case SIM_STATE_FDN_STATE:
			return GSM_SIM_FDN_STATE;
		case SIM_STATE_PIN2_OK:
			return GSM_SIM_PIN2_OK;

		case GSM_SIM_READY:
			return SIM_STATE_READY;
		case GSM_SIM_PIN_REQ_STATE:
			return SIM_STATE_PIN_REQ ;
		case GSM_SIM_PUK_REQ:
			return SIM_STATE_PUK_REQ;
		case GSM_SIM_PIN2_REQ:
			return SIM_STATE_PIN2_REQ;
		case GSM_SIM_PUK2_REQ:
			return SIM_STATE_PUK2_REQ;
		case GSM_SIM_FRIZEN:
			return SIM_STATE_FRIZEN;
		case GSM_SIM_NO_SIM:
			return SIM_STATE_NO_SIM;
		case GSM_SIM_FDN_STATE:
			return SIM_STATE_FDN_STATE;
		case GSM_SIM_PIN2_OK:
			return SIM_STATE_PIN2_OK;

		default:
		break;
	}
	return 1;
}


/*
	flag value ->
	GSM_SEC_LOCK_MODE_UNLOCK : 0x00 : Disable
	GSM_SEC_LOCK_MODE_LOCK	: 0x01 : Enable
*/
void server_sec_set_facility(int type,int flag)
{
	SIM_DEBUG("\n");

	if(GSM_SIM_FACILITY_PIN == type )
	{
		g_facility_pin = flag;
		db_sim_sec_edit(DB_F_PIN,NULL,g_facility_pin);
	}

	if(GSM_SIM_FACILITY_FDN == type )
	{
		g_facility_fdn = flag;
		db_sim_sec_edit(DB_F_FDN,NULL,g_facility_fdn);
	}
}

int server_sec_get_facility(int type)
{
	SIM_DEBUG("\n");

	if(GSM_SIM_FACILITY_PIN == type )
		return g_facility_pin;
	if(GSM_SIM_FACILITY_FDN == type )
		return g_facility_fdn;

	return 1;
}


/*
	do not use now.
*/
char * server_sec_get_password(int type)
{
	SIM_DEBUG("\n");

	switch(type)
	{
		case GSM_SIM_PIN_REQ_STATE:
			SIM_DEBUG("g_pin_value = [%s]  \n",g_pin_value);
			return  g_pin_value;

		case GSM_SIM_PIN2_REQ:
			return  g_pin2_value;

		case GSM_SIM_PUK_REQ:
			return  g_puk_value;

		case GSM_SIM_PUK2_REQ:
			return g_puk2_value;

		default:
			break;
      	}

	return 0;
}


// this func change sim_db - pin1 or pin2 password col.
int server_sec_change_password(char *password,int type,int length)
{
	int ret	=	0;

	SIM_DEBUG("\n");

	LXT_MESSAGE packet;
	TAPIMessageInit(&packet);

	packet.group	=	GSM_SIM;
	packet.action	=	GSM_SIM_CHANGE_PASS_SET;
	packet.length	=	0;

	switch(type)
	{
		case GSM_SIM_PIN_REQ_STATE:
		{
			memset(g_pin_value,0,8);
			memcpy(g_pin_value,password,length);

			ret	=	db_sim_sec_edit(DB_PIN ,g_pin_value,0);

			if(ret == SIM_SUCCESS)
			{
				SIM_DEBUG("change sim_db pin1 password success\n");
			      	FuncServer->Cast( &GlobalPS, LXT_ID_CLIENT_EVENT_INJECTOR, &packet );
			}
			else
			{
				SIM_DEBUG("FAIL to updata information at DB\n");
				return 0;
			}

			return 1;
		}

		case GSM_SIM_PIN2_REQ:
		{
			memset(g_pin2_value,0,8);
			memcpy(g_pin2_value,password,length);

			ret	=	db_sim_sec_edit(DB_PIN2 ,g_pin2_value,0);

			if(ret == SIM_SUCCESS)
			{
				SIM_DEBUG("change sim_db pin2 password success\n");
			      	FuncServer->Cast( &GlobalPS, LXT_ID_CLIENT_EVENT_INJECTOR, &packet );
			}
			else
			{
				SIM_DEBUG("FAIL to updata information at DB\n");
				return 0;
			}

			return 1;
		}


		case GSM_SIM_PUKSET_REQ:
		{
			memset(g_puk_value,0,8);
			memcpy(g_puk_value,password,length);

			ret	=	db_sim_sec_edit(DB_PUK ,g_puk_value,0);

			if(ret == SIM_SUCCESS)
			{
				SIM_DEBUG("change sim_db pin1 password success\n");
			      	FuncServer->Cast( &GlobalPS, LXT_ID_CLIENT_EVENT_INJECTOR, &packet );
			}
			else
			{
				SIM_DEBUG("FAIL to updata information at DB\n");
				return 0;
			}


			return 1;
		}

		case GSM_SIM_PUK2SET_REQ:
		{
			memset(g_puk2_value,0,8);
			memcpy(g_puk2_value,password,length);

			ret	=	db_sim_sec_edit(DB_PUK2 ,g_puk2_value,0);

			if(ret == SIM_SUCCESS)
			{
				SIM_DEBUG("change sim_db pin1 password success\n");
			      	FuncServer->Cast( &GlobalPS, LXT_ID_CLIENT_EVENT_INJECTOR, &packet );
			}
			else
			{
				SIM_DEBUG("FAIL to updata information at DB\n");
				return 0;
			}


			return 1;
		}

		default:
			SIM_DEBUG("ERROR-Not handled type=[%x]",type);
			break;
	}

	return 0;
}


int server_sec_verify_password(char *password,int type,int length)
{
	SIM_DEBUG("\n");

	switch(type)
	{
		case GSM_SIM_PIN_REQ_STATE:
		{
			length = strlen(g_pin_value);

			SIM_DEBUG("g_pin_value =[%s]\n",g_pin_value);
                        SIM_DEBUG("input password value=[%s]\n",password);

			if(!memcmp(g_pin_value,password,length))  /* correct case */
			{
				 g_pin_cnt	=	3;
				 db_sim_sec_edit(DB_PIN_CNT,NULL,g_pin_cnt);
				 return SIM_SUCCESS;
			}
			else
			{
				return SIM_FAIL;
			}
		}
			
		case GSM_SIM_PIN2_REQ:
		{
			length = strlen(g_pin2_value);

			SIM_DEBUG("g_pin2_value =[%s]\n",g_pin2_value);
			SIM_DEBUG("input password value=[%s]\n",password);

			if(!memcmp(g_pin2_value,password,length))
			{
				g_pin2_cnt	=	3;
				db_sim_sec_edit(DB_PIN2_CNT,NULL,g_pin2_cnt);
				return SIM_SUCCESS;
			}
			else
			{
				return SIM_FAIL;
			}
		}

		case GSM_SIM_PUK_REQ:
		{
			length = strlen(g_puk_value);

			SIM_DEBUG("g_puk_value=[%s]\n",g_puk_value);
			SIM_DEBUG("input password=[%s]\n",password);

			if(!memcmp(g_puk_value,password,length))
			{
				 g_pin_cnt	=	3;
				 g_puk_cnt	=	10;

				db_sim_sec_edit(DB_PIN_CNT, NULL,g_pin_cnt);
				db_sim_sec_edit(DB_PUK_CNT, NULL,g_puk_cnt);

				return SIM_SUCCESS;
			}
			else
			{
				return SIM_FAIL;
			}
		}

		case GSM_SIM_PUK2_REQ:
		{
			length = strlen(g_puk2_value);

			if(!memcmp(g_puk2_value,password,length))
			{
				g_pin2_cnt	=	3;
				g_puk2_cnt	=	10;

				db_sim_sec_edit(DB_PIN2_CNT,NULL,g_pin2_cnt);
				db_sim_sec_edit(DB_PUK2_CNT, NULL,g_puk2_cnt);

				return SIM_SUCCESS;
			}
			else
			{
				return SIM_FAIL;
			}
		}

		default:
			break;

	}
	return SIM_FAIL;
}


int server_sec_status_mgr(int type)
{
	SIM_DEBUG("\n");

	switch(type)
	{
		case GSM_SIM_PIN_REQ_STATE:
		{
			g_pin_cnt--;
			db_sim_sec_edit(DB_PIN_CNT, NULL,g_pin_cnt);

			if(g_pin_cnt == 0 )
			{
				server_sec_set_sec_status(GSM_SIM_PUK_REQ);
			}
		}
		break;

		case GSM_SIM_PIN2_REQ:
		{
			g_pin2_cnt--;
			db_sim_sec_edit(DB_PIN2_CNT, NULL,g_pin2_cnt);

			if(g_pin2_cnt == 0 )
			{
				server_sec_set_sec2_status(GSM_SIM_PUK2_REQ);
			}
		}
		break;

		case GSM_SIM_PUK_REQ:
		{
			g_puk_cnt--;
			db_sim_sec_edit(DB_PUK_CNT, NULL,g_puk_cnt);

			if(g_puk_cnt == 0 )
			{
				server_sec_set_sec_status(GSM_SIM_FRIZEN);
			}
		}
		break;

		case GSM_SIM_PUK2_REQ:
		{
			g_puk2_cnt--;
			db_sim_sec_edit(DB_PUK2_CNT, NULL,g_puk2_cnt);

			if(g_puk2_cnt == 0 )
			{
				server_sec_set_sec_status(GSM_SIM_FRIZEN);
			}
		}
		break;

		default:
			SIM_DEBUG("No Matched lock type=[%x]\n",type);
			break;

	}

	return 1;
}


int server_sec_set_sec_status(int status)
{
	int ret	=	0;
	LXT_MESSAGE packet;
	TAPIMessageInit(&packet);

	packet.group	=	GSM_SIM;
	packet.action	=	GSM_SIM_STATUS_SET;
	packet.length	=	0;
	SIM_DEBUG("\n");

	SIM_DEBUG("status=[%x]\n",status);

	g_sim_state = status;

	status=_trans_db_sim_state_type(g_sim_state);

	SIM_DEBUG("after trans status is %x\n",status);

	ret	=	db_sim_sec_edit(DB_SIM_STATE, NULL,status);

	if(ret == SIM_SUCCESS)
	{
		SIM_DEBUG("change sim_db pin1 status success\n");
	      	FuncServer->Cast( &GlobalPS, LXT_ID_CLIENT_EVENT_INJECTOR, &packet );
	}

	return g_sim_state;
}


int server_sec_get_sec_status()
{
	SIM_DEBUG("\n");

	return g_sim_state ;
}


int server_sec_set_sec2_status(int status)
{
	int ret	=	0;

	SIM_DEBUG("\n");

	LXT_MESSAGE packet;
	TAPIMessageInit(&packet);

	packet.group	=	GSM_SIM;
	packet.action	=	GSM_SIM_STATUS_SET;
	packet.length	=	0;

	SIM_DEBUG("status=[%x]\n",status);

	g_lock_sc2_state = status;

	status=_trans_db_sim_state_type(g_lock_sc2_state);

	ret	=	db_sim_sec_edit(DB_LOCK_SC2_STATE, NULL,status);

	if(ret == SIM_SUCCESS)
	{
		SIM_DEBUG("change sim_db pin2 status success\n");
	      	FuncServer->Cast( &GlobalPS, LXT_ID_CLIENT_EVENT_INJECTOR, &packet );
	}

	return g_lock_sc2_state;
}


int  server_sec_get_sec2_status()
{
	SIM_DEBUG("\n");

	return g_lock_sc2_state ;
}


void server_sec_reset_remain_cnt(int type)
{
	SIM_DEBUG("\n");

	switch(type)
	{
		case GSM_SIM_PIN_REQ_STATE:
		{
			 g_pin_cnt = 3;
			 db_sim_sec_edit(DB_PIN_CNT, NULL,g_pin_cnt);
		}
		break;

		case GSM_SIM_PIN2_REQ:
		{
			g_pin2_cnt = 3;
			db_sim_sec_edit(DB_PIN2_CNT, NULL,g_pin2_cnt);
		}
		break;

		case GSM_SIM_PUK_REQ:
		{
			g_puk_cnt = 10;
			db_sim_sec_edit(DB_PUK_CNT, NULL,g_puk_cnt);
		}
		break;

		case GSM_SIM_PUK2_REQ:
		{
			g_puk2_cnt = 10;
			db_sim_sec_edit(DB_PUK2_CNT, NULL,g_puk2_cnt);
		}
		break;

		case GSM_SIM_READY:
		{
			g_pin_cnt = 3;
			g_puk_cnt = 10;
			db_sim_sec_edit(DB_PIN_CNT, NULL,g_pin_cnt);
			db_sim_sec_edit(DB_PUK_CNT, NULL,g_puk_cnt);
		}
		break;

		case GSM_SIM_PIN2_OK:
		{
			g_pin2_cnt = 3;
			g_puk2_cnt = 10;
			db_sim_sec_edit(DB_PIN2_CNT, NULL,g_pin2_cnt);
			db_sim_sec_edit(DB_PUK2_CNT, NULL,g_puk2_cnt);
		}
		break;

		default:
			SIM_DEBUG("Unhandled type=[%x]\n", type);
			break;

	}

}


void server_sec_set_cnt_zero(int type)
{
	SIM_DEBUG("\n");

	switch(type)
	{
		case GSM_SIM_PIN_REQ_STATE:
		{
			 g_pin_cnt = 0;
			 db_sim_sec_edit(DB_PIN_CNT, NULL,g_pin_cnt);
		}
		break;

		case GSM_SIM_PIN2_REQ:
			g_pin2_cnt = 0;
			db_sim_sec_edit(DB_PIN2_CNT, NULL,g_pin2_cnt);
		break;

		case GSM_SIM_PUK_REQ:
			g_puk_cnt = 0;
			db_sim_sec_edit(DB_PUK_CNT, NULL,g_puk_cnt);
		break;

		case GSM_SIM_PUK2_REQ:
			g_puk2_cnt = 0;
			db_sim_sec_edit(DB_PUK2_CNT, NULL,g_puk2_cnt);
		break;

		default:
			SIM_DEBUG("Unhandled type=[%x]\n", type);
			break;
	}
}


int server_sec_get_remain_cnt(int type)
{
	SIM_DEBUG("\n");

	switch(type)
	{
		case GSM_SIM_PIN_REQ_STATE:
			return	 g_pin_cnt;
		case GSM_SIM_PIN2_REQ:
			return	 g_pin2_cnt;
		case GSM_SIM_PUK_REQ:
			return	 g_puk_cnt;
		case GSM_SIM_PUK2_REQ:
			return	 g_puk2_cnt;
		default:
			SIM_DEBUG("Unhandled type=[%x]\n", type);
			break;
	}
	return 0;
}

