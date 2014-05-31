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

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <gsmstate.h>
#include "phoneserver.h"
#include <linux/capability.h>
#include "eloop.h"
#include "gsm.h"
#include "logmsg.h"

#include <signal.h>

#include "server_common.h"		// for init_vgsm()

#include "at_send.h"
#include "at_tx_func.h"

extern GSM_StateMachine  GlobalS;

#include <limits.h>

//#include "db_ss.h"

static int eventloop(PhoneServer * ps)
{
    DPRAM       * dpram       = &ps->Dpram;
    Server      * server      = &ps->ServerSocket;

    TRACE(MSGL_VGSM_INFO, "\n");

    if( vgsm_server_port  ==0 ){
	if  (dpram->fd > 0)
	{
	    int iotag;
	    iotag = eloop_add_input(	ps, dpram->fd,  ELOOP_READ,
		    dpram->Functions->CallBack, 0);	// &DpramHandle->dpram_callback
	    TRACE(MSGL_VGSM_INFO, "DPRAM callback is registerd at [%d,fd=%d]\n", iotag, dpram->fd);
	}

    }

    if  (server->fd > 0)
    {
	int iotag;
	iotag = eloop_add_input(	ps, server->fd, ELOOP_READ,
		server->Functions->CallBack, 0);	// &ServerHandle->server_callback
	TRACE(MSGL_VGSM_INFO, "Server callback is registerd at [%d,fd=%d]\n", iotag, server->fd);
    }

    if  (server->inet_fd > 0)
    {
	int iotag;
	iotag = eloop_add_input(	ps, server->inet_fd, ELOOP_READ,
		server->Functions->CallBackInet, 0);	// &ServerHandle->server_inet_callback
	TRACE(MSGL_VGSM_INFO, "Server inet callback is registerd at [%d,fd=%d]\n", iotag, server->inet_fd);
    }

    // main event loop
    eloop_main();

    return 0;
}

static void signalHandler(int iSigno)
{
    TRACE(MSGL_VGSM_INFO, "signo = %d\n", iSigno);
    printf("signo = %d\n", iSigno);
    exit(1);
    return;
}

static void  argument_handling(int argc, char *argv[])
{
    vgsm_server_port=0;

    int i;

    if((argc%2) == 0) {
	TRACE(MSGL_FATAL, "bad arguments\n");
    }

    if (argc >= 2) {
	for (i=1; i<argc; i+=2)
	    if(strcmp(argv[i], "--port") == 0) {
		vgsm_server_port=atoi(argv[i+1]);
		TRACE(MSGL_WARN," ====== vgsm server port  :   %d    ", vgsm_server_port);
		TRACE(MSGL_WARN," ====== Important !!!! VDPRAM driver is not used as transport ====== ");
	    }
	    else {
		TRACE(MSGL_FATAL,"\nTo set up the port  --port  [ portnumber ]\n ");
		exit(1);
	    } 
    }	
}

int main(int argc, char *argv[])
{
    GSM_Error error;
    bool phone_power;

    ge_log_msg_init("vgsm");

    struct sigaction sa;
    sa.sa_handler = signalHandler;
    sigemptyset (&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO;

    if (sigaction (SIGINT, &sa, NULL) == -1) {
	TRACE(MSGL_FATAL, "sigaction error\n");
	exit (1);
    }

    signal (SIGTERM, signalHandler);
    signal (SIGPIPE, signalHandler);

    TRACE(MSGL_VGSM_INFO, "START VIRTUAL GSM\n");

    argument_handling(argc, argv);

    error = GSM_Init(true);
    if (error != ERR_NONE)
    {
	TRACE(MSGL_FATAL, "GSM_Init failed\n");
	exit(-1);
    }
    else
	TRACE(MSGL_VGSM_INFO, "GSM_Init ok\n");

    if (PhoneServer_Init(&GlobalPS, &GlobalS) < 0)
    {
	TRACE(MSGL_FATAL, "PhoneServer_Init failed\n");
	exit(-1);
    }
    else if(PhoneServer_Connect() < 0) {
	TRACE(MSGL_VGSM_INFO, "PhoneServer_Connect failed\n");
	exit(-1);
    }

    phone_power = GSM_PhonePowerStatus();

#ifdef __arm__
    error = GSM_PhonePowerOn(1);
#else
    FuncPhoneServer->BroadcastInternalStatus(&GlobalPS, LXT_STATE_ON);	// &PhoneServerHandle->BroadcastInternalStatus
    GSM_Start();
#endif

    FuncPhoneServer->BroadcastInternalStatus(&GlobalPS, LXT_STATE_ON);	// &PhoneServerHandle->BroadcastInternalStatus
    FuncPhoneServer->SetInternalState(&GlobalPS, LXT_STATE_ON);			// &PhoneServerHandle->SetInternalState

    init_vgsm();

    eventloop(&GlobalPS);

    return 0;
}
