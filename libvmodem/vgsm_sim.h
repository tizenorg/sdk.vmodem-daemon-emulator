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

#ifndef __VGSM_SIM_H__
#define __VGSM_SIM_H__

/* ICC TYPE field */
typedef enum{
    GSM_SEC_SIM_UNKNOWN	=0x00,	/* 0x00 : UNKNOWN	*/
    GSM_SEC_SIM_2GGSM,		/* 0x01 : 2G GSM		*/
    GSM_SEC_SIM_3G,			/* 0x02 : 3G			*/
    GSM_SEC_SIM_MAX
} gsm_sec_sim_icc_e_type;


/* SIM SECURITY STATUS */
typedef enum{
    GSM_SEC_STATUS_PIN_READY,		/* 0x00 : Not necessary */
    GSM_SEC_STATUS_PIN_REQ,         		/* 0x01 : PIN required as a password */
    GSM_SEC_STATUS_PUK_REQ,         		/* 0x02 : PUK required as a password */
    GSM_SEC_STATUS_PIN2_REQ,       		/* 0x03 : PIN2 required as a password */
    GSM_SEC_STATUS_PUK2_REQ,       		/* 0x04 : PUK2 required as a password */
    GSM_SEC_STATUS_PERM_BLOCKED,	/* 0x05 : PIN Permanent Blocked */   // JBG 2005.05.18
    GSM_SEC_STATUS_PIN2_READY,		/* 0x06 : PIN2 Lock Disabled*/
    GSM_SEC_STATUS_NO_SIM,			/* 0x07 : SIM isn`t inserted	*/
    GSM_SEC_STATUS_MAX
} gsm_sec_status_type;


/* LOCK_TYPE  field */
typedef enum{
    GSM_SEC_LOCK_TYPE_READY,			/* 0x00 : ME is not locked */
    GSM_SEC_LOCK_TYPE_PS,				/* 0x01 : PH-SIM, Lock Phone to SIM/UICC card
							   ( MT asks password when other than current SIM/UICC card inserted;
							   MT may remember certain amount of previously used cards thus not
							   requiring password when they are inserted ) */
    GSM_SEC_LOCK_TYPE_PF,				/* 0x02 : PH-FSIM, Lock Phone to the very First inserted SIM/UICC card
							   ( MT asks password when other than the first SIM/UICC card is inserted ) */
    GSM_SEC_LOCK_TYPE_SC,				/* 0x03 : Lock SIM/UICC card ( SIM asks password in ME power-up and when this command is issued ) */
    GSM_SEC_LOCK_TYPE_FD,			/* 0x04 : SIM card or active application in the UICC (GSM or USIM) fixed dialing memory feature */
    GSM_SEC_LOCK_TYPE_PN,			/* 0x05 : Network Personalization */
    GSM_SEC_LOCK_TYPE_PU,			/* 0x06 : Network subset Personalization */
    GSM_SEC_LOCK_TYPE_PP,				/* 0x07 : Service Provider Personalization */
    GSM_SEC_LOCK_TYPE_PC,				/* 0x08 : Corporate Personalization */
    GSM_SEC_LOCK_TYPE_SC2,			/* 0x09 : Lock PIN2 ( ... ) */
    GSM_SEC_LOCL_TYPE_PUK2,			/* 0x0A : Lock PUK2 (... ) */
    GSM_SEC_LOCK_TYPE_ACL,			/* 0x0B: ACL */
    GSM_SEC_LOCL_TYPE_PUK,			/* 0x0C : Lock PUK (... ) 090215*/

    /* 0x0a - 0x7F are Reserved */

    /* SIM Status Information which are used only in GSM_SEC_PIN_STATUS Noti */
    GSM_SEC_LOCK_TYPE_NO_SIM=0x80,	/* 0x80 : SIM is not inserted */
    GSM_SEC_LOCK_TYPE_UNAVAIL,		/* 0x81 : SIM is inserted but can not communicate with SIM ( SIM interface error ) */
    GSM_SEC_SIM_INIT_COMPLETED,		/* 0x082 : SIM Initialize Completed */  // JBG 2005.05.18
    GSM_SEC_PB_INIT_COMPLETED,		/* 0x83 : Phonebook Initialize Completed*/

    GSM_SEC_LOCK_TYPE_MAX
}gsm_sec_lock_e_type;


/* LOCK_MODE  field */
/* The enable or disable of facility */
typedef enum{
    GSM_SEC_LOCK_MODE_UNLOCK,	/* 0x00 : Disable	*/
    GSM_SEC_LOCK_MODE_LOCK,		/* 0x01 : Enable	*/
    GSM_SEC_LOCK_MODE_MAX
} gsm_sec_lock_mode_e_type;


/* LOCK_KEY */
typedef enum{
    GSM_SEC_LOCK_KEY_UNLOCKED,		/* 0x00 : Not necessary */
    GSM_SEC_LOCK_KEY_PIN,				/* 0x01 : PIN required as a password */
    GSM_SEC_LOCK_KEY_PUK,			/* 0x02 : PUK required as a password */
    GSM_SEC_LOCK_KEY_PIN2,			/* 0x03 : PIN2 required as a password */
    GSM_SEC_LOCK_KEY_PUK2,			/* 0x04 : PUK2 required as a password */
    GSM_SEC_LOCK_KEY_PERM_BLOCKED,	/* 0x05 : PIN Permanent Blocked */   // JBG 2005.05.18
    GSM_SEC_LOCK_KEY_PIN2_DISABLE,	/* 0x06 : PIN2 Lock Disabled*/
    GSM_SEC_LOCK_KEY_MAX
} gsm_sec_lock_key_e_type;


/* ACCESS_CMD  Field : see  3GPP TS11.11, TS27.07 */
typedef enum {
    GSM_SEC_SIM_ACC_READ_BINARY		=	0xB0,	/*Reads a string of bytes from the current transparent EF.*/
    GSM_SEC_SIM_ACC_READ_RECORD	=	0xB2,	/*Reads one complete record in the current linear fixed or cyclic EF.*/
    GSM_SEC_SIM_ACC_GET_RESPONSE	=	0xC0,	/*The response data depends on the preceding command.*/
    GSM_SEC_SIM_ACC_UPDATE_BINARY	=	0xD6,	/*Updates the current transparent EF with a string of bytes.*/
    GSM_SEC_SIM_ACC_UPDATE_RECORD	=	0xDC,	/*Updates one complete record in the current linear fixed or cyclic EF.*/
    GSM_SEC_SIM_ACC_STATUS			=	0xF2,	/*Information concerning the current directory.*/
    GSM_SEC_SIM_ACC_MAX
} gsm_sec_sim_access_e_type;


typedef enum
{
    /* common Elementary files */
    GSM_SEC_EFILE_DIR			= 0x2F00, 		/**<  for the USIM					*/
    GSM_SEC_EFILE_ICCID 		= 0x2FE2,		/**< the ICC Identification file		*/
    GSM_SEC_EFILE_ADN			= 0x6F3A,		/**< the Abbrv. Dialing Num file		*/
    GSM_SEC_EFILE_FDN			= 0x6F3B,		/**< the Fixed Dialing Num file		*/
    GSM_SEC_EFILE_SMS			= 0x6F3C,		/**< the Short Messages file   		*/
    GSM_SEC_EFILE_CCP			= 0x6F3D,		/**< the Capab. Config Param file	*/
    GSM_SEC_EFILE_IMSI		= 0x6F07,		/**< the IMSI file                		*/
    GSM_SEC_EFILE_SST			= 0x6F38,		/**< the SIM Service Table file   		*/
    GSM_SEC_EFILE_EST			= 0x6F56,	
    GSM_SEC_EFILE_ACM			= 0x6F39,		/**< the Accumul. Call Meter file 		*/
    GSM_SEC_EFILE_MBDN		= 0x6FC7,		/** < SIM Mail Box Dailing Number file */

    GSM_SEC_EFILE_GID1		= 0x6F3E,		/**< the Group Identifier Level 1		*/
    GSM_SEC_EFILE_GID2		= 0x6F3F,		/**< the Group Identifier Level 2		*/

    /* For PHASE 2 only */
    GSM_SEC_EFILE_MSISDN		= 0x6F40,		/**< the MSISDN file              		*/
    GSM_SEC_EFILE_SMSP		= 0x6F42,		/**< the Short Msg Srv. Param file	*/
    GSM_SEC_EFILE_SMSS		= 0x6F43,		/**< the SMS Status				*/
    GSM_SEC_EFILE_EXT1		= 0x6F4A,		/**< the Extension 1 file			*/
    GSM_SEC_EFILE_EXT2		= 0x6F4B,		/**< the Extension 2 file			*/
    GSM_SEC_EFILE_LP			= 0x2F05,		/**< the Language Preference file	*/
    GSM_SEC_EFILE_ACM_MAX	= 0x6F37,		/**< the ACM maximum value file	*/
    GSM_SEC_EFILE_PUCT		= 0x6F41,		/**< the Price per Unit and. file		*/
    GSM_SEC_EFILE_CBMI		= 0x6F45,		/**< the Cell Brd msg Id sel file		*/
    GSM_SEC_EFILE_PHASE		= 0x6FAE,		/**< the PHASE identification file		*/

    /* For Phase 2+ */
    GSM_SEC_EFILE_SDN			= 0x6F49,		/**< the Service Dialing Numbers	*/
    GSM_SEC_EFILE_EXT3		= 0x6F4C,		/**< the Extension 3 file         		*/
    GSM_SEC_EFILE_BDN			= 0x6F4D,		/**< the BDN file                 		*/
    GSM_SEC_EFILE_EXT4		= 0x6F4E,		/**< the Extension 4 file       		*/
    GSM_SEC_EFILE_ECC			= 0x6FB7,		/**< the Emergency Call Codes		*/
    GSM_SEC_EFILE_SPN			= 0x6F46,		/**< the Service Provider Name		*/
    GSM_SEC_EFILE_PNN			= 0x6FC5,		/**< the Service Provider Name		*/
    GSM_SEC_EFILE_OPL           = 0x6FC6,
    GSM_SEC_EFILE_SPDI          = 0x6FCD,

    /* Contents of files at the DF GRAPHICS level */
    GSM_SEC_EFILE_IMG			= 0x4F20,		/**< image */

    /* 4.2 Contents of files at the USIM ADF (Application DF) level */
    GSM_SEC_EFILE_USIM_LI		= 0x6F05,		/**< Language Indication */

#if 1	
    GSM_SEC_EFILE_USIM_IMSI			= 0x6A07,			/**< IMSI */
    GSM_SEC_EFILE_USIM_ACM_MAX		= 0x6A37,			/**< ACM maximum value */
    GSM_SEC_EFILE_USIM_UST			= 0x6A38,			/**< USIM Service Table */
    GSM_SEC_EFILE_USIM_ACM			= 0x6A39,			/**< Accumulated Call Meter */
    GSM_SEC_EFILE_USIM_SPN			= 0x6A46,			/**< Service Provider Name */
    GSM_SEC_EFILE_USIM_PUCT			= 0x6A41,			/**< Price per Unit and Currency Table */
    GSM_SEC_EFILE_USIM_CBMI			= 0x6A45,			/**< Cell Broadcaster Message Identifier selection */
    GSM_SEC_EFILE_USIM_ECC			= 0x6AB7,			/**< Emergency Call Codes */
    GSM_SEC_EFILE_USIM_FDN			= 0x6A3B,
    GSM_SEC_EFILE_USIM_SMS			= 0x6A3C,			/** < My own number */
    GSM_SEC_EFILE_USIM_MSISDN		= 0x6A40,
    GSM_SEC_EFILE_USIM_SMSP			= 0x6A42,			/** < SMS parameter */
    GSM_SEC_EFILE_USIM_SMSS			= 0x6A43,			/** < SMS status */
    GSM_SEC_EFILE_USIM_SDN			= 0x6A49,			/** < Service dialing number */
    GSM_SEC_EFILE_USIM_EXT1			= 0X6A4A,			/** < EXT1 record file */
    GSM_SEC_EFILE_USIM_EXT2			= 0x6A4B,			/** < EXT2 record file */
    GSM_SEC_EFILE_USIM_EXT3			= 0x6A4C,			/** < EXT3 record file */
    GSM_SEC_EFILE_USIM_EXT4			= 0x6A55,			/** < EXT4 record file */
    GSM_SEC_EFILE_USIM_EXT5			= 0x6A4E,			/**< Extension 5 */
#endif	
    GSM_SEC_EFILE_USIM_EXT6			= 0x6FC8,			/** < EXT6 record file */
    GSM_SEC_EFILE_USIM_CCP2			= 0x6A4F,			/**< Capability Configuration Parameters 2 */
    GSM_SEC_EFILE_USIM_HIDDEN_KEY	= 0x6FC3,			/**< Key for hidden phonebook entries */
    GSM_SEC_EFILE_USIM_BDN			= 0x6A4D,			/**< Barred Dialing Numbers */
    GSM_SEC_EFILE_USIM_MBDN			= 0x6FC7, 			/** < USIM Mail box dialing number */
    GSM_SEC_EFILE_USIM_MBI			= 0x6FC9,			/** < Mailbox Identifier */

    GSM_SEC_EFILE_USIM_APN_ACL		= 0x6F57,			/** < Access point name control list */

    /* Contents of files at USIM ADF PHONEBOOK level */
    GSM_SEC_EFILE_USIM_PHONEBOOK_PBR	= 0x4FF0,		/**< Phonebook Reference File */
    GSM_SEC_EFILE_USIM_PHONEBOOK_CCP1	= 0x4FFE,		/**< Capability Configuration Parameters 1 */
    GSM_SEC_EFILE_USIM_PHONEBOOK_UID	= 0x4FF8,		/**< unique identifier */
    GSM_SEC_EFILE_USIM_PHONEBOOK_PSC	= 0x4FFF,		/**< phonebook synchronization counter */
    GSM_SEC_EFILE_USIM_PHONEBOOK_CC	= 0x4FEF,		/**< change counter */
    GSM_SEC_EFILE_USIM_PHONEBOOK_PUID	= 0x4FEE,		/**< previous unique identifier */

    GSM_SEC_EFILE_USIM_PHONEBOOK_ADN	= 0x4FF1,		/**< Abbreviated Dialing Number */
    GSM_SEC_EFILE_USIM_PHONEBOOK_IAP	= 0x4FF2,		/**< Index Administration Phonebook */
    GSM_SEC_EFILE_USIM_PHONEBOOK_PBC	= 0x4FF3,		/**< Phonebook Control */
    GSM_SEC_EFILE_USIM_PHONEBOOK_EXT1	= 0x4FFB,		/**< Extension 1 */
    GSM_SEC_EFILE_USIM_PHONEBOOK_GRP	= 0x4FF7,		/**< Grouping File */

    GSM_SEC_EFILE_USIM_PHONEBOOK_AAS	= 0x4FFC,		/**< Additional number Alpha String */
    GSM_SEC_EFILE_USIM_PHONEBOOK_AAS1	= 0x4FFD,		/**< Additional number Alpha String */

    GSM_SEC_EFILE_USIM_PHONEBOOK_GAS	= 0x4F0E,		/**< Additional Number */
    GSM_SEC_EFILE_USIM_PHONEBOOK_GAS1	= 0x4F0F,		/**< Second Name Entry */

    GSM_SEC_EFILE_USIM_PHONEBOOK_ANR	= 0x4FF4,		/**< Additional Number */
    GSM_SEC_EFILE_USIM_PHONEBOOK_SNE	= 0x4FF9,		/**< Second Name Entry */
    GSM_SEC_EFILE_USIM_PHONEBOOK_EMAIL	= 0x4FFA,		/**< e-mail address */
#if 1
    GSM_SEC_EFILE_USIM_PHONEBOOK_ANRA	= 0x4F13,		/**< Additional Number */
    GSM_SEC_EFILE_USIM_PHONEBOOK_ANRB	= 0x4F14,		/**< Additional Number */
    GSM_SEC_EFILE_USIM_PHONEBOOK_ANRC	= 0x4F15,		/**< Additional Number */
    GSM_SEC_EFILE_USIM_PHONEBOOK_ANR1	= 0x4F16,		/**< Additional Number */
    GSM_SEC_EFILE_USIM_PHONEBOOK_ANRA1	= 0x4F17,	/**< Additional Number */
    GSM_SEC_EFILE_USIM_PHONEBOOK_ANRB1	= 0x4F18,	/**< Additional Number */
    GSM_SEC_EFILE_USIM_PHONEBOOK_ANRC1	= 0x4F21,	/**< Additional Number */

    GSM_SEC_EFILE_USIM_PHONEBOOK_ADN1	= 0x4F1A,		/**< Abbreviated Dialing Number */
    GSM_SEC_EFILE_USIM_PHONEBOOK_PBC1	= 0x4F1B,		/**< Phonebook Control */
    GSM_SEC_EFILE_USIM_PHONEBOOK_GRP1	= 0x4F1C,		/**< Grouping File */
    GSM_SEC_EFILE_USIM_PHONEBOOK_SNE1	= 0x4F1D,		/**< Second Name Entry */
    GSM_SEC_EFILE_USIM_PHONEBOOK_UID1	= 0x4F1E,		/**< unique identifier */

    GSM_SEC_EFILE_USIM_PHONEBOOK_EMAIL1	= 0x4F1F,	/**< e-mail address */
    GSM_SEC_EFILE_USIM_PHONEBOOK_IAP1	= 0x4FF5,		/**< Index Administration Phonebook */
#endif
    /* Contents of files at TELECOM DF PHONEBOOK level */
    GSM_SEC_EFILE_TELECOM_PHONEBOOK_PBR		= 0x4F30,	/**< Phonebook Reference File */
    GSM_SEC_EFILE_TELECOM_PHONEBOOK_PBR1		= 0x4F29,	/**< Phonebook Reference File 1 */
    GSM_SEC_EFILE_TELECOM_PHONEBOOK_PBR2		= 0x4F28,	/**< Phonebook Reference File 1 */
    GSM_SEC_EFILE_TELECOM_PHONEBOOK_CCP1		= 0x4F3D,	/**< Capability Configuration Parameters 1 */
    GSM_SEC_EFILE_TELECOM_PHONEBOOK_UID		= 0x4F00,	/**< unique identifier */
    GSM_SEC_EFILE_TELECOM_PHONEBOOK_PSC		= 0x4F22,	/**< Phonebook synchronization counter */
    GSM_SEC_EFILE_TELECOM_PHONEBOOK_CC			= 0x4F23,		/**< change counter */
    GSM_SEC_EFILE_TELECOM_PHONEBOOK_PUID		= 0x4F24,	/**< previous unique identifier */

    GSM_SEC_EFILE_TELECOM_PHONEBOOK_ADN		= 0x4F3A,	/**< Abbreviated Dialing Number */
    GSM_SEC_EFILE_TELECOM_PHONEBOOK_IAP		= 0x4F25,	/**< Index Administration Phonebook */
    GSM_SEC_EFILE_TELECOM_PHONEBOOK_PBC		= 0x4F09,	/**< Phonebook Control */
    GSM_SEC_EFILE_TELECOM_PHONEBOOK_EXT1		= 0x4F4A,	/**< Extension 1 */
    GSM_SEC_EFILE_TELECOM_PHONEBOOK_GRP		= 0x4F26,	/**< Grouping File */

    GSM_SEC_EFILE_TELECOM_PHONEBOOK_AAS		= 0x4F4B,	/**< Additional number Alpha String */
    GSM_SEC_EFILE_TELECOM_PHONEBOOK_GAS		= 0x4F4C,	/**< Additional Number */

    GSM_SEC_EFILE_TELECOM_PHONEBOOK_ANR		= 0x4F11,	/**< Additional Number */
    GSM_SEC_EFILE_TELECOM_PHONEBOOK_SNE		= 0x4F19,	/**< Second Name Entry */
    GSM_SEC_EFILE_TELECOM_PHONEBOOK_EMAIL		= 0x4F50,	/**< e-mail address */
#if 1
    GSM_SEC_EFILE_TELECOM_PHONEBOOK_ANRA		= 0x4F31,	/**< Additional Number */
    GSM_SEC_EFILE_TELECOM_PHONEBOOK_ANRB		= 0x4F32,	/**< Additional Number */
    GSM_SEC_EFILE_TELECOM_PHONEBOOK_ANRC		= 0x4F40,	/**< Additional Number */
    GSM_SEC_EFILE_TELECOM_PHONEBOOK_ANR1		= 0x4F33,	/**< Additional Number */
    GSM_SEC_EFILE_TELECOM_PHONEBOOK_ANRA1		= 0x4F34,	/**< Additional Number */
    GSM_SEC_EFILE_TELECOM_PHONEBOOK_ANRB1		= 0x4F35,	/**< Additional Number */

    GSM_SEC_EFILE_TELECOM_PHONEBOOK_ADN1		= 0x4F36,	/**< Abbreviated Dilling Number */
    GSM_SEC_EFILE_TELECOM_PHONEBOOK_PBC1		= 0x4F37,	/**< Phonebook Control */
    GSM_SEC_EFILE_TELECOM_PHONEBOOK_GRP1		= 0x4F38,	/**< Grouping File */
    GSM_SEC_EFILE_TELECOM_PHONEBOOK_SNE1		= 0x4F39,	/**< Second Name Entry */
    GSM_SEC_EFILE_TELECOM_PHONEBOOK_UID1		= 0x4F3B,	/**< unique identifier */

    GSM_SEC_EFILE_TELECOM_PHONEBOOK_EMAIL1	= 0x4F3C,	/**< e-mail address */
    GSM_SEC_EFILE_TELECOM_PHONEBOOK_IAP1		= 0x4F3E,	/**< Index Administration Phonebook */
#endif
    GSM_SEC_EFILE_MSG_WAITING_INDICATION_STATUS			=0x6FCA,		/** < MSG waiting indication  */

    /* CPHS FILE ID */
    GSM_SEC_EFILE_CPHS_VOICE_MSG_WAITING					= 0x6F11,	/** < voice MSG waiting indication  */
    GSM_SEC_EFILE_CPHS_SERVICE_STRING_TABLE				= 0x6F12,	/** < CPHS service string table  */
    GSM_SEC_EFILE_CPHS_CALL_FORWARD_FLAGS					= 0x6F13,	/** < CPHS call forward flags  */
    GSM_SEC_EFILE_CPHS_OPERATOR_NAME_STRING				= 0x6F14,	/** < CPHS operator name string  */
    GSM_SEC_EFILE_CPHS_CUSTOMER_SERVICE_PROFILE			= 0x6F15,	/** < CPHS custormer service profile  */
    GSM_SEC_EFILE_CPHS_CPHS_INFO							= 0x6F16,	/** < CPHS informatoin  */
    GSM_SEC_EFILE_CPHS_MAILBOX_NUMBERS						= 0x6F17,	/** < CPHS mail box numbers  */
    GSM_SEC_EFILE_CPHS_OPERATOR_NAME_SHORT_FORM_STRING	= 0x6F18,	/** < CPHS operator name short form string  */
    GSM_SEC_EFILE_CPHS_INFORMATION_NUMBERS				= 0x6F19,	/** < CPHS information numbers  */


    GSM_SEC_EFILE_CPHS_CFIS									= 0x6FCB, 	/** < call forward indication status  */
    GSM_SEC_EFILE_EXT7										= 0x6FCC,	/** < Ext 7 file   */
    /*  CPHS ALS FILE ID */
    GSM_SEC_CPHS_DYNAMICFLAGS								= 0x6F9F,	/** < CPHS Dynamics flags  */
    GSM_SEC_CPHS_DYNAMIC2FLAG 								= 0x6F92,	/** < CPHS Dynamics2 flags  */
    GSM_SEC_CPHS_CUSTOMER_SERVICE_PROFILE_LINE2			= 0x6F98,	/** < CPHS CSP2  */

    /* ISIM files */
    GSM_SEC_EFILE_ISIM_IMPI 		= 0x6F02,		/** < IMS private user identiry   */
    GSM_SEC_EFILE_ISIM_DOMAIN	= 0x6F03,		/** < IMS Home Network Domain Name   */
    GSM_SEC_EFILE_ISIM_IMPU	 	= 0x6F04,		/** < IMS public user identity    */
    GSM_SEC_EFILE_ISIM_AD 		= 0x6FAD,		/** < Administrative data     */  /* NOT implemented */
    GSM_SEC_EFILE_ISIM_ARR		= 0x6F06,		/** < Access control file     */  /* NOT implemented */

    /* ID clashes with IMSI file Id so changed to  6FFF - actuall Id is 0x6F07 */ /* modem no support ?*/
    GSM_SEC_EFILE_ISIM_IST 		= 0x6FFF,		/** <ISIM keys     */
    GSM_SEC_EFILE_ISIM_KEYS		= 0x6F08,		/** < Access control file     */
    GSM_SEC_EFILE_ISIM_PCSCF		= 0x6F09,		/** < PCSCF address     */
    GSM_SEC_EFILE_ISIM_GBABP		= 0x6FD5,		/* NO RSIM access */

    /* Invalid File ID, All the file ID are less than this Value*/
    GSM_SEC_EFILE_INVALID			=0xFFFF

} gsm_sec_sim_file_id_t;

///////////////////////////////////////////////////////////////////////////////////////////
//	SIM FILE TYPE
///////////////////////////////////////////////////////////////////////////////////////////
/**
 * @enum tapi_sim_file_struct_type_t
 * This enumeration defines the file type.
 * @since 2.0
 */
typedef enum
{
    GSM_SEC_FTYPE_DEDICATED		= 0x00,			/**< Dedicated */
    GSM_SEC_FTYPE_TRANSPARENT	= 0x01,			/**< Transparent */
    GSM_SEC_FTYPE_LINEAR_FIXED	= 0x02,			/**< Linear fixed */
    GSM_SEC_FTYPE_CYCLIC			= 0x04,			/**< Cyclic */
    GSM_SEC_FTYPE_INVALID_TYPE	= 0xFF			/**< Invalid type */

} gsm_sec_sim_file_struct_type_t;

///////////////////////////////////////////////////////////////////////////////////////////
//	SIM LINEAR FIXED/ CYCLIC FILES RECORD TYPE
///////////////////////////////////////////////////////////////////////////////////////////
/**
 * @enum tapi_sim_record_mode_t
 * This enumeration defines the record mode.
 */
typedef enum
{
    GSM_SEC_RECORD_NONE				= 0x00,		/**< N/A, meaning the target file isn't a record type */
    GSM_SEC_RECORD_ABSOLUTE			= 0x01,		/**< The specified record will be read or updated */
    GSM_SEC_RECORD_CURRENT			= 0x02,		/**< The current record will be read or updated */
    GSM_SEC_RECORD_NEXT				= 0x03,		/**< The next record will be read or updated */
    GSM_SEC_RECORD_PREVIOUS			= 0x04,		/**< The previous record will be read or updated */
    /**< All records from the record specified by record_index to the last one will be read */
    /*	GSM_SEC_RECORD_TO_THE_LAST		= 0x05,	*/ /**< To the last */
    /**< All records from the first record to the one specified by record_index will be read */
    /*	GSM_SEC_RECORD_FROM_THE_FIRST	= 0x06	*/	/**< From the first */

} gsm_sec_sim_record_mode_t;


/* for EI <-> VGSM - DB	*/
typedef struct _SimSecurity SimSecurity;

struct _SimSecurity {
    char pin_value[9];
    char pin2_value[9];
    char puk_value[9];
    char puk2_value[9];

    int pin_cnt;
    int pin2_cnt;
    int puk_cnt;
    int puk2_cnt;
    int sim_state;
    int facility_pin;
    int facility_fdn;
    int lock_sc2_state;
};


// 090601
typedef struct
{
    int type1;				// info : 0, data : 1
    int type2;				
    //	info -> 0:usim_li, 1:ecc, 2:sst, 3:est, 4:cphs, 5:spn, 6:imsi
    //	data -> 0:iccid, 1:usim_li, 2:ecc, 3:sst, 4:est, 5:spn, 6:imsi
    int size;
    unsigned char tmp[50];
}_SIMD_t;

#if 0
/*********************************************************************************

  Main Command

 *********************************************************************************/
typedef enum {
    GSM_PWR_CMD=0x01,		/* 0x01 : Power Control Commands */
    GSM_CALL_CMD,			/* 0x02 : Call Control Commands */
    GSM_DATA_CMD,			/* 0x03 : Data Control Commands */
    GSM_SMS_CMD,			/* 0x04 : Short Message Service Commands */
    GSM_SEC_CMD,			/* 0x05 : Security - SIM control Commands */
    GSM_PB_CMD,			/* 0x06 : Phonebook Control Commands */
    GSM_DISP_CMD,			/* 0x07 : Display Control Commands */
    GSM_NET_CMD,			/* 0x08 : Network Commands */
    GSM_SND_CMD,			/* 0x09 : Sound Control Commands */
    GSM_MISC_CMD,			/* 0x0A : Miscellaneous Control Commands */
    GSM_SVC_CMD,			/* 0x0B : Service Mode Control Commands - Factory Test or Debug Screen Control */
    GSM_SS_CMD,			/* 0x0C : Supplementary Service Control Command */
    GSM_GPRS_CMD,			/* 0x0D : GPRS Commands */  // joonook 20041011 create
    GSM_SAT_CMD,			/* 0x0E : SIM Toolkit Commands */
    GSM_CFG_CMD,			/* 0x0F : Configuration Commands */
    GSM_IMEI_CMD,			/* 0x10 : IMEI Tool Commands */
    GSM_GPS_CMD,			/* 0x11 : GPSl Commands */
    GSM_SAP_CMD,			/* 0x12 : SIM Access Profile Commands */
    GSM_GEN_CMD=0x80,		/* 0x80 : General Response Command */
    GSM_CMD_MAX
}gsm_main_cmd_type;

/*********************************************************************************/


/*********************************************************************************

  Command Type( PDA -> PHONE )

 *********************************************************************************/
typedef enum {
    GSM_CMD_EXEC	=	0x01,	/* 0x01 : Execute - Request the action to phone */
    GSM_CMD_GET,				/* 0x02 : Read the data from phone */
    GSM_CMD_SET,				/* 0x03 : Write the data to phone, Response must be GENERAL_RESPONSE */
    GSM_CMD_CFRM,				/* 0x04 : Response of the INDICATION */
    GSM_CMD_EVENT,			/* 0x05 : Notify the status of PDA */
    GSM_PDA_MAX
} gsm_pda_cmd_e_type;

/*********************************************************************************/


/*********************************************************************************

  Command Type( PHONE -> PDA )

 **********************************************************************************/
typedef enum {
    GSM_CMD_INDI	=	0x01,	/* 0x01 : Indication - Request the action or data to PDA */
    GSM_CMD_RESP,				/* 0x02 : Response - Response of the GET */
    GSM_CMD_NOTI,				/* 0x03 : Notification - Notify the status of phone */
    GSM_PHONE_MAX
} gsm_phone_cmd_e_type;

/*********************************************************************************/


/*********************************************************************************

  Sub Command

 **********************************************************************************/
typedef enum{
    GSM_SEC_PIN_STATUS			=0x01,    /* 0x01 : PIN Status Message */
    GSM_SEC_PHONE_LOCK,					/* 0x02 : Phone Lock Message */
    GSM_SEC_CHANGE_LOCKING_PW,			/* 0x03 : Change Locking Password Message */
    GSM_SEC_SIM_LANG,						/* 0x04 : SIM Language Message */
    GSM_SEC_RSIM_ACCESS,					/* 0x05 : +CRSM, Restricted SIM Access Message */
    GSM_SEC_GSIM_ACCESS,					/* 0x06 : +CSIM, General SIM Access Message */
    GSM_SEC_SIM_ICC_TYPE,					/* 0x07 : SIM ICC Type Message*/
    GSM_SEC_LOCK_INFO,					/* 0x08 : Lock Information Message*/
    GSM_SEC_ISIM_AUTH,					/* 0x9 : ISIM Authentication Message */
    GSM_SEC_MAX
} gsm_sec_sub_cmd_type;

/*********************************************************************************/
#endif

#endif	/*	__VGSM_SIM_H__	*/

