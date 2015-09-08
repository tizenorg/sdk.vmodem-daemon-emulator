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

#include <stdio.h>
#include <errno.h>
#include <glib.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "sms_util.h"



/*==================================================================================================
  IMPLEMENTATION OF MsgConvertText - Member Functions
  ==================================================================================================*/
// ETSI GSM 03.38 7-bit default alphabet -> Unicode scalar value (UCS2)
// index : gsm 7bit char value
static const unsigned short g_GSM7BitToUCS2Table[] =
{
    0x40/*@*/, 0xA3, 0x24, 0xA5, 0xE8, 0xE9, 0xF9, 0xEC, 		0xF2, 0xC7/*0xE7*/, 0x0A, 0xD8, 0xF8, 0x0D, 0xC5, 0xE5,
    0x0394, 0x005F, 0x03A6, 0x0393, 0x039B, 0x03A9, 0x03A0, 	0x03A8, 0x03A3, 0x0398, 0x039E, 0x0000, 0x00C6, 0x00E6, 0x00DF, 0x00C9,
    0x20/*space*/, 33, 34, 35, 164, 37, 38, 39, 				40, 41, 42, 43, 44, 45, 46, 47,
    48, 49, 50, 51, 52, 53, 54, 55, 							56, 57, 58, 59, 60, 61, 62, 63,
    161, 65, 66, 67, 68, 69, 70, 71, 							72, 73, 74, 75, 76, 77, 78, 79,
    80, 81, 82, 83, 84, 85, 86, 87, 							88, 89, 90, 196, 214, 209, 220, 167,
    191, 97, 98, 99, 100, 101, 102, 103, 						104, 105, 106, 107, 108, 109, 110, 	111,
    112, 113, 114, 115, 116, 117, 118, 119, 					120, 121, 122, 228, 246, 241, 252, 224
	/*
	   '@', 0xa3, '$', 0xa5, 0xe8, 0xe9, 0xf9, 0xec,0xf2, 0xc7, '\n', 0xd8, 0xf8, '\r', 0xc5, 0xe5,
	   '?', '_', '?', '?', '?', '?', '?', '?','?', '?', '?', '?', 0xc6, 0xe6, 0xdf, 0xc9,
	   ' ', '!', '\"', '#', 0xa4, '%', '&','\'','(', ')', '*', '+', ',', '-', '.', '/',
	   '0', '1', '2', '3', '4', '5', '6', '7','8', '9', ':', ';', '<', '=', '>', '?',0xa1, 'A', 'B', 'C', 'D', 'E', 'F', 'G',
	   'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O','P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',
	   'X', 'Y', 'Z', 0xc4, 0xd6, 0xd1, 0xdc, 0xa7,0xbf, 'a', 'b', 'c', 'd', 'e', 'f', 'g',
	   'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o','p', 'q', 'r', 's', 't', 'u', 'v', 'w',
	   'x', 'y', 'z', 0xe4, 0xf6, 0xf1, 0xfc, 0xe0
	 */
};

static const char g_UCS2ToGSM7BitTable[] =
{
    0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b,			0x1b, 0x1b, 0x0a, 0x1b, 0x1b, 0x0d, 0x1b, 0x1b,
    0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b,			0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b,
    0x20, 0x21, 0x22, 0x23, 0x02, 0x25, 0x26, 0x27,			0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,	/*40, 41, 42, 43, 44, 45, 46, 47,*/
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 			0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f,	/*48, 49, 50, 51, 52, 53, 54, 55, 	56, 57, 58, 59, 60, 61, 62, 63,	*/
    0x00, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,			0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,	/*0, 65, 66, 67, 68, 69, 70, 71, 	72, 73, 74, 75, 76, 77, 78, 79,*/
    0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,			0x58, 0x59, 0x5a, 0x1b, 0x1b, 0x1b, 0x1b, 0x11, 	/*80, 81, 82, 83, 84, 85, 86, 87,	88, 89, 90, 0x1b, 0x1b, 0x1b, 0x1b, 0x11,		*/
    0x1b, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,			0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f,	/*0x1b, 97, 98, 99, 100, 101, 102, 103,		104, 105, 106, 107, 108, 109, 110, 111,*/
    0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77,			0x78, 0x79, 0x7a, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b,	/*112, 113, 114, 115, 116, 117, 118, 119,					120, 121, 122, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b,*/
    0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b,			0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b,
    0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b,			0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b,
    0x1b, 0x40, 0x1b, 0x01, 0x24, 0x03, 0x1b, 0x5f,			0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b,	// 0xA0 ~
    0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b,			0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x60,	
    0x1b, 0x1b, 0x1b, 0x1b, 0x5B, 0x0E, 0x1C, 0x09,			0x1b, 0x1f, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b,
    0x1b, 0x5D, 0x1b, 0x1b, 0x1b, 0x1b, 0x5c, 0x1b,			0x0b, 0x1b, 0x1b, 0x1b, 0x5e, 0x1b, 0x1b, 0x1e,
    0x7f, 0x1b, 0x1b, 0x1b, 0x7b, 0x0f, 0x1d, 0x09,			0x04, 0x05, 0x1b, 0x1b, 0x07, 0x1b, 0x1b, 0x1b,	
    0x1b, 0x7d, 0x08, 0x1b, 0x1b, 0x1b, 0x7C, 0x1b,			0x0c, 0x06, 0x1b, 0x1b, 0x7e, 0x1b, 0x1b, 0x1b
};

static const  unsigned short g_GSM7BitToUCS2TableExt[] =
{	// 0x20 -> for invalid code
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 			0x20, 0x20, 12/**/, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 94/**/, 0x20, 0x20, 0x20, 			0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,			123/**/, 125/**/, 0x20, 0x20, 0x20, 0x20, 0x20, 92/**/,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 			0x20, 0x20, 0x20, 0x20, 91/**/, 126/**/, 93/**/, 0x20,	//64
    124/**/, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 		0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,			0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,		//104
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20AC/**/, 0x20, 0x20,		0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,			0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20
};

static const char g_UCS2ToGSM7BitTableExt[] =
{	// 0x1b -> for invalid code
    0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b,			0x1b, 0x1b, 0x1b, 0x1b, 0x0a/**/, 0x1b, 0x1b, 0x1b,		//16
    0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b,			0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b,			//32
    0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b,			0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b,
    0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b,			0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b,			//64
    0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b,			0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b,
    0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b,			0x1b, 0x1b, 0x1b, 0x3c/**/, 0x2f/**/, 0x3e/**/, 0x14/**/, 0x1b,	//96
    0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b,			0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b,
    0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b,			0x1b, 0x1b, 0x1b, 0x28/**/, 0x40/**/, 0x29/**/, 0x3d/**/, 0x1b,	//128
    0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b,			0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b,			//144
    0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b,			0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b,			//160
    0x1b, 0x1b, 0x1b, 0x1b, 0x1b/*0x65?*/, 0x1b, 0x1b, 0x1b,		0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b,			//176	// 0xA0~
    0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b,			0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b,			//192
    0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b,			0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b,			//208
    0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b,			0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b,			//224
    0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b,			0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b,			//240
    0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b,			0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b			//256
};


#ifdef CONVERT_DUMP

#define MAX_DUMP_COLUMN	16
void MsgConvertDumpTextToHex(const unsigned char *pText, int length)
{
    int i, j;

    fprintf(stderr, "\n=======================================\n");
    fprintf(stderr, "   Dump Text To Hex - Length :%d\n", length);
    fprintf(stderr, "=======================================");
    for ( i = 0; i < length; i++ )
    {
	if ( i % MAX_DUMP_COLUMN == 0 )
	{
	    fprintf(stderr, "\n\t");
	}
	fprintf(stderr, "%02x ", pText[i]);
    }
    fprintf(stderr, "\n=======================================\n\n");
}

#else

#define MsgConvertDumpTextToHex(pText, length)

#endif	// CONVERT_DUMP


/**
args :
unsigned char *pDestText				: destination text (UCS2) - byte order depends on local byte order
const unsigned char *pSrcText		: source text (gsm7bit)
int maxLength			: max destination buffer size
int srcTextLen			: byte length of source text (gsm7bit)
return :
byte length of converted UCS2 characters
-1 : The alpha isn't the gsm 7bit code
 */
int MsgConvertGSM7bitToUCS2(unsigned char *pDestText, int maxLength, const unsigned char *pSrcText, int srcTextLen)
{
    int outTextLen = 0;
    int i = 0;
    unsigned char lowerByte, upperByte;

    if(srcTextLen == 0 || pSrcText == NULL || pDestText ==  NULL || maxLength == 0)
    {
	fprintf(stderr, "UCS2 to GSM7bit Failed as text length is 0\n");
	return -1;
    }	

    for(i = 0 ; i<srcTextLen ; i++)
    {
	if ( maxLength == 0 )
	{
	    break;
	}
	if (pSrcText[i] >= 0x80)
	{
	    //error
	    fprintf(stderr, ">>>>>>>a_pTextString[i]=%x, The alpha isn't the gsm 7bit code, Never Come here!!!\n", pSrcText[i]);
	    return -1;
	}

	unsigned char utf8Buf[2], ucs2Buf[4];
	if(g_GSM7BitToUCS2Table[ pSrcText[i] ] == 0x0/*0*/)
	{	//extended gsm7bit
	    fprintf(stderr, "extended gsm7bit!\n");
	    i++;
	    ucs2Buf[0] = (g_GSM7BitToUCS2TableExt[ pSrcText[i] ] & 0xFF00) >> 8;
	    ucs2Buf[1] = g_GSM7BitToUCS2TableExt[ pSrcText[i] ] & 0x00FF;
	    MsgConvertUCS2toUTF8(&utf8Buf, 256, ucs2Buf, 2);
	    pDestText[outTextLen++] = utf8Buf[0];
	}
	else
	{
	    lowerByte = g_GSM7BitToUCS2Table[ pSrcText[i] ] & 0x00FF;
	    upperByte = (g_GSM7BitToUCS2Table[ pSrcText[i] ] & 0xFF00) >> 8;

	    if(upperByte != 0x00)
	    {
		fprintf(stderr, "upperByte is not 0x00\n");
		ucs2Buf[0] = upperByte;
		ucs2Buf[1] = lowerByte;
		MsgConvertUCS2toUTF8(&utf8Buf, 256, ucs2Buf, 2);
		pDestText[outTextLen++] = utf8Buf[0];
		pDestText[outTextLen++] = utf8Buf[1];
		fprintf(stderr,"upperByte:%x, lowerByte:%x\n", upperByte, lowerByte);
	    }
	    else
	    {
		pDestText[outTextLen++] = lowerByte;
		fprintf(stderr,"lowerByte:%x\n", lowerByte);
	    }
	}
	//pDestText[outTextLen++] = upperByte;
	//pDestText[outTextLen++] = lowerByte;
	maxLength -= 2;
    }

#ifdef CONVERT_DUMP
    fprintf(stderr, "\n########## Dump GSM7bit -> UCS2\n");
    MsgConvertDumpTextToHex((unsigned char*)pSrcText, srcTextLen);
    MsgConvertDumpTextToHex((unsigned char*)pDestText, outTextLen);
#endif

    return outTextLen;
}


/**

args:
unsigned char *pDestText
int maxLength				: max destination buffer size
const unsigned char *pSrcText
int srcTextLen				: BYTE length of src text (UCS2)
return:
bytelength of gsm7bit text
-1 : converting error
 */
int MsgConvertUCS2toGSM7bit(unsigned char *pDestText, int maxLength, const unsigned char *pSrcText, int srcTextLen, BOOL *bUnknown)
{
    // for UNICODE
    int outTextLen = 0;
    int nTextLen = srcTextLen;		// # of bytes of UCS2 charaters
    unsigned char lowerByte, upperByte;

    if(srcTextLen == 0 || pSrcText == NULL || pDestText ==  NULL || maxLength == 0)
    {
	fprintf(stderr, "UCS2 to GSM7bit Failed as text length is 0\n");
	return -1;
    }	

    int i = 0;
    for( i = 0 ; i<nTextLen ; i += 2 )
    {
	upperByte = pSrcText[i];
	lowerByte = pSrcText[i+1];
	if ( upperByte == 0x00 )
	{	// 0x00XX -> converts with mapping table
	    if(  g_UCS2ToGSM7BitTable[ lowerByte ] == 0x1b/*27*/ )
	    {
		if(g_UCS2ToGSM7BitTableExt[ lowerByte ] == 0x1b)	
		{	// unknown character
		    pDestText[outTextLen++] = 0x20;
		    *bUnknown = TRUE;
		}
		else											
		{	//extended gsm7bit
		    // prevent buffer overflow.
		    if ( maxLength <= outTextLen + 1 )
		    {
			fprintf(stderr, "Buffer full\n");
			goto FINISH_CONVERT;
		    }			
		    pDestText[outTextLen++] = 0x1b;		
		    pDestText[outTextLen++] = g_UCS2ToGSM7BitTableExt[ lowerByte ];
		}
	    }
	    else
	    {
		pDestText[outTextLen++] = g_UCS2ToGSM7BitTable[ lowerByte ];
	    }		
	}
	else
	{	// unicode : GREEK CAPITAL LETTERS
	    unsigned short ucsChar = (upperByte << 8) | lowerByte;
	    switch ( ucsChar )
	    {
		case 0x0394:	// GREEK CAPITAL LETTER DELTA
		    pDestText[outTextLen++] = 0x10;
		    break;
		case 0x03A6:	// GREEK CAPITAL LETTER PHI
		    pDestText[outTextLen++] = 0x12;
		    break;
		case 0x0393:	// GREEK CAPITAL LETTER GAMMA
		    pDestText[outTextLen++] = 0x13;
		    break;
		case 0x039B:	// GREEK CAPITAL LETTER LAMDA
		    pDestText[outTextLen++] = 0x14;
		    break;
		case 0x03A9:	// GREEK CAPITAL LETTER OMEGA
		    pDestText[outTextLen++] = 0x15;
		    break;
		case 0x03A0:	// GREEK CAPITAL LETTER PI
		    pDestText[outTextLen++] = 0x16;
		    break;
		case 0x03A8:	// GREEK CAPITAL LETTER PSI
		    pDestText[outTextLen++] = 0x17;
		    break;
		case 0x03A3:	// GREEK CAPITAL LETTER SIGMA
		    pDestText[outTextLen++] = 0x18;
		    break;
		case 0x0398:	// GREEK CAPITAL LETTER THETA
		    pDestText[outTextLen++] = 0x19;
		    break;
		case 0x039E:	// GREEK CAPITAL LETTER XI
		    pDestText[outTextLen++] = 0x1A;
		    break;
		case 0x20AC:	// EURO SIGN
		    // prevent buffer overflow.
		    if ( maxLength <= outTextLen + 1 )
		    {
			fprintf(stderr, "Buffer full\n");
			goto FINISH_CONVERT;
		    }					
		    pDestText[outTextLen++] = 0x1B;
		    pDestText[outTextLen++] = 0x65;
		    break;
		default:
		    pDestText[outTextLen++] = 0x20;
		    *bUnknown = TRUE;

	    }
	}

	// prevent buffer overflow.
	if ( maxLength <= outTextLen )
	{
	    fprintf(stderr, "Buffer full\n");
	    goto FINISH_CONVERT;
	}	
    }

FINISH_CONVERT:

#ifdef CONVERT_DUMP
    fprintf(stderr, "\n########## Dump UCS2 -> GSM7bit\n");
    MsgConvertDumpTextToHex((unsigned char*)pSrcText, srcTextLen);
    MsgConvertDumpTextToHex((unsigned char*)pDestText, outTextLen);
#endif

    return outTextLen;
}


/**
  if srcTextLen ispSrcText should be null terminated
return :
byte length of converted UCS2 characters
-1 : converting error
 */
int MsgConvertUTF8toUCS2(unsigned char *pDestText, int maxLength, const unsigned char *pSrcText, int srcTextLen)
{
    int i, j;
    int textLen;	
    unsigned char *unicodeTemp = (unsigned char*)pDestText;
    int ucs2Length = 0;
    int remainedBuffer = maxLength;

#ifdef CONVERT_DUMP
    int srcLen = srcTextLen;
    const unsigned char * pTempSrcText = pSrcText;
    const unsigned char * pTempDestText = pDestText;
#endif

    i = j = 0;	

    if(maxLength == 0 || pSrcText == NULL || pDestText ==  NULL)
    {
	fprintf(stderr, "UTF8 to UCS2 Failed as text length is 0\n");
	return -1;
    }	

    // null terminated string
    if ( srcTextLen == -1 )
    {
	textLen = strlen((char*)pSrcText);
	srcTextLen = textLen;
    }
    else
    {
	textLen = srcTextLen;
    }

    GIConv cd;
    int err=0;

    cd = g_iconv_open("UCS-2BE", "UTF8");

    if (cd > 0)
    {
	err = g_iconv(cd, (char**)&pSrcText, (gsize*)&textLen, (char**)&unicodeTemp, (gsize*)&remainedBuffer);
    }

    ucs2Length = maxLength - remainedBuffer;

#ifdef CONVERT_DUMP
    fprintf(stderr, "\n########## Dump UTF8 -> UCS2\n");
    MsgConvertDumpTextToHex((unsigned char*)pTempSrcText, srcLen);
    MsgConvertDumpTextToHex((unsigned char*)pTempDestText, ucs2Length);
#endif

    g_iconv_close(cd);

    return ucs2Length;
}


/**
args:
unsigned char *pDestText
int maxLength		: max byte length of destination text
const unsigned char *pSrcText
int srcTextLen		: byte length of UCS2 source text
return :
byte length of converted UTF8 characters
-1 : The alpha isn't the gsm 7bit code
 */
int MsgConvertUCS2toUTF8(unsigned char *pDestText, int maxLength, const unsigned char *pSrcText,  int srcTextLen)
{
    int i , j ;
    int remainedBuffer = maxLength;
    int utf8Length;

#ifdef CONVERT_DUMP
    int srcLen = srcTextLen;
    const unsigned char * pTempSrcText = pSrcText;
#endif
    unsigned char * pTempDestText = pDestText;

    i= j = 0;
    if(srcTextLen == 0 || pSrcText == NULL || pDestText ==  NULL || maxLength == 0)
    {
	fprintf(stderr, "UCS2 to UTF8 Failed as text length is 0\n");
	return FALSE;
    }		

    GIConv cd;
    int err=0;

    cd = g_iconv_open( "UTF8", "UCS-2BE" );

    if (cd > 0)
    {
	err = g_iconv(cd, (char**)&pSrcText, (gsize*)&srcTextLen, (char**)&pDestText, (gsize*)&remainedBuffer);
    }

    utf8Length = maxLength - remainedBuffer;
    pTempDestText[utf8Length] = 0x00;

#ifdef CONVERT_DUMP
    fprintf(stderr, "\n########## Dump UCS2 -> UTF8\n");
    MsgConvertDumpTextToHex((unsigned char*)pTempSrcText, srcLen);
    MsgConvertDumpTextToHex((unsigned char*)pTempDestText, utf8Length);
#endif

    g_iconv_close(cd);

    return utf8Length;
}


/**
  pDestText must be freed when it is not used.
 */
int MsgConvertLocaleToUTF8(unsigned char *pDestText, int maxLength, const unsigned char *pSrcText, int srcTextLen)
{
    int utf8Length = 0;
    unsigned char *utf8Text = NULL;

    utf8Text = (unsigned char *)g_locale_to_utf8((char*)pSrcText, srcTextLen, NULL, (gsize*)&utf8Length, NULL);
    if ( utf8Text == NULL )
    {
	fprintf(stderr, "Convert Error : Local to UTF8\n");
	return -1;
    }

    if (utf8Length >=  maxLength )
    {
	utf8Length = maxLength - 1;
    }
    strncpy((char*)pDestText, (char*)utf8Text,  utf8Length);
    pDestText[utf8Length] = 0x00;
    //	utf8Length++;		// 091029 is NULL char not included in length? should be confirmed


#ifdef CONVERT_DUMP
    fprintf(stderr, "\n########## Dump Locale -> UTF8\n");
    MsgConvertDumpTextToHex((unsigned char*)pSrcText, srcTextLen);
    MsgConvertDumpTextToHex((unsigned char*)pDestText, utf8Length);
#endif

    free(utf8Text);

    return utf8Length;
}


/**
return:
bytelength of UTF8 text
 */
int MsgConvertGSM7bitToUTF8(unsigned char *pDestText, int maxLength,  const unsigned char *pSrcText, int srcTextLen)
{
    int utf8Length = 0;
    int ucs2Length = 0;
    int maxUCS2Length = srcTextLen;		// max # of UCS2 chars, NOT bytes. when all gsm7 chars are only one byte(-there is no extenstion), UCS2Length is maxUCS2 Length. otherwise(ex: gsm7 char starts with 0x1b) UCS2Length must be less than gsm7 legnth
    unsigned short pUCS2Text[maxUCS2Length];
    bzero(pUCS2Text, maxUCS2Length);

    fprintf(stderr, "srcTextLen = %d\n", srcTextLen);
    fprintf(stderr, "max dest Length = %d\n", maxLength);

    ucs2Length = MsgConvertGSM7bitToUCS2((unsigned char*)pUCS2Text, maxUCS2Length * sizeof(unsigned short), pSrcText,  srcTextLen);
    if(ucs2Length < 0) {
	fprintf(stderr, "MsgConvertGSM7bitToUTF8: UCS2 to GSM7bit Failed as text length is 0\n");
	return ucs2Length;
    }
    memcpy(pDestText, pUCS2Text, ucs2Length);
    //utf8Length = MsgConvertUCS2toUTF8(pDestText, maxLength, (unsigned char*)pUCS2Text, ucs2Length);

    //return utf8Length;
    return ucs2Length;
}


int MsgConvertUTF8ToGSM7bit(unsigned char *pDestText, int maxLength,  const unsigned char *pSrcText, int srcTextLen)
{
    int utf8Length = 0;
    int gsm7bitLength = 0;
    int ucs2Length = 0;

    BOOL bUnknown = FALSE;

    if ( srcTextLen == -1 )
    {
	utf8Length = strlen((char*)pSrcText);
	srcTextLen = utf8Length;
    }
    else
    {
	utf8Length = srcTextLen;
    }
    int maxUCS2Length = utf8Length;		// max # of UCS2 chars, NOT bytes. when all utf8 chars are only one byte, UCS2Length is maxUCS2 Length. otherwise (ex: 2 bytes of UTF8 is one char) UCS2Length must be  less than utf8Length
    unsigned short pUCS2Text[maxUCS2Length];
    bzero(pUCS2Text, maxUCS2Length);

    fprintf(stderr, "srcTextLen = %d\n", srcTextLen);
    fprintf(stderr, "temp buffer size = %d\n", maxUCS2Length * sizeof(unsigned short));
    fprintf(stderr, "max dest Length = %d\n", maxLength);

    ucs2Length = MsgConvertUTF8toUCS2((unsigned char*)pUCS2Text, maxUCS2Length * sizeof(unsigned short), pSrcText, srcTextLen);
    gsm7bitLength = MsgConvertUCS2toGSM7bit(pDestText, maxLength, (unsigned char*)pUCS2Text, ucs2Length, &bUnknown);

    return gsm7bitLength;
}

#if CPU_ENDIAN == CPU_ENDIAN_LITTLE
#define prv_endian_swap_uint16(_x) (_x)

#else
#define prv_endian_swap_uint16(_x)	(((((uint16_t) (_x)) << 8) & 0xFF00) | \
	((((uint16_t) (_x)) >> 8) & 0x00FF))
#endif

#define BUFF_SIZE 256

BOOL EncodeSmsDeliverTpdu(SmsAddressInfo SCA, TPDU_SMS_DELIVER tpdu_deliver, char *rawdata, int *rawdata_len, BYTE pid)
{
    BYTE *packet = (BYTE *)rawdata;
    UINT8 tp_pid = pid;
    BYTE tp_dcs; //=17; //class_1(17),class_2(18)
    BYTE tmp_buff[BUFF_SIZE];;
    unsigned char timeStamp[20];
    int index = 0;
    int i = 0, size = 0;

    unsigned char  scaNumber[ADDRESS_LEN_MAX + 1];
    int			  scaLength;

    if(tpdu_deliver.dcs.bSMSCinSIM) {
	//packet[index++] = 0x0;
	index += SmsReadSMSCfromSIM(packet);
    }
    else {		
	if(SCA.diallingNum[0]=='+')
	{
	    memcpy(scaNumber,&SCA.diallingNum[1],SCA.dialnumLen -1);
	    scaLength = SCA.dialnumLen -1;
	    scaNumber[scaLength+1] = '\0';
	}
	else
	{
	    memcpy(scaNumber,SCA.diallingNum,SCA.dialnumLen);
	    scaLength = SCA.dialnumLen;
	    scaNumber[scaLength] = '\0';
	}

	index += SmsUtilEncodeAddrField_sca(packet, scaNumber, scaLength, SCA.ton, SCA.npi);
    }

    /* TP-MTI, TP-MMS, TP-RP, TP_UDHI, TP-SRI */
    tpdu_deliver.msgType = SMS_TPDU_DELIVER;

    packet[index] =  tpdu_deliver.msgType;							/* DELIVER: bits 0, 1 */
    packet[index] |= tpdu_deliver.mms? 0x04 : 0;						/* bit 2 */
    packet[index] |= tpdu_deliver.sri ? 0x20 : 0;						/* bit 5 */
    packet[index] |= tpdu_deliver.udhi? 0x40 : 0;						/* bit 6 */
    packet[index] |= tpdu_deliver.rp ? 0x80 : 0;						/* bit 7 */

    index++;

    /* TP-OA */
    unsigned char  orgNumber[ADDRESS_LEN_MAX + 1];
    int			  orgLength;

    if(tpdu_deliver.orgAddr.diallingNum[0]=='+')
    {
	memcpy(orgNumber,&tpdu_deliver.orgAddr.diallingNum[1],tpdu_deliver.orgAddr.dialnumLen -1);
	orgLength = tpdu_deliver.orgAddr.dialnumLen -1;
	orgNumber[orgLength+1] = '\0';
    }
    else
    {
	memcpy(orgNumber,tpdu_deliver.orgAddr.diallingNum,tpdu_deliver.orgAddr.dialnumLen);
	orgLength = tpdu_deliver.orgAddr.dialnumLen;
	orgNumber[orgLength] = '\0';
    }

    fprintf(stderr, "\nafter: orgNumber : %s , %x\n",orgNumber, orgLength);

    index +=  SmsUtilEncodeAddrField_dst(packet+index, orgNumber, orgLength, tpdu_deliver.orgAddr.ton, tpdu_deliver.orgAddr.npi);

    /*TP-PID*/
    packet[index++] = tp_pid;

    SmsUtilEncodeDCS(&tp_dcs, &tpdu_deliver.dcs);

    //packet[index++] = dcs;
    packet[index++] = tp_dcs;

    /*TP-SCTS*/
    SmsUtilEncodeTimeStamp(&tpdu_deliver.scts, timeStamp);
    memcpy(&packet[index], timeStamp, 7);
    index += 7;

    /*********TP-UDL & TP-UD **********/
    int pos;
    switch(tpdu_deliver.dcs.alphabetType) {
	case TAPI_NETTEXT_ALPHABET_DEFAULT:
	    size = strlen((char*)tpdu_deliver.userData);
	    packet[index++] = size + 1;
	    memset(tmp_buff, '\0', BUFF_SIZE);
	    pos = MsgConvertUTF8ToGSM7bit ( tmp_buff, BUFF_SIZE, (char*)tpdu_deliver.userData, size);
	    if(pos < 0) {
		fprintf(stderr, "EncodeSmsDeliverTpdu: UTF8 to GSM7bit Failed as text length is 0\n");
		*rawdata_len = index;
		return FALSE;
	    }
	    memcpy(tpdu_deliver.userData, tmp_buff, pos);
	    pos =SmsUtilPackGSMCode( packet + index, (char *)tpdu_deliver.userData, pos);
	    index += pos;
	    break;
	case TAPI_NETTEXT_ALPHABET_8BIT:
	    packet[index++] = strlen((char *)tpdu_deliver.userData);
	    pos =SmsUtilpackGSM8Code( packet + index, (char *)tpdu_deliver.userData, strlen((char *)tpdu_deliver.userData));
	    index += pos;
	    break;
	case TAPI_NETTEXT_ALPHABET_UCS2:
	    size = strlen((char*)tpdu_deliver.userData) * 2;
	    packet[index++] = size; //one unicode has 2 bytes
	    memset(tmp_buff, '\0', BUFF_SIZE);
	    pos = MsgConvertUTF8toUCS2 ( tmp_buff, BUFF_SIZE, (char*)tpdu_deliver.userData, size );
	    if(pos < 0) {
		fprintf(stderr, "EncodeSmsDeliverTpdu: UTF8 to UCS2 Failed as text length is 0\n");
		*rawdata_len = index;
		return FALSE;
	    }
	    memcpy(tpdu_deliver.userData, tmp_buff, pos);
	    memcpy(packet + index, tmp_buff, pos);
	    index += pos;
	    break;
	default:
	    fprintf(stderr, "Unknown alphabetType!\n");
	    exit(1);
    }

    *rawdata_len = index;

    for(i=0; i< index ; i++ )
    {
	fprintf(stderr, "[%02x]", packet[i]);
	if( i%10 == 9)
	    fprintf(stderr, "\n");
    }
    fprintf(stderr, "\n");

    return TRUE;

}

int DecodeSmsSubmitTpdu(TPDU_SMS_SUBMIT *tpdu_submit, int pdu_len , char * pPDU, int MO_sms)
{
    BYTE              scaAddr_len =0;
    int		     destAddr_len = 0;
    BYTE    	     diallingNum[TAPI_NETTEXT_ADDRESS_LEN_MAX];
    BYTE              scaAddr[TAPI_NETTEXT_SCADDRESS_LEN_MAX+1]; //service center address
    BYTE		     destAddr[TAPI_NETTEXT_ADDRESS_LEN_MAX+1]; //destination number
    int 		     sca_ton, sca_npi, dest_ton,dest_npi;
    int 		     position;
    BYTE	inData[TAPI_NETTEXT_SMDATA_SIZE_MAX+1];
    BYTE    tmp_buff[BUFF_SIZE];
    int size, udhl = 0;
    int i = 0, fillbits = 0;
    size_t limit_len = TAPI_NETTEXT_SCADDRESS_LEN_MAX;
    position=0;

    /* SCA_ADDR */
    memset(diallingNum, 0, sizeof(diallingNum));

    SmsUtilDecodeAddrField_sca((char *)diallingNum, (unsigned char *)&pPDU[position],&sca_ton, &sca_npi);

    position+=2;  //include Address-Length, Type of Address

    scaAddr_len = strlen( (char *)diallingNum );

    if ( scaAddr_len % 2 )
	position += scaAddr_len / 2 + 1;
    else
	position += scaAddr_len / 2;


    if ( sca_ton == SIM_TON_INTERNATIONAL )
    {
	scaAddr[0] = '+';
	limit_len = ((limit_len < (scaAddr_len+1)) ? limit_len : (scaAddr_len+1));
	memcpy( &scaAddr[1], diallingNum, limit_len-1 );
	scaAddr[limit_len] = '\0';
    }
    else
    {
	limit_len = ((limit_len < scaAddr_len) ? limit_len : scaAddr_len);
	memcpy( scaAddr, diallingNum, limit_len );
	scaAddr[limit_len] = '\0';
    }

    limit_len = TAPI_NETTEXT_SCADDRESS_LEN_MAX + 1;

    /* TP-MTI, TP-RD, TP-VPF,  TP-RP, TP_UDHI, TP-SRR */

    tpdu_submit->msgType = SMS_TPDU_SUBMIT; // bit 0,1(Message Type Indicator )
    tpdu_submit->rd = ( pPDU[position] & 0x04 ) ? TRUE : FALSE;// bit 2 (Message Type Indicator)
    tpdu_submit->vpf =( pPDU[position] & 0x18) >> 3; //  bits 3, 4 ( validity period format)
    tpdu_submit->srr=( pPDU[position] & 0x20 ) ? TRUE : FALSE;	// bit 5 (Status Report Request)
    tpdu_submit->udhi=( pPDU[position] & 0x40 ) ? TRUE : FALSE;	// bit 6 (User Data Header Indicator)
    tpdu_submit->rp = ( pPDU[position] & 0x80 ) ? TRUE : FALSE;	// bit 7 (Reply Path)

    position++ ;

    if( MO_sms == 1 ){
	/* Only for Mobile oriented sms */
	/* TP-MR */
	tpdu_submit->mr = pPDU[position];
	position++ ;
    }

    /* TP-DA */
    memset(diallingNum, 0, sizeof(diallingNum));
    SmsUtilDecodeAddrField_dst((char *)diallingNum, (unsigned char *)&pPDU[position],&dest_ton, &dest_npi);

    position+=2;

    destAddr_len = strlen( (char*) diallingNum );
    if ( destAddr_len % 2 )
	position += destAddr_len / 2 + 1;
    else
	position += destAddr_len / 2;

    if ( dest_ton == SIM_TON_INTERNATIONAL )
    {
	destAddr[0] = '+';
	memcpy( &destAddr[1], diallingNum, destAddr_len );
	limit_len = ((limit_len < (destAddr_len+1)) ? limit_len : (destAddr_len+1));
	destAddr[limit_len] = '\0';
	tpdu_submit->desAddr.dialnumLen = limit_len;
	memcpy(tpdu_submit->desAddr.diallingNum, destAddr , limit_len);
    }
    else
    {
	memcpy( destAddr, diallingNum, destAddr_len );
	limit_len = ((limit_len < destAddr_len) ? sizeof(tpdu_submit->desAddr.dialnumLen) : destAddr_len);
	destAddr[limit_len] = '\0';
	tpdu_submit->desAddr.dialnumLen = limit_len;
	memcpy(tpdu_submit->desAddr.diallingNum, destAddr , limit_len);
    }

    tpdu_submit->desAddr.npi = dest_npi;
    tpdu_submit->desAddr.ton = dest_ton;

    /* TP-PID */
    tpdu_submit->pId = pPDU[position];
    position++ ;

    /* TP DCS */
    SmsUtilDecodeDCS( &tpdu_submit->dcs, pPDU[position] );
    position ++;

    /* TP VP */
    //tpdu_submit->vp.vpType = TAPI_NETTEXT_VP_RELATIVE;  //temp

    switch (tpdu_submit->vpf)
    {
	case TAPI_NETTEXT_VP_RELATIVE:
	    {
		tpdu_submit->vp.vpValue = pPDU[position];
		tpdu_submit->vp.vpRelativeType = TAPI_NETTEXT_VP_REL_1D;
		position++;
		break;
	    }
	case TAPI_NETTEXT_VP_ABSOLUTE:
	case TAPI_NETTEXT_VP_ENHANCED:
	    {
		position += 7;
		break;
	    }
	default:
	    break;
    }

    /* TP UDL */
    tpdu_submit->udl = pPDU[position] & 0x00FF;

    if ( tpdu_submit->udl > TAPI_NETTEXT_SMDATA_SIZE_MAX )
	tpdu_submit->udl = TAPI_NETTEXT_SMDATA_SIZE_MAX;

    position++;

    /* TP UD */
    tpdu_submit->userData = malloc( sizeof ( unsigned char ) * ( TAPI_NETTEXT_SMDATA_SIZE_MAX + 1 ) );
    memset(tpdu_submit->userData, 0, sizeof ( unsigned char ) * ( TAPI_NETTEXT_SMDATA_SIZE_MAX + 1 ) );

    memset( inData, 0x00, TAPI_NETTEXT_SMDATA_SIZE_MAX + 1 );
    memcpy( (void*) inData, (void*) &pPDU[position], tpdu_submit->udl );
    fprintf(stderr, "inData [%s] \n", inData);

    if(tpdu_submit->udhi)
    {
	udhl = pPDU[position] + 1;
	fillbits = 7 - ((udhl * 8) % 7);
    }

    switch(tpdu_submit->dcs.alphabetType) {
	case TAPI_NETTEXT_ALPHABET_DEFAULT:		
	    //size = SmsUtilUnpackGSMCode((char *)tpdu_submit->userData, inData, (UINT8) tpdu_submit->udl);
	    {
		int i;
		BYTE    tmpData[TAPI_NETTEXT_SMDATA_SIZE_MAX+1];
		memset( tmpData, 0x00, TAPI_NETTEXT_SMDATA_SIZE_MAX + 1 );
		memcpy( (void*) tmpData, (void*)(inData + udhl), tpdu_submit->udl - udhl);
		if(tpdu_submit->udhi) {
		    for(i = 0; i < (tpdu_submit->udl - udhl); i++)
		    {
			tmpData[i] = (tmpData[i] >> fillbits) | (tmpData[i+1] << (8-fillbits));
		    }
		}
		size = SmsUtilUnpackGSMCode((char *)(tpdu_submit->userData + udhl), tmpData, (UINT8) tpdu_submit->udl - udhl);
	    }

	    memset(tmp_buff, '\0', BUFF_SIZE);
	    //MsgConvertGSM7bitToUTF8( tmp_buff, BUFF_SIZE, (char *)tpdu_submit->userData, size );
	    MsgConvertGSM7bitToUTF8( tmp_buff, BUFF_SIZE, (char *)(tpdu_submit->userData + udhl), size );
	    memcpy(tpdu_submit->userData, inData, udhl);
	    memcpy(tpdu_submit->userData + udhl, tmp_buff, size);
	    break;	
	case TAPI_NETTEXT_ALPHABET_8BIT:
	    SmsUtilUnpackGSM8Code((char *)tpdu_submit->userData, inData, (UINT8) tpdu_submit->udl);
	    break;
	case TAPI_NETTEXT_ALPHABET_UCS2:	
	    //size = SmsUtilUnpackUCS2Code((char *)tpdu_submit->userData, inData, (UINT8) tpdu_submit->udl);
	    memcpy((void*)tpdu_submit->userData, (void*) inData, tpdu_submit->udl );
	    memset(tmp_buff, '\0', BUFF_SIZE);
	    MsgConvertUCS2toUTF8( tmp_buff, BUFF_SIZE, (char *)tpdu_submit->userData, (UINT8) tpdu_submit->udl );
	    //memcpy(tpdu_submit->userData, tmp_buff, size);
	    break;
	default:
	    fprintf(stderr, "Unknown alphabetType!\n");
    }
    fprintf(stderr, "*************************************\n");
    fprintf(stderr, "Msg Type: TPDU_SUBMIT\n");

    if(tpdu_submit->rd)
	fprintf(stderr, "RD: Reject Duplicate\n");
    else if(!tpdu_submit->rd)
	fprintf(stderr, "RD: Accept Duplicate\n");

    if(tpdu_submit->srr)
	fprintf(stderr, "SRR: A status report is requested\n");
    else if(!tpdu_submit->srr)
	fprintf(stderr, "SRR: A status report is not  requested\n");

    if(tpdu_submit->udhi)
	fprintf(stderr, "UDHI: Contains a Header in addition to the short message.\n");
    else if(!tpdu_submit->udhi)
	fprintf(stderr, "UDHI: Only Short Message\n");

    if(tpdu_submit->rp)
	fprintf(stderr, "RP: Set Reply Path\n");
    else if(!tpdu_submit->rp)
	fprintf(stderr, "RP: Not set Reply Path\n");

    fprintf(stderr, "SCA Addr : %s\n",scaAddr);
    fprintf(stderr, "To : %s, %s\n" , destAddr, tpdu_submit->desAddr.diallingNum);
    fprintf(stderr, "Message : %s \n" , tpdu_submit->userData);
    fprintf(stderr, "*************************************\n");

    //return TRUE;
    return position;
}

//BOOL EncodeSmsSubmitReportTpdu()
BOOL EncodeSmsSubmitReportTpdu(unsigned char *packet, int *rawdata_len)
{
    TPDU_SMS_SUBMIT_REPORT submit_report;
    BYTE tp_dcs;
    BYTE mask=0x69;
    int index=0;
    int i ;
    TmDateTime tmTimeStamp;
    unsigned char timeStamp[20];
    time_t c_time;
    struct tm *tmTime;

    /* TP-MTI , TP-UDHI */
    submit_report.msgType = SMS_TPDU_SUBMIT_REPORT;
    submit_report.udhi = FALSE;

    packet[index] = submit_report.msgType;				 		//DELIVER REPORT
    packet[index] |= submit_report.udhi? 0x40 : 0;	//bit 6

    index ++;

    /* TP -PI */
    submit_report.paraInd = mask;
    packet[index] = submit_report.paraInd ;

    /*TP-SCTS*/
    c_time = time((time_t *)NULL);
    tmTime = localtime(&c_time);
    index++;
    tmTimeStamp.year = tmTime->tm_year+1900;
    tmTimeStamp.month = tmTime->tm_mon+1;
    tmTimeStamp.day = tmTime->tm_mday;
    tmTimeStamp.hour = tmTime->tm_hour;
    tmTimeStamp.minute = tmTime->tm_min;
    tmTimeStamp.second = tmTime->tm_sec;
    SmsUtilEncodeTimeStamp(&tmTimeStamp, timeStamp);
    memcpy(&packet[index], timeStamp, 7);
    index += 7;

    if ( ( mask != 0xFF ) && ( mask != 0 ) )
    {
	index++;
	/* TP-PID */
	if ( mask & 0x01)
	{
	    submit_report.pId = 0x65 ;
	    packet[index++] = submit_report.pId;
	}

	/* TP-DCS */
	if ( mask & 0x02)
	{
	    submit_report.dcs.bCompressed = FALSE;
	    submit_report.dcs.bMsgClassSet = TRUE;
	    submit_report.dcs.codingGroupType =TAPI_NETTEXT_CODGRP_SM_GENERAL_DCS;
	    submit_report.dcs.alphabetType = TAPI_NETTEXT_ALPHABET_DEFAULT;
	    submit_report.dcs.classType = TAPI_NETTEXT_CLASS_2;

	    SmsUtilEncodeDCS(&tp_dcs, &submit_report.dcs);
	    packet[index++] = tp_dcs;
	}
	/* TP-UDL */
	/* This if statement couldn't be true because mask var is fixed to 0x69 on declaration.
	   if ( mask & 0x04)
	   {
	   submit_report.udl =0x00;
	   packet[index++] = submit_report.udl;

	// TP-UD

	if (submit_report.dcs.alphabetType == TAPI_NETTEXT_ALPHABET_DEFAULT )
	{
	//SmsUtilPackGSMCode( packet + index, msg, msg_len);
	}
	} */
    }

    *rawdata_len = index;

    fprintf(stderr, "\nEncodeSmsSubmitReportTpdu");
    fprintf(stderr, "\nSend packet to phonseSever[%d] -----\n", index);

    for(i=0; i< index ; i++ )
    {
	fprintf(stderr, "[%02x]", packet[i]);
	if( i%10 == 9)
	    fprintf(stderr, "\n");
    }
    fprintf(stderr, "\n");

    return TRUE;
}
BOOL EncodeCB_GSM(CELLBROADCASTING cb_msg, char *rawdata, int *rawdata_len)
{
    BYTE *packet = (BYTE *)rawdata;
    BYTE tmp_buff[BUFF_SIZE];

    int 		index = 0;
    int 		i = 0;
    int 		pos, size;

    fprintf(stderr, "\n\nEncodeCB_GSM.......\n");

    if (cb_msg.accessNetwork== CB_GSM)
    {
	// Serial Number

	BYTE serialNumber[2];
	serialNumber[0] = (cb_msg.geographicalScope <<6)|((cb_msg.messageCode>>4) & 0x3f);
	serialNumber[1] = ((cb_msg.messageCode & 0x0f)<<4)|(cb_msg.updateNumber & 0x0f);

	fprintf(stderr, "Serial Number : [%02x][%02x]\n", serialNumber[0], serialNumber[1]);

#if CPU_ENDIAN == CPU_ENDIAN_LITTLE
	packet[index++] = serialNumber[0];
	packet[index++] = serialNumber[1];
#else
	packet[index++] = serialNumber[1];
	packet[index++] = serialNumber[0];
#endif

	fprintf(stderr, " packet[0] = %02x, packet[1] = %02x\n", packet[0], packet[1]);

	// Message Id
	BYTE messageID[2];

	messageID[0] = (cb_msg.messageId >> 8) & 0xff;
	messageID[1] = cb_msg.messageId & 0xff;
	fprintf(stderr, "messageId = [%02x],[%02x]\n", messageID[0],messageID[1]);

#if CPU_ENDIAN == CPU_ENDIAN_LITTLE
	packet[index++] = messageID[0];
	packet[index++] = messageID[1];
#else
	packet[index++] = messageID[1];
	packet[index++] = messageID[0];
#endif

	fprintf(stderr, " packet[2] = %02x, packet[3] = %02x\n", packet[2], packet[3]);

	// DCS
	packet[index++] = cb_msg.language;

	// Page Parameter
	packet[index++] =  cb_msg.page;


    }

    else if(cb_msg.accessNetwork== CB_UMTS)
    {

	// Message Type
	packet[index++] = cb_msg.messageType;

	// Message Id
	BYTE messageID[2];

	messageID[0] = (cb_msg.messageId >> 8) & 0xff;
	messageID[1] = cb_msg.messageId & 0xff;

	fprintf(stderr, "messageId = [%02x],[%02x]\n", messageID[0],messageID[1]);

#if CPU_ENDIAN == CPU_ENDIAN_LITTLE
	packet[index++] = messageID[0];
	packet[index++] = messageID[1];
#else
	packet[index++] = messageID[1];
	packet[index++] = messageID[0];
#endif

	fprintf(stderr, " packet[1] = %02x, packet[2] = %02x\n", packet[1], packet[2]);


	// Serial Number

	BYTE serialNumber[2];
	serialNumber[0] = (cb_msg.geographicalScope <<6)|((cb_msg.messageCode>>4) & 0x3f);
	serialNumber[1] = ((cb_msg.messageCode & 0x0f)<<4)|(cb_msg.updateNumber & 0x0f);

	fprintf(stderr, "Serial Number : [%02x][%02x]\n", serialNumber[0], serialNumber[1]);

#if CPU_ENDIAN == CPU_ENDIAN_LITTLE
	packet[index++] = serialNumber[0];
	packet[index++] = serialNumber[1];
#else
	packet[index++] = serialNumber[1];
	packet[index++] = serialNumber[0];
#endif

	fprintf(stderr, " packet[3] = %02x, packet[4] = %02x\n", packet[3], packet[4]);

	// DCS
	packet[index++] = cb_msg.language;

    }

    memset( tmp_buff, '\0', BUFF_SIZE);
    size = strlen((char*)cb_msg.message);
    MsgConvertUTF8ToGSM7bit ( tmp_buff, BUFF_SIZE, (char *)cb_msg.message, size );
    memcpy( cb_msg.message, tmp_buff, size );
    pos =SmsUtilPackGSMCode( packet + index, (char *)cb_msg.message, strlen((char *)cb_msg.message));
    index += pos;

    *rawdata_len = index;

    for(i=0; i< index ; i++ )
    {
	fprintf(stderr, "[%02x]", packet[i]);
	if( i%10 == 9)
	    fprintf(stderr, "\n");
    }
    fprintf(stderr, "\n");

    return TRUE;
}

