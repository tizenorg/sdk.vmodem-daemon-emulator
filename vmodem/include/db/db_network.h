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

#ifndef __DB_NETWORK_H__
#define __DB_NETWORK_H__


typedef struct _NetworkEntry {
	int id;
	int PLMN_status;
//	int number_PLMN;
	char number_PLMN[6];
	int ACT;
	void * callback;
	struct _NetworkEntry * next;
} NetworkEntry;


// initialize the database
int vgsm_network_database_init(void);

// add a registration
int vgsm_network_database_add(NetworkEntry * entry);

// remove a registration
int vgsm_network_database_remove(NetworkEntry * entry);

// restore all previously save registrations
int vgsm_network_database_restore(void);


#endif // __DB_NETWORK_H__

