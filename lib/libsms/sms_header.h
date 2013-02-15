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

#ifndef	_SMS_HEADER_H_
#define _SMS_HEADER_H_

typedef unsigned char BYTE;
typedef unsigned short UINT8;
typedef unsigned short UINT;
typedef unsigned int UINT32;
typedef int BOOL;

#define TAPI_NETTEXT_CB_SIZE_MAX	    255
#define TAPI_NETTEXT_MSG_SIZE_MAX	    500
#define TAPI_NETTEXT_SCADDRESS_LEN_MAX	    20
#define TAPI_NETTEXT_SMDATA_SIZE_MAX	    255
#define TAPI_NETTEXT_ADDRESS_LEN_MAX	    255

#ifndef	TRUE
#define TRUE	1
#endif /* TRUE */

#ifndef FALSE
#define FALSE	0
#endif /* FALSE */

/**
 * @enum TapiNetTextVPType
 * This enumeration defines the validity period type.
 */
typedef enum
{
	TAPI_NETTEXT_VP_NOT_USED = 0,		/**< validity period not used */
	TAPI_NETTEXT_VP_RELATIVE = 2,		/**< after a predetermined time period after the service center has received the message */
	TAPI_NETTEXT_VP_ABSOLUTE = 3,
	TAPI_NETTEXT_VP_ENHANCED = 1,

	TAPI_NETTEXT_VP_MAX
} TapiNetTextVPType;

/**
 * @enum TapiNetTextVPRelValue
 * This structure defines the validity period type.
 */
typedef enum
{
	TAPI_NETTEXT_VP_REL_1H,		/**< One hour */
	TAPI_NETTEXT_VP_REL_6H,		/**< Six hours */
	TAPI_NETTEXT_VP_REL_12H,	/**< Twelve hours */
	TAPI_NETTEXT_VP_REL_1D,		/**< One day */
	TAPI_NETTEXT_VP_REL_3D,		/**< Three days */
	TAPI_NETTEXT_VP_REL_1W,		/**< One week */
	TAPI_NETTEXT_VP_REL_1M,		/**< One Month */
	TAPI_NETTEXT_VP_REL_MAX		/**< Max = 63 weeks */
} TapiNetTextVPRelValue;

typedef struct
{
	TapiNetTextVPType vpType;			  /**< Validity period format */
	TapiNetTextVPRelValue vpRelativeType; /**< Validity period value duration the message must retain in the service center while application attempts delivery */
	UINT32 vpValue;						  /** Validity period value */
} TapiNetTextVP;

/**
 * @enum TapiNetTextCodingGroupType
 */
typedef enum
{
	TAPI_NETTEXT_CODGRP_SM_GENERAL_DCS,			/**< Bits 7..4 00xx */
	TAPI_NETTEXT_CODGRP_CB_GENERAL_LANG,		/**< 0000 */
	TAPI_NETTEXT_CODGRP_CB_ALPHA,				/**< 0001 */
	TAPI_NETTEXT_CODGRP_CB_RES1,				/**< 0010 */
	TAPI_NETTEXT_CODGRP_CB_RES2,				/**< 0011 */
	TAPI_NETTEXT_CODGRP_SM_AUTO_DELETION,		/**< 01xx Bits 5..0 are coded the same as SM_GENERAL_DCS */
	TAPI_NETTEXT_CODGRP_CB_GENERAL_DCS,			/**< 01xx behavior is as SM_GENERAL_DCS */
	TAPI_NETTEXT_CODGRP_SM_WAITING_DISCARD,		/**< 1100 */
	TAPI_NETTEXT_CODGRP_SM_WAITING_STORE,		/**< 1101 */
	TAPI_NETTEXT_CODGRP_SM_WAITING_STORE_UCS2,	/**< 1110 */
	TAPI_NETTEXT_CODGRP_CB_WAP,					/**< 1110 Cell Broadcast */
	TAPI_NETTEXT_CODGRP_SM_CLASS_CODING,		/**< 1111 */
	TAPI_NETTEXT_CODGRP_CB_CLASS_CODING,		/**< 1111 Cell Broadcast */
	TAPI_NETTEXT_CODGRP_SM_RESERVED,			/* reserved coding group */
	TAPI_NETTEXT_CODGRP_MAX
} TapiNetTextCodingGroupType;

/**
 * @enum TapiNetTextWaitingType
 */
typedef enum
{
	TAPI_NETTEXT_WAITING_VOICE_MSG,		/**< voice message waiting */
	TAPI_NETTEXT_WAITING_FAX_MSG,		/**< FAX message waiting */
	TAPI_NETTEXT_WAITING_EMAIL_MSG,		/**< E-mail waiting */
	TAPI_NETTEXT_WAITING_OTHER_MSG,		/**< Other message waiting */

	TAPI_NETTEXT_WAITING_MAX
} TapiNetTextWaitingType;

/**
 * @enum TapiNetTextAlphabetType
 */
typedef enum
{
	TAPI_NETTEXT_ALPHABET_DEFAULT,	/**< GSM alphabet default 7bit */
	TAPI_NETTEXT_ALPHABET_8BIT,		/**< 8bit */
	TAPI_NETTEXT_ALPHABET_UCS2,		/**< 16bit ucs2 */

	TAPI_NETTEXT_ALPHABET_KSC5601,	/**< for Korean */

	TAPI_NETTEXT_ALPHABET_MAX
} TapiNetTextAlphabetType;

/**
 * @enum TapiNetTextClassType
 */
typedef enum
{
	TAPI_NETTEXT_CLASS_0,	/**< display immediately */
	TAPI_NETTEXT_CLASS_1,	/**< may be stored */
	TAPI_NETTEXT_CLASS_2,	/**< store in SIM */
	TAPI_NETTEXT_CLASS_3,	/**< transfer to TE */
	TAPI_NETTEXT_CLASS_NONE,

	TAPI_NETTEXT_CLASS_MAX
} TapiNetTextClassType;

typedef struct
{
	BOOL							bSMSCinSIM;			/**< if SMSC is stored in SIM this is TRUE */
	BOOL							bCompressed;		/**< if text is compressed this is TRUE */
	BOOL							bMsgClassSet;		/**< message class indication. if TRUE = bits 1..0, if FALSE = reserved */
	BOOL							bMsgIndActive;		/**< message indication group. if TRUE = on, FALSE = off */
	TapiNetTextCodingGroupType		codingGroupType;	/**< Coding group, GSM 03.38 */
	TapiNetTextWaitingType			waitingType;		/**< Type of message waiting */
	TapiNetTextAlphabetType			alphabetType;		/**< How to encode a message. */
	TapiNetTextClassType			classType;			/**< The message class */
	BYTE					codingGroup;		/* reserved coding group 7...4 */
	BYTE					code;			/* code 3...0 */

} TapiNetTextCodingScheme;

////////////////////////////////Adddress Field/////////////////////////////////
#define ADDRESS_LEN_MAX 	20

//type of number
typedef enum
{
	TYPEOFNUMBER_UNKNOWN                          			=0x00,
	TYPEOFNUMBER_INTERNATION_NUMER             		=0x01,
	TYPEOFNUMBER_NATIONAL_NUMBER                  		=0x02,
	TYPEOFNUMBER_NETWORK_SPECIFIC_NUMBER    	=0x03,
	TYPEOFNUMBER_SUBSCRIBER_NUMBER              		=0x04,
	TYPEOFNUMBER_ALPHANUMERIC                     		=0x05,
	TYPEOFNUMBER_ABBREVIATED                      		=0x06,
	TYPEOFNUMBER_RESERVED_FOR_EXTENSION     	=0x07
	
}NumberType;

// Numbering-plan-identification (applies for type of number 000,001,010)
typedef enum
{
	NUMBERING_PLAN_IDENTIFICATION_UNKNOWN                 	=0x00,
	NUMBERING_PLAN_IDENTIFICATION_ISDN_TELEPHONE         	=0x01,
	NUMBERING_PLAN_IDENTIFICATION_DATA                    		=0x03,
	NUMBERING_PLAN_IDENTIFICATION_TELEX                   		=0x04,
	NUMBERING_PLAN_IDENTIFICATION_NATIONAL                	=0x08,
	NUMBERING_PLAN_IDENTIFICATION_PRIVATE                 		=0x09,
	NUMBERING_PLAN_IDENTIFICATION_ERMES                   		=0x0A, 	// refer 3GPP_ts_23.040-9.1.2.5
	NUMBERING_PLAN_IDENTIFICATION_RESERVED                	=0x0F
	
}NumberingPlan;

typedef struct 
{
	NumberType 	        ton;
	
	NumberingPlan	        npi;
		
	UINT32	      		dialnumLen;
	
	unsigned char		diallingNum[ADDRESS_LEN_MAX+1];
	
} SmsAddressInfo;

#endif /* _SMS_HEADER_H_ */
