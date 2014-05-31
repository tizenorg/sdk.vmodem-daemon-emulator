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

#ifndef _PHONEPROTOCOL_H_
#define _PHONEPROTOCOL_H_


// define of environment+++++++++++++++++++++++++++++++++++++++++++++++++++++++


// define of phone state+++++++++++++++++++++++++++++++++++++++++++++++++++++++
typedef enum
{
    LXT_STATE_OFF                                                           = 0x01,
    LXT_STATE_ON                                                            = 0x02,
    LXT_STATE_UIM_NOT_READY                                                 = 0x03,
    LXT_STATE_UIM_READY                                                     = 0x08,
    LXT_STATE_NO_SERVICE                                                    = 0x10,
    LXT_STATE_STANDBY                                                       = 0x20,
    LXT_STATE_WAITING_OUTGOING_CALL                                         = 0x31,
    LXT_STATE_WAITING_INCOMING_CALL                                         = 0x32,
    LXT_STATE_WAITING_DATA_SERVICE                                          = 0x33,
    LXT_STATE_CONVERSATION                                                  = 0x40,
    LXT_STATE_DATA_SERVICED                                                 = 0x50,
    LXT_STATE_DIAL_UP_DATA_SERVICED                                         = 0x55,
    LXT_STATE_RELEASED                                                      = 0x60,
    LXT_STATE_RESERVED                                                      = 0x1F

} LXT_PHONE_STATE;


// define of message cast method++++++++++++++++++++++++++++++++++++++++++++++
enum LXT_MESSAGE_CAST_METHOD
{
    LXT_MESSAGE_CAST_METHOD_BY_MYSELF                                       = 0x40,  // ID of by me(comsumption by server)
    LXT_MESSAGE_CAST_METHOD_BY_OWNER                                        = 0x41,  // ID of by owner(decide by server)
    LXT_MESSAGE_CAST_METHOD_BROADCAST                                       = 0x42   // ID of all(broadcast by server)
};


// define of id+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
typedef enum
{
    LXT_ID_CLIENT_RESERVED                                                  = 0x00, // ID of Reserved
    LXT_ID_CLIENT_INDICATOR                                                 = 0x01, // ID of GUI Server
    LXT_ID_CLIENT_PHONE                                                     = 0x02, // ID of Phone Client
    LXT_ID_CLIENT_SMS                                                       = 0x03, // ID of SMS Client
    LXT_ID_CLIENT_RUIM                                                      = 0x04, // ID of RUIM Manager
    LXT_ID_CLIENT_SERVICEMODE                                               = 0x05, // ID of Service Mode
    LXT_ID_CLIENT_DATASERVICE                                               = 0x06, // ID of Data service
    LXT_ID_CLIENT_DIAL_ON_DEMAND                                            = 0x07, // ID of Dial on demand
    LXT_ID_CLIENT_UTK                                                       = 0x08, // ID of UTK Client
    LXT_ID_CLIENT_ADDRESS                                                   = 0x09, // ID of Address Book
    LXT_ID_CLIENT_PHONE_SETUP                                               = 0x0A, // ID of Phone setup
    LXT_ID_CLIENT_CARD_MANAGER                                              = 0x0B, // ID of CARD Manager
    LXT_ID_CLIENT_SMS_DAEMON                                                = 0x0C, // ID of SMS Daemon
    LXT_ID_CLIENT_GPS                                                       = 0x0D, // ID of GPS 
    LXT_ID_CLIENT_SECURITY_SETUP                                            = 0x0E, // ID of Security Setup 
    LXT_ID_CLIENT_SAT                                            			= 0x0F, // ID of sim application toolkit 
    LXT_ID_CLIENT_DATA_1                                            		= 0x10, // ID of data 1 
    LXT_ID_CLIENT_DATA_2                                            		= 0x11, // ID of data 2 
    LXT_ID_CLIENT_DATA_3                                            		= 0x12, // ID of data 3 
    LXT_ID_CLIENT_DATA_4                                            		= 0x13, // ID of data 4 
    LXT_ID_CLIENT_DATA_5                                            		= 0x14, // ID of data 5 
    LXT_ID_CLIENT_DATA_6                                            		= 0x15, // ID of data 6
    LXT_ID_CLIENT_EVENT_INJECTOR					    = 0x16, //ID of event injector 
    LXT_ID_CLIENT_EVENT_MANAGER_RX					    = 0x17, //ID of event injector 
    LXT_ID_CLIENT_EVENT_MANAGER_TX					    = 0x18, //ID of event injector 
    LXT_ID_CLIENT_EVENT_MANAGER					    = 0x19, //ID of event manager
    LXT_ID_CLIENT_INVALID                                                   = 0x39, // ID of invalid
    LXT_ID_CLIENT_ANY                                                       = 0xff
}
LXT_ID_CLIENT;





// define of main group+++++++++++++++++++++++++++++++++++++++++++++++++++++++
enum LXT_GRP
{
    LXT_GRP_INTERNAL                                                        = 0x40,  // Internal
    LXT_GRP_POWER                                                           = 0x51,  // power
    LXT_GRP_CONFIGURATION                                                   = 0x52,  // configuration
    LXT_GRP_CALL                                                            = 0x53,  // call
    LXT_GRP_DISPLAY                                                         = 0x54,  // display
    LXT_GRP_SMS                                                             = 0x55,  // sms
    LXT_GRP_SERVICEMODE                                                     = 0x56,  // service mode
    LXT_GRP_SOUND                                                           = 0x57,  // sound
    LXT_GRP_UIM                                                             = 0x58,  // UIM
    LXT_GRP_DATASERVICE                                                     = 0x59,  // Data service
    LXT_GRP_UTK                                                             = 0x5A,  // UTK
    LXT_GRP_SIOMODE                                                         = 0x5B,  // SIO Mode
    LXT_GRP_RESPONSE                                                        = 0x5F,  // response
    LXT_GRP_PHONE_BOOK                                                      = 0x61,  // phone book
    LXT_GRP_CARD_SMS                                                        = 0x62,  // card sms
    LXT_GRP_GPS                                                             = 0x63,  // GPS 
    LXT_GRP_EOT                                                             = 0x80   // eot

};


// define of ppp status +++++++++++++++++++++++++++++++++++++++++++++++++++++++
typedef enum
{
    PPP_STATUS_IDLE                         = 0x01,
    PPP_STATUS_WORKING                      = 0x02,
    PPP_STATUS_BUSY                         = 0x04,
    PPP_STATUS_DORMANT                      = 0x05,
    PPP_STATUS_SUPER                        = 0x06,
    PPP_EVENT_IP_REASSIGNED                 = 0x10

}
PPP_STATUS;

// define of action group (client info)++++++++++++++++++++++++++++++++++++++++++
enum LXT_PDA_CLIENT_INFO
{
    LXT_PDA_INTERNAL_ID_REQUEST                                             = 0x41,   // save id
    LXT_PDA_INTERNAL_STATE_REQUEST                                          = 0x42,
    LXT_PDA_INTERNAL_PHONE_NUMBER_REQUEST                                   = 0x43,
    LXT_PDA_INTERNAL_AUDIO_TO_SPEAKER_REQUEST                               = 0x44,
    LXT_PDA_INTERNAL_AUDIO_TO_RECEIVER_REQUEST                              = 0x45,
    LXT_PDA_INTERNAL_FACTORY_MODE_REQUEST                                   = 0x46,
    LXT_PDA_INTERNAL_MISSED_MESSAGE_REQUEST                                 = 0x47,
    LXT_PDA_INTERNAL_SEND_RAWDATA_REQUEST                                   = 0x50,
    LXT_PDA_INTERNAL_LOOP_BACK_REQEUST                                      = 0x51,
    LXT_PHN_INTERNAL_NOTIFY_PHONEBOOK_LOADING_COMPLETION_REQUEST            = 0x52,
    LXT_PHN_INTERNAL_PHONEBOOK_LOADING_COMPLETION_STATUS_REQUEST            = 0x53,
    LXT_PHN_INTERNAL_BACKUP_BATTERY_CONTROL_REQUEST                         = 0x55,
    LXT_PHN_INTERNAL_EARJACK_STATUS_REQUEST                                 = 0x56,
    LXT_PDA_INTERNAL_LAUNCH_SAT_APP_REQUEST									= 0x57,
    LXT_PDA_INTERNAL_SETUP_SAT_APP_REQUEST									= 0x58,
    LXT_PDA_INTERNAL_PHONE_BATTERY_REQUEST									= 0x59,
};

enum LXT_PHN_PHONE_INFO
{
    LXT_PHN_INTERNAL_STATE_RESPONSE                                         = 0x42,   // state change
    LXT_PHN_INTERNAL_STATE_CHANGE_NOTIFICATION                              = 0x43,   // state change
    LXT_PHN_INTERNAL_PHONE_NUMBER_RESPONSE                                  = 0x44,
    LXT_PHN_INTERNAL_PDA_SLEEP_NOTIFICATION                                 = 0x45,
    LXT_PHN_INTERNAL_SERVER_DIE_EMERGENCY                                   = 0x49,
    LXT_PHN_INTERNAL_SERVER_SIM_COVER_OPENED                                = 0x50,
    LXT_PHN_INTERNAL_SERVER_SIM_COVER_CLOSED                                = 0x51,
    LXT_PHN_INTERNAL_SERVER_PHONE_REBUILD_START                             = 0x52,
    LXT_PHN_INTERNAL_SERVER_PHONE_REBUILD_END                               = 0x53,                
    LXT_PHN_INTERNAL_SERVER_PHONE_FATAL_ERROR                               = 0x54,
    LXT_PHN_INTERNAL_SERVER_PHONE_NV_BACKUP_NOTIFICATION                    = 0x55,
    LXT_PHN_INTERNAL_PHONEBOOK_LOADING_COMPLETION_NOTIFICATION              = 0x56,
    LXT_PHN_INTERNAL_EARJACK_STATUS_RESPONSE                                = 0x57,
    LXT_PDA_INTERNAL_LAUNCH_SAT_APP_RESPONSE								= 0x58,
    LXT_PDA_INTERNAL_SETUP_SAT_APP_RESPONSE								    = 0x59,
};

// define of action group (power)+++++++++++++++++++++++++++++++++++++++++++++
enum LXT_PDA_POWER
{
    LXT_PDA_POWER_ON_REQUEST                                                = 0x41,  // power on
    LXT_PDA_POWER_RESET_REQUEST                                             = 0x42,  // power reset
    LXT_PDA_POWER_OFF_REQUEST                                               = 0x43   // power off

};

enum LXT_PHN_POWER
{
    LXT_PHN_POWER_BATTERY_STATUS_NOTIFICATION                               = 0x41,
    LXT_PHN_POWER_PHONE_BOOT_COMPLETED_NOTIFICATION                         = 0x42
};


// define of action group (configuration)+++++++++++++++++++++++++++++++++++++
/*
   enum LXT_PDA_CONFIGURATION
   {
   LXT_PDA_CONFIGURATION_SET_DEFAULT_CONFIGURATION_REQUEST                 = 0x41,
   LXT_PDA_CONFIGURATION_CURRENT_TIME_REQUEST                              = 0x42,  // current time request
   LXT_PDA_CONFIGURATION_PHONE_NUMBER_REQUEST                              = 0x43,
   LXT_PDA_CONFIGURATION_PHONE_VERSION_REQUEST                             = 0x44,
   LXT_PDA_CONFIGURATION_GET_PHONE_LOCK_REQUEST                            = 0x45,
   LXT_PDA_CONFIGURATION_SET_PHONE_LOCK_REQUEST                            = 0x46,
   LXT_PDA_CONFIGURATION_GET_RADIO_CONFIGURATION_REQUEST                   = 0x47,
   LXT_PDA_CONFIGURATION_SET_RADIO_CONFIGURATION_REQUEST                   = 0x48,
   LXT_PDA_CONFIGURATION_GET_SYSTEM_PREFERENCE_REQUEST                     = 0x49,
   LXT_PDA_CONFIGURATION_SET_SYSTEM_PREFERENCE_REQUEST                     = 0x4A,
   LXT_PDA_CONFIGURATION_GET_SYSTEM_PARAMETER_REQUEST                      = 0x4B,
   LXT_PDA_CONFIGURATION_SET_SYSTEM_PARAMETER_REQUEST                      = 0x4C,
   LXT_PDA_CONFIGURATION_GET_NAM_INFORMATION_REQUEST                       = 0x4D,
   LXT_PDA_CONFIGURATION_GET_NAM_DATA_REQUEST                              = 0x4E,
   LXT_PDA_CONFIGURATION_GET_PHONE_STATE_CHANGING_REQUEST                  = 0x4F,
   LXT_PDA_CONFIGURATION_SET_PHONE_STATE_CHANGING_REQUEST                  = 0x50,
   LXT_PDA_CONFIGURATION_BATTERY_MEDIATENESS_CONFIRM                       = 0x51,
   LXT_PDA_CONFIGURATION_GET_PDA_BATTERY_REQUEST                           = 0x52
   };

   enum LXT_PHN_CONFIGURATION
   {
   LXT_PHN_CONFIGURATION_SET_DEFAULT_CONFIGURATION_RESPONSE                = 0x41,
   LXT_PHN_CONFIGURATION_CURRENT_TIME_RESPONSE                             = 0x42,
   LXT_PHN_CONFIGURATION_CURRENT_TIME_NOTIFICATION                         = 0x43,
   LXT_PHN_CONFIGURATION_PHONE_NUMBER_RESPONSE                             = 0x44,
   LXT_PHN_CONFIGURATION_PHONE_VERSION_RESPONSE                            = 0x45,
   LXT_PHN_CONFIGURATION_PHONE_STATE_CHANGE_NOTIFICATION                   = 0x46,
   LXT_PHN_CONFIGURATION_PHONE_SERVICE_READY_NOTIFICATION                  = 0x47,
   LXT_PHN_CONFIGURATION_GET_PHONE_LOCK_RESPONSE                           = 0x48,
   LXT_PHN_CONFIGURATION_PHONE_LOCK_NOTIFICATION                           = 0x49,
   LXT_PHN_CONFIGURATION_GET_RADIO_CONFIGURATION_RESPONSE                  = 0x4A,
   LXT_PHN_CONFIGURATION_GET_SYSTEM_PREFERENCE_RESPONSE                    = 0x4B,
   LXT_PHN_CONFIGURATION_GET_SYSTEM_PARAMETER_RESPONSE                     = 0x4C,
   LXT_PHN_CONFIGURATION_SYSTEM_PARAMETER_CHANGE_NOTIFICATION              = 0x4D,
   LXT_PHN_CONFIGURATION_GET_NAM_INFORMATION_RESPONSE                      = 0x4E,
   LXT_PHN_CONFIGURATION_GET_NAM_DATA_RESPONSE                             = 0x4F,
   LXT_PHN_CONFIGURATION_GET_PHONE_STATE_CHANGING_RESPONSE                 = 0x50,
   LXT_PHN_CONFIGURATION_SET_PHONE_STATE_CHANGING_RESPONSE                 = 0x51,
   LXT_PHN_CONFIGURATION_HFK2_PDA_PASSWORD_INDICATION                      = 0x52,
   LXT_PHN_CONFIGURATION_BATTERY_MEDIATENESS_INDICATION                    = 0x53,
   LXT_PHN_CONFIGURATION_GET_PDA_BATTERY_RESPONSE                          = 0x54
   };
 */

enum LXT_PDA_CONFIGURATION2
{
    LXT_PDA_CONFIGURATION2_GET_PHONE_LOCK_REQUEST                           = 0x40,
    LXT_PDA_CONFIGURATION2_SET_PHONE_LOCK_REQUEST                           = 0x41,
    LXT_PDA_CONFIGURATION2_SET_DEFAULT_CONFIGURATION_REQUEST                = 0x42,
    LXT_PDA_CONFIGURATION2_GET_SIO_MODE_REQUEST                             = 0x43,
    LXT_PDA_CONFIGURATION2_SET_SIO_MODE_REQUEST                             = 0x44,
    LXT_PDA_CONFIGURATION2_CURRENT_TIME_REQUEST                             = 0x45,
    LXT_PDA_CONFIGURATION2_PHONE_NUMBER_REQUEST                             = 0x46,
    LXT_PDA_CONFIGURATION2_PHONE_VERSION_REQUEST                            = 0x47,
    LXT_PDA_CONFIGURATION2_GET_SYSTEM_PARAMETER_REQUEST                     = 0x48,
    LXT_PDA_CONFIGURATION2_SET_SYSTEM_PARAMETER_REQUEST                     = 0x49,
    LXT_PDA_CONFIGURATION2_GET_MODE_PREFERENCE_REQUEST                      = 0x4A,
    LXT_PDA_CONFIGURATION2_SET_MODE_PREFERENCE_REQUEST                      = 0x4B,
    LXT_PDA_CONFIGURATION2_GET_SYSTEM_PREFERENCE_REQUEST                    = 0x4C,
    LXT_PDA_CONFIGURATION2_SET_SYSTEM_PREFERENCE_REQUEST                    = 0x4D,
    LXT_PDA_CONFIGURATION2_GET_NAM_INFORMATION_REQUEST                      = 0x4E,
    LXT_PDA_CONFIGURATION2_GET_NAM_DATA_REQUEST                             = 0x4F,
    LXT_PDA_CONFIGURATION2_GET_PHONE_STATE_CHANGING_REQUEST                 = 0x50,
    LXT_PDA_CONFIGURATION2_SET_PHONE_STATE_CHANGING_REQUEST                 = 0x51,
    LXT_PDA_CONFIGURATION2_GET_GPS_MODE_REQUEST                             = 0x52,
    LXT_PDA_CONFIGURATION2_SET_GPS_MODE_REQUEST                             = 0x53,
    LXT_PDA_CONFIGURATION2_GET_RADIO_CONFIGURATION_REQUEST                  = 0x54,
    LXT_PDA_CONFIGURATION2_SET_RADIO_CONFIGURATION_REQUEST                  = 0x55,
    LXT_PDA_CONFIGURATION2_GET_VOCODER_OPTION_REQUEST                       = 0x56,
    LXT_PDA_CONFIGURATION2_SET_VOCODER_OPTION_REQUEST                       = 0x57,
    LXT_PDA_CONFIGURATION2_BATTERY_TYPE_CONFIRM                             = 0x58,
    LXT_PDA_CONFIGURATION2_BATTERY_CALIBRATION_CONFIRM                      = 0x59,
    LXT_PDA_CONFIGURATION2_GET_SLOT_MODE_REQUEST                            = 0x5A,
    LXT_PDA_CONFIGURATION2_SET_SLOT_MODE_REQUEST                            = 0x5B,
    LXT_PDA_CONFIGURATION2_GET_ESN_REQUEST                                  = 0x5C,
    LXT_PDA_CONFIGURATION2_PDA_CURR_BATT_ADC_CONFIRM                        = 0x5D,
    LXT_PDA_CONFIGURATION2_GET_PDA_LOW_BATT_ADC_REQUEST                     = 0x5E,
    LXT_PDA_CONFIGURATION2_SET_PDA_LOW_BATT_ADC_REQUEST                     = 0x5F,
    LXT_PDA_CONFIGURATION2_MAC_ADDRESS_REQUEST                              = 0x60,
    LXT_PDA_CONFIGURATION2_GET_IMEI_REQUEST                                 = 0x61,
    LXT_PDA_CONFIGURATION2_PDA_VERIFY_RESET_DONE_CONFIRM                    = 0x62,
    LXT_PHN_CONFIGURATION2_PDA_PASSWORD_CONFIRM                             = 0x63,
    LXT_PHN_CONFIGURATION2_DEVICE_AUTO_TEST_CONFIRM                             = 0x64,
    LXT_PHN_CONFIGURATION2_SERVICE_LED_ONOFF_CONFIRM                        = 0x65,
    LXT_PHN_CONFIGURATION2_SET_PDA_VERSION_REQUEST                          = 0x66
	/*
	   LXT_PDA_CONFIGURATION2_BATTRY_MEDIATENESS_CONFIRM                      = 0x51,
	   LXT_PDA_CONFIGURATION2_GET_PDA_BATTERY_REQUEST                          = 0x52
	 */
};

enum LXT_PHN_CONFIGURATION2
{
    LXT_PHN_CONFIGURATION2_PHONE_SERVICE_READY_NOTIFICATION                 = 0x40,
    LXT_PHN_CONFIGURATION2_PHONE_LOCK_RESPONSE                              = 0x41,
    LXT_PHN_CONFIGURATION2_PHONE_LOCK_NOTIFICATION                          = 0x42,
    LXT_PHN_CONFIGURATION2_CURRENT_TIME_RESPONSE                            = 0x43,
    LXT_PHN_CONFIGURATION2_CURRENT_TIME_NOTIFICATION                        = 0x44,
    LXT_PHN_CONFIGURATION2_PHONE_NUMBER_RESPONSE                            = 0x45,
    LXT_PHN_CONFIGURATION2_PHONE_VERSION_RESPONSE                           = 0x46,
    LXT_PHN_CONFIGURATION2_GET_NAM_INFORMATION_RESPONSE                     = 0x47,
    LXT_PHN_CONFIGURATION2_GET_NAM_DATA_RESPONSE                            = 0x48,
    LXT_PHN_CONFIGURATION2_PHONE_STATE_CHANGED_RESPONSE                     = 0x49,
    LXT_PHN_CONFIGURATION2_PHONE_STATE_CHANGED_NOTIFICATION                 = 0x4A,
    LXT_PHN_CONFIGURATION2_MODE_PREFERENCE_RESPONSE                         = 0x4B,
    LXT_PHN_CONFIGURATION2_SYSTEM_PREFERENCE_RESPONSE                       = 0x4C,
    LXT_PHN_CONFIGURATION2_SYSTEM_PARAMETER_RESPONSE                        = 0x4D,
    LXT_PHN_CONFIGURATION2_SYSTEM_PARAMETER_CHANGE_NOTIFICATION             = 0x4E,
    LXT_PHN_CONFIGURATION2_SIO_MODE_RESPONSE                                = 0x4F,
    LXT_PHN_CONFIGURATION2_GET_GPS_MODE_RESPONSE                            = 0x50,
    LXT_PHN_CONFIGURATION2_GET_RADIO_CONFIGURATION_RESPONSE                 = 0x51,
    LXT_PHN_CONFIGURATION2_GET_VOCODER_OPTION_RESPONSE                      = 0x52,
    LXT_PHN_CONFIGURATION2_SET_DEFAULT_CONFIGURATION_RESPONSE               = 0x53,
    LXT_PHN_CONFIGURATION2_BATTERY_TYPE_INDICATION                          = 0x54,
    LXT_PHN_CONFIGURATION2_BATTERY_CALIBRATION_INDICATION                   = 0x55,
    LXT_PHN_CONFIGURATION2_BATTERY_CALIBRATION_NOTIFICATION                 = 0x56,
    LXT_PHN_CONFIGURATION2_BATTERY_WRITE_CALIBRATION_NOTIFICATION           = 0x57,
    LXT_PHN_CONFIGURATION2_GET_SLOT_MODE_RESPONSE                           = 0x58,
    LXT_PDA_CONFIGURATION2_GET_ESN_RESPONSE                                 = 0x59,
    LXT_PHN_CONFIGURATION2_PDA_CURR_BATT_ADC_INDICATION                     = 0x5A,
    LXT_PHN_CONFIGURATION2_GET_PDA_LOW_BATT_ADC_RESPONSE                    = 0x5B,
    LXT_PHN_CONFIGURATION2_PDA_DEVICE_AUT_TEST_INDICATION                   = 0x5C,
    LXT_PHN_CONFIGURATION2_MAC_ADDRESS_RESPONSE                             = 0x5D,
    LXT_PHN_CONFIGURATION2_GET_IMEI_RESPONSE                                = 0x5E,
    LXT_PHN_CONFIGURATION2_SET_DEFAULT_CONFIGURATION_INDICATION             = 0x5F,
    LXT_PHN_CONFIGURATION2_PDA_PASSWORD_INDICATION                          = 0x60,
    LXT_PHN_CONFIGURATION2_BT_EQUIPMENT_TEST_ADDRESS_NOTIFICATION           = 0x61
	/*
	   LXT_PHN_CONFIGURATION2_BATTERY_MEDIATENESS_INDICATION                   = 0x53,
	   LXT_PHN_CONFIGURATION2_GET_PDA_BATTERY_RESPONSE                         = 0x54
	 */
};

// define of action group (call)++++++++++++++++++++++++++++++++++++++++++++++
enum LXT_PDA_CALL
{
    LXT_PDA_CALL_CALL_ORIGINATION_REQUEST                                   = 0x41, // call originnation
    LXT_PDA_CALL_END_CALL_REQUEST                                           = 0x42, // end call
    LXT_PDA_CALL_ANSWER_REQUEST                                             = 0x43, // call answer
    LXT_PDA_CALL_SEND_BURST_DTMF_REQUEST                                    = 0x44,
    LXT_PDA_CALL_SEND_CONTINUOUS_DTMF_REQUEST                               = 0x45,
    LXT_PDA_CALL_STOP_CONTINUOUS_DTMF_REQUEST                               = 0x46,
    LXT_PDA_CALL_FLASH_INFORMATION_REQUEST                                  = 0x47,
    LXT_PDA_CALL_GET_CALL_TIME_REQUEST                                      = 0x4E,
    LXT_PDA_CALL_SET_E911_NUMBER_REGISTRATION_REQUEST                       = 0x4F,
    LXT_PDA_CALL_RETRY_REQUEST                                              = 0x50,
    LXT_PDA_CALL_CALL_ORIGINATION2_REQUEST                                  = 0x51,
    LXT_PDA_CALL_GET_VOICE_PRIVACY_OPTION_REQUEST                           = 0x52,
    LXT_PDA_CALL_SET_VOICE_PRIVACY_OPTION_REQUEST                           = 0x53,
    LXT_PDA_CALL_CURRENT_VOICE_PRIVACY_STATUS_REQUEST                       = 0x54,    
    LXT_PDA_CALL_SOS_CALL_REQUEST                                           = 0x55,
    LXT_PDA_CALL_REJECT_CALL_REQUEST                                        = 0x56    
};

enum LXT_PHN_CALL
{
    LXT_PHN_CALL_INCOMING_CALL_NOTIFICATION                                 = 0x41,
    LXT_PHN_CALL_CALL_STATUS_NOTIFICATION                                   = 0x42,
    LXT_PHN_CALL_SEND_BURST_DTMF_TONE_NOTIFICATION                          = 0x43,
    LXT_PHN_CALL_START_END_ALERT_INFORMATION_NOTIFICATION                   = 0x44,
    LXT_PHN_CALL_ALERT_INFORMATION_NOTIFICATION                             = 0x45,
    LXT_PHN_CALL_SIGNALING_INFORMATION_NOTIFICATION                         = 0x46,
    LXT_PHN_CALL_OTA_STATE_CHANGE_NOTIFICATION                              = 0x47,
    LXT_PHN_CALL_IOTA_PROCESSING_RESPONSE                                   = 0x48,
    LXT_PHN_CALL_VOICE_PRIVACY_OPTION_RESPONSE                              = 0x49,
    LXT_PHN_CALL_CURRENT_VOICE_PRIVACY_AVAIABLE_STATUS_RESPONSE             = 0x4A,
    LXT_PHN_CALL_VOICE_PRIVACY_SERVICED_NOTIFICATION                        = 0x4B,
    LXT_PHN_CALL_CURRENT_CALL_INFORMATION_RESPONSE                          = 0x4C,
    LXT_PHN_CALL_DIAGNOSTIC_CALL_ORIGINATION_NOTIFICATION                   = 0x4D,
    LXT_PHN_CALL_GET_CALL_TIME_RESPONSE                                     = 0x4E





};

// define of action group (display)+++++++++++++++++++++++++++++++++++++++++++
enum LXT_PDA_DISPLAY
{
    LXT_PDA_DISPLAY_RING_TYPE_ICON_REQUEST                                  = 0x41,
    LXT_PDA_DISPLAY_ICON_REQUEST                                            = 0x47,
    LXT_PDA_DISPLAY_MISSED_ICON_CHANGED                                     = 0x48,
};

enum LXT_PHN_DISPLAY
{

    LXT_PHN_DISPLAY_RING_TYPE_ICON_NOTIFICATION                             = 0x41,
    LXT_PHN_DISPLAY_PHONE_ICON_CHANGED_NOTIFICATION                         = 0x42,
    LXT_PHN_DISPLAY_RSSI_ICON_CHANGED_NOTIFICATION                          = 0x43,
    LXT_PHN_DISPLAY_ROAM_ICON_CHANGED_NOTIFICATION                          = 0x44,
    LXT_PHN_DISPLAY_BATTERY_ICON_CHANGED_NOTIFICATION                       = 0x45,
    LXT_PHN_DISPLAY_MESSAGE_ICON_CHANGED_NOTIFICATION                       = 0x46,
    LXT_PHN_DISPLAY_ICON_RESPONSE                                           = 0x47,
    LXT_PHN_DISPLAY_MISSED_ICON_NOTIFICATION                                = 0x48,

};


// define of action group (sms)+++++++++++++++++++++++++++++++++++++++++++++++
enum LXT_PDA_SMS
{
    LXT_PDA_SMS_SMS_SEND_MESSAGE_REQUEST                                    = 0x41,
    LXT_PDA_SMS_SMS_SET_LOCAL_COUNT_INDICATION                              = 0x42,
    LXT_PDA_SMS_SMS_GET_LOCAL_STORAGE_SIZE_REQUEST                          = 0x43,
    LXT_PDA_SMS_SMS_SET_LOCAL_STORAGE_SIZE_INDICATION                       = 0x44
};

enum LXT_PHN_SMS
{
    // CDMA
    LXT_PHN_SMS_PHONE_SMS_INCOMING_NOTIFICATION                             = 0x41,
    LXT_PHN_SMS_PHONE_UIM_SMS_INCOMING_NOTIFICATION                         = 0x42,
    LXT_PHN_SMS_PHONE_WAP_SMS_INCOMING_NOTIFICATION                         = 0x43,
    LXT_PHN_SMS_PHONE_VOICE_SMS_INCOMING_NOTIFICATION                       = 0x44,
    LXT_PHN_SMS_PHONE_SMS_INCOMING_ALERT_NOTIFICATION                       = 0x45,
    LXT_PHN_SMS_PHONE_SMS_ACKNOWLEDGMENT_MESSAGE_NOTIFICATON                = 0x46,
    LXT_PHN_SMS_PHONE_SMS_MESSAGE_COUNT_NOTIFICATON                         = 0x47,
    LXT_PHN_SMS_PHONE_SMS_LOCAL_STORAGE_SIZE_RESPONSE                       = 0x48,
    LXT_PHN_SMS_PHONE_SMS_DELIVERY_ACKNOWLEDGMENT_NOTIFICATION              = 0x49,
    LXT_PHN_SMS_PHONE_SMS_REJECT_NOTIFICATION                               = 0x4A,
    // GSM
    LXT_PHN_SMS_PHONE_GSM_SMS_DELIVER_NOTIFICATION                          = 0x51,
    LXT_PHN_SMS_PHONE_GSM_SMS_STATUS_REPORT_NOTIFICATION                    = 0x52,
    LXT_PHN_SMS_PHONE_GSM_SMS_SUBMIT_REPORT_NOTIFICATION                    = 0x53,
    LXT_PHN_SMS_PHONE_GSM_SMS_SMMA_NOTIFICATION                             = 0x54,
    LXT_PHN_SMS_PHONE_EOT
};


// define of action group (sms)+++++++++++++++++++++++++++++++++++++++++++++++
enum LXT_PDA_CARD_SMS
{
    LXT_PDA_CARD_SMS_EOT
};

enum LXT_PHN_CARD_SMS
{
    // CDMA
    LXT_PHN_CARD_SMS_INFORMATION_RESPONSE                                   = 0x41,
    LXT_PHN_CARD_SMS_READ_RESPONSE                                          = 0x42,
    LXT_PHN_CARD_SMS_WRITE_RESPONSE                                         = 0x43,
    LXT_PHN_CARD_SMS_DELETE_RESPONSE                                        = 0x44,
    LXT_PHN_CARD_SMS_SET_TAG_RESPONSE                                       = 0x45,
    LXT_PHN_CARD_SMS_CLEAR_RESPONSE                                         = 0x46,
    LXT_PHN_CARD_SMS_MESSAGE_READY_NOTIFICATION                             = 0x47,    
    LXT_PHN_CARD_SMS_COMPLETE_NOTIFICATION                                  = 0x48,

    // GSM
    LXT_PHN_CARD_GSM_SMS_INFORMATION_RESPONSE                               = 0x61,
    LXT_PHN_CARD_GSM_SMS_READ_RESPONSE                                      = 0x62,
    LXT_PHN_CARD_GSM_SMS_WRITE_RESPONSE                                     = 0x63,
    LXT_PHN_CARD_GSM_SMS_DELETE_RESPONSE                                    = 0x64,
    LXT_PHN_CARD_GSM_SMS_SET_TAG_RESPONSE                                   = 0x65,
    LXT_PHN_CARD_GSM_SMS_CLEAR_RESPONSE                                     = 0x66,
    LXT_PHN_CARD_GSM_SMS_COMPLETE_NOTIFICATION                              = 0x67,
    LXT_PHN_CARD_GSM_SMS_PARAMETER_READ_RESPONSE                            = 0x68,
    LXT_PHN_CARD_GSM_SMS_PARAMETER_WRITE_RESPONSE                           = 0x69,
    LXT_PHN_CARD_GSM_SMS_MESSAGE_REFERENCE_WRITE_RESPONSE                   = 0x6A,
};


// define of action group (service mode)++++++++++++++++++++++++++++++++++++++

enum LXT_PDA_SERVICEMODE
{
    LXT_PDA_SERVICEMODE_ENTER_SERVICE_MODE_REQUEST                          = 0x41,
    LXT_PDA_SERVICEMODE_END_SERVICE_MODE_REQUEST                            = 0x42,
    LXT_PDA_SERVICEMODE_PROCESS_KEY_CODE_REQUEST                            = 0x43,
    LXT_PDA_SERVICEMODE_FACTORY_MODE_ENABLE_REQUEST                         = 0x44
};


enum LXT_PHN_SERVICEMODE
{
    LXT_PHN_SERVICEMODE_END_SERVICE_MODE_NOTIFICATION                       = 0x41,
    LXT_PHN_SERVICEMODE_DISPLAY_MONITOR_NOTIFICATION                        = 0x42,
    LXT_PHN_SERVICEMODE_PHONE_LCD_MODE_NOTIFICATION                         = 0x43,
    LXT_PHN_SERVICEMODE_DISPLAY_PHONE_LCD_NOTIFICATION                      = 0x44,
    LXT_PHN_SERVICEMODE_DEDICATE_LCD_MODE_NOTIFICATION                      = 0x45,


    LXT_PHN_SERVICEMODE_DISPLAY_DEDICATE_LCD_NOTIFICATION                   = 0x46,
    LXT_PHN_SERVICEMODE_DOWNLOAD_TYPE_NOTIFICATION                          = 0x47,
    LXT_PDA_SERVICEMODE_FACTORY_MODE_ENABLE_RESPONSE                        = 0x48
};

// define of action group (sound)+++++++++++++++++++++++++++++++++++++++++++++
enum LXT_PDA_SOUND
{
    LXT_PDA_SOUND_START_KEY_TONE_REQUEST                                    = 0x41,
    LXT_PDA_SOUND_STOP_KEY_TONE_REQUEST                                     = 0x42,
    LXT_PDA_SOUND_START_NOTIFICATION_TONE_REQUEST                           = 0x43,
    LXT_PDA_SOUND_START_LAMP_REQUEST                                        = 0x44,
    LXT_PDA_SOUND_START_VIBRATE_REQUEST                                     = 0x45,
    LXT_PDA_SOUND_STOP_SOUND_REQUEST                                        = 0x47,
    LXT_PDA_SOUND_SET_VOLUME_CONTROL_REQUEST                                = 0x48,
    LXT_PDA_SOUND_SET_MUTE_REQUEST                                          = 0x49,
    LXT_PDA_SOUND_GET_MUTING_STATUS_REQUEST                                 = 0x4A,
    LXT_PDA_SOUND_SET_SPEAKER_PHONE_REQUEST                                 = 0x4B,
    LXT_PDA_SOUND_SET_AUDIO_PATH_CONTROL_REQUEST                            = 0x4C
};

enum LXT_PHN_SOUND
{
    LXT_PHN_SOUND_MUTING_STATUS_RESPONSE                                    = 0x41,
    LXT_PHN_SOUND_SOUND_PDA_PATH_ON_OFF_NOTIFICATION                        = 0x42
};




// define of action group (UIM) ++++++++++++++++++++++++++++++++++++++++++++++
enum LXT_PDA_UIM
{

    // i819
    LXT_PDA_RUIMCONF_AUTHENTICATION_REQUEST                                 = 0x11,
    LXT_PDA_RUIMCONF_SELECT_NETWORK_REQUEST                                 = 0x12,
    LXT_PDA_RUIMCONF_ICCID_REQUEST                                          = 0x13,
    LXT_PDA_RUIMCONF_SELECTION_MODE_REQUEST                                 = 0x14,
    LXT_PDA_RUIMCONF_CHANGE_AUTHENTICATION_REQUEST                          = 0x15,
    LXT_PDA_RUIMCONF_CURRENT_PIN_CHECK_STATE_REQUEST                        = 0x16,
    LXT_PDA_RUIMCONF_CHANGE_PIN_CHECK_STATE_REQUEST                         = 0x17,
    LXT_PDA_RUIMCONF_RUNTIME_CHANGE_NETWORK_REQUEST                         = 0x18,
    LXT_PDA_RUIMCONF_DIR_NAME_REQUEST                                       = 0x19,
    LXT_PDA_RUIMCONF_DIR_NAME_WRITE_REQUEST                                 = 0x1A,
    LXT_PDA_RUIMCONF_CHANGE_FDN_STATE_REQUEST                               = 0x1B,
    LXT_PDA_RUIMCONF_CURRENT_FDN_STATE_REQUEST                              = 0x1C,
    LXT_PDA_RUIMCONF_CHECK_SERVICE_AVAILABLE_REQUEST                        = 0x1D,
    LXT_PDA_RUIMCONF_IMSI_REQUEST                                           = 0x1E,

    // i519
    LXT_PDA_UIM_GET_UIM_LOCK_REQUEST                                        = 0x31,
    LXT_PDA_UIM_SET_UIM_LOCK_REQUEST                                        = 0x32,
    LXT_PDA_UIM_UIM_LOCK_VERIFY_INDICATION                                  = 0x33,
    LXT_PDA_UIM_UIM_LOCK_PASSWORD_WRITE_REQUEST                             = 0x34,
    LXT_PDA_UIM_CARD_STATUS_REQUEST                                         = 0x41,
    LXT_PDA_UIM_GET_PIN_CODE_STATUS_REQUEST                                 = 0x42,
    LXT_PDA_UIM_SET_PIN_CHECK_REQUEST                                       = 0x43,
    LXT_PDA_UIM_RUIM_PIN_VERIFY_REQUEST                                     = 0x44,
    LXT_PDA_UIM_CHANGE_PIN_REQUEST                                          = 0x45,
    LXT_PDA_UIM_GET_ICCID_REQUEST                                           = 0x46,
    LXT_PDA_UIM_CHV_UNLOCK_REQUEST                                          = 0x47,
    LXT_PDA_UIM_PDA_CARD_SMS_MESSAGE_INFORMATION_REQUEST                    = 0x51,
    LXT_PDA_UIM_PDA_CARD_SMS_MESSAGE_READ_REQUEST                           = 0x52,
    LXT_PDA_UIM_PDA_CARD_SMS_MESSAGE_WRITE_REQUEST                          = 0x53,
    LXT_PDA_UIM_PDA_CARD_SMS_MESSAGE_ERASE_REQUEST                          = 0x54,
    LXT_PDA_UIM_PDA_CARD_SMS_MESSAGE_CLEAR_REQUEST                          = 0x55,
    LXT_PDA_UIM_PDA_CARD_SMS_MESSAGE_ALL_LIST_REQUEST                       = 0x56,
    LXT_PDA_UIM_PDA_CARD_SMS_MESSAGE_MIRRORING_REQUEST                      = 0x57,
    LXT_PDA_UIM_PDA_CARD_SMS_MESSAGE_SET_TAG_REQUEST                        = 0x58,
    LXT_PDA_UIM_PDA_CARD_PHONE_BOOK_INFORMATION_REQUEST                     = 0x61,
    LXT_PDA_UIM_PDA_CARD_PHONE_BOOK_READ_REQUEST                            = 0x62,
    LXT_PDA_UIM_PDA_CARD_PHONE_BOOK_WRITE_REQUEST                           = 0x63,
    LXT_PDA_UIM_PDA_CARD_PHONE_BOOK_ERASE_REQUEST                           = 0x64,
    LXT_PDA_UIM_PDA_CARD_PHONE_BOOK_CLEAR_REQUEST                           = 0x65,
    LXT_PDA_UIM_PDA_CARD_PHONE_BOOK_ALL_LIST_REQUEST                        = 0x66,
    LXT_PDA_UIM_PDA_CARD_PHONE_BOOK_MIRRORING_REQUEST                       = 0x67,
    LXT_PDA_UIM_DIR_NAME_READ_REQUEST                                       = 0x68,
    LXT_PDA_UIM_DIR_NAME_WRITE_REQUEST                                      = 0x69,
    LXT_PDA_UIM_PDA_CARD_SMS_CLEAR_REQUEST                                  = 0x70,
    LXT_PDA_UIM_GET_PERMISSION_STATUS_REQUEST                               = 0x80
};

enum LXT_PHN_UIM
{

    // i819
    LXT_PHN_RUIMCONF_STATUS_NOTIFICATION                                    = 0x11,
    LXT_PHN_RUIMCONF_BLOCK_STATUS_NOTIFICATION                              = 0x12,
    LXT_PHN_RUIMCONF_AUTHENTICATION_RESPONSE                                = 0x13,
    LXT_PHN_RUIMCONF_CARD_STATUS_NOTIFICATION                               = 0x14,
    LXT_PHN_RUIMCONF_SELECT_NETWORK_RESPONSE                                = 0x15,
    LXT_PHN_RUIMCONF_SELECTION_MODE_NOTIFICATION                            = 0x16,
    LXT_PHN_RUIMCONF_ICCID_RESPONSE                                         = 0x17,
    LXT_PHN_RUIMCONF_NETWORK_START_NOTIFICATION                             = 0x18,
    LXT_PHN_RUIMCONF_SELECTION_MODE_RESPONSE                                = 0x19,
    LXT_PHN_RUIMCONF_CHANGE_AUTHENTICATION_RESPONSE                         = 0x1A,
    LXT_PHN_RUIMCONF_CURRENT_PIN_CHECK_STATE_RESPONSE                       = 0x1B,
    LXT_PHN_RUIMCONF_CHANGE_PIN_CHECK_STATE_RESPONSE                        = 0x1C,
    LXT_PHN_RUIMCONF_RUNTIME_CHANGE_NETWORK_RESPONSE                        = 0x1D,
    LXT_PHN_RUIMCONF_DIR_NAME_RESPONSE                                      = 0x1E,
    LXT_PHN_RUIMCONF_DIR_NAME_WRITE_RESPONSE                                = 0x1F,
    LXT_PHN_RUIMCONF_CHANGE_FDN_STATE_RESPONSE                              = 0x20,
    LXT_PHN_RUIMCONF_CURRENT_FDN_STATE_RESPONSE                             = 0x21,
    LXT_PHN_RUIMCONF_CHECK_SERVICE_AVAILABE_RESPONSE                        = 0x22,
    LXT_PDA_RUIMCONF_IMSI_RESPONSE                                          = 0x23,

    // i519
    LXT_PHN_UIM_GET_UIM_LOCK_RESPONSE                                       = 0x31,
    LXT_PDA_UIM_UIM_LOCK_PASSWORD_WRITE_RESPONSE                            = 0x32,
    LXT_PHN_UIM_CARD_STATUS_RESPONSE                                        = 0x41,
    LXT_PHN_UIM_GET_PIN_CODE_STATUS_RESPONSE                                = 0x42,
    LXT_PHN_UIM_SET_PIN_CHECK_RESPONSE                                      = 0x43,
    LXT_PHN_UIM_PIN_VERIFY_RESPONSE                                         = 0x44,
    LXT_PHN_UIM_CHANGE_PIN_RESPONSE                                         = 0x45,
    LXT_PHN_UIM_GET_ICCID_RESPONSE                                          = 0x46,
    LXT_PHN_UIM_UIM_LOCK_RESPONSE                                           = 0x47,
    LXT_PHN_UIM_CHV_UNBLOCK_RESPONSE                                        = 0x48,
    LXT_PDA_UIM_DIR_NAME_WRITE_RESPONSE                                     = 0x49,
    LXT_PHN_UIM_PHONE_CARD_SMS_MESSAGE_INFORMATION_RESPONSE                 = 0x51,
    LXT_PHN_UIM_PHONE_CARD_SMS_MESSAGE_READ_RESPONSE                        = 0x52,
    LXT_PHN_UIM_PHONE_CARD_SMS_MESSAGE_WRITE_RESPONSE                       = 0x53,
    LXT_PHN_UIM_PHONE_CARD_SMS_MESSAGE_ERASE_RESPONSE                       = 0x54,
    LXT_PHN_UIM_PHONE_CARD_SMS_MESSAGE_CLEAR_RESPONSE                       = 0x55,
    LXT_PHN_UIM_PHONE_CARD_SMS_MESSAGE_ALL_LIST_RESPONSE                    = 0x56,
    LXT_PHN_UIM_PHONE_CARD_SMS_MESSAGE_MIRRORING_RESPONSE                   = 0x57,
    LXT_PHN_UIM_PHONE_CARD_SMS_MESSAGE_MIRRORING_DONE_NOTIFICATION          = 0x58,
    LXT_PHN_UIM_PHONE_CARD_PHONE_BOOK_INFORMATION_RESPONSE                  = 0x61,
    LXT_PHN_UIM_PHONE_CARD_PHONE_BOOK_READ_RESPONSE                         = 0x62,
    LXT_PHN_UIM_PHONE_CARD_PHONE_BOOK_WRITE_RESPONSE                        = 0x63,
    LXT_PHN_UIM_PHONE_CARD_PHONE_BOOK_ERASE_RESPONSE                        = 0x64,
    LXT_PHN_UIM_PHONE_CARD_PHONE_BOOK_CLEAR_RESPONSE                        = 0x65,
    LXT_PHN_UIM_PHONE_CARD_PHONE_BOOK_ALL_LIST_RESPONSE                     = 0x66,
    LXT_PHN_UIM_PHONE_CARD_PHONE_BOOK_MIRRORING_RESPONSE                    = 0x67,
    LXT_PHN_UIM_PHONE_CARD_PHONE_BOOK_MIRRORING_DONE_NOTIFICATION           = 0x68,
    LXT_PHN_UIM_DIR_NAME_READ_RESPONSE                                      = 0x69,
    LXT_PHN_UIM_DIR_NAME_WRITE_RESPONSE                                     = 0x70,
    LXT_PHN_UIM_PHONE_CARD_SMS_CLEAR_RESPONSE                               = 0x71,
    LXT_PDA_UIM_GET_PERMISSION_STATUS_RESPONSE                              = 0x80
};


enum LXT_PDA_PHONE_BOOK
{
    LXT_PDA_PHONE_BOOK_INFORMATION_REQUEST                                  = 0x51,
    LXT_PDA_PHONE_BOOK_READ_REQUEST                                         = 0x52,
    LXT_PDA_PHONE_BOOK_READ_ENTRIES_REQUEST                                 = 0x53,
    LXT_PDA_PHONE_BOOK_WRITE_REQUEST                                        = 0x54,
    LXT_PDA_PHONE_BOOK_ERASE_REQUEST                                        = 0x55,
    LXT_PDA_PHONE_BOOK_CLEAR_REQUEST                                        = 0x56,
    LXT_PDA_PHONE_BOOK_GET_ENTRY_LIST_REQUEST                               = 0x57,
    LXT_PDA_PHONE_BOOK_GET_CACHED_REQUEST                                   = 0x58
};

enum LXT_PHN_PHONE_BOOK
{
    LXT_PHN_PHONE_BOOK_INFORMATION_RESPONSE                                 = 0x51,
    LXT_PHN_PHONE_BOOK_READ_RESPONSE                                        = 0x52,
    LXT_PHN_PHONE_BOOK_READ_ENTRIES_RESPONSE                                = 0x53,
    LXT_PHN_PHONE_BOOK_GET_ENTRY_LIST_RESPONSE                              = 0x54,
    LXT_PHN_PHONE_BOOK_ACTION_NOTIFICATION                                  = 0x55,
    LXT_PHN_PHONE_BOOK_COMPLETE_NOTIFICATION                                = 0x56,
    LXT_PHN_PHONE_BOOK_GET_CACHED_RESPONSE                                  = 0x57,
    LXT_PHN_PHONE_BOOK_MIRRORING_START_NOTIFICATION                         = 0xA1,
    LXT_PHN_PHONE_BOOK_MIRRORING_DONE_NOTIFICATION                          = 0xA2,
};

// define of action group (UTK) ++++++++++++++++++++++++++++++++++++++++++++++

enum LXT_PDA_UTK
{
    LXT_PDA_UTK_START_REQUEST                                               = 0x41,
    LXT_PDA_UTK_GET_KEY_REQUEST                                             = 0x42,
    LXT_PDA_UTK_END_INPUT_REQUEST                                           = 0x43,
    LXT_PDA_UTK_SET_UP_MAIN_MENU_REQUEST                                    = 0x44
};


enum LXT_PHN_UTK
{
    LXT_PHN_UTK_MENU_TITLE_NOTIFICATION                                     = 0x41,
    LXT_PHN_UTK_NENU_ITEM_NOTIFICATION                                      = 0x42,
    LXT_PHN_UTK_SEND_SMS_NOTIFICATION                                       = 0x43,
    LXT_PHN_UTK_GET_START_INPUT_NOTIFICATION                                = 0x44,
    LXT_PHN_UTK_DISPLAY_TEXT_NOTIFICATION                                   = 0x45,
    LXT_PHN_UTK_END_NOTIFICATION                                            = 0x46,
    LXT_PHN_UTK_CARD_START_RESPONSE                                         = 0x47
};


// define of action group () ++++++++++++++++++++++++++++++++++++++++++++++
enum LXT_PDA_GPS
{
    LXT_PDA_GPS_MO_SMS_REQUEST                                              = 0x41,
    LXT_PDA_GPS_MT_SMS_REQUEST                                              = 0x42,
    LXT_PDA_GPS_SPP_REQUEST                                                 = 0x43,
    LXT_PDA_GPS_URL_INFO_REQUEST                                            = 0x44,
    LXT_PDA_GPS_END_REQUEST                                                 = 0x45
};

enum LXT_PHN_GPS
{
    LXT_PHN_GPS_MT_SMS_ARRIVAL_NOTIFICATION                                 = 0x41,
    LXT_PHN_GPS_MT_SMS_RESPONSE                                             = 0x42,
    LXT_PHN_GPS_WAP_BASED_SESSION_STATUS_NOTIFICATION                       = 0x43,
    LXT_PHN_GPS_URL_INFO_RESPONSE                                           = 0x44,
    LXT_PHN_GPS_SMS_BASED_SESSION_STATUS_NOTIFICATION                       = 0x45
};


// define of action group (Data service) +++++++++++++++++++++++++++++++++++++
enum LXT_PDA_DATASERVICE
{
    LXT_PDA_DATASERVICE_PIN_CONTROL_REQUEST                                 = 0x41
};


enum LXT_PHN_DATASERVICE
{

    LXT_PHN_DATASERVICE_PIN_CONTROL_NOTIFICATION                            = 0x41,
    LXT_PHN_DATASERVICE_DS_TE2_DIALING_INDICATION                           = 0x42,
    LXT_PHN_DATASERVICE_PPP_CONTROL_DISCONNECTING_NOTIFICATION              = 0x43,
    LXT_PHN_DATASERVICE_PPP_CONTROL_DONE_NOTIFICATION                       = 0x44,
    LXT_PHN_DATASERVICE_DS_TE2_DATA_RATE_INFO_NOTIFICATION                  = 0x45,
    LXT_PHN_DATASERVICE_TE2_STATUS_NOTIFICATION                             = 0x46,
    LXT_PHN_DATASERVICE_DISPLAY_NOTIFICATION                                = 0x47
};

// define of action group (response)++++++++++++++++++++++++++++++++++++++++++

enum LXT_PHN_RESPONSE
{
    LXT_PHN_RESPONSE_GENERAL_RESPONSE                                       = 0x41
};




// define of action group (SIO Mode)++++++++++++++++++++++++++++++++++++++++++
enum LXT_PDA_SIOMODE
{
    LXT_PDA_SIOMODE_SIO_BAUDRATE_GET_REQUEST                                 = 0x41,
    LXT_PDA_SIOMODE_SIO_BAUDRATE_SET_REQUEST                                 = 0x42,
    LXT_PDA_SIOMODE_SLOT_MODE_GET_REQUEST                                    = 0x43,
    LXT_PDA_SIOMODE_SLOT_MODE_SET_REQUEST                                    = 0x44,
    LXT_PDA_SIOMODE_VOICE_OP_GET_REQUEST                                     = 0x45,
    LXT_PDA_SIOMODE_VOICE_OP_SET_REQUEST                                     = 0x46,
    LXT_PDA_SIOMODE_SIO_MODE_GET_REQUEST                                     = 0x47,
    LXT_PDA_SIOMODE_SIO_MODE_SET_REQUEST                                     = 0x48,
    LXT_PDA_SIOMODE_LIFETIME_CLEAR_REQUEST                                   = 0x49,
};

// define of action group (SIO Mode)++++++++++++++++++++++++++++++++++++++++++
enum LXT_PHN_SIOMODE
{
    LXT_PHN_SIOMODE_SIO_BAUDRATE_GET_RESPONSE                                = 0x41,
    LXT_PHN_SIOMODE_SLOT_MODE_GET_RESPONSE                                   = 0x42,
    LXT_PHN_SIOMODE_VOICE_OP_GET_RESPONSE                                    = 0x43,
    LXT_PHN_SIOMODE_SIO_MODE_GET_RESPONSE                                    = 0x44
};









#endif // _PHONEPROTOCOL_H_
