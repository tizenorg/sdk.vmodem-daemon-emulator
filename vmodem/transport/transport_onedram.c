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

#include "gsmstate.h"
#include "logmsg.h"
#include <sys/file.h>
#include <sys/time.h>
#include <string.h>
#include <stdlib.h>
#include <termios.h>
#include <errno.h>


#include <sys/un.h>
#include <arpa/inet.h>
#include "phoneserver.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

#include "onedram.h"

int UART_PSI_read(int AT_socket, void *buf, int nbytes)
{
	struct timeval				timeout2;
	fd_set             		 	readfds;
	int							actual = 0;
	
	FD_ZERO(&readfds);
	FD_SET(AT_socket, &readfds);

	timeout2.tv_sec     = 3;
	timeout2.tv_usec    = 0;

	if (select(AT_socket+1, &readfds, NULL, NULL, &timeout2))
	{
		actual = read(AT_socket, buf, nbytes);
		if (actual == -1) {
			printf("UART_PSI_read error\n");
		}		
	}
	
	return actual;
}

int UART_PSI_write(int AT_socket, void *buf, int nbytes)
{
    int             ret;
    size_t          actual = 0;
	unsigned char *p = buf;

	do {
        ret = write(AT_socket, p, nbytes - actual);
        if (ret < 0 && errno == EAGAIN)
            continue;

        if (ret < 0) {
            if (actual != nbytes)
				printf("UART_PSI_write error\n");
			return actual;
        }

        actual  += ret;
        p       += ret;
    } while (actual < nbytes);

    return actual;
}


int UART_PSI_load (void)
{
	int AT_socket;	
	struct sockaddr_in AT_addr;
	long sockarg;
	struct timeval tv;
	fd_set fdset;
	socklen_t lon;
	int valopt;
	unsigned char ch;
	int length = 0;
	unsigned int data_len = 0;
	unsigned char *data_buffer = NULL;
	int i;
	int nCRC = 0;
	int at_num = 0;
	unsigned char *pDataPSI;
	unsigned char PSI_Header[InfoSize]={0,};
	int retry = 0;

	AT_socket = socket(PF_INET, SOCK_STREAM, 0);
	if(AT_socket < 0) {
		printf("Create AT socket failed\n");
		goto error;
	}
	/* Set to non-blocking socket */
	sockarg =  fcntl(AT_socket, F_GETFL, NULL);
	sockarg |= O_NONBLOCK;
	fcntl(AT_socket, F_SETFL, sockarg);				

		
	bzero((char *)&AT_addr, sizeof(AT_addr));
	AT_addr.sin_family = AF_INET;
	AT_addr.sin_addr.s_addr = inet_addr(UART_PSI_IP);

	AT_addr.sin_port = htons(UART_PSI_PORT);

	while(connect(AT_socket, (struct sockaddr*)&AT_addr,sizeof(AT_addr)) < 0) {		
		if(errno == EINPROGRESS) {
			/* Try 3 seconds more */
			tv.tv_sec = 3;
			tv.tv_usec = 0;
			FD_ZERO(&fdset);
			FD_SET(AT_socket, &fdset);
			if (select(AT_socket+1, NULL, &fdset, NULL, &tv) > 0) {
				lon = sizeof(int);
				getsockopt(AT_socket, SOL_SOCKET, SO_ERROR, (void*)(&valopt), &lon);
			}

		}
		sleep(1);
		if(retry ++ >= 10)
			goto error;;
	}

	printf("Connect with AP successfully\n");
	
	/* Waiting for the "AT" Command from AP */
	while(1) {		
		ch = 0;
		length = UART_PSI_read(AT_socket, (void *)&ch, sizeof(unsigned char));
		if (length < 0) {
			printf("AT read failed\n");
			goto error;
		}
		if(ch == 'A') {
			ch = 0;
			length = UART_PSI_read(AT_socket, (void *)&ch, sizeof(unsigned char));
			if (length < 0) {
				printf("AT read failed\n");
				goto error;
			}
				
			if(ch == 'T') {
				at_num ++;
				if(at_num == UART_PSI_RETRY)
				break;
			}
		}
	}			

	/* Send VERS back to AP */
	ch = VERS;
	length = UART_PSI_write(AT_socket, &ch, sizeof(unsigned char));
	if (length <= 0) {
		printf("VERS write failed\n");
		goto error;
	}
	else
		printf("Has written Version number\n");

	/* Send Version version back to AP */
	ch = InfoSize;
	length = UART_PSI_write(AT_socket, &ch, sizeof(unsigned char));
	if (length <= 0) {
		printf("InfoSize write failed\n");
		goto error;
	}
	else
		printf("Has written InfoSize\n");

	/* Write header to AP */	
	for(i=0; i< InfoSize; i++) {
		length = UART_PSI_write(AT_socket, PSI_Header, sizeof(unsigned char));
		if(length <=0) {
			printf("Write header failed\n");
			goto error;
		}
	}	
	
	/* Read Indication from AP */
	ch = 0;
	length = UART_PSI_read(AT_socket, (void *)&ch, sizeof(unsigned char));
	if (length <= 0) {
		printf("Failed to get INDICATION from AP\n");
		goto error;
	}	
	else {	
		if(ch == IND) {
			printf("Get IND from AP\n");
		}
		else {
			printf("Unknow IND\n");
			goto error;
		}
			
	}

	/* Read length from AP in little end format */
	data_len = 0;
	length = UART_PSI_read(AT_socket, (void *)&data_len, 2 * sizeof(unsigned char));
	if (length <= 0) {
		printf("Failed to get Length from AP\n");
		goto error;
	}		
	else {
		printf("Data length is %d\n", data_len);
	}
	
	/* Read PSI from AP */
	data_buffer = (unsigned char *)malloc(data_len);
	if(!data_buffer)
		goto error;
	bzero((char *)data_buffer, data_len);

	nCRC = 0;
	pDataPSI = data_buffer;

	for(i=0; i< data_len; i++) {
		length = UART_PSI_read(AT_socket, pDataPSI, sizeof(unsigned char));
		if(length <=0) {
			printf("read PSI failed\n");
			goto error;
		}
		nCRC ^= *pDataPSI++;
	}
	

	/* Read CRC of PSI */
	ch = 0;
	length = UART_PSI_read(AT_socket, (void *)&ch, sizeof(unsigned char));
	if (length <= 0) {
		printf("Failed to get CRC from AP\n");
		goto error;
	}		
	else {
		printf("CRC is %d\n", ch);
		if(ch == nCRC)
			printf("CRC is correct!\n");
		else {
			printf("CRC is wrong!\n");
			goto error;
		}
	}
			
			
	/* Send ACK to AP */
	ch = CRC_OK;
	length = UART_PSI_write(AT_socket, &ch, sizeof(unsigned char));
	if (length <= 0) {
		printf("Failed to write CRC_OK to AP\n");
		goto error;
	}

	/* Finish the downloading of PSI*/
	close(AT_socket);
	if(data_buffer)
		free(data_buffer);
	return 1;
	
error:
	if(AT_socket >= 0)
		close(AT_socket);
	if(data_buffer)
		free(data_buffer);
	return 0;
}

static GSM_Error onedram_close(GSM_StateMachine *s)
{
	/* need to check what vgsm_server_port used for */
	if( vgsm_server_port > 0 ) {
		  GSM_Device_DPRAMData *d = &s->Device.Data.DPRAM;
		  close(d->hPhone);
		  return ERR_NONE;
	}
	else {
		GSM_Device_DPRAMData *d = &s->Device.Data.DPRAM;
	    close(d->hPhone);
	    return ERR_NONE;
	}
}

static GSM_Error onedram_open (GSM_StateMachine *s)
{
	GSM_Device_DPRAMData   *d = &s->Device.Data.DPRAM;

	struct sockaddr_in onedram_addr;
	long sockarg;
	struct timeval tv;
	fd_set fdset;
	socklen_t lon;
	int valopt;
	char buffer[BUFFER_SIZE];
	unsigned int send_buf = CONNECT_AT;
	int length = 0;
	
	if( vgsm_server_port > 0 ){
		d->Power = false;
		return ERR_NONE;
	}

	log_msg(MSGL_VGSM_INFO,"[%s] onedram open\n", DEBUG_TITLE);	

	if(!UART_PSI_load()) {
		GSM_OSErrorInfo(s,"Faile to load the PSI from AP\n");
		goto error;
	}
		
	d->hPhone = socket(PF_INET, SOCK_STREAM, 0);
	if(d->hPhone < 0) {
		GSM_OSErrorInfo(s,"Create vmodem socket failed\n");
		goto error;
	}
	/* Set to non-blocking socket */
	sockarg =  fcntl(d->hPhone, F_GETFL, NULL);
	sockarg |= O_NONBLOCK;
	fcntl(d->hPhone, F_SETFL, sockarg);				

	bzero((char *)&onedram_addr, sizeof(onedram_addr));
	onedram_addr.sin_family = AF_INET;
	onedram_addr.sin_addr.s_addr = inet_addr(ONEDRAM_IP);
	
	onedram_addr.sin_port = htons(ONEDRAM_PORT);

	if(connect(d->hPhone, (struct sockaddr*)&onedram_addr,sizeof(onedram_addr)) < 0) {		
		if(errno == EINPROGRESS) {
			/* Try 3 seconds more */
			tv.tv_sec = 3;
			tv.tv_usec = 0;
			FD_ZERO(&fdset);
			FD_SET(d->hPhone, &fdset);
			if (select(d->hPhone+1, NULL, &fdset, NULL, &tv) > 0) {
				lon = sizeof(int);
				getsockopt(d->hPhone, SOL_SOCKET, SO_ERROR, (void*)(&valopt), &lon);
				if(valopt) {
					GSM_OSErrorInfo(s,"Connect to onedram server failed\n");
					goto error;
				}
			}
			else {
				GSM_OSErrorInfo(s,"Connect to onedram server time out\n");
				goto error;
			}
		}
		else {
			GSM_OSErrorInfo(s,"Connect to onedram server time out\n");
			goto error;
		}						
	}	
	onedram_write(s, &send_buf, 4);

	if((length = onedram_read(s, (void *)buffer, (size_t)BUFFER_SIZE))> 0) {
		unsigned int CMD = (unsigned int)(buffer[0]);
		if(CMD == CONNECT_RES)
			printf("Connected with onedram\n");
		else {
			GSM_OSErrorInfo(s,"Unknow ACK from onedram");
			goto error;
		}			
	}
	else {
		GSM_OSErrorInfo(s,"No response from onedram");
		goto error;
	}

	d->Power = true;
	return ERR_NONE;
	
error:
	if(d->hPhone > 0)
		close(d->hPhone);
	d->Power = false;
	return ERR_DEVICEOPENERROR;

}

static int onedram_poweron(GSM_StateMachine * s, int first_boot)
{
	GSM_Phone_Data          *Data = &s->Phone.Data;
	Data->PhonePower = true;
	return 1;
}


static int onedram_poweroff(GSM_StateMachine * s)
{
	GSM_Phone_Data          *Data = &s->Phone.Data;
	Data->PhonePower = false;
	return 1;

}

static int onedram_read(GSM_StateMachine *s, void *buf, size_t nbytes)
{
	GSM_Device_DPRAMData		*d = &s->Device.Data.DPRAM;
	struct timeval				timeout2;
	fd_set             		 	readfds;
	int							actual = 0;

	FD_ZERO(&readfds);
	FD_SET(d->hPhone, &readfds);

	timeout2.tv_sec     = 1;
	timeout2.tv_usec    = 1;

	if  (select(d->hPhone+1, &readfds, NULL, NULL, &timeout2))
	{
		actual = read(d->hPhone, buf, nbytes);
		if (actual == -1)
			GSM_OSErrorInfo(s,"onedram_read");
	}
	return actual;
}

static int onedram_write(GSM_StateMachine *s, void *buf, size_t nbytes)
{
    GSM_Device_DPRAMData   *d = &s->Device.Data.DPRAM;
    int             ret;
    size_t          actual = 0;
	unsigned char *p = buf;

	do {
        ret = write(d->hPhone, p, nbytes - actual);
        if (ret < 0 && errno == EAGAIN)
            continue;

        if (ret < 0) {
            if (actual != nbytes)
				GSM_OSErrorInfo(s, "dpram_write");
			return actual;
        }

        actual  += ret;
        p       += ret;
    } while (actual < nbytes);

    return actual;
}

static bool onedram_powerstatus(GSM_StateMachine *s)
{
    GSM_Device_DPRAMData   *d = &s->Device.Data.DPRAM;

	log_msg(MSGL_VGSM_INFO,"\n");
	
    return d->Power;
}

static int onedram_write_magic_code(GSM_StateMachine * s, unsigned char req)
{
	return 1;
}

GSM_Device_Functions DPRAMDevice = {
    onedram_open,
    onedram_close,
    onedram_read,
    onedram_write,
    onedram_poweron,
    onedram_poweroff,
    onedram_powerstatus,
    onedram_write_magic_code
};


