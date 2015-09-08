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

/* * 	ondram.h 		* */
#ifndef _ONEDRAM_H_
#define _ONEDRAM_H_

#define DEBUG_TITLE	"VGSM"

#define ONEDRAM_IP "127.0.0.1"
#define ONEDRAM_PORT 7777

#define UART_PSI_IP "127.0.0.1"
#define UART_PSI_PORT 7776

#define BUFFER_SIZE 1024
#define CONNECT_AT 0x1234ABCD
#define CONNECT_RES 0xABCD1234


#define RAMDUMP_YES 0x00BB
#define RAMDUMP_NO  0x00CC

#define IND				0x30
#define VERS			0xf0
#define CRC_OK			0x01
#define CRC_ERR			0xff

#define UART_PSI_RETRY		20
#define InfoSize 0x16

int UART_PSI_read(int AT_socket, void *buf, int nbytes);
int UART_PSI_write(int AT_socket, void *buf, int nbytes);
int UART_PSI_load (void);

static GSM_Error onedram_close(GSM_StateMachine *s);
static GSM_Error onedram_open (GSM_StateMachine *s);
static int onedram_poweron(GSM_StateMachine * s, int first_boot);
static int onedram_poweroff(GSM_StateMachine * s);
static int onedram_read(GSM_StateMachine *s, void *buf, size_t nbytes);
static int onedram_write(GSM_StateMachine *s, void *buf, size_t nbytes);
static bool onedram_powerstatus(GSM_StateMachine *s);
static int onedram_write_magic_code(GSM_StateMachine * s, unsigned char req);

#endif /* _ONEDRAM_H_ */



