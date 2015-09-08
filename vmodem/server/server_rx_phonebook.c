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
#include <stdlib.h>
#include <string.h>

#include "vgsm_sim.h"
#include "vgsm_pb.h"
#include "db_phonebook.h"
#include "oem_tx_phonebook.h"
#include "server_rx_phonebook.h"

#include "server_common_security.h"
#include "at_recv.h"

int server_rx_sim_pb_get_access_req(void* ptr_data, int data_len)
{
    int ret;
    int data_size ;
    unsigned char *p;
    int num_len;
    int name_len;
    int add_len;
    int n = 0;
    PB pb;

    SIM_DEBUG("\n");

    assert(ptr_data);
    assert(data_len > 0);

    memset(&pb, 0, sizeof(PB));

    db_sim_pb_get(&pb);
    db_sim_pb_restore(&pb);

    add_len		= strlen(pb.address);
    num_len		= strlen(pb.number);
    name_len	= strlen(pb.name);
    data_size	= 1+add_len+1+2+1+1+num_len+1+1+name_len;

    TRACE(MSGL_VGSM_INFO, "address = %s\n", pb.address);
    TRACE(MSGL_VGSM_INFO, "number  = %s\n", pb.number);
    TRACE(MSGL_VGSM_INFO, "name    = %s\n", pb.name);

    p = malloc(data_size);

    p[n++]	=	0x03;

    memcpy(&p[n], pb.address, add_len);
    n += add_len;

    p[n++]	=	0x01;
    p[n++]	=	0;
    p[n++]	=	0;
    p[n++]	=	num_len;
    p[n++]	=	0x00;

    memcpy(&p[n], pb.number, num_len);
    n += num_len;

    p[n++]	=	name_len;
    p[n++]	=	0x01;

    memcpy(&p[n], pb.name, name_len);
    n += name_len;

    ret	= oem_tx_sim_pb_get_access_res(p, n);

    free(p);

    return ret;
}


int server_rx_sim_pb_set_access_req(void* ptr_data, int data_len)
{
    PB *pb;
    unsigned char *p = 0;
    //unsigned short index_;
    int num_len,name_len,cnt,add_len;

    SIM_DEBUG("\n");

    if(ptr_data && data_len >0)
    {
	p=(unsigned char*)ptr_data;
	cnt=0;

	switch(p[0])
	{
	    case 0x01: //ADD   2G..
		{
		    cnt++;//mode
		    pb= malloc(sizeof(PB));
		    if(!pb)
			return -1;
		    memset(pb,0,(sizeof(PB)));
		    add_len=p[cnt];

		    //memcpy(&index_,&p[cnt] ,strlen());
		    //cnt+=2; //idex
		    cnt++; //3 // 3G or 2G

		    pb->db_st_type=0;
		    //pb->index=index_;
		    memcpy(pb->address,&p[cnt],add_len);
		    cnt+=add_len;

		    num_len=p[cnt];
		    cnt++;//num len
		    cnt++;//num type
		    memcpy(pb->number,&p[cnt],num_len);
		    cnt+=num_len;  //num
		    name_len=p[cnt];
		    cnt++;//name len
		    cnt++;//name enctype
		    memcpy(pb->name,&p[cnt],name_len);

		    SIM_DEBUG("\n rx_sim_pb_access <%d,%s,%s,%s>\n",pb->db_st_type,pb->name,pb->number,pb->address);

		    db_sim_pb_add(pb);
		    db_sim_pb_restore(pb);

		    //set_sim_sec_general_response_error(0x8000);

		    oem_tx_sim_pb_set_access_res(0x8000);

		    if(pb) free(pb);
		}
		break;

	    case 0x02://DELETE
		{
		    pb= malloc(sizeof(PB));
		    if(pb == NULL)
			return -1;
		    memset(pb,0,(sizeof(PB)));

		    add_len=p[cnt];

		    //memcpy(&index_,&p[1] ,sizeof(unsigned short));
		    //pb->index=index_;

		    db_sim_pb_remove(pb);
		    db_sim_pb_restore(pb);

		    //set_sim_sec_general_response_error(0x8000);

		    oem_tx_sim_pb_set_access_res(0x8000);

		    if(pb) free(pb);
		}
		break;

	    case 0x03://EDIT       NOT support...  EDIT = DELETE + ADD ...
		break;

	    default:
		break;

	}
    }
    return 1;
}

int server_rx_sim_pb_get_storage_cnt_req(void* ptr_data, int data_len)
{
    int setted_st = db_sim_pb_set_mgr(0,0);
    unsigned short used	= db_sim_pb_cnt_mgr(NULL,setted_st,0);
    unsigned short total = 255;
    unsigned char p[5];
    int n = 0;

    SIM_DEBUG("\n");

    assert(ptr_data);
    assert(data_len > 0);

    p[n++] = db_sim_pb_set_mgr(0,0);
    copy_unsigned_short(&p[n], total);
    n += 2;
    copy_unsigned_short(&p[n], used);
    n += 2;

    return oem_tx_sim_pb_get_storage_cnt_res(p, n);
}

int server_rx_sim_pb_get_entry_info_req(void* ptr_data, int data_len)
{
    unsigned short index_min = 0;
    unsigned short index_max = 255;
    unsigned short num_max = 255;
    unsigned short text_max = 255;
    unsigned char p[8];
    int n = 0;

    SIM_DEBUG("\n");

    assert(ptr_data);
    assert(data_len > 0);

    copy_unsigned_short(&p[n], index_min);
    n += 2;
    copy_unsigned_short(&p[n], index_max);
    n += 2;
    copy_unsigned_short(&p[n], num_max);
    n += 2;
    copy_unsigned_short(&p[n], text_max);
    n += 2;

    return oem_tx_sim_pb_get_entry_info_res(p, n);
}

int server_rx_sim_pb_get_storage_list_req(void* ptr_data, int data_len)
{
    unsigned char p[4];
    int n = 0;

    SIM_DEBUG("\n");

    assert(ptr_data);
    assert(data_len > 0);

    p[n++] = 0xff;
    p[n++] = 0xff;
    p[n++] = 0xff;
    p[n++] = 0xff;

    return oem_tx_sim_pb_get_storage_list_res(p, n);
}

int server_rx_sim_pb_update_record(char *api)
{
    at_unimplemented(api);
    return 1;
}

int server_rx_sim_pb_del_record(char *api)
{
    at_unimplemented(api);
    return 1;
}

int server_rx_sim_pb_get_capability_info(char *api)
{
    at_unimplemented(api);
    return 1;
}
