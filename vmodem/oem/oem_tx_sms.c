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

#include "at_tx_sms.h"
#include "oem_tx_sms.h"

int oem_tx_sms_deviceReady(void *data, int len)
{
	return at_tx_sms_deviceReady(data, len);
}

int oem_tx_sms_NotiIncomingMsg(void *data, int len)
{
	return at_tx_sms_NotiIncomingMsg(data, len);
}

int oem_tx_sms_sendDeliverReport(void *data, int len)
{
	return at_tx_sms_sendDeliverReport(data, len);
}

int oem_tx_sms_sendMsg(void *data, int len)
{
	return at_tx_sms_sendMsg(data, len);
}

int oem_tx_sms_sendCBSMsg(void *data, int len)
{
	return at_tx_sms_sendCBSMsg(data, len);
}

int oem_tx_sms_SCA(void *data, int len)
{
	return at_tx_sms_SCA(data, len);
}

