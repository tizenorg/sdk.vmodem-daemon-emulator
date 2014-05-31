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

#include "at_msg.h"
#include "oem_rx_sms.h"
#include "at_rx_sms.h"
#include "at_tx_sms.h"
#include "at_tx_func.h"
#include "at_func.h"

/* temporarily vconf setup */
//#define SMS_PARAM_SET_VCONF 
#ifdef SMS_PARAM_SET_VCONF
#include <vconf/vconf.h>
#include <vconf/vconf-keys.h>
int sms_param_set_vconf();
#endif

#define SMS_ERROR_OK	0x0000

#define PARAM_RECORD_COUNT     1
/* Maximum number of bytes SMSP Record size (Y + 28), y : 0 ~ 128 */
/* #define MAX_GSM_SMS_PARAM_RECORD_SIZE       156  */
#define PARAM_RECORD_LEN_MAX   156
static unsigned char g_record_len = 0;
static unsigned char g_record[PARAM_RECORD_LEN_MAX];

#define CB_MAX_SELECTED_IDS 0xff
static unsigned char g_cb_enabled = 1;      /* enabled */
static unsigned char g_cb_selected_id = 1;  /* all selected */
static unsigned char g_cb_selected_count = 0;  /* number of selected ids */
static const unsigned char g_cb_max_selected_ids = CB_MAX_SELECTED_IDS;
static unsigned short g_cb_selected_list[CB_MAX_SELECTED_IDS];  /* number of selected ids */

int at_rx_sms_SendMsg(char* atmsg)
{
    TRACE(MSGL_VGSM_INFO, "%s\n", atmsg);
    char smsdata[8 * 1024];
    char tmpmsg[8 * 1024];

    memset(tmpmsg, '\0', 8 * 1024);
    memcpy(tmpmsg, atmsg, strlen(atmsg));

    char* tdata = strchr(tmpmsg, '=');
    char token[] = "\r";
    char* ret = NULL;
    int data_len = 0;
    int start_index = 0;
    int pdu_len = 0;	
    ret = strtok(tdata+1, token);
    if(ret)
	pdu_len = atoi(ret); 
    else
	return 0;

    start_index = strlen(CMGS) + strlen(ret) + strlen(token);

    smsdata[0] = 0x02;		// network type gsm
    smsdata[1] = 0x02;		// MORETOSEND field	

    char byteStringBuf[SEND_BUF_SIZE];
    memset(byteStringBuf, '\0', SEND_BUF_SIZE);

    int byteStrLen = hexStringToBytes(byteStringBuf, &atmsg[start_index]);	
    smsdata[2] = (char)byteStrLen;     // pdu length
    data_len = byteStrLen + pdu_len;

    TRACE(MSGL_VGSM_INFO, "byteStrLen:%d\n", byteStrLen);
    memcpy(&smsdata[3], byteStringBuf, byteStrLen);

    return oem_rx_sms_SendMsg(smsdata, data_len);
}

int at_rx_sms_deliver_report_exec(char* atmsg)
{
    return at_tx_sms_deliver_report_noti(SMS_ERROR_OK);
}

int at_rx_sms_DR(char* atmsg)
{
    //	return oem_rx_sms_DR(atmsg, strlen(atmsg));
    return 0;
}

int at_rx_sms_SCA(char* atmsg)
{

    /*	
    // set
    oem_rx_sms_SCA_set("TelTapiSmsSetSCA");

    // get
    oem_rx_sms_SCA(atmsg, strlen(atmsg));

     */	return -1;
}

int at_rx_sms_DefaultSms(char* atmsg)
{
    TRACE(MSGL_VGSM_INFO, "\n");

    return -1;
}

static int at_rx_sms_cbconfig_get(char* atmsg)
{
    return at_tx_sms_cbconfig_resp(g_cb_enabled, g_cb_selected_id
	    , g_cb_max_selected_ids, g_cb_selected_count, g_cb_selected_list);
}

static int at_rx_sms_cbconfig_set(char* atmsg)
{
    TRACE(MSGL_VGSM_INFO, "unimplemented!\n");
    return 0;
}

int at_rx_sms_cbconfig(char* atmsg)
{
    /*
    // set
    at_rx_sms_cbconfig_set(atmsg);

    // get
    at_rx_sms_cbconfig_get(atmsg);
     */
    return -1;
}

unsigned char at_g_sms_service_option= 0x02; // CS_ONLY

int at_rx_sms_param(char* atmsg)
{
    return -1;
}

int at_rx_sms_stored_msg_status(char* atmsg)
{
    return oem_rx_sms_stored_msg_status("TelTapiSmsSetMessageStatus");
}

int at_rx_sms_mem_status_set(char* atmsg)
{
    return 1;
}

static void at_rx_sms_param_init()
{
    int i, n = 0;

    //Alpha-Id
    g_record[n++] = '1';
    g_record[n++] = '2';
    g_record[n++] = '3';        
    g_record[n++] = '4';
    g_record[n++] = '5';
    for(i=0; i<(24-5);i++){   
	g_record[n++] = 0x00;   
    }   

    // parameter indicator
    g_record[n++] = 0xe9;  

    // TP-Destination Addr.
    for(i=0; i<12;i++){   
	g_record[n++] = 0xff;   
    }   

    //TP-SC addr. // this is raw g_record. +8210911111
    g_record[n++] = 0x06;   
    g_record[n++] = 0x91;   
    g_record[n++] = 0x28;    
    g_record[n++] = 0x01;    
    g_record[n++] = 0x19;
    g_record[n++] = 0x11;
    g_record[n++] = 0x11;
    g_record[n++] = 0xff;
    g_record[n++] = 0xff;
    g_record[n++] = 0xff;
    g_record[n++] = 0xff;
    g_record[n++] = 0xff;

    //PID
    g_record[n++] = 0x01;

    //DCS
    g_record[n++] = 0xff;

    //VP
    g_record[n++] = 0x00;

    /* init end */
    /* 52 = (24+1+12+12+3), (=apha id + TP DA+...) */
    g_record_len = 52;

    return;
}

int at_rx_sms_param_count_get(char* atmsg)
{
    TRACE(MSGL_VGSM_INFO, "unimplemented!\n");
    return 0;
}

int at_rx_sms_param_set(char* atmsg)
{
    TRACE(MSGL_VGSM_INFO, "unimplemented!\n");
    return 1;
}

unsigned char* at_rx_sms_param_get(unsigned char* len)
{
    TRACE(MSGL_VGSM_INFO, "g_record_len:0x%02x\n", g_record_len);

    if(g_record_len == 0)
    {
	at_rx_sms_param_init();
    }

    *len = g_record_len;

    return g_record;
}

int at_rx_sms_stored_msg_count_get(char* atmsg)
{
    return at_tx_sms_stored_msg_count_resp();
}

int at_rx_sms_svc_option_set(char* atmsg)
{
    TRACE(MSGL_VGSM_INFO, "unimplemented!\n");
    return 1;
}


#ifdef SMS_PARAM_SET_VCONF

/* reference */
/* - libslp-tapi/include/common/TelNetText.h */
/* - msg-service/plugin/sms_plugin/SmsPluginSetting.cpp */

#define SMSC_NAME_MAX						127
#define TAPI_NETTEXT_SMSP_PARAMS_MAX_LEN	28
#define TAPI_SIM_SMSP_ALPHA_ID_LEN_MAX  	128

#define SMSPValidDestAddr       			0x01
#define SMSPValidSvcAddr        			0x02
#define SMSPValidPID            			0x04
#define SMSPValidDCS            			0x08
#define SMSPValidVP             			0x10

#define nDestAddrOffset         			1
#define nSCAAddrOffset          			13
#define nPIDOffset              			25
#define nDCSOffset              			26
#define nVPOffset               			27

#define TAPI_SIM_SMSP_ADDRESS_LEN 			20
#define MAX_GSM_SMS_PARAM_RECORD_SIZE       156 


/* msg-service/include/common/MsgSettingTypes.h */
enum  _MSG_SMS_PID_E
{
    MSG_PID_NONE,       /**< None */
    MSG_PID_TEXT,       /**< Text */
    MSG_PID_VOICE,      /**< Voice */
    MSG_PID_FAX,        /**< Fax */
    MSG_PID_X400,       /**< X400 */
    MSG_PID_ERMES,      /**< Ermes */
    MSG_PID_EMAIL,      /**< Email */
    MSG_PID_MAX     /**< Default */
};          

/* msg-service/plugin/sms_plugin/include/SmsPluginTypes.h */
enum _SMS_TON_E
{ 
    SMS_TON_UNKNOWN = 0,        /* unknown */
    SMS_TON_INTERNATIONAL,  /* international */
    SMS_TON_NATIONAL,           /* national */
    SMS_TON_NETWORK,            /* network */
    SMS_TON_SUBSCRIBER,     /* subscriber */
    SMS_TON_ALPHANUMERIC,       /* alpha numberic */
    SMS_TON_ABBREVIATED,        /* abbreviated */
    SMS_TON_RESERVE,            /* reserve */
};

/* msg-service/plugin/sms_plugin/include/SmsPluginTypes.h */
enum _SMS_PID_E
{
    SMS_PID_NORMAL = 0x00,

    SMS_PID_TELEX = 0x21,       
    SMS_PID_GROUP3_TELEX = 0x22,
    SMS_PID_GROUP4_TELEX = 0x23,
    SMS_PID_VOICE = 0x24,
    SMS_PID_ERMES = 0x25,
    SMS_PID_NPS = 0x26,
    SMS_PID_VIDEO = 0x27, 
    SMS_PID_TELETEX_UNSPEC = 0x28,
    SMS_PID_TELETEX_PSPDN = 0x29,
    SMS_PID_TELETEX_CSPDN = 0x2A,
    SMS_PID_TELETEX_PSTN = 0x2B,
    SMS_PID_TELETEX_ISDN = 0x2C,
    SMS_PID_UCI = 0x2D,
    SMS_PID_x400 = 0x31,
    SMS_PID_EMAIL = 0x32,

    SMS_PID_TYPE0 = 0x40,
    SMS_PID_REPLACE_TYPE1 = 0x41,
    SMS_PID_REPLACE_TYPE2 = 0x42,
    SMS_PID_REPLACE_TYPE3 = 0x43,
    SMS_PID_REPLACE_TYPE4 = 0x44,
    SMS_PID_REPLACE_TYPE5 = 0x45,
    SMS_PID_REPLACE_TYPE6 = 0x46,
    SMS_PID_REPLACE_TYPE7 = 0x47,

    SMS_PID_RETURN_CALL = 0x5F,
};

typedef unsigned char SMS_PID_T;
typedef unsigned char MSG_SMS_PID_T;

/* msg-service/include/common/MsgInternalTypes.h */
#define DEFAULT_SETTING_PATH        "db/msg"
#define DEFAULT_SMSC_INFO_PATH      DEFAULT_SETTING_PATH"/smsc"

#define SMSC_NAME                   DEFAULT_SMSC_INFO_PATH"/name"
#define SMSC_ADDRESS                DEFAULT_SMSC_INFO_PATH"/address"
#define SMSC_TON                    DEFAULT_SMSC_INFO_PATH"/ton"
#define SMSC_NPI                    DEFAULT_SMSC_INFO_PATH"/npi"
#define SMSC_PID                    DEFAULT_SMSC_INFO_PATH"/pid"
#define SMSC_VAL_PERIOD             DEFAULT_SMSC_INFO_PATH"/val_period"

static MSG_SMS_PID_T convertPid(SMS_PID_T pid)
{
    MSG_SMS_PID_T retPid;

    switch (pid) {   
	case SMS_PID_NORMAL:
	    retPid = MSG_PID_TEXT;
	    break;  
	case SMS_PID_VOICE:
	    retPid = MSG_PID_VOICE;
	    break;  
	case SMS_PID_TELEX:
	    retPid = MSG_PID_FAX;
	    break;  
	case SMS_PID_x400:
	    retPid = MSG_PID_X400;
	    break;  
	case SMS_PID_ERMES:
	    retPid = MSG_PID_ERMES;
	    break;  
	case SMS_PID_EMAIL:
	    retPid = MSG_PID_EMAIL;
	    break;  
	default:
	    retPid = MSG_PID_TEXT;
	    break;  
    } 

    return retPid;
}


static int convertBcdToDigit(const unsigned char *pBcd, int BcdLen, char *pDigit)
{
    int i, offset = 0;
    unsigned char temp;

    for (i = 0; i < BcdLen; i++)
    {   
	temp = pBcd[i] & 0x0F;

	if (temp == 0x0A)
	    pDigit[offset++] = '*';
	else if (temp == 0x0B)
	    pDigit[offset++] = '#';
	else if (temp == 0x0C)
	    pDigit[offset++] = 'P';
	else
	    pDigit[offset++] = temp + '0';

	temp = (pBcd[i] & 0xF0) >> 4;

	if (temp == 0x0F)
	{   
	    pDigit[offset] = '\0';
	    return offset;
	}   

	if (temp == 0x0A)
	    pDigit[offset++] = '*';
	else if (temp == 0x0B)
	    pDigit[offset++] = '#';
	else if (temp == 0x0C)
	    pDigit[offset++] = 'P';
	else
	    pDigit[offset++] = temp + '0';
    }   

    pDigit[offset] = '\0';

    return offset;
}

static void decodeSMSC(unsigned char* pAddress, int AddrLen, int ton, char *pDecodeAddr)
{
    if (pAddress == NULL || AddrLen == 0)
	return;

    if (ton == SMS_TON_INTERNATIONAL)
    {
	pDecodeAddr[0] = '+';
	convertBcdToDigit(pAddress, AddrLen, &(pDecodeAddr[1]));
    }
    else
    {
	convertBcdToDigit(pAddress, AddrLen, pDecodeAddr);
    }
}

int sms_param_set_vconf()
{
    int i, alpha_id_len = 0,nOffset = 0;
    unsigned long AlphaIdLen; 
    char szAlphaId[TAPI_SIM_SMSP_ALPHA_ID_LEN_MAX + 1];
    char smscAddr[TAPI_SIM_SMSP_ALPHA_ID_LEN_MAX*2 + 1];
    unsigned char ParamIndicator;
    unsigned int DialNumLen; 
    unsigned char szDiallingNum[TAPI_SIM_SMSP_ADDRESS_LEN + 1];
    int Ton=0,  Npi=0; 
    unsigned short TpProtocolId = 0; 
    unsigned short TpDataCodingScheme = 0;
    unsigned short TpValidityPeriod = 0; 
    const unsigned char *p = NULL;

    p = &g_record[0];

    alpha_id_len = g_record_len - TAPI_NETTEXT_SMSP_PARAMS_MAX_LEN;
    if ( alpha_id_len > 0 ) {

	if(alpha_id_len > TAPI_SIM_SMSP_ALPHA_ID_LEN_MAX) {
	    alpha_id_len = TAPI_SIM_SMSP_ALPHA_ID_LEN_MAX;
	}

	for( i=0 ; i < alpha_id_len ; i++) {
	    if( 0x00 == p[i]) { /* 0xff check */
		TRACE(MSGL_VGSM_INFO, " found \n");
		break;
	    }
	}

	memcpy(&szAlphaId, p, i);
	AlphaIdLen = i;
	TRACE(MSGL_VGSM_INFO, "Alpha id length = %d \n", i);

    } else {
	AlphaIdLen = 0;
	TRACE(MSGL_VGSM_INFO, "Alpha id length is zero \n");
    }

    ParamIndicator = p[alpha_id_len];
    TRACE(MSGL_VGSM_INFO, "Param Indicator = %02x \n", ParamIndicator);

    if((ParamIndicator & SMSPValidDestAddr) == 0) {

	nOffset = nDestAddrOffset;

	if( 0x00 == p[alpha_id_len + nOffset] || 0xff == p[alpha_id_len + nOffset] ){
	    TRACE(MSGL_VGSM_INFO, "DestAddr Length is 0 \n");
	}else{
	    /* check */
	    TRACE(MSGL_VGSM_INFO, "Can't set this feild by setting app(DestAddr) \n");
	}
    }

    if( (ParamIndicator & SMSPValidSvcAddr) == 0) {

	nOffset = nSCAAddrOffset;

	if( 0x00 == p[alpha_id_len + nOffset] || 0xff == p[alpha_id_len + nOffset] ){

	    TRACE(MSGL_VGSM_INFO, "SCAAddr Length is 0 \n");

	} else {

	    TRACE(MSGL_VGSM_INFO, "SCAAddr \n");

	    if ( 0 < (int)p[alpha_id_len + nOffset]  ) {

		DialNumLen = (int)(p[alpha_id_len + nOffset] - 1);

		TRACE(MSGL_VGSM_INFO, "DialNumLen Len(%d) \n", DialNumLen);

		if(DialNumLen > TAPI_SIM_SMSP_ADDRESS_LEN)
		    DialNumLen = TAPI_SIM_SMSP_ADDRESS_LEN;

		Npi = p[alpha_id_len + (++nOffset)] & 0x0f ;
		Ton = (p[alpha_id_len + nOffset] & 0x70) >>4 ;

		memcpy( szDiallingNum, &p[alpha_id_len + (++nOffset)], DialNumLen);

		TRACE(MSGL_VGSM_INFO, "SCAddr Length = %d \n", DialNumLen);

		/* libslp-tapi/include/common/TelSim.h */
		/* 1: international number */
		TRACE(MSGL_VGSM_INFO, "SCAddr TON is %d \n", Ton);
		/* 1: ISDN/Telephone numbering plan  */
		TRACE(MSGL_VGSM_INFO, "SCAddr NPI is %d \n", Npi);

		memset(smscAddr, 0x00, sizeof(smscAddr));
		decodeSMSC(szDiallingNum, DialNumLen, Ton, smscAddr);
		TRACE(MSGL_VGSM_INFO, "smacAddr(%s) \n", smscAddr);

		//for( i = 0 ; i < DialNumLen ; i ++)
		//	TRACE(MSGL_VGSM_INFO, "SCAddr = %d [%02x] \n", i, szDiallingNum[i]);

	    } else{
		/* */
		TRACE(MSGL_VGSM_INFO, "No SCAAddr\n");
	    }
	}

    }else{
	/* */
    }

    /* msg-service/include/common/MsgSettingTypes.h */

    /* TEXT(0), VOICE(36), FAX(33), Email(34) */
    if( (ParamIndicator & SMSPValidPID) == 0 &&
	    (alpha_id_len + nPIDOffset) < MAX_GSM_SMS_PARAM_RECORD_SIZE)
    {
	TpProtocolId = p[alpha_id_len + nPIDOffset];
    }

    if( (ParamIndicator & SMSPValidDCS) == 0 &&
	    (alpha_id_len + nDCSOffset) < MAX_GSM_SMS_PARAM_RECORD_SIZE)
    {
	TpDataCodingScheme = p[alpha_id_len + nDCSOffset];
    }

    /*  1DAY = 167, 2DAYS = 168,1WEEK = 173,2WEEKS = 180, MAXIMUM = 255 */
    if( (ParamIndicator & SMSPValidVP) == 0 &&
	    (alpha_id_len + nVPOffset) < MAX_GSM_SMS_PARAM_RECORD_SIZE)
    {
	TpValidityPeriod = p[alpha_id_len + nVPOffset];
    }

    TRACE(MSGL_VGSM_INFO, " Alpha Id(Len) = %d \n",(int)AlphaIdLen);
    TRACE(MSGL_VGSM_INFO, " Alpha Id = [%s] \n", szAlphaId);

    TRACE(MSGL_VGSM_INFO, " PID = %d \n",TpProtocolId);
    TRACE(MSGL_VGSM_INFO, " DCS = %d \n",TpDataCodingScheme);
    TRACE(MSGL_VGSM_INFO, " VP = %d \n",TpValidityPeriod);

    if (vconf_set_str(SMSC_NAME"/0", szAlphaId) != 0 ){
	TRACE(MSGL_VGSM_INFO, "[FAIL] vconf error: [%s] => [%s]" 
		, SMSC_NAME"/0", szAlphaId);
    }
    if (vconf_set_str(SMSC_ADDRESS"/0", smscAddr) != 0 ){
	TRACE(MSGL_VGSM_INFO, "[FAIL] vconf error: [%s]=> [%s]" 
		, SMSC_ADDRESS"/0", smscAddr);
    }
    if (vconf_set_int(SMSC_TON"/0", Ton) != 0 ){
	TRACE(MSGL_VGSM_INFO, "[FAIL] vconf error: [%s] => [%d]" 
		, SMSC_TON"/0", Ton);
    }
    if (vconf_set_int(SMSC_NPI"/0", Npi) != 0 ){
	TRACE(MSGL_VGSM_INFO, "[FAIL] vconf error: [%s] => [%d]" 
		, SMSC_NPI"/0", Npi);
    }
    if (vconf_set_int(SMSC_PID"/0", (int)convertPid(TpProtocolId)) != 0 ){
	TRACE(MSGL_VGSM_INFO, "[FAIL] vconf error: [%s] => [%d]" 
		, SMSC_PID"/0", (int)convertPid(TpProtocolId));
    }
    if (vconf_set_int(SMSC_VAL_PERIOD"/0", TpValidityPeriod) != 0 ){
	TRACE(MSGL_VGSM_INFO, "[FAIL] vconf error: [%s] => [%d]" 
		, SMSC_VAL_PERIOD"/0", TpValidityPeriod);
    }

    return 1;
}

#endif
