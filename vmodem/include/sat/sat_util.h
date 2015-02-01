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

#ifndef	_SAT_UTIL_H_
#define _SAT_UTIL_H_

#include "sat_values.h"

//#include <linuxtapi.h>

#define 	SIM_TON_UNKNOWN					0 //unknown
#define	SIM_TON_INTERNATIONAL			 	1 //international number
#define	SIM_TON_NATIONAL				        2 //national number
#define  	SIM_TON_NETWORK_SPECIFIC		 	3 //natwork specific number
#define	SIM_TON_DEDICATED_ACCESS		 	4 //subscriber number
#define	SIM_TON_ALPHA_NUMERIC			 	5//alphanumeric, GSM 7-bit default alphabet)
#define	SIM_TON_ABBREVIATED_NUMBER		6 //abbreviated number
#define	SIM_TON_RESERVED_FOR_EXT		 	7 //reserved for extension


#define MAX_UDH_LEN		137
#define UDH_MAX				7
#define SMDATA_SIZE_MAX 	160


#define FAIL			0x00
#define SUCCESS 		0x01





//////////////////////////////////TPDU type////////////////////////////////
#if 0
typedef enum
{
    SMS_TPDU_DELIVER				= 0x00, // sc -> ms
    SMS_TPDU_DELIVER_REPORT		= 0x00, // ms -> sc
    SMS_TPDU_SUBMIT				= 0x01, // ms -> sc
    SMS_TPDU_SUBMIT_REPORT		= 0x01, // sc -> ms
    SMS_TPDU_STATUS_REPORT		= 0x02, // sc -> ms
    SMS_TPDU_COMMAND				= 0x02, // ms -> sc
    SMS_PDU_INVALID_TYPE

} SmsTPDUType;

typedef struct
{
    int		year;
    int		month;
    int		day;
    int		hour;
    int		minute;
    int		second;

} TmDateTime;
#endif

#define BASE_YEAR		70L					/* 1970 is the base year */
#define MAX_YEAR			130L					/* 2030 is the max year */


#define	SIM_SMSP_ADDRESS_LEN					20	// digit length

#define SET_TON_NPI(dest, ton, npi) {	\
    dest = 0x80;						\
    dest |= (ton & 0x07) << 4;			\
    dest |= npi & 0x0F;					\
}


int   Display_Text( DP_TEXT* dp_text);
int   Get_Inkey( GET_INKEY* get_inkey);
int   Get_Input( GET_INPUT* get_input);
int   Set_Menu(void *data);
int   Select_Item(void *data);
int   Send_SMS(void *data);
int   Set_Up_Call(void *data);

int sat_apdu_encode(unsigned char tag, SatApduEncoderCallback getDataF, void* HLDataP, char** stringPP);


#if 0


typedef struct
{
    /************HEADER************/

    // message type [M]
    SmsTPDUType  msgType ;   //bit 0 ,1

    // reject duplicates [M]
    BOOL rd ;  // bit 2

    // validity period format [M]
    TapiNetTextVPType vpf ; //  bits 3, 4

    // reply path [M]
    BOOL rp ; //bit 7

    // user data header indication
    BOOL udhi ;   //bit 6

    // status report request
    BOOL  srr ;  // bit 5

    /************BODY************/

    //message reference [M]
    UINT mr;

    // destination address [M]
    SmsAddressInfo desAddr;

    // protocol identifier [M]
    BYTE pId;

    // data coding scheme [M]
    TapiNetTextCodingScheme dcs;

    // validity period
    TapiNetTextVP vp;

    // user data length [M]
    UINT udl;

    // user data
    BYTE *  userData;


}TPDU_SMS_SUBMIT;

typedef struct
{
    /************HEADER************/

    // message type [M]
    SmsTPDUType  msgType;  //bit 0 ,1

    // user data header indication
    BOOL udhi ;   //bit 6

    /************BODY************/

    // failure Cause [M]
    UINT	fcs;

    // parameter indicator [M]
    BYTE paraInd;

    // service centre time stamp [M]
    TmDateTime scts;

    // protocol identifier [M]
    BYTE pId;

    // data coding scheme
    TapiNetTextCodingScheme dcs;

    // user data length [M]
    UINT udl;

    // user data
    BYTE *  userData;

}TPDU_SMS_SUBMIT_REPORT;


typedef struct
{	
    /************HEADER************/
    SmsTPDUType msgType;   //bit 0 ,1

    BOOL mms; // bit 2

    BOOL rp; // bit 7

    BOOL udhi;  //bit 6

    BOOL sri; // bit , 5status_report_indication

    /************BODY************/

    SmsAddressInfo orgAddr;

    BYTE  pId;

    TmDateTime scts;

    TapiNetTextCodingScheme dcs;

    UINT udl;

    BYTE  * userData;

}TPDU_SMS_DELIVER;

typedef struct
{
    /************HEADER************/
    SmsTPDUType msgType ;   //bit 0 ,1

    BOOL udhi ;  //bit 6

    /************BODY************/

    UINT fcs;

    BYTE paraInd;

    BYTE pId;

    TapiNetTextCodingScheme dcs;

    UINT udl;

    BYTE * userData;

}TPDU_SMS_DELIVER_REPORT;


typedef struct
{
    /************HEADER************/
    SmsTPDUType msgType ;   //bit 0 ,1

    BOOL udhi ; //bit 6

    BOOL srr; //bit 5, status_report_request

    /************BODY************/

    UINT  mr; //message_ref

    BYTE pId;

    BYTE cmdType;

    BYTE msgNum;

    SmsAddressInfo destAddr;

    UINT udl;

    BYTE * userData;

}TPDU_SMS_COMMAND;


typedef struct
{
    /************HEADER************/

    SmsTPDUType msgType ;   //bit 0 ,1

    BOOL  udhi ; //bit 6

    BOOL mms ; //bit 2

    BOOL srq; //bit 5, status_report_qualifier

    /************BODY************/

    UINT  mr;

    SmsAddressInfo rcpAddr; //recipient_addr

    TmDateTime scts;

    TmDateTime dt; //discharge_time

    BYTE  status;

    BYTE paraInd;

    BYTE pId;

    TapiNetTextCodingScheme dcs;

    UINT udl;

    BYTE * userData;

}TPDU_SMS_STATUS_REPORT;

#endif

/**************************************************************************************
 *** (main.c) function header****************************************************************
 ***************************************************************************************
 ***************************************************************************************/
#if 0
BOOL EncodeSmsDeliverTpdu(SmsAddressInfo SCA, TPDU_SMS_DELIVER tpdu_deliver, char *rawdata, int *rawdata_len);
//BOOL EncodeSmsDeliverTpdu(char *rawdata, int *rawdata_len, char* diallingNum, int dialNumLen, char* msg, int msg_len) ;
BOOL EncodeSmsSubmitReportTpdu(char *rawdata, int *rawdata_len);
BOOL DecodeSmsSubmitTpdu(TPDU_SMS_SUBMIT *tpdu_submit, int pdu_len , char * pPDU);
#endif
/*
   int SendMessage();
   int ReadMessage();
   int DeleteMessage();
   int ReceiveMessage(int pdu_len, char * pPDU);
   int GetInformation();
   int Setting();

   BOOL DecodeCellBroadcastMsg(int length,char * pPDU);
   BOOL EncodeSmsSubmitTpdu(char* diallingNum, int dialNumLen, char* msg, int msg_len) ;
//BOOL DecodeSmsSubmitTpdu(int pdu_len , char * pPDU);
int  DecodeSmsDeliverTpdu(int pdu_len , char * pPDU);
BOOL EncodeSmsDeliverReportTpdu();
BOOL DecodeSmsSubmitReportTpdu(int length , char *data);
BOOL DecodeSmsStatusReportTpdu(int pdu_len, char * pPDU);
 */

/***************************************************************************************/
#if 0
void reverse(char* x, int len);
char* AcItoa(int n, char* str, int b) ;
int AcToupper(int ch);

char* SmsUtilUnpackGSMCode( char* szData, const BYTE* pIn, int in_len );
int SmsUtilPackGSMCode( BYTE* pOut, const char* szData, int in_len );

void SmsUtilConvertBCD2Digit( char* pDigits, char* pBCD, int digitLen );
void SmsUtilConvertDigit2BCD( char* pBCD, char* pDigits, int digitLen );

TmDateTime* SmsUtilDecodeTimeStamp( unsigned char* pTimeStamp, TmDateTime *tmDateTime );
unsigned char* SmsUtilEncodeTimeStamp( TmDateTime* tmDateTime, unsigned char* pTimeStamp );


void  SmsUtilDecodeAddrField(char *diallingNum, unsigned char * pAddrField, int *result_ton, int *result_npi );
int SmsUtilEncodeAddrField( unsigned char* pAddrField, unsigned char* diallingNum, int dialnumLen, int ton, int npi );

int SmsUtilDecodeScAddrField( SmsAddressInfo* pSmsAddrField, BYTE* pAddrField );
int SmsUtilEncodeScAddrField( BYTE* pAddrField, SmsAddressInfo * pSmsAddrField );

void SmsUtilDecodeDCS( TapiNetTextCodingScheme* pCodingScheme,   unsigned char dcs );
void SmsUtilEncodeDCS( BYTE* pDCS, TapiNetTextCodingScheme* pCodingScheme);

UINT8 SmsUtilEncodeValidity( BYTE* pValidity, TapiNetTextVP* pVP );
#endif

#endif /* _SAT_UTIL_H_ */

