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

#ifndef _SERVER_RX_SS_
#define _SERVER_RX_SS_

int server_rx_ss_cw_get(int tel_class);
int server_rx_ss_cb_get(int tel_class, int cb_type);
int server_rx_ss_cf_get(int tel_class, int cf_type);

int server_rx_ss_cw_set(int tel_class, int ss_mode);
//int server_rx_ss_cf_set(int ss_mode, int type, int tel_class, int replyTime, int num_len, char *number, int num_type, int satype, char* subaddr);
int server_rx_ss_cf_set(int ss_mode, int type, int tel_class, int replyTime, int num_len, char *number, int num_type);
int server_rx_ss_cb_set(int cb_type, int ss_mode, int tel_class, char *passwd);

int server_rx_ss_cb_passwd_set(char* curr_passwd, char* new_passwd);

int server_rx_ss_cli(char *api);

int server_rx_ss_ussd_get(void);

int server_rx_ss_aoc_get(void);

int server_rx_ss_manage_call_set(char* ptr_data, int data_len);
void set_outgoing_video_call_barring_state(int state);
void set_incoming_video_call_barring_state(int state);
void set_outgoing_voice_call_barring_state(int state);
void set_incoming_voice_call_barring_state(int state);
int get_outgoing_voice_call_barring_state(void);
int get_incoming_voice_call_barring_state(void);
int get_outgoing_video_call_barring_state(void);
int get_incoming_video_call_barring_state(void);

extern int g_ss_hold_response_error ;
extern int g_ss_retrieve_response_error ;
extern int g_ss_join_response_error ;
extern int g_ss_split_response_error ;
extern int g_ss_transfer_response_error ;

#endif

