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

#include <string.h>
//#include <common/state.h>
#include "eloop.h"

#include "logmsg.h"

static TimeoutStruct *timeout_head = 0;
static InOutStruct *inout_head = 0;
static int timeout_tag = 1;
static int inout_tag = 1;
static int inout_num = 0;

static void insert_timeout(TimeoutStruct *me)
{
    TimeoutStruct *ts = timeout_head, *prev = 0;
    while(ts)
    {
        if (ts->expire > me->expire)
        {
            /* prev me ts */
            me->next = ts;
            me->prev = prev;
            if (prev)
            {
                prev->next = me;
            }
            else
            {
                timeout_head = me;
            }
            ts->prev = me;
            return;
        }
        prev = ts;
        ts = ts->next;
    }
    me->next = 0;
    me->prev = prev;
    if (prev)
    {
        prev->next = me;
    }
    else
    {
        timeout_head = me;
    }
}

static void remove_timeout(TimeoutStruct *me)
{
    if (me->prev)
    {
        me->prev->next = me->next;
    }
    else
    {
        timeout_head = me->next;
    }
    if (me->next)
    {
        me->next->prev = me->prev;
    }
}

static void invoke_timeout(TimeoutStruct *me)
{
    int ret = (*me->cb)(me->data);
    remove_timeout(me);
    if (ret >= 0)
    {
        me->expire += me->time_slice * 1000;
        insert_timeout(me);
    }
}

static void insert_input(InOutStruct *me)
{
	if (inout_head)
		inout_head->prev = me;
	me->next = inout_head;
	me->prev = 0;
	inout_head = me;
	inout_num++;
}

static void remove_input(InOutStruct *me)
{
	TRACE(MSGL_VGSM_INFO,"\n");

	if (me->prev)
	{
		me->prev->next = me->next;
	}
	else
	{
		inout_head = me->next;
	}

	if (me->next)
	{
		me->next->prev = me->prev;
	}
	inout_num--;
}

static void invoke_inout(InOutStruct *me, EloopCondition cond)
{
    int ret;

	TRACE(MSGL_VGSM_INFO,"\n");

    ret = (me->cb)(me->ps, me->fd, cond, me->data);
    if (ret < 0)
    {
        remove_input(me);
    }
	//ServerHandle
	//DPRAMHandle
	//ClientHandle
}

static int64_t time_get_current(void)
{
    struct timeval tv;
    gettimeofday(&tv, 0);
    return tv.tv_sec * 1000000LL + tv.tv_usec;
}

/*	++ 2008-08-06.
	++ This function is not used anywhere.
	++ I recommend removing this function.
*/
int eloop_add_timeout(unsigned int time_slice, EloopCallback cb, void *data)
{
    TimeoutStruct *ts = malloc(sizeof (TimeoutStruct));
    ts->cb = cb;
    ts->data = data;
    ts->expire = time_get_current() + time_slice * 1000;
    ts->time_slice = time_slice;
    ts->tag = timeout_tag++;
    insert_timeout(ts);
    return ts->tag;
}

void eloop_remove_timeout(int tag)
{
    TimeoutStruct *ts = timeout_head;
    while(ts)
    {
        if (ts->tag == tag)
        {
            remove_timeout(ts);
            free(ts);
            return;
        }
        ts = ts->next;
    }
}

int eloop_add_input(PhoneServer * ps, int fd, EloopCondition cond, EloopInputCallback cb, void *data)
{
    InOutStruct *io = malloc(sizeof (InOutStruct));
    io->ps = ps;
    io->fd = fd;
    io->cond = cond;
    io->cb = cb;
    io->data = data;
    io->tag = inout_tag++;
    insert_input(io);

    return io->tag;
}

void eloop_remove_input(int tag)
{
    InOutStruct *io = inout_head;
    while(io)
    {
        if (io->tag == tag)
        {
            remove_input(io);
            free(io);
            return;
        }
        io = io->next;
    }
}

static int loop_cont;

static void main_iter(void)
{
    int timeout = -1;
    struct pollfd *ufds = 0;
    int nfds = inout_num;
    int i;
    int n;

   TRACE(MSGL_VGSM_INFO,"\n");

    if (timeout_head)
    {
        timeout = timeout_head->expire - time_get_current();
        timeout /= 1000; /* us -> ms */

        while(timeout < 0)
        {
            invoke_timeout(timeout_head);
            if (!timeout_head)
                break;
            timeout = timeout_head->expire - time_get_current();
            timeout /= 1000; /* us -> ms */
        }

        if (!timeout_head)
            timeout = -1;
    }

    if (!loop_cont)
        return;

    if (nfds > 0)
    {
        InOutStruct *io = inout_head;
        ufds = malloc(sizeof (struct pollfd) * nfds);

        for(i=0;i<nfds;i++)
        {
            ufds[i].fd = io->fd;
            ufds[i].events = 0;

            if (io->cond & ELOOP_READ)
            {
                ufds[i].events |= POLLIN;
            }

            if (io->cond & ELOOP_WRITE)
            {
                ufds[i].events |= POLLOUT;
            }

            io = io->next;
        }
    }
    n = poll(ufds, nfds, timeout);

    if (n == 0)
    {
	if(timeout_head) {
	    invoke_timeout(timeout_head);
	}
    }
    else if (n > 0)
    {
        InOutStruct *ts = inout_head;

        for(i=0;i<nfds;i++)
        {
            InOutStruct *next = ts->next;

            if (ufds[i].revents)
            {
                int cond = 0;

                if (ufds[i].revents & POLLIN)
                {
					TRACE(MSGL_VGSM_INFO, "[%d,fd=%d] READ Condition\n", i, ufds[i].fd);
					cond |= ELOOP_READ;
                }
                if (ufds[i].revents & POLLOUT)
                {
                    cond |= ELOOP_WRITE;
					TRACE(MSGL_VGSM_INFO, "[%d,fd=%d] WRITE Condition\n", i, ufds[i].fd);
                }
                if (ufds[i].revents & POLLERR)
                {
                    cond |= ELOOP_EXCEPTION;
					TRACE(MSGL_VGSM_INFO, "[%d,fd=%d] EXCEPTION Condition\n", i, ufds[i].fd);
                }

                invoke_inout(ts, cond);
            }
            ts = next;
        }
    }
    else
    {
        perror("poll");
    }
    free(ufds);
}

void eloop_main(void)
{
    //eloop_add_timeout(100, time_callback, 0);
	TRACE(MSGL_VGSM_INFO, "loop start!!!\n");

    loop_cont = 1;
    do
    {
        main_iter();
    }
    while(loop_cont);
}

void eloop_quit(void)
{
    loop_cont = 0;
}
