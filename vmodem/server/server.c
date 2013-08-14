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

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>


#include "phoneserver.h"
#include "eloop.h"
//#include "data_conn_mgr.h"
#include "misc.h"
#include "state.h" //define ENTER()
#include "logmsg.h"

#define MAX_LISTEN_COUNT       32
//#define DEF_DOMAIN_SOCKET_NAME "/tmp/.phone_socket"
#define DEF_DOMAIN_SOCKET_NAME "/tmp/.vgsm_socket"

int vgsm_server_port=0;


extern GSM_StateMachine           GlobalS;

static void server_save_message(PhoneServer * ps, LXT_ID_CLIENT klass, LXT_MESSAGE * packet)
{
    int ii;

    Server * server = &ps->ServerSocket;
    if  (server->mmsg.current >= MAX_MISSED_MESSAGE)
    {
		TRACE(MSGL_WARN, "Warning!!! overrun max missed call\n");
        server->mmsg.current = 0;
    }

    // save currunt
    ii = server->mmsg.current;
	TRACE(MSGL_VGSM_INFO, "Add current missed cast index: %d (client id: 0x%x)\n", ii, klass);
	TRACE(MSGL_VGSM_INFO, "packet->group: 0x%x packet->action: 0x%x\n", packet->group, packet->action);

#ifdef _DEBUG_
	if (server->mmsg.mmsg_info[ii].klass != LXT_STATE_RELEASED) {
		TRACE(MSGL_VGSM_ERR, "CRITICAL!!! overwrite data that not yet sent message to client\n");
		TRACE(MSGL_VGSM_ERR, "group:0x%x, action: 0x%x\n", server->mmsg.mmsg_info[ii].group, server->mmsg.mmsg_info[ii].action);
	}
#endif
    // set id
    server->mmsg.mmsg_info[ii].klass = klass;

    // clear message
    TAPIMessageFree(&(server->mmsg.mmsg_info[ii].mmsg));

    // copy message
    server->mmsg.mmsg_info[ii].mmsg.length = packet->length;
    server->mmsg.mmsg_info[ii].mmsg.group  = packet->group;
    server->mmsg.mmsg_info[ii].mmsg.action = packet->action;

    if  (packet->length > 0)
    {
        server->mmsg.mmsg_info[ii].mmsg.data =
            (unsigned char *)PacketDataMalloc(packet->length);
        memcpy(server->mmsg.mmsg_info[ii].mmsg.data, packet->data, packet->length);
    }

    server->mmsg.current ++;
}

static int server_send_to_client(int handle, LXT_MESSAGE * packet)
{
	int datasize;
	int rc = -1;

	TRACE(MSGL_VGSM_INFO, "server_send_to_client : fd = %d, total size = 4+%d ... \n", handle, packet->length);
	// save size (length of packet is only datasize)
	datasize = packet->length;

	rc = WriteBytes(handle, packet, 4);
	if( rc != 4 )
		TRACE(MSGL_VGSM_ERR, "server_send_to_client1 : write error : rc = %d\n", rc);

	// FIXME : Broken PIPE error
	if  (datasize > 0)
	{
		rc = WriteBytes(handle, packet->data, datasize);
		if (rc != datasize )
			TRACE(MSGL_VGSM_ERR, "server_send_to_client2 : write error : rc = %d\n", rc);
	}
	return rc;
}

static TClientInfo * server_get_client(PhoneServer * ps, int clientid)
{
    Server * server = &ps->ServerSocket;

    TClientInfo * ci = server->current_ci;
    if  (ci)
        TRACE(MSGL_VGSM_INFO, "server get client is not null\n");

    while (ci)
    {
        //if  (ci->klass == clientid)
        if  (ci->klass == LXT_ID_CLIENT_EVENT_INJECTOR || ci->klass == clientid)
            return ci;
        ci = ci->next;
    }
    return NULL;
}

#if 0
static void get_domain_socket_name(char * result)
{
    strcpy(result, DEF_DOMAIN_SOCKET_NAME);
    strcat(result, (const char*)"-");
    strcat(result, getenv("USER"));
    return;
}
#endif

static void server_initialize(PhoneServer * ps)
{
    Server * server = &ps->ServerSocket;
    int ii;

	TRACE(MSGL_VGSM_INFO, "\n");

    server->fd = -1;
    server->inet_fd = -1;
    server->current_ci = NULL;

    // This is needed for save the message which arrive before the client connecting.
    for (ii = 0; ii < MAX_MISSED_MESSAGE; ii ++)
    {
        server->mmsg.mmsg_info[ii].klass = LXT_ID_CLIENT_RESERVED;
        TAPIMessageInit( &(server->mmsg.mmsg_info[ii].mmsg) );
    }
}

static int  server_open(PhoneServer * ps)
{
    Server * server = &ps->ServerSocket;

    int rc = -1;
    int len;
    struct sockaddr_un serverAddr;
    char socket_name[64] = {0, };

	TRACE(MSGL_VGSM_INFO, "\n");

	//090501
    //get_domain_socket_name(socket_name);

    server->fd = socket(AF_UNIX, SOCK_STREAM, 0);

	log_msg(MSGL_VGSM_INFO, "fd =%d \n", server->fd);

    if (server->fd < 0)
    {
        TRACE(MSGL_VGSM_ERR, "socket() failed\n");
        return -1;
    }

    unlink(socket_name);

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sun_family = AF_UNIX;
    strcpy(serverAddr.sun_path, socket_name);
    len = sizeof(serverAddr.sun_family) + strlen(socket_name);

    rc = bind(server->fd, (struct sockaddr *)&serverAddr, len);

    if (rc != 0)
    {
        TRACE(MSGL_VGSM_ERR, "bind() failed\n");
        close(server->fd);
        return -1;
    }

    rc = listen(server->fd, MAX_LISTEN_COUNT);

    if  (rc != 0)
    {
        TRACE(MSGL_VGSM_ERR, "listen() failed\n");
        close(server->fd);
        return -1;
    }

    return 0;
}


static int server_callback(PhoneServer * ps, int fd, EloopCondition cond, void * data)
{
	log_msg(MSGL_VGSM_INFO, "fd = %d \n",fd);
	Server * server = &ps->ServerSocket;

	struct sockaddr_in addr;
	int len = sizeof(addr);
	memset(&addr, 0, len);

	int client_fd = accept(server->fd, (struct sockaddr *)&addr, (socklen_t *)&len);

	TRACE(MSGL_VGSM_INFO, "Client fd %d server fd %d\n", client_fd, server->fd);

	if  (client_fd > 0)
	{
		server->ci = malloc(sizeof (TClientInfo));
		server->ci->Functions = NULL;
		server->ci->Functions = &ClientHandle;
		server->ci->fd = client_fd;
		server->ci->klass = -1;
		server->ci->next = server->current_ci;
		server->current_ci = server->ci;
		TAPIMessageInit(&(server->ci->request));
		TAPIMessageInit(&(server->ci->notification));
		int tag = eloop_add_input(ps, server->ci->fd, ELOOP_READ,
		            server->ci->Functions->CallBack, server->ci);
		server->ci->tag = tag;
		TRACE(MSGL_VGSM_INFO, "Server inet callback is registerd at [%d]\n", tag);
	}
	else
	{
		perror("accept 2");
	}
	return 1;
}



static int  server_inet_open(PhoneServer * ps)
{
	Server * server = &ps->ServerSocket;
	struct sockaddr_in server_addr;
	int val;

	memset(&server_addr, 0, sizeof server_addr);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(3578);
	server_addr.sin_addr.s_addr = INADDR_ANY;

	TRACE(MSGL_VGSM_INFO, "listening to port %d\n", server_addr.sin_port);

	if((server->inet_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket() failed");
		return -1;
	}

	val = 1;
	setsockopt(server->inet_fd, SOL_SOCKET, SO_REUSEADDR
		, (const char *)&val, sizeof(val));

	if(bind(server->inet_fd, (struct sockaddr *)&server_addr, sizeof server_addr) == -1)
	{
		perror("bind() failed");
		close(server->inet_fd);
		return -1;
	}

	if(listen(server->inet_fd, MAX_LISTEN_COUNT) == -1)
	{
		perror("listen() failed");
		close(server->inet_fd);
		return -1;
	}

	return 0;
}


#if 1
static int server_inet_callback(PhoneServer * ps, int fd, EloopCondition cond, void * data)
{
    Server * server = &ps->ServerSocket;

    struct sockaddr_in addr;
    int len = sizeof(addr);
	memset(&addr, 0, len);

    int client_fd = accept(server->inet_fd, (struct sockaddr *)&addr, (socklen_t *)&len);

	TRACE(MSGL_VGSM_INFO, "\n\n\ninet socket accept!!!\n\n\n");
    TRACE(MSGL_VGSM_INFO, "Client fd %d server fd %d\n",
					client_fd, server->inet_fd);
	/*
	if  (client_fd > 0)
	{
		DPRAM * dpram = &GlobalPS.Dpram;

		//	++ 2008-08-05
		//	++ Data.DPRAM is union. So, DPRAM is not clear whether it is fake or DPRAM.
		//	++ It should be sure what is.
		//
		GSM_Device_DPRAMData   *d = &GlobalS.Device.Data.DPRAM;

		d->Power = true;
		d->hPhone = client_fd;

		dpram->fd=client_fd;

		int iotag;
		iotag = eloop_add_input(ps, dpram->fd, ELOOP_READ,
								dpram->Functions->CallBack, 0);	// dpram_callback
		TRACE(MSGL_VGSM_INFO, "Server inet callback is registerd at [%d]\n", iotag);
	}
	else
	{
		perror("accept 3");
	}
	return 1;
	*/

	if  (client_fd > 0)
	{
		server->ci = malloc(sizeof (TClientInfo));
		server->ci->Functions = NULL;
		server->ci->Functions = &ClientHandle;
		server->ci->fd = client_fd;
		server->ci->klass = -1;
		server->ci->next = server->current_ci;
		server->current_ci = server->ci;
		TAPIMessageInit(&(server->ci->request));
		TAPIMessageInit(&(server->ci->notification));
		int tag = eloop_add_input(ps, server->ci->fd, ELOOP_READ,
		            server->ci->Functions->CallBack, server->ci);
		server->ci->tag = tag;
		TRACE(MSGL_VGSM_INFO, "Server inet callback is registerd at [%d]\n", tag);
	}
	else
	{
		perror("accept 2");
	}
	return 1;

}
#else

static int server_inet_callback(PhoneServer * ps, int fd, EloopCondition cond, void * data)
{
	log_msg(MSGL_VGSM_INFO, "fd = %d \n",fd);
	Server * server = &ps->ServerSocket;

	struct sockaddr_in addr;
	int len = sizeof(addr);
	memset(&addr, 0, len);

	int client_fd = accept(server->fd, (struct sockaddr *)&addr, (socklen_t *)&len);

	TRACE(MSGL_VGSM_INFO, "Client fd %d server fd %d\n", client_fd, server->fd);

	if  (client_fd > 0)
	{
		server->ci = g_new(TClientInfo, 1);
		server->ci->Functions = NULL;
		server->ci->Functions = &ClientHandle;
		server->ci->fd = client_fd;
		server->ci->klass = -1;
		server->ci->next = server->current_ci;
		server->current_ci = server->ci;
		TAPIMessageInit(&(server->ci->request));
		TAPIMessageInit(&(server->ci->notification));
		int tag = eloop_add_input(ps, server->ci->fd, ELOOP_READ,
		            server->ci->Functions->CallBack, server->ci);
		server->ci->tag = tag;
		TRACE(MSGL_VGSM_INFO, "Server inet callback is registerd at [%d]\n", tag);
	}
	else
	{
		perror("accept 2");
	}
	return 1;
}

#endif

const char *str_main_cmd[] = {
	"GSM_COMMON", // 0x00
"GSM_CALL",//                             0x01
"GSM_NETWORK",//                     0x02
"GSM_SUPS",//                             0x03
"GSM_SMS",//                              0x04
"GSM_MISC", //                             0x05
"GSM_DISPLAY", //                          0x06
"GSM_SIM", //                              0x07
"GSM_DATA", //                             0x08
"GSM_SAT", //				          0x09
"GSM_SOUND", //				0x0A
"GSM_EXT",//                              0x0B
"GSM_SERVICEMODE",//		0x0C
"GSM_EMULATOR ",//                   0x0D
"GSM_GPRS", //                            0x0E
"GSM_POWER",//                         0x0F
"GSM_CLIENT" //		               0x10
};

static void server_cast(PhoneServer * ps, int clientid, LXT_MESSAGE * packet)
{
	_ENTER();
    Server * server = &ps->ServerSocket;

	TRACE(MSGL_VGSM_INFO, "clientid(%d) \n", clientid);

    if  (server == NULL)
        TRACE(MSGL_VGSM_INFO, "server cast server is null\n");
    TClientInfo *ci = 0;
    int clientfd = -1;

    ci = server_get_client(ps, clientid);

    if  (ci)
        clientfd = ci->fd;
    else
        TRACE(MSGL_VGSM_INFO, "server cast client is null\n");

    if  (clientfd > 0)
    {
        TRACE(MSGL_VGSM_INFO, "CAST MESSAGE TO [0x%x], MAIN[%s], SUB[0x%x] : ci fd [%d]\n", ci->klass, str_main_cmd[packet->group], packet->action, clientfd);

        server_send_to_client(clientfd, packet);
    }
    else
    {
        server_save_message(ps, (LXT_ID_CLIENT)clientid, packet);
    }
}

static void server_broadcast(PhoneServer * ps, LXT_MESSAGE * packet)
{
	_ENTER();
	Server * server = &ps->ServerSocket;

	TRACE(MSGL_VGSM_INFO, "\n");

	TClientInfo * ci = server->current_ci;
	int clientfd = -1;

	if  (ci)
		TRACE(MSGL_VGSM_INFO, "server get client is not null\n");

	while (ci)
	{
		if  (ci->fd > 0)
		{
			if  (ci->klass == 0x10)
			{
				ci = ci->next;
				continue;
			}

			WriteBytes(ci->fd, packet, 4);
			if  (packet->length > 0)
				WriteBytes(ci->fd, packet->data, packet->length);
		}

		ci = ci->next;
	}

	// nsclass : Especially, cast internal message to phone indicator, because, phone applet broadcast
	// phone state to other client.(save missed message)
	// 1. LXT_ID_CLIENT_INDICATOR
	ci = server_get_client(ps, LXT_ID_CLIENT_INDICATOR);
	if (ci)
		clientfd = ci->fd;

	if (clientfd < 0)
		server_save_message(ps, LXT_ID_CLIENT_INDICATOR, packet);

	// 2. LXT_ID_CLIENT_DATASERVICE
	clientfd = -1;
	ci = server_get_client(ps, LXT_ID_CLIENT_DATASERVICE);
	if (ci)
		clientfd = ci->fd;

	if (clientfd < 0)
		server_save_message(ps, LXT_ID_CLIENT_DATASERVICE, packet);

	// 3. LXT_ID_CLIENT_CARD_MANAGER
	clientfd = -1;
	ci = server_get_client(ps, LXT_ID_CLIENT_CARD_MANAGER);
	if (ci)
		clientfd = ci->fd;

	if (clientfd < 0)
		server_save_message(ps, LXT_ID_CLIENT_CARD_MANAGER, packet);

	return;
}

static void clear_client_related_data(int client_id)
{
	TRACE(MSGL_VGSM_INFO, "\n");

	// 1. Multiple PDP related
	//GetDataConnectMgr()->m_pFunc->ClearClient((unsigned char)client_id);

	// 2. RequestResponse related
}

static void server_remove_client(PhoneServer * ps, TClientInfo *me)
{
	Server * server = &ps->ServerSocket;
	TClientInfo *curr = server->current_ci, *prev = 0;

	TRACE(MSGL_VGSM_INFO, "\n");

	while (curr)
	{
		if (curr == me)
		{
			if  (prev)
			{
				prev->next = curr->next;
			}
			else
			{
				server->current_ci = curr->next;
			}
			TAPIMessageFree( &(curr->notification) );
			TAPIMessageFree( &(curr->request) );
			close(curr->fd);

			clear_client_related_data(curr->klass);

			PacketDataFree(curr);
			return;
		}
		prev = curr;
		curr = curr->next;
	}
}

//void server_cast_missed_message(PhoneServer * ps, LXT_ID_CLIENT klass,
static void server_cast_missed_message(PhoneServer * ps, int klass, int clientfd)
{
	Server * server = &ps->ServerSocket;
	//TClientInfo *curr = server->current_ci, *prev = 0;
	int ii = 0;
	//int jj;
	int rc;

	TRACE(MSGL_VGSM_INFO, "\n");

	for (ii = 0; ii < MAX_MISSED_MESSAGE; ii ++)
	{
		if  (server->mmsg.mmsg_info[ii].klass == klass)
		{
			rc = WriteBytes( clientfd, &(server->mmsg.mmsg_info[ii].mmsg), 4);
			if  (server->mmsg.mmsg_info[ii].mmsg.length > 0)
			{
				// tx data to client
				rc = WriteBytes(	clientfd,
									server->mmsg.mmsg_info[ii].mmsg.data,
									server->mmsg.mmsg_info[ii].mmsg.length);
            }

			server->mmsg.mmsg_info[ii].klass = LXT_ID_CLIENT_RESERVED;
			TRACE(MSGL_VGSM_INFO, "removed missed cast index: %d by client(0x%x)\n", ii, klass);

			// clear message
			TAPIMessageFree(&(server->mmsg.mmsg_info[ii].mmsg));
		}
	}
}

FunctionsServer ServerHandle =
{
	server_initialize,
	server_open,
	server_callback,
	server_inet_open,
	server_inet_callback,
	server_cast,
	server_broadcast,
	server_remove_client,
	server_cast_missed_message
};

