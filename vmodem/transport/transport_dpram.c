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


#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include "phoneserver.h"

#define DEBUG_TITLE	"VGSM"

#ifdef __arm__
#include "dpram.h"
#endif

#ifndef TIOCMODG
#  ifdef TIOCMGET
#    define TIOCMODG TIOCMGET
#  else
#    ifdef MCGETA
#      define TIOCMODG MCGETA
#    endif
#  endif
#endif

#ifndef TIOCMODS
#  ifdef TIOCMSET
#    define TIOCMODS TIOCMSET
#  else
#    ifdef MCSETA
#      define TIOCMODS MCSETA
#    endif
#  endif
#endif

/* Set hardware flow control. */
static GSM_Error dpram_sethwf(GSM_StateMachine * s, int on)
{
    GSM_Device_DPRAMData *d = &s->Device.Data.DPRAM;
    struct termios tty;

    tcgetattr(d->hPhone, &tty);
    if (on)
        tty.c_cflag |= CRTSCTS;
    else
        tty.c_cflag &= ~CRTSCTS;
    tcsetattr(d->hPhone, TCSANOW, &tty);

    return ERR_NONE;
}

/* Set RTS line. */
static GSM_Error dpram_setrts(GSM_StateMachine * s)
{
    GSM_Device_DPRAMData *d = &s->Device.Data.DPRAM;
    int mcs;

    ioctl(d->hPhone, TIOCMODG, &mcs);
    mcs |= TIOCM_RTS;
    ioctl(d->hPhone, TIOCMODS, &mcs);

    return ERR_NONE;
}

/*
 * Set baudrate, parity and number of bits.
 */
static GSM_Error dpram_setparms(GSM_StateMachine * s,
char * baudr, char * par, char * bits, char * stop, int hwf, int swf)
{
    GSM_Device_DPRAMData *d = &s->Device.Data.DPRAM;

    int spd = -1;
    int newbaud;
    int bit = bits[0];
    int stop_bit = stop[0];

    struct termios tty;

    if  (tcgetattr(d->hPhone, &tty) < 0)
    {
        GSM_OSErrorInfo(s,"tcgetattr");
        return ERR_DEVICEREADERROR;
    }

    if (tcgetattr(d->hPhone, &d->old_settings) < 0)
    {
        GSM_OSErrorInfo(s,"tcgetattr");
        return ERR_DEVICEREADERROR;
    }

    fflush(stdout);

    /* We generate mark and space parity ourself. */
    if (bit == '7' && (par[0] == 'M' || par[0] == 'S'))
        bit = '8';

    /* Check if 'baudr' is really a number */
    if ((newbaud = (atol(baudr) / 100)) == 0 && baudr[0] != '0')
        newbaud = -1;

    switch (newbaud)
    {
        case 0:    spd = 0;       break;
        case 3:    spd = B300;    break;
        case 6:    spd = B600;    break;
        case 12:   spd = B1200;   break;
        case 24:   spd = B2400;   break;
        case 48:   spd = B4800;   break;
        case 96:   spd = B9600;   break;
        case 192:  spd = B19200;  break;
        case 384:  spd = B38400;  break;
        case 576:  spd = B57600;  break;
        case 1152: spd = B115200; break;
    }

    if (spd != -1)
    {
        cfsetospeed(&tty, (speed_t) spd);
        cfsetispeed(&tty, (speed_t) spd);
    }

    switch (bit)
    {
        case '5':
            tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS5;
            break;
        case '6':
            tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS6;
            break;
        case '7':
            tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS7;
            break;
        case '8':
        default:
            tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;
            break;
    }

    switch (stop_bit)
    {
        case '1':
            tty.c_cflag &= ~CSTOPB;
            break;
        case '2':
        default :
            tty.c_cflag |= CSTOPB;
            break;
    }

    /* Set into raw, no echo mode */
    tty.c_iflag = IGNBRK;
    tty.c_lflag = 0;
    tty.c_oflag = 0;
    tty.c_cflag |= CLOCAL | CREAD;
    tty.c_cc[VMIN] = 1;
    tty.c_cc[VTIME] = 1;

    if (swf)
        tty.c_iflag |= IXON | IXOFF;
    else
        tty.c_iflag &= ~(IXON | IXOFF | IXANY);

    tty.c_cflag &= ~(PARENB | PARODD);

    if (par[0] == 'E')
        tty.c_cflag |= PARENB;
    else if (par[0] == 'O')
        tty.c_cflag |= (PARENB | PARODD);

    if  (tcsetattr(d->hPhone, TCSANOW, &tty) < 0)
    {
        GSM_OSErrorInfo(s,"tcsetattr");
        return ERR_DEVICEPARITYERROR;
    }

    dpram_setrts(s);

    dpram_sethwf(s, hwf);

    return ERR_NONE;
}

static GSM_Error dpram_close(GSM_StateMachine *s)
{

	if( vgsm_server_port > 0 ){

		  GSM_Device_DPRAMData *d = &s->Device.Data.DPRAM;
		  close(d->hPhone);
		  return ERR_NONE;

	}else{

#ifdef __arm__
		    GSM_Device_DPRAMData *d = &s->Device.Data.DPRAM;

		    if  (tcsetattr(d->hPhone, TCSAFLUSH, &d->old_settings) < 0)
		        log_msg(MSGL_VGSM_INFO,"[%s] tcsetattr", DEBUG_TITLE);

		    close(d->hPhone);

		    return ERR_NONE;
#else
		    GSM_Device_DPRAMData *d = &s->Device.Data.DPRAM;

		    close(d->hPhone);
		    return ERR_NONE;
#endif


		}

}

#include <sys/socket.h>
#define MAX_LISTEN_NUMBER  32
static GSM_Error dpram_open (GSM_StateMachine *s)
{
	if( vgsm_server_port > 0 ){
		GSM_Device_DPRAMData   *d = &s->Device.Data.DPRAM;
		d->Power = false;
		return ERR_NONE;
	}
	else{
		log_msg(MSGL_VGSM_INFO,"[%s] vdpram open\n", DEBUG_TITLE);
#ifdef __arm__
		GSM_Device_DPRAMData   *d = &s->Device.Data.DPRAM;

		int i;
		int val = 1;

		d->Power = false;
		d->hPhone = open("/dev/vdpram1", O_RDWR | O_NOCTTY | O_NONBLOCK);
		if (d->hPhone < 0) {
                        i = errno;
                        log_msg(MSGL_VGSM_INFO," dpram open errno = %d : %s \n", i, strerror(i));
                        GSM_OSErrorInfo(s,"open in dpram_open");
                        if (i ==  2) return ERR_DEVICENOTEXIST;     //no such file or directory
                        if (i == 13) return ERR_DEVICENOPERMISSION; //permission denied
                        return ERR_DEVICEOPENERROR;
                }

		if  (dpram_setparms(s, "115200", "N", "8", "1", 0, 0) != ERR_NONE)
		{
			dpram_close(s);
			GSM_OSErrorInfo(s, "dpram set parameter");
			return ERR_DEVICEOPENERROR;
		}
		
		if  (ioctl(d->hPhone, HN_DPRAM_PHONE_GETSTATUS, &val) < 0)
		{
			dpram_close(s);
			GSM_OSErrorInfo(s, "dpram phone power status error");
			return ERR_DEVICEOPENERROR;
		}
		
		if  (val == HN_DPRAM_PHONE_STATUS_OFF)
			d->Power = false;
		else
			d->Power = true;
		return ERR_NONE;
#else
		return ERR_NONE;
#endif

		}

}

static bool _IsFlightMode(void)
{
	char	buf[32];
	int		fd;
	bool	ret = false;
#ifdef USE_SYSPATH
	char *syspath, path[PATH_MAX];

	if ((syspath = getenv("LINUETTEDIR")) == NULL)
		return false;

	sprintf(path, "%s/setting/flightmode.conf", syspath);

	fd = open(path, O_RDONLY);
	if (fd == -1) {
#endif
		fd = open("/setting/flightmode.conf", O_RDONLY);
		if (fd == -1)
			return false;
#ifdef USE_SYSPATH
	}
#endif

	if (read(fd, buf, 1) == 1 && buf[0] == '1')
		ret = true;

	close(fd);

	log_msg(MSGL_VGSM_INFO,"[%s] ============ Flight mode value: %d ==========\n", DEBUG_TITLE, ret);

	return ret;
}

static int dpram_poweron(GSM_StateMachine * s, int first_boot)
{
	log_msg(MSGL_VGSM_INFO,"[VGSM] vgsm_server_port= %d\n",vgsm_server_port);

	if( vgsm_server_port > 0 ){
		GSM_Phone_Data          *Data = &s->Phone.Data;

		Data->PhonePower = true;
		return 1;
	}
	else{
#ifdef __arm__
		if (!_IsFlightMode() || first_boot) { 
		    GSM_Device_DPRAMData	*d = &s->Device.Data.DPRAM;
			GSM_Phone_Data			*Data = &s->Phone.Data;
		    int rc = -1;

		    log_msg(MSGL_VGSM_INFO,"[%s] ============ dpram_power_on ==========\n", DEBUG_TITLE);
			rc = ioctl(d->hPhone, HN_DPRAM_PHONE_ON, NULL);
			if  (rc < 0)
		        GSM_OSErrorInfo(s,"Phone Power On failed");

		    Data->PhonePower = true;
			return rc;
		}
		return 1;
#else
	    return 1;
#endif
	}
}

static int dpram_poweroff(GSM_StateMachine * s)
{
	if( vgsm_server_port > 0 ){
		return 1;
	}
	else{
#ifdef __arm__
	    GSM_Device_DPRAMData       *d = &s->Device.Data.DPRAM;
	    int rc = -1;

		rc = ioctl(d->hPhone, HN_DPRAM_PHONE_OFF, NULL);
		if  (rc < 0)
	        GSM_OSErrorInfo(s,"Phone Power Off failed");

		return rc;
#else
	    return 1;
#endif
	}
}

static int dpram_read(GSM_StateMachine *s, void *buf, size_t nbytes)
{
	GSM_Device_DPRAMData       *d = &s->Device.Data.DPRAM;
	struct timeval          timeout2;
	fd_set              readfds;
	int                 actual = 0;

	FD_ZERO(&readfds);
	FD_SET(d->hPhone, &readfds);

	timeout2.tv_sec     = 0;
	timeout2.tv_usec    = 1;

	if  (select(d->hPhone+1, &readfds, NULL, NULL, &timeout2))
	{
		actual = read(d->hPhone, buf, nbytes);
		if (actual == -1)
			GSM_OSErrorInfo(s,"dpram_read");
	}
	return actual;
}

static int dpram_write(GSM_StateMachine *s, void *buf, size_t nbytes)
{
    GSM_Device_DPRAMData   *d = &s->Device.Data.DPRAM;
    int             ret;
    size_t          actual = 0;
        unsigned char *p = buf;

    usleep(100000);
//    log_msg(MSGL_VGSM_INFO, "%d\n", nbytes);

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

static bool dpram_powerstatus(GSM_StateMachine *s)
{
    GSM_Device_DPRAMData   *d = &s->Device.Data.DPRAM;

    TRACE(MSGL_VGSM_INFO,"power status:[%d] \n", d->Power);

    return d->Power;
}

#define RAMDUMP_YES 0x00BB
#define RAMDUMP_NO  0x00CC

static int dpram_write_magic_code(GSM_StateMachine * s, unsigned char req)
{


#ifdef __arm__
    GSM_Device_DPRAMData       *d = &s->Device.Data.DPRAM;
    unsigned short magic_code;
    int rc = -1;

    if  (req == 0x01) // Yes I'd like to trace the reason about RAMDUMP
    {
        log_msg(MSGL_VGSM_INFO,"[%s] ## RAMDUMP Yes ##\n", DEBUG_TITLE);
        magic_code = RAMDUMP_YES;
    }
    else
    {
        log_msg(MSGL_VGSM_INFO,"[%s] ## RAMDUMP No  ##\n", DEBUG_TITLE);
        magic_code = RAMDUMP_NO;
    }

	rc = ioctl(d->hPhone, HN_DPRAM_PHONE_MAGICCODE, &magic_code);
	if  (rc < 0)
        GSM_OSErrorInfo(s,"Write Magic code failed\n");
    else
        log_msg(MSGL_VGSM_INFO,"[%s] Write Magic code ok\n", DEBUG_TITLE);

	return rc;
#else
    return 1;
#endif
}

GSM_Device_Functions DPRAMDevice = {
    dpram_open,
    dpram_close,
    dpram_read,
    dpram_write,
    dpram_poweron,
    dpram_poweroff,
    dpram_powerstatus,
    dpram_write_magic_code

};

