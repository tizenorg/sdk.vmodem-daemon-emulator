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

#ifndef _TAPI4PHONEBOOK_H_
#define _TAPI4PHONEBOOK_H_


#ifdef __cplusplus
extern "C"
{
#endif

// ++++++++++++++++++++++++++++++++++++++++++++++include about standard library
#include <stdio.h>
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++include user define

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++define area












#define MAX_ALPHA_SIZE                                                      32
#define MAX_DIGITS_SIZE                                                     32
#define MAX_PHONEBOOK_ITEM                                                 256

typedef enum
{
    LXT_PHONEBOOK_PBM_DEFAULT   =       0x00,       // do not use
    LXT_PHONEBOOK_PBM_SLOT1_ADN =       0x01,       // adn of slot1
    LXT_PHONEBOOK_PBM_SLOT2_ADN =       0x02,       // adn of slot2
    LXT_PHONEBOOK_PBM_SLOT1_FDN =       0x03,       // fdn of slot1
    LXT_PHONEBOOK_PBM_SLOT2_FDN =       0x04,       // fdn of slot2
    LXT_PHONEBOOK_PBM_MAX       =       0x05,       // do not use
    LXT_PHONEBOOK_PBM_EOT       =       0x0f        // do not use
}
LXT_PHONEBOOK_PBM;


// card phone book
typedef struct
{
    int index;                          // 4  byte
    int alpha_size;                     // 4  byte
    int num_digits;                     // 4  byte
    char alpha[MAX_ALPHA_SIZE];         // 32 byte
    char digits[MAX_DIGITS_SIZE];       // 32 byte
}
LXT_PHONEBOOK_ONE_ITEM;

// phone book set
typedef struct
{
    int mpb_id;                         // 4  byte
    int num_total;                      // 4  byte
    int num_used;                       // 4  byte
    int max_entry_alpha_size;           // 4  byte
    LXT_PHONEBOOK_ONE_ITEM phonebook_item[MAX_PHONEBOOK_ITEM];
}
LXT_PHONEBOOK_ALL;

#ifdef __cplusplus
}
#endif

#endif // _TAPI4PHONEBOOK_H_


// ============================================================================ end of file

