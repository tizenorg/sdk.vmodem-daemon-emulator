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

#ifndef _LXTSMSTYPEDEF_H_
#define _LXTSMSTYPEDEF_H_


#ifdef __cplusplus
extern "C"
{
#endif


    // ++++++++++++++++++++++++++++++++++++++++++++++include about standard library

    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++include user define

    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++define area

    typedef enum
    {
        SMS_PARAM_ID_TELESERVICE_IDENTIFIER=                                    0x01,
        SMS_PARAM_ID_BROADCAST_SERVICE_CATEGORY=                                0x02,
        SMS_PARAM_ID_ADDRESS=                                                   0x03,
        SMS_PARAM_ID_SUBADDRESS=                                                0x04,
        SMS_PARAM_ID_BEARER_REPLY_OPTION=                                       0x05,
        SMS_PARAM_ID_CAUSE_CODES=                                               0x06,
        SMS_PARAM_ID_MESSAGE_IDENTIFIER=                                        0x07,
        SMS_PARAM_ID_USER_DATA=                                                 0x08,
        SMS_PARAM_ID_USER_RESPONSE_CODE=                                        0x09,
        SMS_PARAM_ID_MESSAGE_CENTER_TIME_STAMP=                                 0x0a,
        SMS_PARAM_ID_VALIDITY_PERIOD_ABSOLUTE=                                  0x0b,
        SMS_PARAM_ID_VALIDITY_PERIOD_RELATIVE=                                  0x0c,
        SMS_PARAM_ID_DEFERRED_DELIVERY_TIME_ABSOLUTE=                           0x0d,
        SMS_PARAM_ID_DEFERRED_DELIVERY_TIME_RELATIVE=                           0x0e,
        SMS_PARAM_ID_PRIORITY_INDICATOR=                                        0x0f,
        SMS_PARAM_ID_PRIVACY_INDICATOR=                                         0x10,
        SMS_PARAM_ID_REPLY_OPTION=                                              0x11,
        SMS_PARAM_ID_NUMBER_OF_MESSAGES=                                        0x12,
        SMS_PARAM_ID_ALERT_ON_MESSAGE_DELIVERY=                                 0x13,
        SMS_PARAM_ID_LANGUAGE_INDICATOR=                                        0x14,
        SMS_PARAM_ID_CALL_BACK_NUMBER=                                          0x15,
        SMS_PARAM_ID_DISPLAY_MODE=                                              0x16,
        SMS_PARAM_ID_MULTIPLE_ENCODING_USER_DATA=                               0x17,
        SMS_PARAM_ID_MEMORY_INDEX=                                              0x18
    }SMS_PARAM_ID;



    typedef enum
    {
        SMS_ADDRESS_DIGIT_MODE_4BIT_DTMF=                                       0x00,
        SMS_ADDRESS_DIGIT_MODE_8BIT_CODE=                                       0x01
    }SMS_ADDRESS_DIGIT_MODE;

    typedef enum
    {
        SMS_ADDRESS_NUMBER_MODE_ANSI_TI_DOT_607=                                0x00,
        SMS_ADDRESS_NUMBER_MODE_DATA_NETWORK=                                   0x01
    }SMS_ADDRESS_NUMBER_MODE;

    typedef unsigned char SMS_ADDRESS_NUMBER_TYPE;

    typedef enum
    {
        SMS_ADDRESS_NUMBER_TYPE_ANSI_TI_DOT_607_UNKNOWN=                        0x00,
        SMS_ADDRESS_NUMBER_TYPE_ANSI_TI_DOT_607_INTERNATIONAL_NUMBER=           0x01,
        SMS_ADDRESS_NUMBER_TYPE_ANSI_TI_DOT_607_NATIONAL_NUMBER=                0x02,
        SMS_ADDRESS_NUMBER_TYPE_ANSI_TI_DOT_607_NETWORK_SPECIFIC_NUMBER=        0x03,
        SMS_ADDRESS_NUMBER_TYPE_ANSI_TI_DOT_607_SUBSCRIBER_NUMBER=              0x04,
        SMS_ADDRESS_NUMBER_TYPE_ANSI_TI_DOT_607_RESERVED=                       0x05,
        SMS_ADDRESS_NUMBER_TYPE_ANSI_TI_DOT_607_ABBREVIATED_NUMBER=             0x06,
        SMS_ADDRESS_NUMBER_TYPE_ANSI_TI_DOT_607_RESERVED_FOR_EXTENSION=         0x07
    }SMS_ADDRESS_NUMBER_TYPE_ANSI_TI_DOT_607;

    typedef enum
    {
        SMS_ADDRESS_NUMBER_TYPE_DATA_NETWORK_UNKNOWN=                           0x00,
        SMS_ADDRESS_NUMBER_TYPE_DATA_NETWORK_INTERNET_PROTOCOL_RFC791=          0x01,
        SMS_ADDRESS_NUMBER_TYPE_DATA_NETWORK_INTERNET_EMAIL_ADDRESS_RFC822=     0x02
    }SMS_ADDRESS_NUMBER_TYPE_DATA_NETWORK;

    typedef enum
    {
        SMS_ADDRESS_NUMBER_PLAN_UNKNOWN=                                                                                                0x00,
        SMS_ADDRESS_NUMBER_PLAN_ISDN_TELEPHONY_NUMBERING_PLAN=                                                                          0x01,
        SMS_ADDRESS_NUMBER_PLAN_DATA_NUMBERING_PLAN=                                                                                    0x02,
        SMS_ADDRESS_NUMBER_PLAN_TELEX_NUMBERING_PLAN=                                                                                   0x03,
        SMS_ADDRESS_NUMBER_PLAN_PRIVATE_NUMBERING_PLAN=                                                                                 0x09,
        SMS_ADDRESS_NUMBER_PLAN_RESERVED_FOR_EXTENSION=                                                                                 0x0F
    }SMS_ADDRESS_NUMBER_PLAN;


    typedef enum
    {
        SMS_TELESERVICE_ID_TELESRV_CPT_95=                                      0x00,
        SMS_TELESERVICE_ID_TELESRV_CMT_95=                                      0x01,
        SMS_TELESERVICE_ID_TELESRV_VMN_95=                                      0x02,
        SMS_TELESERVICE_ID_TELESRV_CMT_91_CLI=                                  0x03,
        SMS_TELESERVICE_ID_TELESRV_CMT91_VOICE_MAIL=                            0x04,
        SMS_TELESERVICE_ID_TELESRV_CMT_91_SHORT_MSG=                            0x05,
        SMS_TELESERVICE_ID_TELESRV_MSG_WAITING=                                 0x06,
        SMS_TELESERVICE_ID_TELESRV_WAP=                                         0x07
    }SMS_TELESERVICE_ID;

    typedef enum
    {
        SMS_BROADCAST_SERVICE_CATEGORY_UNKNOWN_OR_UNSPECIFIED=                0x0000,
        SMS_BROADCAST_SERVICE_CATEGORY_EMERGENCY_BROADCASTS=                  0x0001,
        SMS_BROADCAST_SERVICE_CATEGORY_ADMINISTRATIVE=                        0x0002,
        SMS_BROADCAST_SERVICE_CATEGORY_MAINTENANCE=                           0x0003,
        SMS_BROADCAST_SERVICE_CATEGORY_GENERAL_NEWS_LOCAL=                    0x0004,
        SMS_BROADCAST_SERVICE_CATEGORY_GENERAL_NEWS_REGIONAL=                 0x0005,
        SMS_BROADCAST_SERVICE_CATEGORY_GENERAL_NEWS_NATIONAL=                 0x0006,
        SMS_BROADCAST_SERVICE_CATEGORY_GENERAL_NEWS_INTERNATIONAL=            0x0007,
        SMS_BROADCAST_SERVICE_CATEGORY_BUSINESS_FINANCIAL_NEWS_LOCAL=         0x0008,
        SMS_BROADCAST_SERVICE_CATEGORY_BUSINESS_FINANCIAL_NEWS_REGIONAL=      0x0009,
        SMS_BROADCAST_SERVICE_CATEGORY_BUSINESS_FINANCIAL_NEWS_NATIONAL=      0x000A,
        SMS_BROADCAST_SERVICE_CATEGORY_BUSINESS_FINANCIAL_NEWS_INTERNATIONAL= 0x000B,
        SMS_BROADCAST_SERVICE_CATEGORY_SPORTS_NEWS_LOCAL=                     0x000C,
        SMS_BROADCAST_SERVICE_CATEGORY_SPORTS_NEWS_REGIONAL=                  0x000D,
        SMS_BROADCAST_SERVICE_CATEGORY_SPORTS_NEWS_NATIONAL=                  0x000E,
        SMS_BROADCAST_SERVICE_CATEGORY_SPORTS_NEWS_INTERNATIONAL=             0x000F,
        SMS_BROADCAST_SERVICE_CATEGORY_ENTERTAINMENT_NEWS_LOCAL=              0x0010,
        SMS_BROADCAST_SERVICE_CATEGORY_ENTERTAINMENT_NEWS_REGIONAL=           0x0011,
        SMS_BROADCAST_SERVICE_CATEGORY_ENTERTAINMENT_NEWS_NATIONAL=           0x0012,
        SMS_BROADCAST_SERVICE_CATEGORY_ENTERTAINMENT_NEWS_INTERNATIONAL=      0x0013,
        SMS_BROADCAST_SERVICE_CATEGORY_LOCAL_WEATHER=                         0x0014,
        SMS_BROADCAST_SERVICE_CATEGORY_AREA_TRAFFIC_REPORTS=                  0x0015,
        SMS_BROADCAST_SERVICE_CATEGORY_LOCAL_AIRPORT_FLIGHT_SCHEDULES=        0x0016,
        SMS_BROADCAST_SERVICE_CATEGORY_RESTAURANTS=                           0x0017,
        SMS_BROADCAST_SERVICE_CATEGORY_LODGINGS=                              0x0018,
        SMS_BROADCAST_SERVICE_CATEGORY_RETAIL_DIRECTORY=                      0x0019,
        SMS_BROADCAST_SERVICE_CATEGORY_ADVERTISEMENTS=                        0x001A,
        SMS_BROADCAST_SERVICE_CATEGORY_STOCK_QUOTES=                          0x001B,
        SMS_BROADCAST_SERVICE_CATEGORY_EMPLOYMENT_OPPORTUNITIES=              0x001C,
        SMS_BROADCAST_SERVICE_CATEGORY_MEDICAL_HEALTH_HOSPITALS=              0x001D,
        SMS_BROADCAST_SERVICE_CATEGORY_TECHNOLOGY_NEWS=                       0x001E,
        SMS_BROADCAST_SERVICE_CATEGORY_MULTI_CATEGORY=                        0x001F
    }SMS_BROADCAST_SERVICE_CATEGORY_CATEGORY;


    typedef enum
    {
        SMS_SUBADDRESS_TYPE_NSAP=                                               0x00,
        SMS_SUBADDRESS_TYPE_USER_SPECIFIED=                                     0x01
    }SMS_SUBADDRESS_TYPE;


    typedef enum
    {
        SMS_CAUSE_CODES_ERROR_CLASS_NONE_ERROR=                                 0x00,
        SMS_CAUSE_CODES_ERROR_CLASS_TEMPORARY_ERROR=                            0x02,
        SMS_CAUSE_CODES_ERROR_CLASS_PERMANENT_ERROR=                            0x03,
        SMS_CAUSE_CODES_ERROR_CLASS_PHONE_INTERNAL_STATUS=                      0x04
    }SMS_CAUSE_CODES_ERROR_CLASS;

    typedef unsigned char SMS_CAUSE_CODE_CAUSE_CODE;

    typedef enum
    {
        SMS_CAUSE_CODE_CAUSE_CODE_NETWORK_ADDRESS_VACANT=                          0,
        SMS_CAUSE_CODE_CAUSE_CODE_NETWORK_ADDRESS_TRANSLATION_FAILURE=             1,
        SMS_CAUSE_CODE_CAUSE_CODE_NETWORK_NETWORK_RESOURCE_SHORTAGE=               2,
        SMS_CAUSE_CODE_CAUSE_CODE_NETWORK_NETWORK_FAILURE=                         3,
        SMS_CAUSE_CODE_CAUSE_CODE_NETWORK_INVALID_TELESERVICE_ID=                  4,
        SMS_CAUSE_CODE_CAUSE_CODE_NETWORK_OTHER_NETWORK_PROBLEM=                   5,
        SMS_CAUSE_CODE_CAUSE_CODE_NETWORK_OTHER_NETWORK_PROBLEM_MORE_FIRST=        6,
        SMS_CAUSE_CODE_CAUSE_CODE_NETWORK_OTHER_NETWORK_PROBLEM_MORE_LAST=        31,
        SMS_CAUSE_CODE_CAUSE_CODE_NETWORK_NO_PAGE_RESPONSE=                       32,
        SMS_CAUSE_CODE_CAUSE_CODE_NETWORK_DESTINATION_BUSY=                       33,
        SMS_CAUSE_CODE_CAUSE_CODE_NETWORK_NO_ACK=                                 34,
        SMS_CAUSE_CODE_CAUSE_CODE_NETWORK_DESTINATION_RESOURCE_SHORTAGE=          35,
        SMS_CAUSE_CODE_CAUSE_CODE_NETWORK_SMS_DELIVERY_POSTPONED=                 36,
        SMS_CAUSE_CODE_CAUSE_CODE_NETWORK_DESTINATION_OUT_OF_SERVICE=             37,
        SMS_CAUSE_CODE_CAUSE_CODE_NETWORK_DESTINATION_NO_LONGER_AT_THIS_ADDRESS=  38,
        SMS_CAUSE_CODE_CAUSE_CODE_NETWORK_OTHER_TERMINAL_PROBLEM=                 39,
        SMS_CAUSE_CODE_CAUSE_CODE_NETWORK_OTHER_TERMINAL_PROBLEM_MORE_FIRST=      40,
        SMS_CAUSE_CODE_CAUSE_CODE_NETWORK_OTHER_TERMINAL_PROBLEM_MORE_LAST=       47,
        SMS_CAUSE_CODE_CAUSE_CODE_NETWORK_SMS_DELIVERY_POSTPONED_MORE_FIRST=      48,
        SMS_CAUSE_CODE_CAUSE_CODE_NETWORK_SMS_DELIVERY_POSTPONED_MORE_LAST=       63,
        SMS_CAUSE_CODE_CAUSE_CODE_NETWORK_RADIO_IF_RESOURCE_SHORTAGE=             64,
        SMS_CAUSE_CODE_CAUSE_CODE_NETWORK_RADIO_IF_INCOMPATIBLE=                  65,
        SMS_CAUSE_CODE_CAUSE_CODE_NETWORK_OTHER_RADIO_IF_PROBLEM=                 66,
        SMS_CAUSE_CODE_CAUSE_CODE_NETWORK_OTHER_RADIO_IF_PROBLEM_MORE_FIRST=      67,
        SMS_CAUSE_CODE_CAUSE_CODE_NETWORK_OTHER_RADIO_IF_PROBLEM_MORE_LAST=       95,
        SMS_CAUSE_CODE_CAUSE_CODE_NETWORK_UNEXPECTED_PARM_SIZE_S=                 96,
        SMS_CAUSE_CODE_CAUSE_CODE_NETWORK_SMS_ORIGINATION_DENIED=                 97,
        SMS_CAUSE_CODE_CAUSE_CODE_NETWORK_SMS_TERMINATION_DENIED=                 98,
        SMS_CAUSE_CODE_CAUSE_CODE_NETWORK_SUPPLMENTARY_SERVICE_NOT_SUPPORTED=     99,
        SMS_CAUSE_CODE_CAUSE_CODE_NETWORK_SMS_NOT_SUPPORTED=                     100,
        SMS_CAUSE_CODE_CAUSE_CODE_NETWORK_RESERVED_101=                          101,
        SMS_CAUSE_CODE_CAUSE_CODE_NETWORK_MISSING_EXPECTED_PARM=                 102,
        SMS_CAUSE_CODE_CAUSE_CODE_NETWORK_MISSING_MANDATORY_PARM=                103,
        SMS_CAUSE_CODE_CAUSE_CODE_NETWORK_UNRECOGNIZED_PARM_VALUE=               104,
        SMS_CAUSE_CODE_CAUSE_CODE_NETWORK_UNEXPECTED_PARM_VALUE=                 105,
        SMS_CAUSE_CODE_CAUSE_CODE_NETWORK_USER_DATA_SIZE_ERROR=                  106,
        SMS_CAUSE_CODE_CAUSE_CODE_NETWORK_OTHER_GENERAL_PROBLEMS=                107,
        SMS_CAUSE_CODE_CAUSE_CODE_NETWORK_OTHER_GENERAL_PROBLEMS_MORE_FIRST=     108,
        SMS_CAUSE_CODE_CAUSE_CODE_NETWORK_OTHER_GENERAL_PROBLEMS_MORE_LAST=      255
    }SMS_CAUSE_CODE_CAUSE_CODE_NETWORK;

    typedef enum
    {
        // 0x00 : layer 2 acknowledgment received
        // 0x02 : SMS transmission failed because a transmission time is expired
        //        or phone internal error occurred during the SMS transmission

        SMS_CAUSE_CODE_CAUSE_CODE_INTERNAL_STATUS_L2ACK_RCVD=                   0x00,
        SMS_CAUSE_CODE_CAUSE_CODE_INTERNAL_STATUS_FAIL=                         0x01,
        SMS_CAUSE_CODE_CAUSE_CODE_INTERNAL_STATUS_FAIL_NO_SVC=                  0x02,
        SMS_CAUSE_CODE_CAUSE_CODE_INTERNAL_STATUS_FAIL_PHONE_BUSY=              0x03,
        SMS_CAUSE_CODE_CAUSE_CODE_INTERNAL_STATUS_FAIL_OTHER=                   0x04
    }SMS_CAUSE_CODE_CAUSE_CODE_INTERNAL;



    typedef enum
    {
        SMS_MESSAGE_TYPE_RESERVED=                                              0x00,
        SMS_MESSAGE_TYPE_SMS_DELIVER_MESSAGE=                                   0x01,
        SMS_MESSAGE_TYPE_SMS_SUBMIT_MESSAGE=                                    0x02,
        SMS_MESSAGE_TYPE_SMS_CANCELLATION_MESSAGE=                              0x03,
        SMS_MESSAGE_TYPE_SMS_DELIVERY_ACKNOWLEDGMENT_MESSAGE=                   0x04,
        SMS_MESSAGE_TYPE_SMS_USER_ACKNOWLEDGMENT_MESSAGE=                       0x05,
        SMS_MESSAGE_TYPE_ALL_MESSAGE=                                           0x0F
    }SMS_MESSAGE_TYPE;



    typedef enum
    {
        SMS_USER_DATA_MSG_ENCODING_OCTET=                                       0x00,
        SMS_USER_DATA_MSG_ENCODING_IS_91_EXTENDED_PROTOCOL=                     0x01,
        SMS_USER_DATA_MSG_ENCODING_7BIT_ASCII=                                  0x02,
        SMS_USER_DATA_MSG_ENCODING_IA5=                                         0x03,
        SMS_USER_DATA_MSG_ENCODING_UNICODE=                                     0x04,
        SMS_USER_DATA_MSG_ENCODING_SHIFT_JIS=                                   0x05,
        SMS_USER_DATA_MSG_ENCODING_KSC5601=                                     0x06,
        SMS_USER_DATA_MSG_ENCODING_ISO_8859_8=                                  0x07,
        SMS_USER_DATA_MSG_ENCODING_ISO_8859_1=                                  0x08,
        SMS_USER_DATA_MSG_ENCODING_KSC5601_KOREAN=                              0x10
    }SMS_USER_DATA_MSG_ENCODING;


    typedef enum
    {
        SMS_PRIORITY_INDICATOR_NORMAL=                                          0x00,
        SMS_PRIORITY_INDICATOR_INTERACTIVE=                                     0x01,
        SMS_PRIORITY_INDICATOR_URGENT=                                          0x02,
        SMS_PRIORITY_INDICATOR_EMERGENCY=                                       0x03
    }SMS_PRIORITY_INDICATOR_PRIORITY;



    typedef enum
    {
        SMS_PRIVACY_INDICATOR_NOT_RESTRICTED=                                   0x00,
        SMS_PRIVACY_INDICATOR_RESTRICTED=                                       0x01,
        SMS_PRIVACY_INDICATOR_CONFIDENTIAL=                                     0x02,
        SMS_PRIVACY_INDICATOR_SECRET=                                           0x03
    }SMS_PRIVACY_INDICATOR_PRIVACY;



    typedef enum
    {
        SMS_ALERT_MESSAGE_DELIVERY_ALERT_PRIORITY_MOBILE_DEFAULT_ALERT=         0x00,
        SMS_ALERT_MESSAGE_DELIVERY_ALERT_PRIORITY_LOW_PRIORITY_ALERT=           0x01,
        SMS_ALERT_MESSAGE_DELIVERY_ALERT_PRIORITY_MEDIUM_PRIORITY_ALERT=        0x02,
        SMS_ALERT_MESSAGE_DELIVERY_ALERT_PRIORITY_HIGH_PRIORITY_ALERT=          0x03
    }SMS_ALERT_MESSAGE_DELIVERY_ALERT_PRIORITY;



    typedef enum
    {
        SMS_LANGUAGE_INDICATOR_LANGUAGE_UNKNOWN=                                0x00,
        SMS_LANGUAGE_INDICATOR_LANGUAGE_ENGLISH=                                0x01,
        SMS_LANGUAGE_INDICATOR_LANGUAGE_FRENCH=                                 0x02,
        SMS_LANGUAGE_INDICATOR_LANGUAGE_SPANISH=                                0x03,
        SMS_LANGUAGE_INDICATOR_LANGUAGE_JAPANESE=                               0x04,
        SMS_LANGUAGE_INDICATOR_LANGUAGE_KOREAN=                                 0x05,
        SMS_LANGUAGE_INDICATOR_LANGUAGE_CHINESE=                                0x06,
        SMS_LANGUAGE_INDICATOR_LANGUAGE_HEBREW=                                 0x07,
        SMS_LANGUAGE_INDICATOR_LANGUAGE_KOREAN_1=                               0x40,
        SMS_LANGUAGE_INDICATOR_LANGUAGE_KOREAN_SKT=                             0xFE
    }SMS_LANGUAGE_INDICATOR_LANGUAGE;



    typedef enum
    {
        SMS_MESSAGE_DISPLAY_MODE_IMMEDIATE_DISPLAY=                             0x00,
        SMS_MESSAGE_DISPLAY_MODE_MOBILE_DEFAULT_SETTING=                        0x01,
        SMS_MESSAGE_DISPLAY_MODE_USER_INVOKE=                                   0x03,
        SMS_MESSAGE_DISPLAY_MODE_RESERVED=                                      0x04
    }SMS_MESSAGE_DISPLAY_MODE;






    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++global variable area

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++function prototype



#ifdef __cplusplus
}
#endif


#endif // _LXTSMSTYPEDEF_H_
