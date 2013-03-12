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

#ifndef _LINUXTAPI_H_
#define _LINUXTAPI_H_


#ifdef __cplusplus
extern "C"
{
#endif


    // ++++++++++++++++++++++++++++++++++++++++++++++include about standard library
#include <stdio.h>
    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++include user define
#include "phoneprotocol.h"
#include "linuxtapi4type.h"
#include "tapi4phonebook.h"
    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++define area
#define DEF_SMS_MAX_DIGIT_NUMBER                                             32
#define DEF_SMS_MAX_DATETIME                                                  6
#define DEF_SMS_MAX_CONTENTS                                                256
#define DEF_SMS_MAX_ELEMENT                                                  64
#define DEF_PHONEBOOK_MAX_ALPHA                                              32
#define DEF_PHONEBOOK_MAX_DIGIT                                              32
#define DEF_PHONEBOOK_MAX_ELEMENT                                           256
#define DEF_CABLE_KIND_NONE                                                   0
#define DEF_CABLE_KIND_TRABAL_CHARGER                                         1
#define DEF_CABLE_KIND_HW_TEST                                              100


    // tapi +++++++++++++++++++++++++++++++++++++++++++++++++++++

    // define of linux tapi message
    //
    typedef struct // lxt_message
    {
        unsigned short length;
        unsigned char group;
        unsigned char action;
        void *data;
    }
    LXT_MESSAGE;

    // define of call back function
    typedef int (*LXT_CALLBACK)(int group, int action, void* data, int length);

    // define of linux tapi handle
    typedef struct _lxt_handle
    {
        int fd;
        int clientclass;
        LXT_CALLBACK cb;

    }
    LXT_HANDLE;

    // sms ++++++++++++++++++++++++++++++++++++++++++++++++++++++

    // define of sms header
    typedef struct
    {

        // memory index
        unsigned char memoryIndex;

        // tag
        unsigned char tag;

        // remaining frame count
        unsigned char rfr_cnt;

        // pTop or broadcast message
        unsigned char sms_msg_cat;

        // sms param mask
        unsigned char sms_param_mask[4];

        // teleservice
        unsigned char teleservice_id;

        // message type
        unsigned char message_type;

        // message id
        unsigned short message_id;

        // bearer reply option
        unsigned char bearer_reply_option;

        // message center time stamp
        unsigned char message_center_time_stamp[DEF_SMS_MAX_DATETIME];

        // validity period
        unsigned char validity_period_absolute[DEF_SMS_MAX_DATETIME];

        // language indicator
        unsigned char language_indicator;

        // user data encoding
        unsigned char data_encoding;

        // destination number
        unsigned char destination_number[DEF_SMS_MAX_DIGIT_NUMBER];

        // call back number
        unsigned char call_back_number[DEF_SMS_MAX_DIGIT_NUMBER];

        // number of message(voice)
        unsigned char number_of_messages;

        // priority indicator
        unsigned char priority_indicator;

        // priority indicator
        unsigned char reply_option;

        // message display mode
        unsigned char message_display_mode[3];

        // size of user data
        unsigned char size;

    }
    LXT_SMS_HEADER;

    // define of sms body
    typedef struct
    {
        // user data
        unsigned char content[DEF_SMS_MAX_CONTENTS];

    }
    LXT_SMS_BODY;

    // define of sms
    typedef struct
    {
        char isRead;
        LXT_SMS_HEADER header;
        LXT_SMS_BODY body;

    }
    LXT_SMS;


    // define sms set
    typedef struct
    {
        int capacityOfRecord;
        int capacityOfAllField;
        int countUnread;
        int countTotal;
        LXT_SMS smsElement[DEF_SMS_MAX_ELEMENT];

    }
    LXT_SMS_SET;


    typedef enum
    {
        LXT_SMS_TAG_SMSI_INVALID                                                = 0x00,
        LXT_SMS_TAG_SMSI_MT_READ                                                = 0x01,
        LXT_SMS_TAG_SMSI_MT_NOT_READ                                            = 0x03,
        LXT_SMS_TAG_SMSI_MO_SENT                                                = 0x05,
        LXT_SMS_TAG_SMSI_MO_NOT_SENT                                            = 0x07

    } LXT_SMS_TAG;

    typedef enum
    {
        LXT_SMS_DELIVERY_STATUS_SUCCESS                                         = 0x00,
        LXT_SMS_DELIVERY_STATUS_FULL                                            = 0x01

    } LXT_SMS_DELIVERY_STATUS;


    // uim phone book +++++++++++++++++++++++++++++++++++++++++++++++
    typedef struct
    {
        int memoryIndex;
        int sizeAlpha;
        char alpha[DEF_PHONEBOOK_MAX_ALPHA];
        int sizeDigit;
        char digit[DEF_PHONEBOOK_MAX_DIGIT];

    }
    LXT_PHONEBOOK_ELEMENT;

    // phone book set
    typedef struct
    {
        int capacityOfRecord;
        int capacityOfNameField;
        LXT_PHONEBOOK_ELEMENT phoneBookElement[DEF_PHONEBOOK_MAX_ELEMENT];

    }
    LXT_PHONEBOOK_SET;



    // utk ++++++++++++++++++++++++++++++++++++++++++++++++++++++

    // define of utk header
    typedef struct
    {
        unsigned char count;
    }
    LXT_UTK_MENU_HEADER;

    // define of utk body
    typedef struct
    {
        // user data
        unsigned char *user_data;
        unsigned char *param_idx;
    }
    LXT_UTK_MENU_BODY;

    // define of utk GET input header
    typedef struct
    {
        unsigned char default_text_size;
        unsigned char item_text_size;
        unsigned char word_input;
        unsigned char in_mode;
        unsigned char min_length;
        unsigned char max_length;
        unsigned char hidden_num;
        unsigned char packed_format;
        unsigned char itemIndex;
        unsigned char input_type;

    }
    LXT_UTK_GETINPUT_HEADER;

    // define of utk get input header
    typedef struct
    {
        unsigned char *default_text;
        unsigned char *item_text;

    }
    LXT_UTK_GETINPUT_BODY;

    // define of utk GET Display Text header
    typedef struct
    {
        unsigned char count;

    }
    LXT_UTK_DISPLAY_TEXT_HEADER;

    // define of utk Display Text header
    typedef struct
    {
        unsigned char *item_text;

    }
    LXT_UTK_DISPLAY_TEXT_BODY;

    // define of utk get input body
    typedef struct
    {
        unsigned char                           utk_status;
        LXT_UTK_MENU_HEADER                     menu_header;
        LXT_UTK_MENU_BODY                       menu_body;
        LXT_UTK_GETINPUT_HEADER                 input_header;
        LXT_UTK_GETINPUT_BODY                   input_body;
        LXT_UTK_DISPLAY_TEXT_HEADER             display_text_header;
        LXT_UTK_DISPLAY_TEXT_BODY               display_text_body;

    }
    LXT_UTK;

    // define of utk start state
    typedef enum
    {
        LXT_UTK_START_SUCCESS                                                   = 0x00,
        LXT_UTK_START_FAIL_GSM_MODE                                             = 0x01,
        LXT_UTK_START_FAIL_NOT_INITIALIZED                                      = 0x02,
        LXT_UTK_START_FAIL_INVALID_STATE                                        = 0x03

    } LXT_UTK_START_STATUS;

    // define of utk input mode
    typedef enum
    {
        LXT_UTK_INPUT_MODE_T9                                                   = 0x00,
        LXT_UTK_INPUT_MODE_YES_NO                                               = 0x01,
        LXT_UTK_INPUT_MODE_NUMBER_ONLY                                          = 0x02,
        LXT_UTK_INPUT_MODE_PINYIN                                               = 0x03

    } LXT_UTK_INPUT_MODE;


    // gps ++++++++++++++++++++++++++++++++++++++++++++++++++++++
    typedef struct
    {
        unsigned char teleservice_id;
        unsigned char digit_mode;
        unsigned char num_mode;
        unsigned char num_type;
        unsigned char num_plan;
        unsigned char num_fields;
        unsigned char data[48];
        unsigned char data_len;
        unsigned char encoding;
        unsigned char number_of_digits;
        unsigned char version;
        unsigned char notification_verification_indicator;
        unsigned char pos_tech_indicator;
        unsigned char corr_id;
        unsigned char pde_ip_address[4];
        unsigned char pde_port_num[2];
        unsigned char req_id[64];
        unsigned char padding_bits;
        unsigned char message_type;
        unsigned char message_id[2];
        unsigned char is_tl_ack_request;
        unsigned char transaction_id[4];

    }
    LXT_GPS_MT_SMS_DATA;

    typedef struct
    {
        unsigned char consent_indicator;
        unsigned char mpc_address[4];
        unsigned char mpc_port[4];
        unsigned char default_or_local;
        unsigned char default_address[4];
        unsigned char default_port[4];
        unsigned char local_address[4];
        unsigned char local_port[4];

    }
    LXT_GPS_MO_DATA;

    typedef struct
    {
        LXT_GPS_MT_SMS_DATA mt_data;
        LXT_GPS_MO_DATA     mo_data;

    }
    LXT_GPS_MO_SMS_DATA;

    typedef struct
    {
        unsigned char wap_url_length;
        unsigned char wap_url[128];
    }
    LXT_GPS_URL_DATA;


    typedef enum
    {
        LXT_GPS_CONSENT_INDICATOR_GIVEN                                     = 0x00,
        LXT_GPS_CONSENT_INDICATOR_DENY                                      = 0x01,
        LXT_GPS_CONSENT_INDICATOR_EXPIRED                                   = 0x02,
        LXT_GPS_CONSENT_INDICATOR_MS_REFUSED                                = 0x03,
        LXT_GPS_CONSENT_INDICATOR_NOT_NEEDED                                = 0x04,
        LXT_GPS_CONSENT_INDICATOR_ERROR                                     = 0x05,
        LXT_GPS_CONSENT_INDICATOR_UNDEFINED                                 = 0xFF

    } LXT_GPS_CONSENT_INDICATOR;

    typedef enum
    {
        LXT_GPS_VERIFICATION_NEEDED                                         = 0x00,
        LXT_GPS_VERIFICATION_NOT_NEEDED                                     = 0x01

    } LXT_GPS_VERIFICATION;

    typedef enum
    {
        LXT_GPS_POS_TECH_IS_801                                             = 0x00,
        LXT_GPS_POS_TECH_CELL_SECTOR                                        = 0x01,
        LXT_GPS_POS_TECH_DO_NOTHING                                         = 0x02

    } LXT_GPS_POS_TECH;

    typedef enum
    {
        LXT_GPS_END_REASON_NONE                                             = 0x00,
        LXT_GPS_END_REASON_BY_USER                                          = 0x01

    } LXT_GPS_END_REASON;

    typedef enum
    {
        LXT_GPS_PDE_SERVER_DEFAULT                                          = 0x00,
        LXT_GPS_PDE_SERVER_LOCAL                                            = 0x01

    } LXT_GPS_PDE_SERVER;


    // about configuration ++++++++++++++++++++++++++++++++++++++

    typedef enum
    {
        LXT_CONFIGURATION_PHONE_LOCK_NON_ACTIVE                                 = 0x00,
        LXT_CONFIGURATION_PHONE_LOCK_ACTIVE                                     = 0x01

    } LXT_CONFIGURATION_PHONE_LOCK;

    typedef enum
    {
        LXT_MISSED_ICON_HIDE	= 0x00,
        LXT_MISSED_ICON_SHOW	= 0x01
    } LXT_MISSED_ICON;

    typedef enum
    {
        LXT_CONFIGURATION_RADIO_MODE_DIS_TEST_RC                                = 0x00,
        LXT_CONFIGURATION_RADIO_MODE_F_RC1R_RC1                                 = 0x01,
        LXT_CONFIGURATION_RADIO_MODE_F_RC2R_RC2                                 = 0x02,
        LXT_CONFIGURATION_RADIO_MODE_F_RC3R_RC3                                 = 0x03,
        LXT_CONFIGURATION_RADIO_MODE_F_RC4R_RC3                                 = 0x04,
        LXT_CONFIGURATION_RADIO_MODE_F_RC5R_RC4                                 = 0x05

    } LXT_CONFIGURATION_RADIO_MODE;

    typedef enum
    {
        LXT_CONFIGURATION_PREF_MODE_HOME_ONLY                                   = 0x00,
        LXT_CONFIGURATION_PREF_MODE_ATOMATIC_A                                  = 0x01,
        LXT_CONFIGURATION_PREF_MODE_ATOMATIC_B                                  = 0x02,
        LXT_CONFIGURATION_PREF_MODE_STANDARD                                    = 0x03 // don't use

    } LXT_CONFIGURATION_PREF_MODE;

    typedef enum
    {
        LXT_CONFIGURATION_PARA_MODE_IS95A                                       = 0x03,
        LXT_CONFIGURATION_PARA_MODE_IS95B                                       = 0x04,
        LXT_CONFIGURATION_PARA_MODE_IS2000                                      = 0x06

    } LXT_CONFIGURATION_PARA_MODE;

    typedef enum
    {
        LXT_CONFIGURATION_PHONE_STATE_ON_LINE                                   = 0x00,
        LXT_CONFIGURATION_PHONE_STATE_OFF_LINE                                  = 0x01

    } LXT_CONFIGURATION_PHONE_STATE;

    typedef enum
    {
        LXT_CONFIGURATION_SIO_MODE_DIAG                                         = 0x00,
        LXT_CONFIGURATION_SIO_MODE_HFK                                          = 0x01

    } LXT_CONFIGURATION_SIO_MODE;

    typedef enum
    {
        LXT_CONFIGURATION_SLOT_MODE_ON                                          = 0x00,
        LXT_CONFIGURATION_SLOT_MODE_OFF                                         = 0x01

    } LXT_CONFIGURATION_SLOT_MODE;

    typedef enum
    {
        LXT_CONFIGURATION_VOCODER_OPTION_EVRC                                   = 0x00,
        LXT_CONFIGURATION_VOCODER_OPTION_13K                                    = 0x01,
        LXT_CONFIGURATION_VOCODER_OPTION_8K                                     = 0x02,
       LXT_CONFIGURATION_VOCODER_OPTION_DEFAULT                                = 0x03

    } LXT_CONFIGURATION_VOCODER_OPTION;

    typedef enum
    {
        LXT_CONFIGURATION_DEFAULT_MODE_USER_SETTING                             = 0x00,
        LXT_CONFIGURATION_DEFAULT_MODE_FACTORY                                  = 0x01,
        LXT_CONFIGURATION_DEFAULT_MODE_REBUILD                                  = 0x02

    } LXT_CONFIGURATION_DEFAULT_MODE;

    typedef enum
    {
        LXT_CONFIGURATION_MAC_ADDRESS_MODE_WIFI                                 = 0x00,
        LXT_CONFIGURATION_MAC_ADDRESS_MODE_BLUETOOTH                            = 0x01

    } LXT_CONFIGURATION_MAC_ADDRESS_MODE;


    // about call +++++++++++++++++++++++++++++++++++++++++++++++
    typedef enum
    {
        LXT_CALL_STATUS_IDLE                                                    = 0x00,
        LXT_CALL_STATUS_WAITING                                                 = 0x01,
        LXT_CALL_STATUS_CONNECTED                                               = 0x02,
        LXT_CALL_STATUS_RELEASED                                                = 0x03
    } LXT_CALL_STATUS;

    typedef enum
    {
        LXT_TONE_LENGTH_SHORT                                                   = 0x00,
        LXT_TONE_LENGTH_LONG                                                    = 0x01

    } LXT_TONE_LENGTH;

    typedef enum
    {
        LXT_CALL_RETRY_DISABLE                                                  = 0x00,
        LXT_CALL_RETRY_ENABLE                                                   = 0x01

    } LXT_CALL_RETRY;

    typedef enum
    {
        LXT_CALL_AT_SO_MARKOV_8K                                               = 0x0501,
        LXT_CALL_AT_SO_MARKOV_13K                                              = 0x0502,
        LXT_CALL_AT_SO_MARKOV_54                                               = 0x0503,
        LXT_CALL_AT_SO_LOOPBACK_8K                                             = 0x0401,
        LXT_CALL_AT_SO_LOOPBACK_13K                                            = 0x0402,
        LXT_CALL_AT_SO_LOOPBACK_55                                             = 0x0403,
        LXT_CALL_AT_SO_SIMPLE_TDSO                                             = 0x0404,
        LXT_CALL_AT_SO_FULL_TDSO                                               = 0x0405

    } LXT_CALL_AT_SO;

    typedef enum
    {
        LXT_CALL_VOICE_PRIVACY_STANDARD                                         = 0x00,
        LXT_CALL_VOICE_PRIVACY_ENHANCED                                         = 0x01
    }
    LXT_CALL_VOICE_PRIVACY;

    typedef enum
    {
        LXT_CALL_SOS_MODE_CDMA                                                  = 0x00,
        LXT_CALL_SOS_MODE_GSM                                                   = 0x01
    }
    LXT_CALL_SOS_MODE;
    // about display ++++++++++++++++++++++++++++++++++++++++++++


    // define of icon type
    typedef enum
    {
        LXT_ICON_TYPE_MAIN                                                      = 0x00,
        LXT_ICON_TYPE_SERVICE                                                   = 0x01,
        LXT_ICON_TYPE_MESSAGE                                                   = 0x02,
        LXT_ICON_TYPE_RINGER                                                    = 0x03,
        LXT_ICON_TYPE_ROAM                                                      = 0x04,
        LXT_ICON_TYPE_DATASERVICE                                               = 0x05,
        LXT_ICON_TYPE_EOT                                                       = 0x06,

    }
    LXT_ICON_TYPE;


    // define of ringer icon type



    typedef enum
    {

        LXT_RING_TYPE_ICON_RING                                                 = 0x00,
        LXT_RING_TYPE_ICON_INCREASING_RING                                      = 0x01,
        LXT_RING_TYPE_ICON_VIBRATE                                              = 0x02,
        LXT_RING_TYPE_ICON_VIBRATE_THEN_RING                                    = 0x03,
        LXT_RING_TYPE_ICON_LAMP                                                 = 0x04

    } LXT_RING_TYPE_ICON;

    // define of message icon type
    typedef enum
    {
        LXT_MESSAGE_ICON_NO_MESSAGE                                             = 0x00,
        LXT_MESSAGE_ICON_UNREAD_MESSAGE                                         = 0x01

    } LXT_MESSAGE_ICON;


    // about data service +++++++++++++++++++++++++++++++++++++++

    // define of RS232 PIN number
    typedef enum
    {
        LXT_RS232_SIGNAL_TYPE_DCD                                               = 0x01,
        LXT_RS232_SIGNAL_TYPE_RX                                                = 0x02,
        LXT_RS232_SIGNAL_TYPE_TX                                                = 0x03,
        LXT_RS232_SIGNAL_TYPE_DTR                                               = 0x04,
        LXT_RS232_SIGNAL_TYPE_GND                                               = 0x05,
        LXT_RS232_SIGNAL_TYPE_DSR                                               = 0x06,
        LXT_RS232_SIGNAL_TYPE_RTS                                               = 0x07,
        LXT_RS232_SIGNAL_TYPE_CTS                                               = 0x08,
        LXT_RS232_SIGNAL_TYPE_RI                                                = 0x09

    } LXT_RS232_SIGNAL_TYPE;

    // define of RS232 PIN status
    typedef enum
    {
        LXT_RS232_SIGNAL_STATUS_DEASSERT                                        = 0x00,
        LXT_RS232_SIGNAL_STATUS_ASSERT                                          = 0x01,

    } LXT_RS232_SIGNAL_STATUS;


    // about uim ++++++++++++++++++++++++++++++++++++++++++++++++

    // define of uim connect status
    typedef enum
    {
        LXT_UIM_CONNECT_STATUS_NOT_CONNECTED                                    = 0x00,
        LXT_UIM_CONNECT_STATUS_CONNECTED                                        = 0x01

    } LXT_UIM_CONNECT_STATUS;


    // define of uim chv status
    typedef enum
    {
        LXT_UIM_CHV_BLOCKED_STATUS_BLOCKED                                      = 0x00,
        LXT_UIM_CHV_BLOCKED_STATUS_NOT_BLOCKED                                  = 0x01,
        LXT_UIM_CHV_BLOCKED_STATUS_MECHANICAL_PROBLEM                           = 0x02
    } LXT_UIM_CHV_BLOCKED_STATUS;


    // define of uim lock stats
    typedef enum
    {

        LXT_UIM_LOCK_STATUS_NON_ACTIVE                                          = 0x00,
        LXT_UIM_LOCK_STATUS_ACTIVE                                              = 0x01

    } LXT_UIM_LOCK_STATUS;




    // define of uim pin code status
    typedef enum
    {
        LXT_UIM_PIN_CODE_STATUS_DISABLE                                         = 0x00,
        LXT_UIM_PIN_CODE_STATUS_ENABLE                                          = 0x01

    } LXT_UIM_PIN_CODE_STATUS;


    // define of uim lock verify
    typedef enum
    {
        LXT_UIM_LOCK_VERIFY_OK                                                  = 0x00,
        LXT_UIM_LOCK_VERIFY_FAIL                                                = 0x01

    } LXT_UIM_LOCK_VERIFY;





    // define of uim set pin code cause
    typedef enum
    {
        LXT_UIM_SET_PIN_CODE_CAUSE_OK                                           = 0x00,
        LXT_UIM_SET_PIN_CODE_CAUSE_FAIL                                         = 0x01

    } LXT_UIM_SET_PIN_CODE_CAUSE;

    // define of ICCID cause
    typedef enum
    {
        LXT_UIM_GET_ICCID_OK                                                    = 0x00,
        LXT_UIM_GET_ICCID_FAIL                                                  = 0x01

    } LXT_UIM_GET_ICCID;


    // define of chv unblock cause
    typedef enum

    {
        LXT_UIM_CHV_UNBLOCK_OK                                                  = 0x00,
        LXT_UIM_CHV_UNBLOCK_FAIL                                                = 0x01,
        LXT_UIM_CHV_UNBLOCK_COMMAND_FAIL                                        = 0x02

    } LXT_UIM_CHV_UNBLOCK;




    // define of uim verify password cause
    typedef enum
    {

        LXT_UIM_VERIFY_PIN_CODE_CAUSE_FAIL                                      = 0x00,
        LXT_UIM_VERIFY_PIN_CODE_CAUSE_PASS                                      = 0x01,
        LXT_UIM_VERIFY_PIN_CODE_CAUSE_BLOCK                                     = 0x02,
        LXT_UIM_VERIFY_PIN_CODE_CAUSE_EMPTY                                     = 0x03,
        LXT_UIM_VERIFY_PIN_CODE_CAUSE_COMMAND_FAIL                              = 0x04

    } LXT_UIM_VERIFY_PIN_CODE_CAUSE;



    // define of uim permission status
    typedef enum
    {
        LXT_UIM_PERMISSION_STATUS_DISABLE                                       = 0x00,
        LXT_UIM_PERMISSION_STATUS_ENABLE                                        = 0x01

    } LXT_UIM_PERMISSION_STATUS;


    // about sound ++++++++++++++++++++++++++++++++++++++++++++++++

    // define speaker phone mode

    typedef enum
    {
        LXT_SOUND_SPEAKER_PHONE_MODE_OFF                                        = 0x00,
        LXT_SOUND_SPEAKER_PHONE_MODE_ON                                         = 0x01,

    } LXT_SOUND_SPEAKER_PHONE_MODE;

    typedef enum
    {
        LXT_SOUND_AUDIO_PATH_HAND_SET                                           = 0x00,
        LXT_SOUND_AUDIO_PATH_HEAD_SET                                           = 0x01,
        LXT_SOUND_AUDIO_PATH_HANDS_FREE                                         = 0x02,
        LXT_SOUND_AUDIO_PATH_BT                                                 = 0x03

    } LXT_SOUND_AUDIO_PATH;

    // about power ++++++++++++++++++++++++++++++++++++++++++++++++

    // define of battery status

    typedef enum
    {
        LXT_BATTERY_STATUS_NORMAL                                               = 0x01,
        LXT_BATTERY_STATUS_WARNING                                              = 0x02,
        LXT_BATTERY_STATUS_POWEROFF                                             = 0x03

    } LXT_BATTERY_STATUS;


    typedef enum
    {
        LXT_BACKUP_BATTERY_CONTROL_OFF                                          = 0x00,
        LXT_BACKUP_BATTERY_CONTROL_ON                                           = 0x01
    } LXT_BACKUP_BATTERY_CONTROL;
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++global variable area

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++function prototype



    //LXT_HANDLE* lxt_initialize(LXT_ID_CLIENT clientclass, LXT_CALLBACK cb);
    int lxt_getDescriptor(LXT_HANDLE* handle);
    //int lxt_release(LXT_HANDLE* handle);
    int lxt_requestCurruntState(LXT_HANDLE* handle);
    int lxt_requestPhoneNumber(LXT_HANDLE* handle);
    //int lxt_callback(LXT_HANDLE* handle);
    int lxt_loop_back_request(LXT_HANDLE* handle, LXT_ID_CLIENT klass, char g, char a, char* rawdata, int size);
    int lxt_sendRawdata(LXT_HANDLE* handle, char g, char a, char* rawdata, int size);
    int lxt_factoryMode(LXT_HANDLE* handle, unsigned char type);
    int lxt_requestNotifyPhonebookCompletion(LXT_HANDLE* handle);
    int lxt_requestPhonebookCompletionStatus(LXT_HANDLE* handle);
    int lxt_requestBackupBatteryControl(LXT_HANDLE* handle, LXT_BACKUP_BATTERY_CONTROL control);
    int lxt_requestEarjackStatus(LXT_HANDLE* handle);
    int lxt_power_on(LXT_HANDLE* handle);
    int lxt_power_off(LXT_HANDLE* handle);
    int lxt_power_reset(LXT_HANDLE* handle);
    int lxt_call_originaton(LXT_HANDLE* handle, char* callnumber);
    int lxt_call_originaton2(LXT_HANDLE* handle, LXT_CALL_AT_SO so, char* callnumber);
    int lxt_call_end(LXT_HANDLE* handle);
    int lxt_call_reject(LXT_HANDLE* handle);
    int lxt_call_answer(LXT_HANDLE* handle);
    int lxt_call_sendBurstDTMF(LXT_HANDLE* handle, LXT_TONE_LENGTH toneLength, int digitNum, char *dtmfDigit);
    int lxt_call_sendContinuousDTMF(LXT_HANDLE* handle, char digit);
    int lxt_call_stopContinuousDTMF(LXT_HANDLE* handle);
    int lxt_call_requestCallTime(LXT_HANDLE* handle);
    int lxt_call_setE911NumberRegistration(LXT_HANDLE* handle, int index, char* digit);
    int lxt_call_flashInformation(LXT_HANDLE* handle, char* callnumber);

    // removed in 819
    //int lxt_call_retry(LXT_HANDLE* handle, LXT_CALL_RETRY mode);

    int lxt_call_requestGetVoicePrivacy(LXT_HANDLE* handle);
    int lxt_call_requestSetVoicePrivacy(LXT_HANDLE* handle, LXT_CALL_VOICE_PRIVACY mode);
    int lxt_call_requestCurrentVoicePrivacyStatus(LXT_HANDLE* handle);
    int lxt_call_requestSOSCall(LXT_HANDLE* handle, LXT_CALL_SOS_MODE mode);
    /*
    int lxt_configuration_requestCurrentTime(LXT_HANDLE* handle);
    int lxt_configuration_requestPhoneNumber(LXT_HANDLE* handle);
    int lxt_configuration_requestPhoneVersion(LXT_HANDLE* handle);
    int lxt_configuration_requestPhoneLockInfo(LXT_HANDLE* handle);
    int lxt_configuration_setPhoneLock(LXT_HANDLE* handle, LXT_CONFIGURATION_PHONE_LOCK lock);

    int lxt_configuration_requestRadioConfiguration(LXT_HANDLE* handle);
    int lxt_configuration_setRadioConfiguration(LXT_HANDLE* handle, LXT_CONFIGURATION_RADIO_MODE mode);
    int lxt_configuration_requestSystemPreference(LXT_HANDLE* handle);
    int lxt_configuration_setSystemPreference(LXT_HANDLE* handle, LXT_CONFIGURATION_PREF_MODE mode);
    int lxt_configuration_requestSystemParameter(LXT_HANDLE* handle);
    int lxt_configuration_setSystemParameter(LXT_HANDLE* handle, LXT_CONFIGURATION_PARA_MODE mode);
    int lxt_configuration_requestNAMInformation(LXT_HANDLE* handle);
    int lxt_configuration_requestNAMData(LXT_HANDLE* handle, int index);
    int lxt_configuration_requestPhoneState(LXT_HANDLE* handle);
    int lxt_configuration_setPhoneState(LXT_HANDLE* handle, LXT_CONFIGURATION_PHONE_STATE state);
    int lxt_configuration_confirmBatteryMediateness(LXT_HANDLE* handle, int value);
    int lxt_configuration_reqeustPDABattery(LXT_HANDLE* handle);
    */

    int lxt_configuration2_requestPhoneLockInfo(LXT_HANDLE* handle);
    int lxt_configuration2_setPhoneLock(LXT_HANDLE* handle, LXT_CONFIGURATION_PHONE_LOCK lock)
        ;
    int lxt_configuration2_requestCurrentTime(LXT_HANDLE* handle);
    int lxt_configuration2_requestPhoneNumber(LXT_HANDLE* handle);
    int lxt_configuration2_requestPhoneVersion(LXT_HANDLE* handle);

    int lxt_configuration2_requestRadioConfiguration(LXT_HANDLE* handle);
    int lxt_configuration2_setRadioConfiguration(LXT_HANDLE* handle, LXT_CONFIGURATION_RADIO_MODE mode);
    int lxt_configuration2_requestSystemPreference(LXT_HANDLE* handle);
    int lxt_configuration2_setSystemPreference(LXT_HANDLE* handle, LXT_CONFIGURATION_PREF_MODE mode);
    int lxt_configuration2_requestSystemParameter(LXT_HANDLE* handle);
    int lxt_configuration2_setSystemParameter(LXT_HANDLE* handle, LXT_CONFIGURATION_PARA_MODE mode);
    int lxt_configuration2_requestNAMInformation(LXT_HANDLE* handle);
    int lxt_configuration2_requestNAMData(LXT_HANDLE* handle, int index);
    int lxt_configuration2_requestPhoneState(LXT_HANDLE* handle);
    int lxt_configuration2_setPhoneState(LXT_HANDLE* handle, LXT_CONFIGURATION_PHONE_STATE state);

    int lxt_configuration2_requestSIOMode(LXT_HANDLE* handle);
    int lxt_configuration2_setSIOMode(LXT_HANDLE* handle, LXT_CONFIGURATION_SIO_MODE mode);
    int lxt_configuration2_requestSlotMode(LXT_HANDLE* handle);
    int lxt_configuration2_setSlotMode(LXT_HANDLE* handle, LXT_CONFIGURATION_SLOT_MODE mode);
    int lxt_configuration2_requestVocoderOption(LXT_HANDLE* handle);
    int lxt_configuration2_setVocoderOption(LXT_HANDLE* handle, LXT_CONFIGURATION_VOCODER_OPTION mode);
    int lxt_configuration2_setDefaultConfiguration(LXT_HANDLE* handle, LXT_CONFIGURATION_DEFAULT_MODE mode);
    int lxt_configuration2_get_ESN(LXT_HANDLE* handle);
    int lxt_configuration2_get_IMEI(LXT_HANDLE* handle);
    int lxt_configuration2_vefify_pda_reset_done(LXT_HANDLE* handle, unsigned char status);
    int lxt_configuration2_set_pda_version_request(LXT_HANDLE* handle, char *version);

    /*
    int lxt_configuration2_confirmBatteryMediateness(LXT_HANDLE* handle, int value);
    int lxt_configuration2_reqeustPDABattery(LXT_HANDLE* handle);
    */

    int lxt_display_ringTypeIconChange(LXT_HANDLE* handle, LXT_RING_TYPE_ICON ringTypeIcon);
    int lxt_display_missedIconChange(LXT_HANDLE* handle, LXT_MISSED_ICON type);
    int lxt_display_icon(LXT_HANDLE* handle);
    int lxt_sound_startKeyTone(LXT_HANDLE* handle, LXT_TONE_LENGTH tone_type, int volume, int key_code);
    int lxt_sound_stopKeyTone(LXT_HANDLE* handle);
    int lxt_sound_setSpeakerPhone(LXT_HANDLE* handle, LXT_SOUND_SPEAKER_PHONE_MODE mode);
    int lxt_sound_setAudioPathControl(LXT_HANDLE* handle, LXT_SOUND_AUDIO_PATH path);

    //int lxt_sms_sendMessage(LXT_HANDLE* handle, char* destination, char* callbacknumber, int messagesize, unsigned char *message, char encoding, int memoryIndex, char tag, char priority, char reply_option);
    //int lxt_sms_sendAck(LXT_HANDLE* handle);
    int lxt_sms_setLocalCount(LXT_HANDLE* handle, unsigned char total, unsigned char unread, unsigned char spool, unsigned char unread_urgent);
    int lxt_sms_setLocalStorageSize(LXT_HANDLE* handle, unsigned char size);
    int lxt_sms_requestLocalStorageSize(LXT_HANDLE* handle);

    int lxt_dataservice_pinControl(LXT_HANDLE* handle, LXT_RS232_SIGNAL_TYPE signal, LXT_RS232_SIGNAL_STATUS status);
    int lxt_uim_cardStatus(LXT_HANDLE* handle);
    int lxt_uim_requestPinCodeStatus(LXT_HANDLE* handle);
    int lxt_uim_setPinCheck(LXT_HANDLE* handle, LXT_UIM_PIN_CODE_STATUS pinCodeStatus, int pinCodeLength, char *pinCode);
    int lxt_uim_pinVerify(LXT_HANDLE* handle, int codeLength, char *code);
    int lxt_uim_changePin(LXT_HANDLE* handle, int currentcodeLength, char *currentcode, int newcodeLength, char *newcode);
    int lxt_uim_getICCID(LXT_HANDLE* handle);
    int lxt_uim_chvUnlock(LXT_HANDLE* handle, int chvUnblockCodeLength, char *chvUnblockCode, int newPinCodeLength, char *newPinCode);
    int lxt_uim_getlock(LXT_HANDLE* handle);
    int lxt_uim_setlock(LXT_HANDLE* handle, LXT_UIM_LOCK_STATUS status);
    int lxt_uim_lockVerifyIndication(LXT_HANDLE* handle, LXT_UIM_LOCK_VERIFY status);
    int lxt_uim_smsInformation(LXT_HANDLE* handle);
    int lxt_uim_smsRead(LXT_HANDLE* handle, int address);
    int lxt_uim_smsWrite(LXT_HANDLE* handle, int address, int tag, char* parm_mask, char* destination, char* callbacknumber, char* timestemp, char encoding, int messagesize, unsigned char *message);
    int lxt_uim_smsErase(LXT_HANDLE* handle, int address);
    int lxt_uim_smsAllList(LXT_HANDLE* handle);
    int lxt_uim_smsMirroring(LXT_HANDLE* handle);
    int lxt_uim_smsSetTag(LXT_HANDLE* handle, int address, LXT_SMS_TAG tag);
    int lxt_uim_phoneBookInformation(LXT_HANDLE* handle);
    int lxt_uim_phoneBookRead(LXT_HANDLE* handle, int startAddress, int endAddress);
    int lxt_uim_phoneBookWrite(LXT_HANDLE* handle, int address, int alphaLength, char *alpha, int digitLength, char *digit);
    int lxt_uim_phoneBookErase(LXT_HANDLE* handle, int address);
    int lxt_uim_phoneBookClear(LXT_HANDLE* handle);
    int lxt_uim_smsClear(LXT_HANDLE* handle);
    int lxt_uim_phoneBookAllList(LXT_HANDLE* handle);
    int lxt_uim_phoneBookMirroring(LXT_HANDLE* handle);
    int lxt_uim_dirNameRead(LXT_HANDLE* handle);
    int lxt_uim_dirNameWrite(LXT_HANDLE* handle, int length, char * data);
    int lxt_uim_getPermissionStatus(LXT_HANDLE* handle);
    int lxt_uim_writeUIMLockPassword(LXT_HANDLE* handle, int length, char * data);



    int lxt_ruimconf_authentication(LXT_HANDLE*, LXT_CARD_AUTHENTICATION*);
    int lxt_ruimconf_select_network(LXT_HANDLE*, LXT_NETWORK_MODE);
    int lxt_ruimconf_iccid(LXT_HANDLE*);
    int lxt_ruimconf_selection_mode(LXT_HANDLE*, LXT_NETWORK_SELECTION_MODE);
    int lxt_ruimconf_change_authentication(LXT_HANDLE*, LXT_CARD_AUTHENTICATION*);
    int lxt_ruimconf_current_pin_check_state(LXT_HANDLE*, LXT_RUIM_SLOT);
    int lxt_ruimconf_change_pin_check_state(LXT_HANDLE*, LXT_CARD_AUTHENTICATION*, LXT_PIN_STATUS);
    int lxt_ruimconf_runtime_change_network(LXT_HANDLE*, LXT_SELECTED_NETWORK_MODE);
    int lxt_ruimconf_read_dir_name(LXT_HANDLE*, LXT_RUIM_SLOT);
    int lxt_ruimconf_write_dir_name(LXT_HANDLE*, char*);
    int lxt_ruimconf_change_fdn_state(LXT_HANDLE*, LXT_FDN_STATE, char*, LXT_RUIM_SLOT);
    int lxt_ruimconf_current_fdn_state(LXT_HANDLE*, LXT_RUIM_SLOT);
    int lxt_ruimconf_check_service_available(LXT_HANDLE*, char, LXT_RUIM_SLOT);
    int lxt_ruimconf_read_IMSI(LXT_HANDLE*);


    int lxt_phonebook_write(LXT_HANDLE* handle, int index, int alpha_size, char* alpha, int number_size, char* number, LXT_PHONEBOOK_PBM id);
    int lxt_phonebook_erase(LXT_HANDLE* handle, int index, LXT_PHONEBOOK_PBM id);
    int lxt_phonebook_clear(LXT_HANDLE* handle, LXT_PHONEBOOK_PBM id);

    int lxt_utk_start(LXT_HANDLE* handle);
    int lxt_utk_get_key(LXT_HANDLE* handle, char key);
    int lxt_utk_end_input(LXT_HANDLE* handle, unsigned char text_length, char* input_text);
    int lxt_utk_setup_main_menu(LXT_HANDLE* handle);

    int lxt_gps_mt_sms_request(LXT_HANDLE* handle);
    int lxt_gps_mo_sms_request(LXT_HANDLE* handle, LXT_GPS_MO_SMS_DATA*);
    int lxt_gps_spp_request(LXT_HANDLE* handle, unsigned char input_length, char* input_text);
    int lxt_gps_url_info_request(LXT_HANDLE* handle);
    int lxt_gps_end_request(LXT_HANDLE* handle, LXT_GPS_END_REASON);

	int lxt_configuration2_pda_curr_batt_adc_cfrm(LXT_HANDLE* handle, int curr_pda_batt);
    int lxt_configuration2_get_pda_low_batt_adc_req(LXT_HANDLE* handle);
    int lxt_configuration2_set_pda_low_batt_adc_req(LXT_HANDLE* handle);
    int lxt_configuration2_mac_address_req(LXT_HANDLE* handle, LXT_CONFIGURATION_MAC_ADDRESS_MODE mode);
    int lxt_configuration2_pda_password_cfrm(LXT_HANDLE* handle, char mode, char *password);
    int lxt_configuration2_device_auto_test_confirm(LXT_HANDLE* handle, char mode);
    int lxt_configuration2_service_led_onoff_confirm(LXT_HANDLE* handle, char mode);

#ifdef __cplusplus
}
#endif

#endif // _LINUXTAPI_H_
