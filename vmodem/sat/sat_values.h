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

#ifndef SAT_VALUES_H
#define SAT_VALUES_H

#undef FALSE
#undef TRUE
#define TRUE 1
#define FALSE 0


#define VGSM_CHARSET_UTF8							1//"UTF-8"
#define VGSM_CHARSET_UCS2							2//"UCS-2"
#define VGSM_CHARSET_GSM							3//"GSM-7"


/////////////
#define ALP_STATUS_OK						0
/////////////



// maximu size of a string in an apdu
#define VGSM_APDU_CONVERTION_BUFFER_SIZE			256


typedef struct _SatApduDecoder		SatApduDecoder;

// Apdu decoder/encoder callbacks
typedef int	(*SatApduEncoderCallback)		(unsigned long request, char* bufferP, int* bufferSize, void* HLData);
typedef int	(*SatApduDecoderCallback)		(char tag, SatApduDecoder* decoderP, char* objectP);

///
#define MAX_STRING	255;

typedef struct _DISPLAY_TEXT  DP_TEXT;
struct _DISPLAY_TEXT {
    char priority;
    char clear_msg;
    char text_string[255];
    char coding_scheme;
    char icon_qualifier;
    char icon_identifier;
    char icon_identifier_flag;
    char immediate_response;
    char duration;
    char duration_unit;

};
///

typedef struct _GET_INKEY  GET_INKEY;
struct _GET_INKEY {
    unsigned char cmd_detail;

    char text_string[255];
    char coding_scheme;
    char icon_qualifier;
    char icon_identifier;
    char icon_identifier_flag;
    char immediate_response;
    char duration;
    char duration_unit;
    //	char duration_interval;
    //	char  text_len;

};
///
typedef struct _GET_INKEY  GET_INPUT;
struct _GET_INPUT {
    unsigned char cmd_detail;

    char text_string[255];
    char coding_scheme;
    char icon_qualifier;
    char icon_identifier;
    char icon_identifier_flag;
    char immediate_response;
    char duration;
    char duration_unit;


};


typedef struct _SET_MENU  SET_MENU;
struct _SET_MENU {
    //unsigned char cmd_detail;
    char id_num;
    char alpah_string[255];
    char item_string[255];
};
///
typedef struct _SEND_SMS  SEND_SMS;
struct _SEND_SMS {
    unsigned char cmd_detail;
    unsigned char address;
    char dial_num[25];
    char  tpdu_sms[255];
};
///

struct _SatApduDecoder
{
    SatApduDecoderCallback	HLSetDataF;
    void*							HLDataP;
    unsigned char*					formatP;
    unsigned char*					apduP;
    int							apduSizeInBytes;
    unsigned char*					apduReadP;
    unsigned char					currentObjectTag;
    unsigned char					currentObjectLength;
};





typedef struct _SatCmdResponseInternal SatCmdResponseInternal;
struct _SatCmdResponseInternal
{
    char cmdId;		//!< Command Identifier
    char resCode;		//!< Result codes applicable to command iCmdId, see ALP_TEL_CAT_RES_XXX
    char infoType;		// type for local information
    int infoLen;			// length for local information
    char *infoData;
};



#define HEX_DIGIT(b) ((b) >= '0' && (b) <= '9' ? (b) - '0' : (b) - 'A' + 10)

#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif




#define CAT_CONFIG_STRING_MAX_SIZE	48			//!< Cat config max size

#define	alp_prv_mbiz_param_to_buffer(bufferP, paramP, paramSize)	\
{																\
    unsigned char* bufferOffset = (unsigned char*) (bufferP);			\
    memmove((bufferP), (paramP), (paramSize));					\
    bufferOffset += (paramSize);									\
    (bufferP) = (void*) bufferOffset;									\
}

// STK config
#define		DEFAULT_PROFIL_SIZE	17

// Structure and coding of TERMINAL PROFILE
//	First byte (Download)
#define		PROFILE_DOWNLOAD_FACILITY			0x01
#define		SMS_PP_DATA_DOWNLOAD_FACILITY	0x02
#define		CELL_BROADCAST_FACILITY			0x04
#define		MENU_SELECTION_FACILITY			0x08
#define		NIEEXX_RESPONSE_CODE_FACILITY		0x10
#define		TIMER_EXPIRATION_FACILITY			0x20
#define		USSD_STRING_FACILITY				0x40
#define		AUTOMATIC_REDIAL_FACILITY			0x80

//	Second byte (Other)
#define		COMMAND_RESULT_FACILITY					0x01
#define		CALL_CONTROL_BY_SIM_FACILITY				0x02
#define		CC_WITH_CELL_IDENTITY_FACILITY				0x04
#define		SHORT_MESSAGE_CONTROL_BY_SIM_FACILITY	0x08
#define		ALPHA_IDENTIFIER_HANDLING_FACILITY		0x10
#define		UCS2_ENTRY_FACILITY							0x20
#define		UCS2_DISPLAY_FACILITY						0x40
#define		DISPLAY_EXTENSION_TEXT_FACILITY			0x80

//	Third byte (Proactive SIM)
#define		PROSIM_DISPLAY_TEXT_FACILITY		0x01
#define		PROSIM_GET_INKEY_FACILITY			0x02
#define		PROSIM_GET_INPUT_FACILITY			0x04
#define		PROSIM_MORE_TIME_FACILITY			0x08
#define		PROSIM_PLAY_TONE_FACILITY			0x10
#define		PROSIM_POLL_INTERVAL_FACILITY		0x20
#define		PROSIM_POLLING_OFF_FACILITY		0x40
#define		PROSIM_REFRESH_FACILITY			0x80

//	Fourth byte (Proactive SIM)
#define		PROSIM_SELECT_ITEM_FACILITY		0x01
#define		PROSIM_SEND_SHORT_MESG_FACILITY	0x02
#define		PROSIM_SEND_SS_FACILITY			0x04
#define		PROSIM_SEND_USSD_FACILITY			0x08
#define		PROSIM_SETUP_CALL_FACILITY			0x10
#define		PROSIM_SETUP_MENU_FACILITY		0x20
#define		PROSIM_LOCAL_INFO1_FACILITY		0x40
#define		PROSIM_LOCAL_INFO2_FACILITY		0x80

//	Fifth byte (Event driven information)
#define		PROSIM_SETUP_EVENT_LIST_FACILITY		0x01
#define		EVENT_MT_CALL_FACILITY					0x02
#define		EVENT_CALL_CONNECTED_FACILITY			0x04
#define		EVENT_CALL_DISCONNECT_FACILITY		0x08
#define		EVENT_LOCATION_STATUS_FACILITY		0x10
#define		EVENT_USER_ACTIVITY_FACILITY			0x20
#define		EVENT_IDLE_SCREEN_FACILITY				0x40
#define		EVENT_CARD_READER_STATUS_FACILITY	0x80

//	Sixth byte (Event driven information extensions)
#define		EVENT_LANGUAGE_SELECTION_FACILITY		0x01
#define		EVENT_BROWSER_TERMINATION_FACILITY	0x02
#define		EVENT_DATA_AVAILABLE_FACILITY			0x04
#define		EVENT_CHANNEL_STATUS_FACILITY			0x08

//	Seventh byte (Multiple card proactive commands) for class "a"
#define		PROSIM_POWER_ON_CARD_FACILITY			0x01
#define		PROSIM_POWER_OFF_CARD_FACILITY			0x02
#define		PROSIM_PERFORM_CARD_APDU_FACILITY		0x04
#define		PROSIM_GET_READER_STATUS_FACILITY		0x08
#define		PROSIM_GET_READER_IDENTIFIER_FACILITY		0x10

//	Eighth byte (Proactive SIM)
#define		PROSIM_TIMER_MANAGEMENT_FACILITY			0x01
#define		PROSIM_GET_TIMER_VALUE_FACILITY			0x02
#define		PROSIM_PROVIDE_LOCAL_INFO3_FACILITY		0x04
#define		BINARY_GET_KEY_FACILITY					0x08
#define		SETUP_IDLE_MODE_TEXT_FACILITY				0x10
#define		RUN_AT_COMMAND_FACILITY					0x20
#define		SETUP_CALL_2ND_ALPHA_ID_FACILITY			0x40
#define		SECOND_CAPABILITY_CONFIG_PARAM_FACILITY		0x80

//	Ninth byte
#define		SUSTAINED_DISPLAY_TEXT_FACILITY			0x01
#define		SEND_DTMF_COMMAND_FACILITY				0x02
#define		PROSIM_PROVIDE_BCCH_INFO_FACILITY		0x04
#define		PROSIM_PROVIDE_LANGUAGE_INFO_FACILITY	0x08
#define		PROSIM_PROVIDE_TIMING_INFO_FACILITY		0x10
#define		PROSIM_LANGUAGE_NOTIFICATION_FACILITY	0x20
#define		PROSIM_LAUNCH_BROWSER_FACILITY			0x40

//	Tenth byte (Soft keys support)
#define		SOFTKEY_FOR_SELECT_ITEM_FACILITY			0x01
#define		SOFTKEY_FOR_SETUP_MENU_FACILITY			0x02

//	Eleventh byte (Soft keys information)
#define		MAXIMUM_NUMBER_OF_SOFTKEYS				0x00

//	Twelfth byte (Bearer Independent protocol proactive commands (class "e")
#define		PROSIM_OPEN_CHANNEL_FACILITY				0x01
#define		PROSIM_CLOSE_CHANNEL_FACILITY				0x02
#define		PROSIM_RECEIVE_DATA_FACILITY				0x04
#define		PROSIM_SEND_DATA_FACILITY					0x08
#define		PROSIM_GET_CHANNEL_STATUS_FACILITY		0x10

//	Thirteenth byte (Bearer Independent protocol supported bearers (class "e")
#define		CSD_SUPPORTED_FACILITY						0x01
#define		GPRS_SUPPORTED_FACILITY					0x02
#define		NUMBER_OF_CHANNELS_SUPPORTED_BY_ME(X)	(((char)X) << 5)

//	Fourteenth byte (Screen height)
#define		NUMBER_OF_CHARACTERS_DOWN_TE_DISPLAY(X)	(((char)X) & 0x1F)
#define		SCREEN_SIZING_PARAMETERS_FACILITY				0x80

//	Fifteenth byte (Screen width)
#define		NUMBER_OF_CHARACTERS_ACROSS_TE_DISPLAY(X)	(((char)X) & 0x7F)
#define		VARIABLE_SIZE_FONTS_FACILITY					0x80

//	Sixteenth byte (Screen effects)
#define		DISPLAY_CAN_BE_RESIZED				0x01
#define		TEXT_WRAPPING_SUPPORTED			0x02
#define		TEXT_SCROLLING_SUPPPORTED			0x04
#define		MENU_WIDTH_REDUCTION(X)			(((char)X) << 5)

//	Seventeenth byte: (Bearer independent protocol supported transport interface) for class "e"
#define		TCP_SUPPORTED						0x01
#define		UDP_SUPPPORTED						0x02

















#define SELECT_BY_PATH_REFERENCING				0x08
#define SELECT_BY_FILE_IDENTIFIER				0x00
#define RETURN_FCP_TEMPLATE						0x04

#define SEND_LINE_MAX_BUFFER					500 // as alp_pdrv_comm_send_line takes into account up to 512 bytes in the arglist
#define SEND_LINE_MAX_BUFFER					500 // as alp_pdrv_comm_send_line takes into account up to 512 bytes in the arglist
// the data buffer to send with the AT command should not exceed: 512 bytes - (max command AT header size)

#define COMMAND_QUALIFIER						objectP[2]	
#define TYPE_OF_COMMAND							objectP[1]

#define NON_EXPLICIT_ICON						0x01

#define GETINPUT_DETAILS_HIDE_INPUT				0x04
#define GETINPUT_DETAILS_PACKED_BIT			0x08

#define GETINKEY_DETAILS_YES_NO					0x04
#define GETINKEY_DETAILS_DCS_MASK				0x03

#define DISPLAY_TEXT_DETAILS_CLEAR_AFTER_DELAY	0x80
#define DISPLAY_TEXT_DETAILS_PRIORITY			0x01

#define SETUP_MENU_DETAILS_SOFTKEY				0x01
#define SELECT_ITEM_DETAILS_SOFTKEY				0x04

#define GET_DETAILS_HELP_INFO 					0x80

#define READ_RECORD_MODE_ABSOLUTE				0x04

#define SELECT_BY_PATH_REFERENCING				0x08
#define SELECT_BY_FILE_IDENTIFIER				0x00

#define RETURN_FCP_TEMPLATE						0x04

#define CAT_CONFIG_STRING_MAX_SIZE				48		//!< Cat config max size



/**
 * @addtogroup ALP_TELEPHONY_SERVICES_CAT
 * @{
 */
typedef struct _SatConfig SatConfig;
struct _SatConfig
{
    char*					profileP;				//!< Terminal Profile standard parameters
    int					       profileSize;			//!< Profile data size, in bytes
    char						lanCode[2]; 			//!< ISO 639 language code
    char						mode;					//!< Result code presentation enable/disable
};

typedef struct _SatFile SatFile;
struct _SatFile
{
    int*					pathP;			//!< Absolute path of the file to read in the SIM.
    //!< Example: { 0x3F00, 0x7F20, 0x6F21 }
    //!< Consists of file identifiers from the Master File
    //!< to the Elementary File to be accessed
    char*					bufP;			//!< Buffer to be filled in with requested file body bytes.
    int						bufSize;		//!< Buffer size in bytes
    int						byteCount;		//!< Amount of applicable bytes in ioBufP

    int					partOffset;		//!< Offset of requested file body part
    int					partSize;		//!< Size of requested file body part

    int					fileSize;		//!< EF size
    char						fileStruct;		//!< EF structure (transparent, linear fixed or cyclic)
    char						mode;			//!< Access mode (see EF access mode)

    char						pathCount;		//!< File identifiers count in iPathP
    char						recId;			//!< Identifier of the record to be read. Range 1..254
    char						recSize;		//!< Size of a record, in bytes.
    //!< 0 if the file is not a Linear Fixed or a Cyclic EF
};

typedef struct _SatCmdParams SatCmdParams;
struct _SatCmdParams
{
    void*						cmdParamP;				//!< Structure associated to the command (according to iCmdId)
    int						cmdParamSize;			//!< Size of the specific command parameter buffer
    //!< Common parameter for all (almost) commands
    char*						textP;					//!< Text to display
    char						iconId;					//!< Icon identifier
    char						cmdId;					//!< See the Proactive Commands Id for more details
    char						explicitIcon; 			//!< Icon is explicit
    char						noResponse;				//!< true if the command does not need a response	
    int						textSize;				//!< The size of the encoded text
};

typedef struct _SatCmdResponse SatCmdResponse;
struct _SatCmdResponse
{
    char*						respP;					//!< Associated text response
    unsigned char				itemIdentifier;			//!< Item identifier  
    int						respSize;				//!< Associated text response size in bytes
    char						cmdId;					//!< Command Identifier
    char						respType;				//!< Expected response type, see ALP_TEL_CAT_RESP_TYPE_XXX
    char						resCode;				//!< Result codes applicable to command iCmdId, see ALP_TEL_CAT_RES_XXX
    char						addInfo;				//!< Additional information code
    //!< -> Yes/No response or Item identifier for instance
};

typedef struct _SatMenuSelection SatMenuSelection;
struct _SatMenuSelection
{
    char						evtCode;				//!< Menu Selection event code, see ALP_TEL_CAT_MENU_SELECTION_XXX
    char						appId;					//!< Associated application identifier
};

typedef struct _SatEventToCard SatEventToCard;
struct _SatEventToCard
{
    char						evtCode;					//!< Event Download code, see ALP_TEL_CAT_EVENT_XXX
    char						lanCode[2];					//!< ISO 639 language code
    char						browserTerminationCause;	//!< Browser Termination cause code, see ALP_TEL_CAT_BROWSER_XXX
};


typedef struct _SatRefreshFileList	SatRefreshFileList;
struct _SatRefreshFileList
{
    unsigned short*				filePathP;			//!< Concatenated absolute paths of the modified EF
    //!< or NULL if no specific file has been mentioned.
    unsigned char				filePathCount;		//!< Number of paths in the file list
    unsigned char				fileIdCount;		//!< Number of files described in the file list
    unsigned char				opCode;				//!< Operation code (see above the refresh command qualifier )
    unsigned char*				AID;				//!< Application identifier (only used for 3G platform)
};

typedef struct _SatSetUpEventList SatSetUpEventList;
struct _SatSetUpEventList
{
    char*					eventP;				//!< Events to be monitored, see ALP_TEL_CAT_EVENT_XXX
    char						eventCount;			//!< Events number in eventP or 0 to stop monitoring
};

typedef struct _SatSetUpCall SatSetUpCall;
struct _SatSetUpCall
{
    char*					bearerCapP;				//!< Bearer capability configuration params (GSM 04.08 5.3.0 section 10.5.4.5)
    char*						numberP;				//!< Dialing number
    char*						userConfTextP;			//!< Text to display. NULL if not provided
    char*						callEstaTextP;			//!< Text to display, NULL if not provided
    char*						subAddressP;			//!< called party subaddress
    int						userConfTextSize;		//!< Text buffer size in bytes
    int						numberSize;				//!< Number buffer size in bytes
    int						callEstaTextSize;		//!< Call establishment text buffer size in bytes
    int						subAddressSize;			//!< subaddress size in bytes
    char						bearerCapSize;			//!< Bearer capability size, in bytes

    unsigned long				duration;				//!< if the time elapsed since the first call set-up attempt has exceeded the duration request, the redial mechanism shall be terminated 
    char						condition;				//!< Set up call commmand qualifier, see ALP_TEL_CAT_SETUP_CALL_XXX

    char						userConfIconId;			//!< Icon id. 0 if no icon
    char						userConfExplicitIcon;	//!< Icon is explicit
    char						callEstaIconId;			//!< Icon id. 0 if no icon
    char						callEstaExplicitIcon;	//!< Icon is explicit

};

typedef struct _SatLaunchBrowser SatLaunchBrowser;
struct _SatLaunchBrowser
{
    char*						urlP;				//!< The URL
    int						urlSize;
    char*						gatewayP;			//!< Gateway name, Proxy identity to be used
    int						gatewaySize;
    int*					filePathP;			//!< Concatenated absolute paths of provisioning EF
    //!< or NULL if no specific file has been mentioned.
    char*					prefBearersP;		//!< Prioritized list of ALP_TEL_CAT_BEARER_XXX bearer codes
    char						fileIdCount;		//!< File identifiers count in filePathP
    char						prefBearerCount;	//!< Number of bearer codes in prefBearersP.
    char						condition;			//!< Launch browser conditions, see ALP_TEL_CAT_LAUNCH_BROWSER_XXX
    char						browserId;			//!< Browser Identity
};

typedef struct _SatPlayTone SatPlayTone;
struct _SatPlayTone
{
    int					sndDuration;		//!< Sound duration in ms. 0 for default or 100..15300000
    char						sndCode;			//!< One of the ALP_TEL_CAT_PLAY_TONE_XXX codes
    unsigned char 				vibrateAlert;		//!< One of the ALP_TEL_CAT_VIBRATE_XXX codes
};

typedef struct _SatDisplayText SatDisplayText;
struct _SatDisplayText
{
    char						priority;			//!< Priority level
    char						clearAfterDelay;	//!< Wait for user's action
    char						immediateResponse;	//!< Send a response to the SIM ASAP
    unsigned long				duration;			//!< the terminal shall continue to dsiplay the text until the expiration of this variable display timeout	
};

typedef struct _SatGetInkey SatGetInkey;
struct _SatGetInkey
{
    char						helpInfo;			//!< Help information is provided by the SIM
    char						respType;			//!< Expected response type. See ALP_TEL_CAT_RESP_TYPE_XXX
    unsigned long				duration; 			//!> the terminal shall wait until either the user enters a single characters or the timeout expires.
};

typedef struct _SatGetInput SatGetInput;
struct _SatGetInput
{
    char*						defRespP;			//!< Default response text to propose
    int						defRespSize;		//!< Default response text size in bytes
    char						hideUserInput;		//!< Mask entered data
    char						helpInfo;			//!< Help information is provided by the SIM
    char						minRespLength;		//!< Minimum response length, in characters
    char						maxRespLength;		//!< Maximum response length, in characters
    char						respType;			//!< Expected response type, see ALP_TEL_CAT_RESP_TYPE_XXX
};

typedef struct _SatItem SatItem;
struct _SatItem
{
    char*						nameP;				//!< Item name
    int						nameSize;			//!< Item name size in bytes
    char						id;					//!< Item identifier
    char						iconId;				//!< Icon Identifier
    char						expIcon;			//!< Icon is explicit
    char						nextActionInd;		//!< Identifier of the next command for this item
    SatItem*				nextItemP;			//!> @ of the next item (if any)
};

typedef struct _SatItemList SatItemList;
struct _SatItemList
{
    SatItem*				firstItemP;			//!< The first Item
    int						itemCount;			//!< Number of items 
    char						softKey;			//!< Item can be selected by tapping on its icon
    char						helpInfo;			//!< Help information is provided by the SIM
    char						defItemId;			//!< Identifier of the item that should be pre-selected.
};

typedef struct _SatSendShortMessage SatSendShortMessage;
struct _SatSendShortMessage
{
    char*						addressP;			//!< RP_Destination_Address (optional)
    int						addressSize;
    char*					TPDUP;				//!< SMS TPDU
    int						TPDUSize;
    char						packingRequired;
};

typedef struct _SatBuffer SatBuffer;
struct _SatBuffer		//!< used in: SendData, SendDTMF, Send USSD, Send SS
{
    char*					bufferP;				//!< the data buffer
    int						bufferSize;				//!< the data buffer size
    char						other;					//!< other parameter specific to the command (if any)
};

typedef struct _SatOpenChannel SatOpenChannel;
struct _SatOpenChannel
{
    char*						addressP;
    char*						subAddressP;
    char*						otherAddressP;
    char*						destinationAddressP;
    char*						loginP;
    char*						passwordP;
    char*					bearerParamsP;
    char*						accessPointP;
    int					duration1;				//!< duration1 in ms
    int					duration2;				//!< duration2 in ms
    int					bufferSize;
    int					transportPort;
    char						onDemand;				//!< Is link established immediately
    char						bearerCode; 			//!< Bearer code, see ALP_TEL_CAT_BEARER_XXX
    char						otherAddressType;		//!< see ALP_TEL_CAT_ADDRESS_XXX
    char						destinationAddressType; //!< see ALP_TEL_CAT_ADDRESS_XXX
    char						transportType;			//!< see ALP_TEL_CAT_TRANSPORT_XXX
    char						addressSize;
    char						subAddressSize;
    char						otherAddressSize;
    char						bearerParamsSize;
    char						loginSize;
    char						passwordSize;
    char						destinationAddressSize;
    char						accessPointSize;
};


//! answer made by the phone module directly to the SIM 
typedef struct _SatMEanswer SatMEanswer;
struct _SatMEanswer	
{
    char						cmdId;					//!< Command Identifier
    char						resCode;				//!< Result codes applicable to command iCmdId 
    char						addInfo;				//!< Additional information code

};


//! answer made by the SIM to the phone module CALL CONTROL or MO Short Message CONTROL command 
typedef struct _SatSIManswer SatSIManswer;
struct _SatSIManswer	
{
    char					result;				//!< call control or MO Short Message control result, see ALP_TEL_CAT_RESULT_CONTROL_XXX

    char*					textP;				//!< Text to display
    int					textSize;			//!< The size of the encoded text

    char*					addressP1;			//!> if it's a call control answer adressP1= dialling number, if the address data is not present, the terminal shall assume the dialling number is not to be modified 
    //!> if it's a MO short message control answer addressP1= RP_Destination_Address, if the address data object 1 is not present, then the ME shall assume the RP_Destination_Address of the Service Centre is not to be modified.
    char					addressSize1;		//!> address size

    char*					addressP2;			//!> apply only if it's a a MO short message control answer addressP2= TP_Destination_Address, if the address data object 2 is not present, then the ME shall assume the TP_Destination_Address is not to be modified.
    char					addressSize2;		//!> address size


    char*					subAddressP;		//!> subaddress
    char					subAddressSize;		//!> subaddress buffer size

    char					BCrepeatIndicator;	//!> BC repeat indicator

    char*				capabilityConfP1;	//!< Capability configuration parameters1
    char					capabilityConfSize1;//!< the capability configuration parameters1 size

    char*				capabilityConfP2;	//!< Capability configuration parameters2 
    char					capabilityConfSize2;//!< the capability configuration parameters2 size
};




// Card command
#define SAT_CARD_COMMAND_INS_READ_BINARY				0xB0
#define SAT_CARD_COMMAND_INS_READ_RECORD				0xB2
#define SAT_CARD_COMMAND_INS_GET_RESPONSE				0xC0
#define SAT_CARD_COMMAND_INS_UPDATE_BINARY				0xD6
#define SAT_CARD_COMMAND_INS_UPDATE_RECORD				0xDC
#define SAT_CARD_COMMAND_INS_STATUS					0xF2


// Card response
#define SAT_CARD_RESPONSE_NORMAL_ENDING					0x90
#define SAT_CARD_RESPONSE_NORMAL_ENDING_WITH_EXTRA_INFO	0x91
#define SAT_CARD_RESPONSE_DATA_LEN_WITH_DOWNLOAD_ERROR		0x9E
#define SAT_CARD_RESPONSE_DATA_LEN							0x9F
#define SAT_CARD_RESPONSE_SIM_TOOLKIT_BUSY					0x93
#define SAT_CARD_RESPONSE_MEMORY_ERROR						0x92
#define SAT_CARD_RESPONSE_REFERENCING_ERROR				0x94
#define SAT_CARD_RESPONSE_SECURITY_ERROR					0x98
#define SAT_CARD_RESPONSE_INCORRECT_PARAM3					0x67
#define SAT_CARD_RESPONSE_INCORRECT_PARAM1OR2				0x6B
#define SAT_CARD_RESPONSE_UNKNOW_INS						0x6D
#define SAT_CARD_RESPONSE_WRONG_INS						0x6E
#define SAT_CARD_RESPONSE_TECHNICAL_PROBLEM				0x6F


// APDU encoder/decoder

// Call control or MO SMS conrol
#define SAT_ALLOWED								0x00
#define	SAT_NOT_ALLOWED							0x01
#define	SAT_ALLOWED_WITH_MODIFICATION				0x02



// BER-tags
#define SAT_RESPONSE								0x00
#define SAT_PROACTIVE_SIM_COMMAND					0xD0
#define	SAT_SMS_PP_DOWNLOAD						0xD1
#define	SAT_CELL_BRAOADCAST_DOWNLOAD				0xD2
#define	SAT_MENU_SELECTION							0xD3
#define	SAT_CALL_CONTROL							0xD4
#define	SAT_MO_SHORT_MESSAGE_CONTROL				0xD5
#define	SAT_EVENT_DOWNLOAD							0xD6
#define	SAT_TIMER_EXPIRATION						0xD7




// SIMPLE-TLV data objects tags 
#define SAT_COMMAND_DETAILS						0x01 //!> Command details tag
#define SAT_DEVICE_IDENTITIES						0x02 //!> Device identity tag
#define SAT_RESULT									0x03 //!> Result tag
#define SAT_DURATION								0x04 //!> Duration tag 
#define SAT_ALPHA_IDENTIFIER						0x05 //!> Alpha identifier tag
#define SAT_ADDRESS								0x06 //!> Address tag
#define SAT_CAPABILILITY_CONFIGURATION_PARAMETERS	0x07 //!> Capability configuration parameters tag
#define SAT_SUBADDRESS								0x08 //!> Subaddress tag
#define SAT_SS_STRING								0x09 //!> SS string tag
#define SAT_USSD_STRING							0x0A //!> USSD string tag
#define SAT_SMS_TPDU								0x0B //!> SMS TPDU tag
#define SAT_CELL_BROADCAST_PAGE					0x0C //!> Cell Broadcast page tag
#define SAT_TEXT_STRING							0x0D //!> Text string tag 
#define SAT_TONE									0x0E //!> Tone tag
#define SAT_ITEM									0x0F //!> Item tag
#define SAT_ITEM_IDENTIFIER						0x10 //!> Item identifier tag 
#define SAT_RESPONSE_LENGTH						0x11 //!> Response length tag 
#define SAT_FILE_LIST								0x12 //!> File List tag
#define SAT_LOCATION_INFORMATION					0x13 //!> Location Information tag
#define SAT_IMEI									0x14 //!> IMEI tag 
#define SAT_HELP_REQUEST							0x15 //!> Help request tag 
#define SAT_NETWORK_MEASUREMENT_RESULTS			0x16 //!> Network Measurement Results tag 
#define SAT_DEFAULT_TEXT							0x17 //!> Default Text tag
#define SAT_ITEMS_NEXT_ACTION_INDICATOR			0x18 //!> Items Next Action Indicator tag
#define SAT_EVENT_LIST								0x19 //!> Event list tag
#define SAT_CAUSE									0x1A //!> Cause tag
#define SAT_LOCATION_STATUS						0x1B //!> Location status tag 
#define SAT_TRANSACTION_IDENTIFIER					0x1C //!> Transaction identifier tag
#define SAT_BCCH_CHANNEL_LIST						0x1D //!> BCCH channel list tag
#define SAT_ICON_IDENTIFIER						0x1E //!> Icon identifier tag
#define SAT_ITEM_ICON_IDENTIFIER_LIST				0x1F //!> Item Icon identifier list tag
//#define SAT_CARD_READER_STATUS						0x20 //!> Card reader status tag
#define SAT_CARD_ATR								0x21 //!> Card ATR tag
#define SAT_C_ADPU									0x22 //!> C-APDU tag 
#define SAT_R_ADPU									0x23 //!> R-APDU tag
#define SAT_TIMER_IDENTIFIER						0x24 //!> Timer identifier tag
#define SAT_TIMER_VALUE							0x25 //!> Timer value tag
#define SAT_DATE_TIME_AND_TIME_ZONE				0x26 //!> Date-Time and Time zone tag
#define SAT_CALL_CONTROL_REQUESTED_ACTION			0x27 //!> Call control requested action tag
#define SAT_AT_COMMAND								0x28 //!> AT Command tag
#define SAT_AT_RESPONSE							0x29 //!> AT Response tag
#define SAT_BC_REPEAT_INDICATOR					0x2A //!> BC Repeat Indicator tag
#define SAT_IMMEDIATE_RESPONSE						0x2B //!> Immediate response tag
#define SAT_DTMF_STRING							0x2C //!> DTMF string tag
#define SAT_LANGUAGE								0x2D //!> Language tag
#define SAT_TIMING_ADVANCE							0x2E //!> Timing Advance tag
#define SAT_AID									0x2F //!> AID tag 
#define SAT_BROWSER_IDENTITY						0x30 //!> Browser Identity tag 
#define SAT_URL									0x31 //!> URL tag
#define SAT_BEARER									0x32 //!> Bearer tag
#define SAT_PROVISIONING_FILE_REFERENCE			0x33 //!> Provisioning Reference File tag
#define SAT_BROWSER_TERMINATION_CAUSE				0x34 //!> Browser Termination Cause tag
#define SAT_BEARER_DESCRIPTION						0x35 //!> Bearer description tag
#define SAT_CHANNEL_DATA							0x36 //!> Channel data tag
#define SAT_CHANNEL_DATA_LENGTH					0x37 //!> Channel data length tag
#define SAT_CHANNEL_STATUS							0x38 //!> Channel status tag
#define SAT_BUFFER_SIZE							0x39 //!> Buffer size tag
//#define SAT_CARD_READER_IDENTIFIER					0x3A //!> Card reader identifier tag
#define SAT_FILE_UPDATE_INFORMATION				0x3B //!> File Update Information tag
#define SAT_SIM_ME_INTERFACE_TRANSPORT_LEVEL		0x3C //!> UICC/terminal interface transport level tag
// 0x3D Not used                                                           
#define SAT_OTHER_ADDRESS							0x3E //!> Other address (data destination address) tag


// 
//Access Technology tag                                             0x3F
//Display parameters tag                                            0x40
//Service Record tag                                                0x41
//Device Filter tag                                                 0x42
//Service Search tag                                                0x43
//Attribute information tag                                         0x44
//Service Availability tag                                          0x45
//Reserved for 3GPP2 (ESN tag)                                      0x46
//Network Access Name tag                                           0x47
//Reserved for 3GPP2 (CDMA-SMS-TPDU tag)                            0x48
//Remote Entity Address tag                                         0x49
//Reserved for 3GPP (I-WLAN Identifier tag)                         0x4A
//Reserved for 3GPP (I-WLAN Access Status tag)                      0x4B
//RFU                                                            	0x4C to 0x4F
//Text attribute tag                                               	'50'
//Item text attribute list tag                                     	'51'
//Reserved for 3GPP (PDP context Activation parameter tag)         	'52'
//RFU                                      							'53' to '61'
//IMEISV tag                                 						'62'
//Battery state tag                          						'63'
//Browsing status tag                       						'64'
//Network Search Mode tag                   						'65'
//Frame Layout tag                           						'66'
//Frames Information tag                     						'67'
//Frame identifier tag                       						'68'
//Reserved for 3GPP (UTRAN Measurement Qualifier tag)       		'69'
//Multimedia Message Reference tag               				    '6A'
//Multimedia Message Identifier tag                  				'6B'
//Reserved for 3GPP (Multimedia Message Transfer Status tag) 1    	'6C'
//MEID tag															'6D' 
//Multimedia Message Content Identifier tag							'6E' 
//Multimedia Message Notification tag								'6F' 


// admissible values for kGetDataDeviceIdentities param
#define SAT_DEVICE_KEYPAD						0x01
#define SAT_DEVICE_DISPLAY						0x02
#define SAT_DEVICE_EARPIECE					0x03
#define SAT_DEVICE_SIM							0x81
#define SAT_DEVICE_ME							0x82
#define SAT_DEVICE_NETWORK						0x83

#define SAT_GET_DATA_FORMAT						0



/**
 * @addtogroup ALP_TELEPHONY_SERVICES_CAT
 * @{
 */

//! Proactive Commands Id
#define SAT_CMD_REFRESH									0x01
#define SAT_CMD_SETUP_EVENTLIST							0x05
#define SAT_CMD_SETUP_CALL								0x10
#define SAT_CMD_SEND_SS									0x11
#define SAT_CMD_SEND_USSD								0x12
#define SAT_CMD_SEND_SHORT_MESSAGE						0x13
#define SAT_CMD_SEND_DTMF								0x14
#define SAT_CMD_LAUNCH_BROWSER							0x15
#define SAT_CMD_PLAY_TONE								0x20
#define SAT_CMD_DISPLAY_TEXT								0x21
#define SAT_CMD_GET_INKEY								0x22
#define SAT_CMD_GET_INPUT								0x23
#define SAT_CMD_SELECT_ITEM								0x24
#define SAT_CMD_SETUP_MENU								0x25
#define SAT_CMD_SETUP_IDLE_MODE_TEXT					0x28
#define SAT_CMD_RUN_AT_COMMAND							0x34
#define SAT_CMD_OPEN_CHANNEL							0x40
#define SAT_CMD_CLOSE_CHANNEL							0x41
#define SAT_CMD_RECEIVE_DATA							0x42
#define SAT_CMD_SEND_DATA								0x43


/******************************************************************************************************************************************************/
//! read file parameters

//! Card EF access mode
#define	SAT_CARD_MODE_GET_INFO								0x00	//!<  Get EF information
#define	SAT_CARD_MODE_READ_FILE								0x01	//!<  Read EF body 
#define	SAT_CARD_MODE_READ_PART								0x02	//!<  Read EF part 
#define	SAT_CARD_MODE_READ_REC								0x03	//!<  Read EF record

//! Card EF structure
#define SAT_CARD_FILE_STRUCT_TRANSPARENT					0x00	//!< Transparent EF file: binary files
#define SAT_CARD_FILE_STRUCT_LINEAR_FIXED					0x01	//!< Linear Fixed File: file divided into several equal-length records
#define SAT_CARD_FILE_STRUCT_CYCLIC							0x03	//!< Cyclic File: used to keep track of data in chronological order

/******************************************************************************************************************************************************/

//! SetUpEventList parameters: each byte in the event list shall be coded with one of the values below: 
#define SAT_EVENT_USER_ACTIVITY								0x04	//!< User activity
#define SAT_EVENT_IDLE_SCREEN_AVAILABLE						0x05	//!< Idle screen available
#define SAT_EVENT_LANGUAGE_SELECTION						0x07	//!< Language selection
#define SAT_EVENT_BROWSER_TERMINATION						0x08	//!< Browser termination
#define SAT_EVENT_DATA_AVAILABLE							0x09	//!< Data available
#define SAT_EVENT_CHANNEL_STATUS							0x0A	//!< Channel status
#define SAT_EVENT_ACCES_TECH_CHANGE							0x0B	//!< Access Technology Change
#define SAT_EVENT_DISPLAY_PARAMETERS_CHANGED				0x0C	//!< Display parameters changed
#define SAT_EVENT_LOCAL_CONNECTION							0x0D	//!< Local connection
#define SAT_EVENT_BROWSING_STATUS							0x0F	//!< Browsing status
#define SAT_EVENT_FRAMES_INFORMATION_CHANGE					0x10	//!< Frames Information Change
#define SAT_EVENT_IWLAN_ACCESS_STATUS						0x11	//!< I-WLAN Access Status

/******************************************************************************************************************************************************/
//! PLAY TONE command parameters

#define SAT_PLAY_TONE_DIAL_TONE									0x01	//!< Dial tone
#define SAT_PLAY_TONE_CALLER_BUSY								0x02	//!< Called subscriber busy
#define SAT_PLAY_TONE_CONGESTION								0x03	//!< Congestion
#define SAT_PLAY_TONE_RADIO_PATH_ACKNOWLEDGE					0x04 	//!< Radio path acknowledge
#define SAT_PLAY_TONE_CALL_DROPPED								0x05 	//!< Radio path not available/Call dropped
#define SAT_PLAY_TONE_SPECIAL_INFORMATION_OR_ERROR				0x06 	//!< Error/Special information
#define SAT_PLAY_TONE_CALL_WAITING_TONE							0x07 	//!< Call waiting tone
#define SAT_PLAY_TONE_RINGING_TONE								0x08 	//!< Ringing tone

//! Terminal proprietary tones:
#define SAT_GENERAL_BEEP										0x10	//!< General beep	
#define SAT_POSITIVE_ACKNOWLEDGE_TONE							0x11	//!< Positive acknowledgement tone
#define SAT_NEGATIVE_ACKNOWLEDGE_TONE							0x12	//!< Negative acknowledgement or error tone
#define SAT_USER_RINGING_TONE									0x13	//!< Ringing tone as selected by the user for incoming speech call
#define SAT_USER_SMS_TONE										0x14	//!< Alert tone as selected by the user for incoming SMS
#define SAT_CRITICAL_ALERET										0x15	//!< Critical Alert - This tone is to be used in critical situations.        terminal;

//! Themed tones:
#define SAT_VIBRATE_ONLY										0x20	//!< vibrate only, if available
#define SAT_HAPPY_TONE											0x30	//!< happy tone
#define SAT_SAD_TONE											0x31	//!< sad tone
#define SAT_URGENT_ACTION_TONE									0x32	//!< urgent action tone
#define SAT_QUESTION_TONE										0x33	//!< question tone
#define SAT_MESSAGE_RECEIVED_TONE								0x34	//!< message received tone

//! Melody tones:
#define SAT_MELODY1_TONE										0x40	//!< Melody 1
#define SAT_MELODY2_TONE										0x41	//!< Melody 2
#define SAT_MELODY3_TONE										0x42	//!< Melody 3
#define SAT_MELODY4_TONE										0x43	//!< Melody 4
#define SAT_MELODY5_TONE										0x44	//!< Melody 5
#define SAT_MELODY6_TONE										0x45	//!< Melody 6
#define SAT_MELODY7_TONE										0x46	//!< Melody 7
#define SAT_MELODY8_TONE										0x47	//!< Melody 8


/**********************************************************************************************************************************************************************
 *     
 *		Command qualifier     
 *     
 **********************************************************************************************************************************************************************/

//! Refresh command qualifier
#define SAT_REFRESH_SIM_INIT_AND_FULL_FILE_CHANGE_NOTIFICATION	0x00	//!< NAA Initialization and Full File Change Notification
#define SAT_REFRESH_FILE_CHANGE_NOTIFICATION					0x01	//!< File Change Notification
#define SAT_REFRESH_SIM_INIT_AND_FILE_CHANGE_NOTIFICATION		0x02	//!< NAA Initialization and File Change Notification
#define SAT_REFRESH_SIM_INIT									0x03	//!< NAA Initialization
#define SAT_REFRESH_SIM_RESET									0x04	//!< UICC Reset
#define SAT_REFRESH_USIM_INIT									0x05	//!< NAA Application Reset, only applicable for a 3G platform
#define SAT_REFRESH_USIM_RESET									0x06	//!< NAA Session Reset, only applicable for a 3G platform;

/******************************************************************************************************************************************************/

//! SetUpCall command qualifier 
#define SAT_SETUP_CALL_ONLY_IF_NOT_BUSY									0x00	//!< Set up call, but only if not currently busy on another call
#define SAT_SETUP_CALL_ONLY_IF_NOT_BUSY_WITH_REDIAL						0x01	//!< Set up call, but only if not currently busy on another call, with redial
#define SAT_SETUP_CALL_AND_PUT_ALL_OTHER_CALLS_ON_HOLD					0x02	//!< Set up call, putting all other calls (if any) on hold
#define SAT_SETUP_CALL_AND_PUT_ALL_OTHER_CALLS_ON_HOLD_WITH_REDIAL		0x03	//!< Set up call, putting all other calls (if any) on hold, with redial
#define SAT_SETUP_CALL_AND_DISCONNECT_ALL_OTHER_CALLS					0x04	//!< Set up call, disconnecting all other calls (if any)
#define SAT_SETUP_CALL_AND_DISCONNECT_ALL_OTHER_CALLS_WITH_REDIAL		0x05	//!< Set up call, disconnecting all other calls (if any), with redial

/******************************************************************************************************************************************************/

//! SEND SHORT MESSAGE command qualifier
#define  SAT_SEND_SMS_PACKING_NOT_REQUIRED 			0x00 //!< bit 1: 0 = packing not required;
#define  SAT_SEND_SMS_PACKING_REQUIRED         		0x01 //!< bit 1: 1 = SMS packing by the ME required.

/******************************************************************************************************************************************************/

//! Play Tone command qualifier
#define SAT_VIBRATE_ALERT_UP_TO_TERMINAL			0x00	//!< use of vibrate alert is up to the terminal
#define SAT_VIBRATE_ALERT_WITH_TONE					0x01	//!< vibrate alert, if available, with the tone

/******************************************************************************************************************************************************/

//! DISPLAY MULTIMEDIA MESSAGE / DISPLAY TEXT command qualifier 

// priority
#define	SAT_NORMAL_PRIORITY							0x00	//!<normal priority
#define	SAT_HIGH_PRIORITY							0x01	//!<high priority

//clearAfterDelay	
#define	SAT_USER_CLEAR								0x00	//!<wait for user to clear message
#define	SAT_CLEAR_AFTER_DELAY						0x01	//!<clear after delay

/******************************************************************************************************************************************************/

//! GET INKEY / GET INPUT command qualifier (common part)

//respType
#define	 SAT_RESP_TYPE_YES_NO_OPTION				0x01	// Character sets disabled and the "Yes/No" response is requested, applies to GetInkey
#define  SAT_RESP_TYPE_UNPACKED_DIGITS_ONLY			0x02	// unpacked digits (0 to 9, *, #, and +) only, applies to GetInkey, GetInput
#define  SAT_RESP_TYPE_PACKED_DIGITS_ONLY			0x03	// packed digits (0 to 9, *, #, and +) only, applies to GetInput
#define  SAT_RESP_TYPE_UCS2_DIGITS_ONLY				0x04	// UCS2 digits (0 to 9, *, #, and +) only, applies to GetInkey, GetInput
#define  SAT_RESP_TYPE_UNPACKED_SMS_ALPHABET		0x05	// SMS default alphabet 8 bits data, applies to GetInkey, GetInput 
#define  SAT_RESP_TYPE_PACKED_SMS_ALPHABET			0x06	// SMS default alphabet 7 bits packed, applies to GetInput
#define  SAT_RESP_TYPE_UCS2_ALPHABET				0x07   	// 16 bits UCS2 alphabet, applies to GetInkey, GetInput

/******************************************************************************************************************************************************/

//! GET INPUT command qualifier dispatched on the field hideUserInput

//hideUserInput
#define  SAT_GET_INPUT_NO_ECHO						0x00	//!< user input shall not be revealed in any way
#define  SAT_GET_INPUT_WITH_ECHO					0x01	//!< terminal may echo user input on the display

/******************************************************************************************************************************************************/

//! SELECT ITEM command qualifier

// specific to SELECT ITEM command
#define SAT_DEFAULT_STYLE							0x00	//!< presentation type is not specified
#define SAT_PRESENT_AS_DATA_VALUES					0x01	//!< presentation as a choice of data values
#define SAT_PRESENT_AS_NAVIGATION_OPTIONS			0x02	//!< presentation as a choice of navigation options


/******************************************************************************************************************************************************/
//! Setup menu command qualifier dispatched on the softkey and helpInfo fields 	

//softKey
#define  SAT_NO_SOFT_KEY							0x00	//!< no selection preference
#define  SAT_SOFT_KEY_PREFERRED						0x01	//!< selection using softkey preferred

/******************************************************************************************************************************************************/

//! Language Notification command qualifier
#define SAT_LANGUAGE_NON_SPECIFIC_NOTIFICATION		0x00	//!< non-specific language notification
#define SAT_LANGUAGE_SPECIFIC_NOTIFICATION			0x01	//!< specific language notification

/******************************************************************************************************************************************************/

//! LaunchBrowser command qualifier 
#define SAT_LAUNCH_BROWSER_IF_NOT_ALREADY_LAUNCHED				0x00	//!< launch browser if not already launched
#define SAT_LAUNCH_BROWSER_USE_EXISTING							0x02	//!< use the existing browser (the browser shall not use the active existing secured session)
#define SAT_LAUNCH_BROWSER_CLOSE_EXISTING_AND_LAUNCH_NEW		0x03	//!< close the existing browser session and launch new browser session

/******************************************************************************************************************************************************/

//! Open Channel command qualifier
#define SAT_ON_DEMAND_LINK_ESTABLISHMENT						0x00	//!< on demand link establishment
#define SAT_IMMEDIATE_LINK_ESTABLISHMENT						0x01	//!< immediate link establishment

/******************************************************************************************************************************************************/
//! common to Setup Menu/Get input / Get inkey

//helpInfo
#define  SAT_NO_HELP											0x00	//!< no help available
#define  SAT_HELP_AVAILABLE										0x01	//!< help available



//!********************************************************************************
//!* TLV data objects parameters                                                  *
//!* 																			  *
//!********************************************************************************

//!Bearer codes used in the LAUNCH BROWSER command within the bearer list
//!the terminal shall use the list of bearers associated with the Launch Broser command to choose which bearers are allowed in order of priority.
#define SAT_BEARER_SMS											0x00	//!< short message
#define SAT_BEARER_CSD											0x01	//!< circuit switched data
#define SAT_BEARER_USSD											0x02	//!<
#define SAT_BEARER_GPRS											0x03	//!< 

/******************************************************************************************************************************************************/
//! Command OPEN CHANNEL parameters or response to OPEN CHANNEL: 

//type of adress in the TLV object "Other adress"
#define	SAT_ADDRESS_IPV4										0x21
#define	SAT_ADDRESS_IPV6										0x97

//!Transport protocol type in the TLV object "UICC/ME interface transport level"
#define	SAT_TRANSPORT_UDP										0x01	//!< UDP, UICC in client mode (as defined in RFC 768 [9])
#define	SAT_TRANSPORT_TCP_CLIENT_MODE							0x02	//!< TCP, UICC in client mode (as defined in RFC 793 [10])
#define	SAT_TRANSPORT_TCP_SERVER_MODE							0x03	//!< TCP, UICC in server mode (as defined in RFC 793 [10])

//! Bearer type in the bearer description parameter 
#define SAT_CSD													0x01	//!< circuit switched data
#define SAT_GPRS_3G_PACKET_SERVICE								0x02	//!< GPRS / 3G packet service
#define SAT_DEFAULT_BEARER										0x03	//!< default bearer for requested transport layer
#define SAT_TECHNOLOGY_INDEPENDANT								0x04	//!< local link technology independent
#define SAT_BLUETOOTH											0x05	//!< Bluetooth
#define SAT_IRDA												0x06	//!< IrDA
#define SAT_RS232												0x07	//!< RS232
#define SAT_PACKET_DATA_SERVICE									0x08	//!< TIA/EIA/IS-820 packet data service 
#define SAT_UTRAN												0x09	//!< UTRAN packet service with extended parameters
#define SAT_USB													0x10	//!< USB

/******************************************************************************************************************************************************/

//! Icon qualifier (indicates to the terminal how the icon is to be used)
#define SAT_SHOW_ICON_WITHOUT_TEXT 								0x00 //!< icon is self-explanatory, i.e. if displayed, it replaces the alpha identifier or text string
#define SAT_SHOW_ICON_WITH_TEXT									0x01 //!< icon is not self-explanatory, i.e. if displayed, it shall be displayed together with the alpha identifier or text string

/******************************************************************************************************************************************************/

//! DISPLAY TEXT Parameters (Data coding scheme)
#define	SAT_DCS_SMS_PACKED										0x00 //!<use the SMS default 7-bit coded alphabet, packed into 8-bit octets, as defined in TS 123 038 [3]
#define	SAT_DCS_SMS_UNPACKED									0x04 //!<use the SMS default 7-bit coded alphabet as defined in TS 123 038 [3] with bit 8 set to 0
#define	SAT_DCS_SMS_UNICODE										0x08 //!<use the UCS2 alphabet if the UCS2 is supported, as defined in TS 123 038 [3]

/******************************************************************************************************************************************************/

// Command Menu Selection parameters:  
#define SAT_MENU_SELECTION_NO_HELP_REQUESTED			0x00 //!< do not insert help request tag in the enveloppe message MENU SELECTION
#define SAT_MENU_SELECTION_HELP_REQUESTED				0x01 //!< insert help request tag in the enveloppe message MENU SELECTION to request help information on the item selected
//#define SAT_MENU_SELECTION_APP_MENU_REQUEST			0x03 //!<  ??? specific to I250 phone driver

/******************************************************************************************************************************************************/

//! Browser termination cause to join in the structure of enveloppe (Browser termination)  
#define SAT_BROWSER_USER_TERMINATION							0x00
#define SAT_BROWSER_ERROR_TERMINATION							0x01

/******************************************************************************************************************************************************/

//! Terminate reasons (not used)
//#define SAT_TERMINATE_USER_STOPPED_REDIALING					0
//#define SAT_TERMINATE_ENDS_OF_REDIALING_REACHED				1
//#define SAT_TERMINATE_USER_ENDS_SESSION						2
/******************************************************************************************************************************************************/

//! Setup Call actions
#define SAT_CALL_REJECT											0x00 //!< User did not accept the proactive command SETUP CALL
#define SAT_CALL_ACCEPT											0x01 //!< User accept to setup the call

/******************************************************************************************************************************************************/

//! General result codes
#define	SAT_RES_COMMAND_SUCCESSFUL									0x00	//!<Command performed successfully
#define	SAT_RES_COMMAND_SUCCESSFUL_WITH_PARTIAL_COMPREHENSION		0x01	//!<Command performed with partial comprehension
#define	SAT_RES_COMMAND_SUCCESSFUL_WITH_MISSING_INFORMATION			0x02	//!<Command performed, with missing information
#define	SAT_RES_REFRESH_SUCCESSFUL_WITH_ADDITIONAL_EFS_READ			0x03	//!<REFRESH performed with additional EFs read
#define	SAT_RES_COMMAND_SUCCESSFUL_BUT_ICON_NOT_FOUND				0x04	//!<Command performed successfully, but requested icon could not be displayed
#define	SAT_RES_COMMAND_SUCCESSFUL_BUT_MODIFIED_BY_CALL_CONTROL		0x05	//!<Command performed, but modified by call control by NAA
#define	SAT_RES_COMMAND_SUCCESSFUL_BUT_LIMITED_SERVICE				0x06	//!<Command performed successfully, limited service
#define	SAT_RES_COMMAND_SUCCESSFUL_WITH_MODIFICATION				0x07	//!<Command performed with modification

#define	SAT_RES_ABORTED_BY_USER										0x10	//!<Proactive UICC session terminated by the user
#define	SAT_RES_BACKWARD											0x11	//!<Backward move in the proactive UICC session requested by the user
#define	SAT_RES_NO_RESPONSE											0x12	//!<No response from user
#define	SAT_RES_HELP_REQUIRED										0x13	//!<Help information required by the user
#define	SAT_RES_USSD_ABORTED_BY_USER								0x14	//!<USSD or SS transaction terminated by the user
#define	SAT_RES_ME_UNABLE_TO_PROCESS_COMMAND						0x20	//!<terminal currently unable to process command
#define	SAT_RES_NETWORK_UNABLE_TO_PROCESS_COMMAND					0x21	//!<Network currently unable to process command
#define	SAT_RES_USER_REJECTED_SETUP_CALL							0x22	//!<User did not accept the proactive command
#define	SAT_RES_USER_CLEARED_BEFORE_RELEASE							0x23	//!<User cleared down call before connection or network release
#define	SAT_RES_ACTION_CONTRADICT_TIMER_STATE						0x24	//!<Action in contradiction with the current timer state
#define	SAT_RES_TEMP_PROBLEM_IN_CALL_CONTROL						0x25	//!<Interaction with call control by NAA, temporary problem
#define	SAT_RES_LAUNCH_BROWSER_ERROR								0x26	//!<Launch browser generic error code
#define	SAT_RES_COMMAND_BEYOND_ME_CAPABILITIES						0x30	//!<Command beyond terminal's capabilities
#define	SAT_RES_COMMAND_TYPE_NOT_UNDERSTOOD							0x31	//!<Command type not understood by terminal
#define	SAT_RES_COMMAND_DATA_NOT_UNDERSTOOD							0x32	//!<Command data not understood by terminal
#define	SAT_RES_COMMAND_NUMBER_NOT_KNOWN							0x33	//!<Command number not known by terminal
#define	SAT_RES_SS_RETURN_ERROR										0x34	//!<SS Return Error
#define	SAT_RES_SMS_RP_ERROR										0x35	//!<SMS RP-ERROR
#define	SAT_RES_REQUIRED_VALUES_MISSING								0x36	//!<Error, required values are missing
#define	SAT_RES_USSD_RETURN_ERROR									0x37	//!<USSD Return Error
//#define	SAT_MULTIPLE_CARD_COMMAND_ERROR						0x38	 //!<MultipleCard commands error
#define	SAT_RES_PERMANENT_PROBLEM_IN_SMS_OR_CALL_CONTROL			0x39	//!<Interaction with call control by USIM or MO short message control by USIM, permanent problem
#define	SAT_RES_BEARER_INDEPENDENT_PROTOCOL_ERROR					0x3A	//!<Bearer Independent Protocol error
#define	SAT_RES_ACCESS_TECHNOLOGY_UNABLE_TO_PROCESS					0x3B	//!<Access Technology unable to process command
#define	SAT_RES_FRAMES_ERROR										0x3C	//!<Frames error
#define	SAT_RES_MMS_ERROR											0x3D	//!<MMS Error



/**
 * @addtogroup SAT
 * @{
 */
#define	ALP_TEL_CARD_GET_FILE_MESSAGE							((SAT_SERVICE_ID << 8) | 0x0000)
#define	SAT_GET_CONFIG_MESSAGE							((SAT_SERVICE_ID << 8) | 0x0001)
#define	SAT_SET_CONFIG_MESSAGE							((SAT_SERVICE_ID << 8) | 0x0002)
#define	SAT_TERMINATE_MESSAGE							((SAT_SERVICE_ID << 8) | 0x0003)
#define	SAT_GET_CMD_PARAMETERS_MESSAGE					((SAT_SERVICE_ID << 8) | 0x0004)
#define	SAT_SET_CMD_RESPONSE_MESSAGE					((SAT_SERVICE_ID << 8) | 0x0005)
#define	SAT_MENU_SELECTION_MESSAGE						((SAT_SERVICE_ID << 8) | 0x0006)
#define	SAT_CALL_ACTION_MESSAGE							((SAT_SERVICE_ID << 8) | 0x0007)
#define	SAT_NOTIFY_CARD_OF_EVENT_MESSAGE				((SAT_SERVICE_ID << 8) | 0x0008)

#define	SAT_MESSAGES_COUNT								((SAT_NOTIFY_CARD_OF_EVENT_MESSAGE & ALP_TEL_MESSAGE_MASK) + 1)	//!< Total count of CAT service messages



#endif	// SAT_VALUES_H
