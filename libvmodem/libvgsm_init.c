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

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <errno.h>

#include "vgsm.h"
#include "libvgsm_debug.h"

static int lxt_internal_setClientClass(LXT_HANDLE* handle);
static int lxt_internal_MissedMessage(LXT_HANDLE* handle);

// +++++++++++++++++++++++++++++++++++++++++++++define area
#define DEF_REV					"20040901.11"
#define DEF_DOMAIN_SOCKET_NAME	"/tmp/.vgsm_socket"
static int exit_status;

// internal function ++++++++++++++++++++++++++++++++++++++
static void get_domain_socket_name(char* result)
{
    strcpy(result, DEF_DOMAIN_SOCKET_NAME);
    strcat(result, (const char*)"-");
    if((strlen(result) + strlen(getenv("USER"))) < 64)
	strcat(result, getenv("USER"));

#ifndef _NO_ESPRESSO_DEBUG_
    LIBVGSM_DEBUG("[VGSM] SOCKET NAME [%s]\n", result);
#endif // _NO_ESPRESSO_DEBUG_

    return;
}


#if 1
static int connect_af_unix()
{
    int sockfd;
    int len;
    struct sockaddr_un address;
    int rc;
    char socket_name[64] = {0, };

#ifndef _NO_ESPRESSO_DEBUG_
    printf("[VGSM] UNIX SOCKET\n");
#endif // _NO_ESPRESSO_DEBUG_

    // get domain socket name
    get_domain_socket_name(socket_name);

    // create socket
    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);

    if (sockfd < 0) {
	perror("socket");
	return -1;
    }

    // set address type
    address.sun_family = AF_UNIX;

    strcpy(address.sun_path, socket_name);
    //strcpy(address.sun_path, PHONE_SERVER_SOCKET_PATH);

    // get size of address
    len = sizeof(address.sun_family) + strlen(socket_name);
    //len = sizeof(address.sun_family) + strlen(PHONE_SERVER_SOCKET_PATH);

    // connect to phone server
    rc = connect(sockfd, (struct sockaddr *)&address, len);

    if (rc == -1)
    {
	perror("connect");
	close(sockfd);
	return -1;
    }

#ifndef _NO_ESPRESSO_DEBUG_
    printf("[VGSM] SOCKET [%d]\n", sockfd);
#endif // _NO_ESPRESSO_DEBUG_

    return sockfd;
}
#endif


#if 0
static int connect_af_inet(const char* hostname)
{
    int sockfd;
    struct sockaddr_in address;
    int rc;

#ifndef _NO_ESPRESSO_DEBUG_
    LIBVGSM_DEBUG("[VGSM] INET SOCKET [%s]\n", hostname);
#endif // _NO_ESPRESSO_DEBUG_

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    //address.sin_port = htons(4007);
    address.sin_port = htons(4009);
    address.sin_addr.s_addr = inet_addr(hostname);

#ifndef _NO_ESPRESSO_DEBUG_
    LIBVGSM_DEBUG("[VGSM] CONNECT [%s]\n", hostname);
#endif // _NO_ESPRESSO_DEBUG_

    // connect to phone server
    rc = connect(sockfd, (struct sockaddr_in *)&address, sizeof(address));

    if (rc == -1)
    {
	perror(" vgsm connect err : ");
	return -1;
    }

#ifndef _NO_ESPRESSO_DEBUG_
    LIBVGSM_DEBUG("[VGSM] CONNECT : OK [%s]\n", hostname);
    LIBVGSM_DEBUG("[VGSM] SOCKET [%d]\n", sockfd);
#endif // _NO_ESPRESSO_DEBUG_

    return sockfd;
}
#else
#define SIZE	sizeof(struct sockaddr_in)

static int connect_af_inet(const char* hostname)
{
    int sockfd;
    struct sockaddr_in server = {0}; // = {AF_INET, 3578, {INADDR_ANY}};

    server.sin_family = AF_INET;
    server.sin_port = htons(3578);
    server.sin_addr.s_addr = inet_addr(hostname);
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
	printf("fail to call socket()\n");
	return -1;
    }

    if(connect(sockfd, (struct sockaddr*)&server, SIZE) == -1)
    {
	perror("connect() failed");
	close(sockfd);
	return -1;
    }

#ifndef _NO_ESPRESSO_DEBUG_
    printf("[VGSM] SOCKET [%d]\n", sockfd);
#endif // _NO_ESPRESSO_DEBUG_

    return sockfd;
}
#endif

#if 0
static char is_inet_mode()
{
    char file_name_cal[128] = {0, };
    int fd = 0;
    char result = 0;

    sprintf(file_name_cal,"/tmp/.vgsm_inet_mode");
    fd = open(file_name_cal, O_RDONLY);

    if(fd >=0)
    {
	result = 1;
	close(fd);
	perror(" is_inet_mode open err:");
    }
    else
    {
	result = 0;
    }

    return result;
}
#endif

// initialize handle
LXT_HANDLE* lxt_initialize(LXT_ID_CLIENT clientclass, LXT_CALLBACK cb)
{
    LXT_HANDLE *handle;
    int sockfd;
    int rc;

    exit_status = 0;

    //if(is_inet_mode()) //FALSE
    if(1) // 090501	prevent segfault in qemu
    {
	//printf("[VGSM] INET MODE : LINUX TAPI with coruscant on i819 : [%s][%s]\n", __DATE__, __TIME__);        
	LIBVGSM_DEBUG("INET MODE : LINUX VGSM\n");        
	if((sockfd = connect_af_inet("127.0.0.1")) == -1)
	{
	    LIBVGSM_DEBUG("LINUX VGSM : connect to target failed");

	    /*
	       if((sockfd = connect_af_unix()) == -1)
	       {
	       return NULL;
	       }
	     */
	}
	LIBVGSM_DEBUG("LINUX VGSM : connect to target");

    }
    else
    {
	//printf("[VGSM] DOMAIN MODE : LINUX TAPI with coruscant on i819 : [%s][%s]\n", __DATE__, __TIME__);
	LIBVGSM_DEBUG("DOMAIN MODE : LINUX VGSM\n");
	sockfd = connect_af_unix();
	LIBVGSM_DEBUG("sockfd = %d\n",sockfd);
    }

    if (sockfd < 0)
	return NULL;

    // allocate memory
    handle = malloc(sizeof(LXT_HANDLE));
    if (!handle) {
	close(sockfd);
	return NULL;
    }

    // save client class
    handle->clientclass = clientclass;

    // save socket
    handle->fd = sockfd;

    // save call back function
    handle->cb = cb;

    // register id
    rc = lxt_internal_setClientClass(handle);

    if (rc < 0)
    {
#ifndef _NO_ESPRESSO_DEBUG_
	printf("[VGSM] id registration failed\n");
#endif // _NO_ESPRESSO_DEBUG_

	// realease memory
	free(handle);
	return NULL;
    }

    // request missed message
    rc = lxt_internal_MissedMessage(handle);

    // request current state
    rc = lxt_requestCurruntState(handle); // not implemented . Is it needed?

    vgsm_ss_restoreEI(handle);

    // return handle
    return handle;
}

int vgsm_injector_get_fd(LXT_HANDLE *handle)
{
    return handle->fd;
}

/*
   int vgsm_release(LXT_HANDLE* handle)
   {
   int rc = -1;
   int exit_status; 

// check handle
if (handle == NULL)
return rc;

// check handle
if (handle->fd > 0)
rc = close(handle->fd);
else
rc = -1;



// free handle's memory
lxt_util_free(handle);

return rc;
}
 */

// missed message request
static int lxt_internal_MissedMessage(LXT_HANDLE* handle)
{
    return lxt_msg_send_message(	handle->fd,
	    LXT_GRP_INTERNAL,
	    LXT_PDA_INTERNAL_MISSED_MESSAGE_REQUEST,
	    0x00,
	    NULL);

}

// tx client id to phone server
static int lxt_internal_setClientClass(LXT_HANDLE* handle)
{
    return lxt_msg_send_message(	handle->fd,
	    LXT_GRP_INTERNAL,
	    LXT_PDA_INTERNAL_ID_REQUEST,
	    sizeof(int),
	    &(handle->clientclass));
}

int lxt_requestCurruntState(LXT_HANDLE* handle)
{
    return lxt_msg_send_message(	handle->fd,
	    LXT_GRP_INTERNAL,
	    LXT_PDA_INTERNAL_STATE_REQUEST,
	    0,
	    NULL);
}

// call back
static int lxt_callback(LXT_HANDLE* handle)
{
    LIBVGSM_DEBUG("\n");

    LXT_MESSAGE packet;
    int length;
    int group;
    int action;
    void *data;
    int rc;

    // check handle
    if (handle == NULL)
	return -1;

    packet.data = NULL;

    // rx information from server
    rc = lxt_util_readRawBytes(handle->fd, &packet, 4);

    if(rc != 4)
    {
	group = LXT_GRP_INTERNAL;
	action = LXT_PHN_INTERNAL_SERVER_DIE_EMERGENCY;
	handle->cb(group, action, 0, 0);
	return -1;
    }

    // check length
    if (packet.length != 0)
    {
	// allocated memory
	packet.data = malloc(packet.length);
	assert(packet.data != NULL);

	// clear memory
	memset(packet.data, 0, packet.length);

	// rx data from server
	rc = lxt_util_readRawBytes(handle->fd, packet.data, packet.length);

#ifndef _NO_ESPRESSO_DEBUG_
	assert(rc == packet.length);
#endif
    }
    else
    {
	// init
	packet.data = NULL;
    }

    // length
    length = packet.length;

    // group
    group = packet.group;

    // action
    action = packet.action;

    // data
    data = packet.data;

    if ( rc != length)
	LIBVGSM_DEBUG ("Read data : read len = %d, len = %d \n",rc, length);

    LIBVGSM_DEBUG("Client Read Data : CID[0x%x]length[%d]group[0x%x]action[0x%x]\n", handle->clientclass, length, group, action);
    //lxt_util_rawdataPrint(data, length, "=============Call Back Data==================");

    // call back
    handle->cb(group, action, data, length);

    if (packet.data != NULL)
	free(packet.data);

    return rc;
}

int vgsm_socket_read_callback(LXT_HANDLE* handle)
{
    return lxt_callback(handle);
}

LXT_HANDLE* vgsm_injector_initialize(LXT_CALLBACK cb)
{
    return lxt_initialize( LXT_ID_CLIENT_EVENT_INJECTOR, cb );
}

LXT_HANDLE* vgsm_manager_rx_initialize(LXT_CALLBACK cb)
{
    return lxt_initialize( LXT_ID_CLIENT_EVENT_MANAGER_RX, cb );
}

LXT_HANDLE* vgsm_manager_tx_initialize(LXT_CALLBACK cb)
{
    return lxt_initialize( LXT_ID_CLIENT_EVENT_MANAGER_TX, cb );
}

LXT_HANDLE* vgsm_manager_initialize(LXT_CALLBACK cb)
{
    return lxt_initialize( LXT_ID_CLIENT_EVENT_MANAGER, cb );
}

int vgsm_injector_release(LXT_HANDLE* handle)
{
    // check handle
    if (handle == NULL)
	return -1;

    LIBVGSM_DEBUG("handle->fd = %d \n", handle->fd);

    vgsm_client_release_notify(handle);

    return 1;
}

