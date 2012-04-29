/*
 *  telephony-emulator
 *
 * Copyright (c) 2000 - 2011 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact: 
 * SungMin Ha <sungmin82.ha@samsung.com>
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

#ifndef _LXTSMS_H_
#define _LXTSMS_H_


#ifdef __cplusplus
extern "C"
{
#endif


    // ++++++++++++++++++++++++++++++++++++++++++++++include about standard library
#include <stdio.h>
    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++include user define
#include "linuxtapi.h"
#include "lxtsmstypedef.h"
    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++define area
#define DEF_INIT_MAGIC_CODE                                                0xFF
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ type define area

    // Global ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // SMS_TIME_STAMP_RELATIVE
    // offset value | relative time period                     | validity period | delivery time
    // =============|==========================================|=================|===============
    // 0 ~ 143      |  (value + 1) * 5 min                     |        x        |        x
    // =============|==========================================|=================|===============
    // 144 ~ 167    |  12 hours + ((value -143) * 30 min)      |        x        |        x
    // =============|==========================================|=================|===============
    // 168 ~ 196    |  (value - 166) days                      |        x        |        x
    // =============|==========================================|=================|===============
    // 197 ~ 244    |  (value - 192) weeks                     |        x        |        x
    // =============|==========================================|=================|===============
    // 245          |  Indefinite                              |        x        |
    // =============|==========================================|=================|===============
    // 246          |  Immediate [1]                           |        x        |
    // =============|==========================================|=================|===============
    // 247          |  Valid until mobile becomes inactive     |        x        |        x
    //              |  Deliver when mobile next becomes active |                 |
    // =============|==========================================|=================|===============
    // 248          |  Valid until registration area changes,  |        x        |
    //              |  discard if not registered               |                 |
    // =============|==========================================|=================|===============
    // 249 ~ 255    |  reserved                                |                 |
    // =============|==========================================|=================|===============


    // Teleservice Identifier (0x01) +++++++++++++++++++++++++++++++++++++++++++++++

    typedef struct
    {
        // length of teleservice id
        unsigned char length;

        // teleservice identifier
        SMS_TELESERVICE_ID service_id;

    }
    SMS_TELESERVICE_IDENTIFIER;


    // Broadcast Service Category (0x02) +++++++++++++++++++++++++++++++++++++++++++

    typedef struct
    {
        // length of broadcast service category
        unsigned char length;

        // service category(2byte)
        SMS_BROADCAST_SERVICE_CATEGORY_CATEGORY category;

    }
    SMS_BROADCAST_SERVICE_CATEGORY;


    // Address (0x03) ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    typedef struct
    {
        // parameter length - 5 + num_fields
        unsigned char length;

        // digit mode indicator - 0x00 : 4BIT_DTMF, 0x01:8BIT_CODE;
        SMS_ADDRESS_DIGIT_MODE digit_mode;

        // number mode indicator - 0x00 : ANSI TI.607, 0x01 : Data Network
        SMS_ADDRESS_NUMBER_MODE number_mode;

        // type of number -
        // if ( ( DIGIT_MODE == 1 ) && ( NUMBER_MODE == 0 ) ) then use SMS_ADDRESS_NUMBER_TYPE_ANSI_TI_DOT_607
        // if ( ( DIGIT_MODE == 1 ) && ( NUMBER_MODE == 1 ) ) then use SMS_ADDRESS_NUMBER_TYPE_DATA_NETWORK
        // if ( DIGIT_MODE == 0 ) then ignore this field
        SMS_ADDRESS_NUMBER_TYPE number_type;

        // numbering plan -
        // if ( ( DIGIT_MODE == 1 ) && ( NUMBER_MODE == 0 ) ) then use SMS_ADDRESS_NUMBER_PLAN
        // if ( ( DIGIT_MODE == 0 ) || ( NUMBER_MODE == 1 ) ) then ignore this field
        SMS_ADDRESS_NUMBER_PLAN number_plan;

        // the number of digit_number
        unsigned char num_fields;

        // digit number
        // if ( DIGIT_MODE == 0 ) then ASCII
        // if ( ( DIGIT_MODE == 1 ) && ( NUMBER_MODE == 0 ) ) then ANSI X3.4(7bit code for american standard, all significant bit '0')
        // if ( ( DIGIT_MODE == 1 ) && ( NUMBER_MODE == 1 ) && ( NUMBER_TYPE == 2 ) ) mail address
        // if ( ( DIGIT_MODE == 1 ) && ( NUMBER_MODE == 1 ) && ( NUMBER_TYPE != 2 ) ) ip address
        unsigned char digit_number[DEF_SMS_MAX_DIGIT_NUMBER];

    }
    SMS_ADDRESS;


    // Subaddress (0x04) +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    typedef struct
    {

        // parameter length - 3 + num_fields
        unsigned char length;

        // subaddress type - use SMS_SUBADDRESS_TYPE
        SMS_SUBADDRESS_TYPE type;

        // odd/even indicator
        // if the last digit_number contains information only in the 4 most sign bit then '1' else '0'
        unsigned char odd;

        // the number of digit_number
        unsigned char num_fields;

        // subaddress octet
        unsigned char digit_number[DEF_SMS_MAX_DIGIT_NUMBER];

    }
    SMS_SUBADDRESS;


    // Bearer Reply Option (0x05) ++++++++++++++++++++++++++++++++++++++++++++++++++
    typedef struct
    {
        // length of rearer reply option
        unsigned char length;

        // reply sequence number
        // This field shall be set to a value identifying the SMS message for
        // which acknowledgment is requested. : 0 ~ 63 ( modulo of 8 )
        unsigned char reply_seq;

    }
    SMS_BEARER_REPLY_OPTION;


    // Cause Codes (0x06) ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    typedef struct
    {
        // parameter length
        unsigned char length;

        // reply sequence number
        // this field shall be set to the reply sequence number in the
        // Bearer Reply Option parameter of the message being acknowledged.
        unsigned char reply_seq;

        // error report class - use SMS_CAUSE_CODES_ERROR_CLASS
        SMS_CAUSE_CODES_ERROR_CLASS error_class;

        // error cause identifier - if ERROR_CLASS is 0, then this field is ignored.
        // if ERROR_CLASS is 2 or 3, then use SMS_CAUSE_CODE_CAUSE_CODE_NETWORK
        // if ERROR_CLASS is 4, then use SMS_CAUSE_CODE_CAUSE_CODE_INTERNAL
        SMS_CAUSE_CODE_CAUSE_CODE cause_code;

    }
    SMS_CAUSE_CODES;


    // Message Identifier (0x07) +++++++++++++++++++++++++++++++++++++++++++++++++++


    typedef struct
    {
        // length of message identifier
        unsigned char length;

        // use SMS_MESSAGE_TYPE
        SMS_MESSAGE_TYPE message_type;

        // Message ID Number(2 byte)
        unsigned short message_id;

    }
    SMS_MESSAGE_IDENTIFIER;


    // User Data (0x08) ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


    typedef struct
    {
        // user data length
        unsigned char length;

        // encoding - use SMS_USER_DATA_MSG_ENCODING
        SMS_USER_DATA_MSG_ENCODING msg_encoding;

        // number of digit_number
        unsigned char num_fields;

        // user data
        unsigned char* user_data;

    }
    SMS_USER_DATA;


    // User Response Code (0x09) +++++++++++++++++++++++++++++++++++++++++++++++++++
    typedef struct
    {
        // length of user response code
        unsigned char length;

        // this field value depends on the teleservices or the carrier
        unsigned char response_code;

    }
    SMS_USER_RESPONSE_CODE;


    // Message Center Time Stamp (0x0A) ++++++++++++++++++++++++++++++++++++++++++++

    typedef struct
    {
        // length of message center time stamp
        unsigned char length;

        // time stamp
        unsigned char time_stamp[6];

    }
    SMS_MESSAGE_CENTER_TIME_STAMP;


    // Validity Period - Absolute (0x0B) +++++++++++++++++++++++++++++++++++++++++++
    typedef struct
    {
        // prameter length
        unsigned char length;

        // time stamp
        unsigned char validity_period[6];

    }
    SMS_VALIDITY_PERIOD_ABSOLUTE;


    // Validity Period - Relative (0x0C) +++++++++++++++++++++++++++++++++++++++++++
    typedef struct
    {
        // parameter length
        unsigned char length;

        // relative value
        unsigned char validity;


    }
    SMS_VALIDITY_PERIOD_RELATIVE;


    // Deferred Delivery Time - Absolute (0x0D) ++++++++++++++++++++++++++++++++++++
    typedef struct
    {
        // parameter length
        unsigned char length;

        // time stamp
        unsigned char deferred_delivery_time[6];

    }
    SMS_DEFERRED_DELIVERY_TIME_ABSOLUTE;


    // Deferred Delivery Time - Relative (0x0E) ++++++++++++++++++++++++++++++++++++
    typedef struct
    {
        // parameter length
        unsigned char length;

        // relative value
        unsigned char deferred_delivery_time;

    }
    SMS_DEFERRED_DELIVERY_TIME_RELATIVE;


    // Priority Indicator (0x0F) +++++++++++++++++++++++++++++++++++++++++++++++++++

    typedef struct
    {
        // parameter length
        unsigned char length;

        // level of priority
        SMS_PRIORITY_INDICATOR_PRIORITY priority;

    }
    SMS_PRIORITY_INDICATOR;


    // Privacy Indicator (0x10) ++++++++++++++++++++++++++++++++++++++++++++++++++++

    typedef struct
    {
        // parameter length
        unsigned char length;

        // privacy level
        SMS_PRIVACY_INDICATOR_PRIVACY privacy;

    }
    SMS_PRIVACY_INDICATOR;


    // Reply Option (0x11) +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    typedef struct
    {
        // parameter length
        unsigned char length;

        // TRUE if User acknowledgment is requested (0x00 : FALSE, 0x01 : TRUE)
        unsigned char user_ack_req;

        // TRUE if Delivery acknowledgment is requested (0x00 : FALSE, 0x01 : TRUE)
        unsigned char dak_req;

    }
    SMS_REPLY_OPTION;


    // Number of Messages (0x12) +++++++++++++++++++++++++++++++++++++++++++++++++++
    typedef struct
    {
        // parameter length
        unsigned char length;

        // number of voice mail (0 ~ 99)
        unsigned char message_count;

    }
    SMS_NUMBER_OF_MESSAGES;


    // Alert on Message Delivery (0x13) ++++++++++++++++++++++++++++++++++++++++++++

    typedef struct
    {
        // parameter length
        unsigned char length;

        // the type of alerts used to distinguish different priorities of the message
        SMS_ALERT_MESSAGE_DELIVERY_ALERT_PRIORITY alert_priority;

    }
    SMS_ALERT_ON_MESSAGE_DELIVERY;


    // Language Indicator (0x14) +++++++++++++++++++++++++++++++++++++++++++++++++++


    typedef struct
    {
        // parameter length
        unsigned char length;

        // language indicator
        SMS_LANGUAGE_INDICATOR_LANGUAGE language;

    }
    SMS_LANGUAGE_INDICATOR;


    // Call Back Number (0x15) +++++++++++++++++++++++++++++++++++++++++++++++++++++
    typedef struct
    {

        // parameter length - 5 + num_fields
        unsigned char length;

        // digit mode indicator - 0x00 : 4BIT_DTMF, 0x01:8BIT_CODE;
        SMS_ADDRESS_DIGIT_MODE digit_mode;

        // type of number -
        // if ( ( DIGIT_MODE == 1 ) && ( NUMBER_MODE == 0 ) ) then use SMS_ADDRESS_NUMBER_TYPE_ANSI_TI_DOT_607
        // if ( ( DIGIT_MODE == 1 ) && ( NUMBER_MODE == 1 ) ) then use SMS_ADDRESS_NUMBER_TYPE_DATA_NETWORK
        // if ( DIGIT_MODE == 0 ) then ignore this field
        SMS_ADDRESS_NUMBER_TYPE number_type;


        // numbering plan -
        // if ( ( DIGIT_MODE == 1 ) && ( NUMBER_MODE == 0 ) ) then use SMS_ADDRESS_NUMBER_PLAN
        // if ( ( DIGIT_MODE == 0 ) || ( NUMBER_MODE == 1 ) ) then ignore this field
        SMS_ADDRESS_NUMBER_PLAN number_plan;

        // the number of digit_number
        unsigned char num_fields;

        // digit number
        // if ( DIGIT_MODE == 0 ) then ASCII
        // if ( ( DIGIT_MODE == 1 ) && ( NUMBER_MODE == 0 ) ) then ANSI X3.4(7bit code for american standard, all significant bit '0')
        // if ( ( DIGIT_MODE == 1 ) && ( NUMBER_MODE == 1 ) && ( NUMBER_TYPE == 2 ) ) mail address
        // if ( ( DIGIT_MODE == 1 ) && ( NUMBER_MODE == 1 ) && ( NUMBER_TYPE != 2 ) ) ip address
        unsigned char digit_number[DEF_SMS_MAX_DIGIT_NUMBER];


    }
    SMS_CALL_BACK_NUMBER;


    // Display Mode (0x16) +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    typedef struct
    {
        // parameter length
        unsigned char length;

        // message display mode
        SMS_MESSAGE_DISPLAY_MODE msg_display_mode;

        // prl update
        unsigned char prl_update;

        // encoding
        unsigned char encoding;

    }
    SMS_DISPLAY_MODE;


    // Multiple Encoding User Data (0x17) ++++++++++++++++++++++++++++++++++++++++++
    // TBD...
    typedef struct
    {
        // message encoding
        unsigned char msg_encoding;

        // user data length
        unsigned char num_fields;

        // user data
        unsigned char* user_data;

    }
    SMS_MULTIPLE_ENCODING_USER_DATA_ENDCODING_SPECIFIC_FIELDS;


    typedef struct
    {
        unsigned char length;
        SMS_MULTIPLE_ENCODING_USER_DATA_ENDCODING_SPECIFIC_FIELDS *encoding_specific_fields;

    }
    SMS_MULTIPLE_ENCODING_USER_DATA;


    // Memory Index (0x18) +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    typedef struct
    {
        // parameter length
        unsigned char length;

        // memory address
        unsigned short mem_address;

    }
    SMS_MEMORY_INDEX;


    // SMS Message +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    typedef struct
    {
        // ruim memory index
        unsigned char uim_memory_index;

        // ruim tag
        unsigned char uim_tag;

        // Remaining frame count
        unsigned char rfr_cnt;

        // 0x00 : SMS Point to Point Message, 0x01 : SMS Broadcast Message
        unsigned char sms_msg_cat;

        // parametr mask
        unsigned char sms_param_mask[4];

        // One of more occurrences of the following parameter record ------------

        // service id (1 byte) -- 0x01
        SMS_TELESERVICE_IDENTIFIER teleserviceIdentifier;

        // broadcast service category (2 byte) -- 0x02
        SMS_BROADCAST_SERVICE_CATEGORY broadcastServiceCategory;

        // Address-Destination Address or Origination Address -- 0x03
        SMS_ADDRESS address;

        // Subaddress-Destination or Origination -- 0x04
        SMS_SUBADDRESS subaddress;

        // Bearer reply option - Reply sequence number -- 0x05
        // This field shall be set to a value identifying the SMS message for
        // which acknowledgment is requested. : 0 ~ 63 ( modulo of 8 )
        SMS_BEARER_REPLY_OPTION bearer_reply_option;

        // Cause Codes -- 0x06
        SMS_CAUSE_CODES cause_codes;

        // message identifier -- 0x07
        SMS_MESSAGE_IDENTIFIER message_identifier;

        // user data -- 0x08
        SMS_USER_DATA user_data;

        // user response code -
        //This field value depends on the teleservices or the carrier-- 0x09
        SMS_USER_RESPONSE_CODE response_code;

        // Message Center Time Stamp -- 0x0A
        SMS_MESSAGE_CENTER_TIME_STAMP message_center_time_stamp;

        // Validity Period - Absolute -- 0x0B
        SMS_VALIDITY_PERIOD_ABSOLUTE validity_period_absolute;

        // Validity Period - Relative -- 0x0C
        SMS_VALIDITY_PERIOD_RELATIVE validity_period_relative;

        // Deferred Delivery Time - Absolute -- 0x0D
        SMS_DEFERRED_DELIVERY_TIME_ABSOLUTE deferred_delivery_time_absolute;

        // Deferred Delivery Time - Relative -- 0x0E
        SMS_DEFERRED_DELIVERY_TIME_RELATIVE deferred_delivery_time_relative;

        // Priority Indicator - use SMS_PRIORITY_INDICATOR 0x0F
        SMS_PRIORITY_INDICATOR priority_indicator;

        // Privacy Indicator - use SMS_PRIVACY_INDICATOR 0x10
        SMS_PRIVACY_INDICATOR privacy_indicator;

        // reply option -- 0x11
        SMS_REPLY_OPTION reply_option;

        // Number of Messages --0x12 (0 ~ 99)
        // The number of messages element is a 0 to 99 decimal number
        SMS_NUMBER_OF_MESSAGES number_of_messages;

        // Alert on Message Delivery -- Ox13 - use SMS_ALERT_MESSAGE_DELIVERY_ALERT_PRIORITY
        SMS_ALERT_ON_MESSAGE_DELIVERY alert_message_delivery;

        // language indicator -- 0x14 - use SMS_LANGUAGE_INDICATOR_LANGUAGE
        SMS_LANGUAGE_INDICATOR language_indicator;

        // call back number -- 0x15
        SMS_CALL_BACK_NUMBER call_back_number;

        // message display mode -- 0x16 - use SMS_MESSAGE_DISPLAY_MODE
        SMS_DISPLAY_MODE message_display_mode;

        // multiple encoding user data -- 0x17
        SMS_MULTIPLE_ENCODING_USER_DATA multiple_encoding_user_data;

        // memory index -- 0x18
        SMS_MEMORY_INDEX memory_index;


    }
    TSamsungSMS;

    // ++++++++++++++++++++++++++++++++++++++++++++++++++ function prototype define
    void lxt_sms_init_message(LXT_SMS *s);
    int lxt_sms_paramsParser(TSamsungSMS *s, unsigned char *r);
    void lxt_sms_parser(LXT_SMS *s,unsigned char memoryIndex,unsigned char tag,unsigned char rfr_cnt,unsigned char msg_cat,unsigned char *param_mask,unsigned char *rawdata,unsigned char length);
    int lxt_sms_paramsDeparser(TSamsungSMS *s, unsigned char param_id, unsigned char *r);
    unsigned char *lxt_sms_deparser(LXT_SMS *s, unsigned char *length);

#ifdef __cplusplus
}
#endif


#endif // _LXTSMS_H_
