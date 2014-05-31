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

#include "at_msg.h"
#include "oem_rx_gprs.h"
#include "at_rx_sap.h"
#include "at_recv.h"

int at_rx_sap_Connect(char* atmsg)
{
    at_unimplemented("TelTapiSimSapConnectReq");

    return 1;
}


int at_rx_sap_Status(char* atmsg)
{
    at_unimplemented("TelTapiSimSapConnectStatusReq");

    return 1;
}

int at_rx_sap_transfer_atr(char* atmsg)
{
    at_unimplemented("TelTapiSimSapTransferAtrRequest");

    return 1;
}

int at_rx_sap_transfer_apdu(char* atmsg)
{
    at_unimplemented("TelTapiSimSapTransferApdu");

    return 1;
}

int at_rx_sap_transport_protocol(char* atmsg)
{
    at_unimplemented("TelTapiSimSapSetTransportProtocol");

    return 1;
}

int at_rx_sap_sim_power(char* atmsg)
{
    at_unimplemented("TelTapiSimSapSetPower");

    return 1;
}

int at_rx_sap_transfer_card_reader_status(char* atmsg)
{
    at_unimplemented("TelTapiSimSapCardReaderStatus");

    return 1;
}

int at_rx_sap_DefaultSAP(char* atmsg)
{
    at_unimplemented("at_rx_sap_DefaultSAP");

    return 1;
}
