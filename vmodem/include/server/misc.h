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

/*
//////////////////////////////////////////////////////////////////
// misc.h
*/

#ifndef __PS_MISC_H__
#define __PS_MISC_H__

#include <linuxtapi.h>

void TAPIMessageInit(LXT_MESSAGE *packet);
void TAPIMessageFree(LXT_MESSAGE * packet);
char const* printCurrentState(LXT_PHONE_STATE CurrentState);
int packed_S32(unsigned char* bytearray);
unsigned short packed_U16(unsigned char* bytearray);
void PacketDataFree(void *p);
int ReadBytes(int fd, void *data, int size);
void* PacketDataMalloc(int size);
int WriteBytes(int fd, void *data, int size);

#endif /* __PS_MISC_H__ */

