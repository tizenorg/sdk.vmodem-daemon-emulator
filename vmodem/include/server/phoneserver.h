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

/*********************************************************************
* $Id: phoneserver.h,v 1.1.1.1 2008-12-08 04:45:34 okdear Exp $
*
* Copyright (C) Samsung Electronics
*
*********************************************************************/

#ifndef __phoneserver_h
#define __phoneserver_h

#ifdef __arm__
#include "dpram.h"
	//#include "mesgbox_set.h"
#else
	#include <sys/ioctl.h>
#endif

#include <linuxtapi.h>
#include <vgsm_sim.h>
#include <vgsm_network.h>
#include <vgsm_call.h>
#include <vgsm_ss.h>

//#include <mzgsmpb.h>

#include "gsmstate.h"

#define MAX_MISSED_MESSAGE 64
#define MAX_CALL_COUNT     6
#define LXT_ID_CLIENT_HANDSFREE 0x1A

typedef enum {
    ELOOP_READ = 1,
    ELOOP_WRITE = 2,
    ELOOP_EXCEPTION = 4,
} EloopCondition;

typedef struct _Phone_Server PhoneServer;

/* ---------------------------------------------------------------------------
     IOManager
   ---------------------------------------------------------------------------*/
#ifdef __arm__
typedef struct
{
    int  (*Open)     (PhoneServer *);
    int  (*Close)    (PhoneServer *);
    void (*SendQuery)(PhoneServer * s, const char * query);
    void (*SendCast) (PhoneServer * s, const LXT_MESSAGE * packet);
    int  (*CallBack) (PhoneServer * s,int fd,EloopCondition cond,void * data);
    int  (*Init)     (PhoneServer *);
} FunctionsIOM;

extern FunctionsIOM IOMHandle;

typedef enum
{
    TFactoryCableStatus_off                 =0x00,
    TFactoryCableStatus_on                  =0x01
}
TFactoryCableStatus;

typedef enum
{
    TFactoryCardStatus_off                  =0x00,
    TFactoryCardStatus_on                   =0x01
}
TFactoryCardStatus;

typedef enum
{
    TFactoryMode_exit                       =0x00,
    TFactoryMode_enter                      =0x01,
    TFactoryMode_unknown                    =0x0f
}
TFactoryMode;

typedef struct
{
    int fd;
    //MBSET mbs;

    signed int          _battery_level;
    TFactoryCableStatus _factory_cable_status;
    TFactoryCardStatus  _factory_card_status;
    TFactoryMode        _factory_mode;
	char                _calibration_status;
    char                _earjack_status;

    FunctionsIOM * Functions;
} IOManager;
#endif

/* ---------------------------------------------------------------------------
     PPP Gateway : comunicate with ppp gateway for Dial up Networking
   ---------------------------------------------------------------------------*/
typedef struct
{
    int (*Open)    (PhoneServer * s);
    int (*Close)   (PhoneServer * s);
    int (*CallBack)(PhoneServer * s, int fd, EloopCondition cond, void * data);
    int (*Read)    (PhoneServer * s, void * data, int count);
    int (*Write)   (PhoneServer * s, void * data, int count);
    int (*Disconnect) (PhoneServer * s);
} FunctionsPPPGW;

extern FunctionsPPPGW PPPGWHandle;

typedef struct
{
    int tx_fd;
    int rx_fd;
    FunctionsPPPGW * Functions;
} PPP_GATEWAY;

/* ---------------------------------------------------------------------------
     dpram : use the 'fd' which is generate by dpram handle GSM InitConnection
   ---------------------------------------------------------------------------*/
typedef struct
{
    int (*CallBack)(PhoneServer * s, int fd, EloopCondition cond, void * data);
} FunctionsDpram;

extern FunctionsDpram DpramHandle;

typedef struct
{
    int fd;

    FunctionsDpram * Functions;
} DPRAM;

/* ---------------------------------------------------------------------------
     dpram event : 'fd' for phone event like ppp, SIM cover.
   ---------------------------------------------------------------------------*/
typedef struct
{
    int (*Open)    (PhoneServer * s);
    int (*Close)   (PhoneServer * s);
    int (*CallBack)(PhoneServer * s, int fd, EloopCondition cond, void * data);
} FunctionsDpramEvent;

extern FunctionsDpramEvent DpramEventHandle;

typedef struct
{
    int fd;
    FunctionsDpramEvent * Functions;
} DPRAM_EVENT;

/* ---------------------------------------------------------------------------
     dpram error : for the phone binary critical message
   ---------------------------------------------------------------------------*/
typedef struct
{
    int (*Open)    (PhoneServer * s);
    int (*Close)   (PhoneServer * s);
    int (*CallBack)(PhoneServer * s, int fd, EloopCondition cond, void * data);
} FunctionsDpramError;

extern FunctionsDpramError DpramErrorHandle;

typedef struct
{
    int fd;
    FunctionsDpramError * Functions;
} DPRAM_ERROR;

/* ---------------------------------------------------------------------------
     socket server : socket server for client connecting
      almost codes are in CallBack of TClientInfo.
   ---------------------------------------------------------------------------*/
typedef struct
{
    int  (*CallBack)(PhoneServer * s, int fd, EloopCondition cond, void * data);
} FunctionsClient;

extern FunctionsClient ClientHandle;

typedef struct clientinfo
{
    int fd;
    int tag;
    int klass;
    LXT_MESSAGE request;
    LXT_MESSAGE notification;
    struct clientinfo *next;
    FunctionsClient * Functions;
} TClientInfo;

typedef struct
{
    void (*Init)     (PhoneServer * );
    int  (*Open)     (PhoneServer * );
    int  (*CallBack) (PhoneServer * s, int fd,EloopCondition cond,void * data);
    int  (*OpenInet)     (PhoneServer * );
    int  (*CallBackInet) (PhoneServer * s, int fd,EloopCondition cond,void * data);
    void (*Cast)     (PhoneServer * s, int clientid, LXT_MESSAGE * packet);
    void (*BroadCast)(PhoneServer * s, LXT_MESSAGE * packet);
    void (*RemoveClient)(PhoneServer * ps, TClientInfo *me);
    void (*CastMissedMessage) (PhoneServer * ps, int klass, int clientfd);
} FunctionsServer;

extern FunctionsServer ServerHandle;

typedef struct
{
    int klass;
    LXT_MESSAGE mmsg;
} MissedMessageInfo;

typedef struct
{
    int current;
    MissedMessageInfo mmsg_info[MAX_MISSED_MESSAGE];
} MissedMessage;

typedef struct
{
    int fd;
    int inet_fd;
    TClientInfo     * ci;
    TClientInfo     * current_ci;
    MissedMessage     mmsg;
    FunctionsServer * Functions;
} Server;

extern int vgsm_server_port;

//
typedef struct
{
    //GSM_WaitingCall WaitingCall; // unused
    LXT_PHONE_STATE CurrentPhoneState;
    // char prevCallList[MAX_CALL_COUNT];
    // unsigned short prevCallList[MAX_CALL_COUNT];
    gsm_call_list_t   prevCallList;
    bool m_waitingmode;
    bool m_deviceCall;
	int 			UIMLockType;
	int				UIMLockKey;
	int				LastNetState;
	int				StatePhoneOn;
	int				GSMDataRequest;
} PhoneServerData;
/* ---------------------------------------------------------------------------
     main structure
   ---------------------------------------------------------------------------*/
typedef struct {
    void (*SetInternalState) (PhoneServer * ps, LXT_PHONE_STATE state);
    LXT_PHONE_STATE (*GetInternalState) (PhoneServer * ps);
    void (*BroadcastInternalStatus) (PhoneServer * ps, LXT_PHONE_STATE state);
    void (*CastCurrentInternalStatus) (PhoneServer *ps, int clientid);
    void (*CallListId_Init)    (PhoneServer *ps);
    void (*RetrievePhoneState) (void);
} PhoneServerFunc;

extern PhoneServerFunc PhoneServerHandle;

struct _Phone_Server
{
#ifdef __arm__
    //IOManager       Iom;
#endif
    PPP_GATEWAY     PPPGW;
    DPRAM           Dpram;
    DPRAM_EVENT     DpramEvent;
    DPRAM_ERROR     DpramError;
    Server          ServerSocket;
    // phone server's data collection
    PhoneServerData Data;
    PhoneServerFunc * Functions1;
};

extern const char * clientName[];


//////////////////////////////////////////////////////////////////////
// by nsclass : remove implicit warning sign.
int PhoneServer_Init(PhoneServer * server, GSM_StateMachine * s);
int PhoneServer_Connect(void);

extern PhoneServer       GlobalPS;
extern PhoneServerFunc * FuncPhoneServer;
extern FunctionsServer * FuncServer;
extern PhoneServerFunc * FuncPhoneServer;


#endif
