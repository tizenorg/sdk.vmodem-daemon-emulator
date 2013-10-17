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
				case 0x0394:	// ¥Ä	GREEK CAPITAL LETTER DELTA
					pDestText[outTextLen++] = 0x10;
					break;
				case 0x03A6:	// ¥Õ	GREEK CAPITAL LETTER PHI
					pDestText[outTextLen++] = 0x12;
					break;
				case 0x0393:	// ¥Ã	GREEK CAPITAL LETTER GAMMA
					pDestText[outTextLen++] = 0x13;
					break;
				case 0x039B:	// ¥Ë	GREEK CAPITAL LETTER LAMDA
					pDestText[outTextLen++] = 0x14;
					break;
				case 0x03A9:	// ¥Ø	GREEK CAPITAL LETTER OMEGA
					pDestText[outTextLen++] = 0x15;
					break;
				case 0x03A0:	// ¥Ð	GREEK CAPITAL LETTER PI
					pDestText[outTextLen++] = 0x16;
					break;
				case 0x03A8:	// ¥×	GREEK CAPITAL LETTER PSI
					pDestText[outTextLen++] = 0x17;
					break;
				case 0x03A3:	// ¥Ò	GREEK CAPITAL LETTER SIGMA
					pDestText[outTextLen++] = 0x18;
					break;
				case 0x0398:	// ¥È	GREEK CAPITAL LETTER THETA
					pDestText[outTextLen++] = 0x19;
					break;
				case 0x039E:	// ¥Î	GREEK CAPITAL LETTER XI
					pDestText[outTextLen++] = 0x1A;
					break;
				case 0x20AC:	// ¢æ: EURO SIGN
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

#if 0
BOOL EncodeSmsSubmitTpdu(char* diallingNum, int dialNumLen, char* msg, int msg_len)
{

	TPDU_SMS_SUBMIT tpdu_submit;

	BYTE  packet[TAPI_NETTEXT_MSG_SIZE_MAX];

	BYTE  SCA[TAPI_NETTEXT_SCADDRESS_LEN_MAX]; // real SCA value for sending SMS
	//BYTE  ScaAddr[TAPI_NETTEXT_SCADDRESS_LEN_MAX];  // in
	SmsAddressInfo  szSCAddr;
	BYTE  decodeScaAddr[TAPI_NETTEXT_SCADDRESS_LEN_MAX];

	UINT8 tp_mr = 0x01;
	UINT8 tp_pid = 65; //reaplace type (65)
	UINT8 tp_dcs; //=17; //class_1(17),class_2(18)
	UINT8 tp_vp;

	int index = 0;
	int i;

	unsigned char             sca_len =0;
	int 				    sca_ton, sca_npi;

	/*
	   TapiNetTextDataPackage *pDataPackage= malloc(sizeof(TapiNetTextDataPackage));

	   memset(pDataPackage, 0, sizeof(TapiNetTextDataPackage));
	 */

	memset(packet, 0, TAPI_NETTEXT_MSG_SIZE_MAX);

	/************SCA ADDRESS GET START**************/
	//TO DO
	//fill the SCA value
	//TapiGetServiceCenterAddress(&szSCAddr);
	strcpy( SCA, "820199111" );
	/*
	   SmsUtilDecodeAddrField(decodeScaAddr, &szSCAddr.diallingNum[0],&szSCAddr.ton, &szSCAddr.npi);
	   sca_len = strlen( decodeScaAddr );

	   memcpy( SCA, decodeScaAddr, sca_len);
	   SCA[sca_len] = '\0';

	   fprintf(stderr, "Decode SCA Address =========\n");
	   fprintf(stderr, "SCA Number :%s, SCA Length :%d \n",SCA , sca_len);
	   fprintf(stderr, "========================\n");
	   fprintf(stderr, "decodeScaAddr :%s\n",decodeScaAddr);
	 */

	/************SCA ADDRESS GET END**************/

	/*SCA*/

	//TODO : SCA
	index +=SmsUtilEncodeAddrField(packet,SCA, strlen(SCA)  ,0x01,0x01);

	//fprintf(stderr, "SCA ADDR Encoded =========\n");

	/* TP-MTI, TP-RD, TP-VPF, TP-SRR, TP_UDHI, TP-RP */
	tpdu_submit.msgType = SMS_TPDU_SUBMIT;
	tpdu_submit.rd   =TRUE; //FALSE :accept duplicate msg , true: reject duplicate msg
	tpdu_submit.vpf = TAPI_NETTEXT_VP_RELATIVE;
	tpdu_submit.srr  = FALSE;   //FALSE :A status report is not requested , true: A status report is requested
	tpdu_submit.udhi = FALSE ; //FALSE: contains only the short message , true :a Header in addition to the short message
	tpdu_submit.rp = TRUE  ; //FALSE :TP Reply Path parameter is not set in this SMS SUBMIT, true : TP Reply Path parameter is set in this SMS SUBMIT

	packet[index] =  tpdu_submit.msgType;							/* SUBMIT: bits 0, 1 */
	packet[index] |= tpdu_submit.rd? 0x04 : 0;						/* bit 2 */
	packet[index] |= tpdu_submit.vpf  << 3;	/* bits 3, 4 */
	packet[index] |= tpdu_submit.srr ? 0x20 : 0;						/* bit 5 */
	packet[index] |= tpdu_submit.udhi? 0x40 : 0;						/* bit 6 */
	packet[index] |= tpdu_submit.rp ? 0x80 : 0;						/* bit 7 */

	index++;

	/* TP-MR */
	packet[index++] = tp_mr;

	/* TP-DA */
	index +=  SmsUtilEncodeAddrField( packet+index, diallingNum, dialNumLen, 0x01, 0x01);

	//fprintf(stderr, "DESTINATTION  ADDR Encoded =========\n");

	/*TP-PID*/
	packet[index++] = tp_pid;

	/* TP_DCS (Data Coding Scheme) */

	tpdu_submit.dcs.bCompressed = FALSE;
	tpdu_submit.dcs.bMsgClassSet = TRUE;
	tpdu_submit.dcs.codingGroupType =TAPI_NETTEXT_CODGRP_SM_GENERAL_DCS;
	tpdu_submit.dcs.alphabetType = TAPI_NETTEXT_ALPHABET_DEFAULT;
	tpdu_submit.dcs.classType = TAPI_NETTEXT_CLASS_2;

	SmsUtilEncodeDCS(&tp_dcs, &tpdu_submit.dcs);

	packet[index++] = tp_dcs;

	//fprintf(stderr, "DCS Encoded:tp_dcs =%d =========\n",tp_dcs);

	/*TP-VP*/

	tpdu_submit.vp.vpRelativeType = TAPI_NETTEXT_VP_REL_6H ;
	tpdu_submit.vp.vpType = TAPI_NETTEXT_VP_RELATIVE;
	tpdu_submit.vp.vpValue =0xff ;

	SmsUtilEncodeValidity(&tp_vp,&tpdu_submit.vp);

	//tp_vp =65;
	packet[index++] = tp_vp;
	//fprintf(stderr, "Validity Encoded :tp_vp =%d =========\n",tp_vp);

	/* UDL */
	packet[index++] = msg_len;

	/*********TP-UD **********/

	if (tpdu_submit.dcs.alphabetType == TAPI_NETTEXT_ALPHABET_DEFAULT )
	{
		int pos;
		memset(tmp_buff, '\0', BUFF_SIZE);
		MsgConvertUTF8ToGSM7bit( tmp_buff, BUFF_SIZE, msg, msg_len);
		memcpy(msg, tmp_buff, msg_len);
		pos =SmsUtilPackGSMCode( packet + index, msg, msg_len);
		index += pos;
	}


	//fprintf(stderr, "\nSend packet to phonseSever[%d] -----\n", index);

	for(i=0; i< index ; i++ )
	{
		fprintf(stderr, "[%02x]", packet[i]);
		if( i%10 == 9)
			fprintf(stderr, "\n");
	}
	fprintf(stderr, "\n");

#if 0
	memcpy( pDataPackage->szData, packet, index);
	pDataPackage->msgLength = index;

	/* SEND SMS */
	TapiSendNetText( pDataPackage, NULL);

	free(pDataPackage);
#endif /* 0 */

	return TRUE;

}

int   DecodeSmsDeliverTpdu(int tpdu_len , char * pTPDU)
{
	TPDU_SMS_DELIVER   tpdu_deliver;
	TmDateTime   		   SCTimeStamp;;
	int		    orgAddr_len = 0;
	BYTE    	    diallingNum[TAPI_NETTEXT_ADDRESS_LEN_MAX];
	BYTE		    orgAddr[TAPI_NETTEXT_ADDRESS_LEN_MAX]; //originating number
	int 		    sca_ton, sca_npi, org_ton,org_npi;
	int 		    position;
	int 		    dcs_pos =0;
	int i;

	position=0;

	/* TP-MTI, TP-MMS, TP-SRI, TP_UDHI, TP-RP */
	tpdu_deliver.msgType = SMS_TPDU_DELIVER;
	tpdu_deliver.mms = ( pTPDU[position] & 0x04 ) ? TRUE : FALSE;// bit 2 (Message Type Indicator)
	tpdu_deliver.sri =( pTPDU[position] & 0x20 ) ? TRUE : FALSE;	// bit 5 (Status Report Indication)
	tpdu_deliver.udhi=( pTPDU[position] & 0x40 ) ? TRUE : FALSE;	// bit 6 (User Data Header Indicator)
	tpdu_deliver.rp = ( pTPDU[position] & 0x80 ) ? TRUE : FALSE;	// bit 7 (Reply Path)

	position++ ;

	/* TP-OA */

	memset(diallingNum, 0, sizeof(diallingNum));

	SmsUtilDecodeAddrField(diallingNum, &pTPDU[position],&org_ton, &org_npi);

	position+=2;

	orgAddr_len = strlen( (char*) diallingNum );
	if ( orgAddr_len % 2 )
		position += orgAddr_len / 2 + 1;
	else
		position += orgAddr_len / 2;


	if ( org_ton == SIM_TON_INTERNATIONAL )
	{
		orgAddr[0] = '+';
		memcpy( &orgAddr[1], diallingNum, orgAddr_len );
		orgAddr[orgAddr_len+1] = '\0';
	}
	else
	{
		memcpy( orgAddr, diallingNum, orgAddr_len );
		orgAddr[orgAddr_len] = '\0';
	}

	/* TP-PID */
	tpdu_deliver.pId= pTPDU[position];

	position ++;

	/* TP-DCS */
	SmsUtilDecodeDCS( &tpdu_deliver.dcs, pTPDU[position] );
	dcs_pos = position;
	position++;

	/* TP-SCTS */
	SmsUtilDecodeTimeStamp( &pTPDU[position], & SCTimeStamp );

	position+=7;

	/* TP-UDL */
	tpdu_deliver.udl = pTPDU[position];

	if ( tpdu_deliver.udl > TAPI_NETTEXT_SMDATA_SIZE_MAX )
	{
		tpdu_deliver.udl = TAPI_NETTEXT_SMDATA_SIZE_MAX;
		pTPDU[position] = tpdu_deliver.udl;
	}

	position++;

	/* TP-UD */
	tpdu_deliver.userData = malloc( sizeof ( unsigned char ) * ( TAPI_NETTEXT_SMDATA_SIZE_MAX + 1 ) );

	memset(tpdu_deliver.userData, 0, sizeof ( unsigned char ) * ( TAPI_NETTEXT_SMDATA_SIZE_MAX + 1 ) );

	if (tpdu_deliver.dcs.alphabetType == TAPI_NETTEXT_ALPHABET_DEFAULT )
	{
		BYTE	inData[TAPI_NETTEXT_SMDATA_SIZE_MAX+1];

		memset( inData, 0x00, TAPI_NETTEXT_SMDATA_SIZE_MAX + 1 );

		memcpy( (void*) inData, (void*) &pTPDU[position], tpdu_deliver.udl );

		SmsUtilUnpackGSMCode(tpdu_deliver.userData, inData, (UINT8) tpdu_deliver.udl);
	}

	fprintf(stderr, "*************************************\n");
	fprintf(stderr, "Msg Type: TPDU_DELIVER\n");

	if(tpdu_deliver.rp )
		fprintf(stderr, "RP: Set Reply Path\n");
	else if(!tpdu_deliver.rp )
		fprintf(stderr, "RP: Not set Reply Path\n");

	if(tpdu_deliver.sri)
		fprintf(stderr, "SRI: A status report shall be returned to the SME\n");
	else if(!tpdu_deliver.sri)
		fprintf(stderr, "SRI: A status report shall be not returned to the SME\n");

	if(tpdu_deliver.mms)
		fprintf(stderr, "MMS: No more messages are waiting for the MS in this SC\n");
	else if(!tpdu_deliver.mms)
		fprintf(stderr, "MMS: More messages are waiting for the MS in this SC\n");

	if(tpdu_deliver.udhi)
		fprintf(stderr, "UDHI: Contains a Header in addition to the short message.\n");
	else	if(!tpdu_deliver.udhi)
		fprintf(stderr, "UDHI: Only short message.\n");

	fprintf(stderr, "From : %s\n" ,orgAddr );
	fprintf(stderr, "Time : %d-%d-%d  , %d:%d:%d\n",
			SCTimeStamp.year, SCTimeStamp.month, SCTimeStamp.day, SCTimeStamp.hour, SCTimeStamp.minute ,SCTimeStamp.second);
	fprintf(stderr, "Message : %s \n" , tpdu_deliver.userData);
	fprintf(stderr, "*************************************\n");


	return dcs_pos;
}

BOOL EncodeSmsDeliverReportTpdu()
{
	TPDU_SMS_DELIVER_REPORT deliver_report;
	unsigned char packet[500];
	//TapiNetTextDataPackage* pDataPackage;
	BYTE tp_dcs;
	BYTE mask=0x69;
	int index=0;
	int i ;

	/*
	   pDataPackage = malloc(sizeof(TapiNetTextDataPackage));
	   memset(pDataPackage, 0, sizeof(TapiNetTextDataPackage));
	 */

	memset(packet, 0, 500);

	/* TP-MTI , TP-UDHI */
	deliver_report.msgType = SMS_TPDU_DELIVER_REPORT;
	deliver_report.udhi = FALSE;

	packet[index] = deliver_report.msgType;				 		//DELIVER REPORT
	packet[index] |= deliver_report.udhi? 0x40 : 0;	//bit 6

	index ++;

	/* TP-FCS */
	//if(result==FAIL)
	//{


	//}


	/* TP -PI */
	deliver_report.paraInd = mask;
	packet[index] = deliver_report.paraInd ;

	if ( ( mask != 0xFF ) && ( mask != 0 ) )
	{
		index++;
		/* TP-PID */
		if ( mask & 0x01)
		{
			deliver_report.pId = 0x65 ;
			packet[index++] = deliver_report.pId;
		}

		/* TP-DCS */
		if ( mask & 0x02)
		{
			deliver_report.dcs.bCompressed = FALSE;
			deliver_report.dcs.bMsgClassSet = TRUE;
			deliver_report.dcs.codingGroupType =TAPI_NETTEXT_CODGRP_SM_GENERAL_DCS;
			deliver_report.dcs.alphabetType = TAPI_NETTEXT_ALPHABET_DEFAULT;
			deliver_report.dcs.classType = TAPI_NETTEXT_CLASS_2;

			SmsUtilEncodeDCS(&tp_dcs, &deliver_report.dcs);
			packet[index++] = tp_dcs;
		}
		/* TP-UDL */
		if ( mask & 0x04)
		{
			deliver_report.udl =0x00;
			packet[index++] = deliver_report.udl;

			/* TP-UD */

			if (deliver_report.dcs.alphabetType == TAPI_NETTEXT_ALPHABET_DEFAULT )
			{
				//SmsUtilPackGSMCode( packet + index, msg, msg_len);
			}
		}
	}
	fprintf(stderr, "\nEncodeSmsDeliverReportTpdu");
	fprintf(stderr, "\nSend packet to phonseSever[%d] -----\n", index);

	for(i=0; i< index ; i++ )
	{
		fprintf(stderr, "[%02x]", packet[i]);
		if( i%10 == 9)
			fprintf(stderr, "\n");
	}
	fprintf(stderr, "\n");

	/*
	   memcpy( pDataPackage->szData, packet, index);
	   pDataPackage->msgLength = index;

	   TapiSetDeliverReport(pDataPackage);
	 */

	return TRUE;
}

BOOL DecodeCellBroadcastMsg(int length,char * pPDU)
{
	UINT8 		msgType;
	int 			msgLength;
	int			sn = 0; //serial number
	int			msgID; //message identifier
	TapiNetTextCodingScheme  dcs;

	UINT8	page[2];
	int 			contents_length;
	UINT8  	 contents[TAPI_NETTEXT_CB_SIZE_MAX];

	UINT8 	gs;
	UINT8  	msgCode;
	UINT8  	updateNumber;
	UINT8	fillBits = 0;

	sn = (pPDU[0] <<8 ) |pPDU[1] ;

	gs = (pPDU[0] & 0xC0) >> 6;  //bit 7, bit 6

	msgCode =((pPDU[0] & 0x3F)<<4) |pPDU[1] & 0xF0 ;

	updateNumber = pPDU[1] & 0x0F;  //bit3, bit2, bit1, bit0

	msgID = pPDU[2] << 4 |pPDU[3];

	SmsUtilDecodeDCS( &dcs, pPDU[4] );

	page[0] = pPDU[5] & 0x0F; //total page
	page[1] = (pPDU[5] & 0xF0)>>4 ; //current page


	msgLength = length -2;

	contents_length = msgLength -6;

	if (dcs.alphabetType == TAPI_NETTEXT_ALPHABET_DEFAULT )
	{
		BYTE	inData[TAPI_NETTEXT_CB_SIZE_MAX+1];

		memset( inData, 0x00, TAPI_NETTEXT_CB_SIZE_MAX + 1 );

		memcpy( (void*) inData, (void*) &pPDU[6], contents_length);
		SmsUtilUnpackGSMCode(contents,  &pPDU[6], msgLength );
	}
	else
	{
		memcpy( contents, &pPDU[6], msgLength);
		contents[msgLength] = NULL;
	}

	fprintf(stderr, "*************************************\n");
	fprintf(stderr, "serical number : 0x%04x \n" , sn);
	fprintf(stderr, "Geographical Scope: %x\n" , gs);
	fprintf(stderr, "message code : %x\n", msgCode);
	fprintf(stderr, "update Number : %x\n",updateNumber);
	fprintf(stderr, "message identifier :  %x\n" , msgID);
	fprintf(stderr, "data coding scheme :  %x\n" , dcs);
	fprintf(stderr, "contents bytes size : %d\n", contents_length);
	fprintf(stderr, "page : (%d /%d) (page /total ) \n", page[1], page[0] );
	fprintf(stderr, "contents: %s\n", contents);
	fprintf(stderr, "*************************************\n");

	/*
	   if(dcs.classType = TAPI_NETTEXT_CLASS_2)
	   {
	   TapiNetTextDataPackage * pDataPackage;

	   pDataPackage = malloc(sizeof(TapiNetTextDataPackage));

	   pDataPackage->msgLength =length ;
	   memcpy(pDataPackage->szData, pPDU, pDataPackage->msgLength);

	   TapiWriteNetText(0x01, pDataPackage);
	   }
	 */

	return TRUE;
}

BOOL DecodeSmsSubmitReportTpdu(int length , char *data)
{

	TPDU_SMS_SUBMIT_REPORT 	submit_report;
	TmDateTime   		   		SCTimeStamp;
	int 						position=0;
	int 						i=0;
	BYTE						msgType, udhi;
	BYTE						result ,msgRef, tpdu_len;
	BYTE 					mask;

	/* Result */

	result = data[position];
	position ++;

	/* Msg Reference */

	msgRef =  data[position];
	position ++;

	/* TPDU Length */

	tpdu_len = data[position];
	position ++;

	position ++; //temp, delete later

	/**** SUBMIT REPORT TPDU PARSING ***/

	/* TP-MTI , TP-UDHI */

	submit_report.msgType = data[position] & 0x03; //bit 0,1
	submit_report.udhi = data[position] & 0x40 ? TRUE : FALSE;	 //bit 6;

	position ++;

	if(result == FAIL)
	{
		/* TP-FCS */
		submit_report.fcs =  data[position] ;
		position ++;
	}

	/* TP-PI  - set to a 1 indicates that a particular optional parameter is present in the fields which follow. */
	/* Masking Bit  - TP -PID (bit 0), TP-DCS(bit 1), TP-UDL(bit 2) */

	submit_report.paraInd = data[position] ;
	mask =  submit_report.paraInd;
	position ++;

	//fprintf(stderr, "Parmeter Indication mask =%x\n", mask);

	/* TP-SCTS */

	SmsUtilDecodeTimeStamp( &data[position], & SCTimeStamp );

	position+=7;

	if( ( mask != 0xFF ) && ( mask != 0 ) )
	{
		/* TP-PID */
		if ( mask & 0x01)
		{
			submit_report.pId= data[position];
			position ++;
			fprintf(stderr, "TP-PID :%x\n", submit_report.pId);
		}

		/* TP-DCS */
		if ( mask & 0x02)
		{
			SmsUtilDecodeDCS( &submit_report.dcs, data[position] );
			position ++;
			fprintf(stderr, "TP-DCS :%x\n",data[position] );
		}

		if ( mask & 0x04)
		{
			/* TP-UDL */
			submit_report.udl = data[position];
			position++;

			fprintf(stderr, "TP-UDL :%x\n",data[position] );

			/* TP-UD */

			submit_report.userData = malloc( sizeof ( BYTE ) * ( TAPI_NETTEXT_SMDATA_SIZE_MAX + 1 ) );

			memset(submit_report.userData, 0, sizeof ( BYTE ) * ( TAPI_NETTEXT_SMDATA_SIZE_MAX + 1 ) );

			if (submit_report.dcs.alphabetType == TAPI_NETTEXT_ALPHABET_DEFAULT )
			{
				BYTE	inData[TAPI_NETTEXT_SMDATA_SIZE_MAX+1];

				memset( inData, 0x00, TAPI_NETTEXT_SMDATA_SIZE_MAX + 1 );

				memcpy( (void*) inData, (void*) &data[position], submit_report.udl );

				SmsUtilUnpackGSMCode(submit_report.userData, inData, (UINT8) submit_report.udl);
			}

		}
	}

	fprintf(stderr, "*************************************\n");

	if(result ==FAIL)
		fprintf(stderr, "SMS SEND FAIL!!\n");
	else if(result ==SUCCESS)
		fprintf(stderr, "SMS SEND SUCCESS!!\n");

	if(result == FAIL)
		fprintf(stderr, "Failure Code : %x\n",submit_report.fcs);

#if 0
	fprintf(stderr, "TPDU Length : %d\n", tpdu_len);

	fprintf(stderr, "Time : %d-%d-%d  , %d:%d:%d\n",
			SCTimeStamp.year, SCTimeStamp.month, SCTimeStamp.day, SCTimeStamp.hour, SCTimeStamp.minute ,SCTimeStamp.second);
	if(result == FAIL)
		fprintf(stderr, "Failure Code : %x\n",submit_report.fcs);

	if ( mask & 0x04)
		fprintf(stderr, "Msg : %s\n",submit_report.userData);
#endif

	fprintf(stderr, "*************************************\n");

	return TRUE;
}

BOOL DecodeSmsStatusReportTpdu(int tpdu_len, char * pTPDU)
{
	TPDU_SMS_STATUS_REPORT status_report;


	TmDateTime   SCTimeStamp;
	TmDateTime   DischargeTime;

	//int                  scaAddr_len =0;
	int		      rcpAddr_len = 0;
	BYTE    	      diallingNum[TAPI_NETTEXT_ADDRESS_LEN_MAX];
	//BYTE              scaAddr[TAPI_NETTEXT_SCADDRESS_LEN_MAX]; //service center address
	BYTE		      rcpAddr[TAPI_NETTEXT_ADDRESS_LEN_MAX]; //recipient number
	//int 		      sca_ton, sca_npi, rcp_ton,rcp_npi;
	int 		      rcp_ton,rcp_npi;
	int 		      position=0;
	UINT		      mask;

	//int i;

	/* TP-MTI, TP MMS, TP SRQ, TP_UDHI */

	status_report.msgType = pTPDU[position] & 0x03; // bit 0,1
	status_report.mms = pTPDU[position] & 0x04 ? TRUE : FALSE ; // bit 2
	status_report.srq = pTPDU[position] &  0x20 ? TRUE : FALSE ; //bit 5
	status_report.udhi = pTPDU[position] & 0x40 ? TRUE : FALSE;	 //bit 6

	position ++;

	/* TP-MR */
	status_report.mr = pTPDU[position];
	position ++;

	/* TP-RA */

	memset(diallingNum, 0, sizeof(diallingNum));

	SmsUtilDecodeAddrField(diallingNum, &pTPDU[position],&rcp_ton, &rcp_npi);

	position+=2;

	rcpAddr_len = strlen( (char*) diallingNum );
	if ( rcpAddr_len % 2 )
		position += rcpAddr_len / 2 + 1;
	else
		position += rcpAddr_len / 2;


	if ( rcp_ton == SIM_TON_INTERNATIONAL )
	{
		rcpAddr[0] = '+';
		memcpy( &rcpAddr[1], diallingNum, rcpAddr_len );
		rcpAddr[rcpAddr_len+1] = '\0';
	}
	else
	{
		memcpy( rcpAddr, diallingNum, rcpAddr_len );
		rcpAddr[rcpAddr_len] = '\0';
	}

	status_report.rcpAddr.dialnumLen= rcpAddr_len;
	memcpy(status_report.rcpAddr.diallingNum, rcpAddr , rcpAddr_len);
	status_report.rcpAddr.npi = rcp_npi;
	status_report.rcpAddr.ton = rcp_ton;

	/* TP-SCTS */
	SmsUtilDecodeTimeStamp( &pTPDU[position], & SCTimeStamp );
	position+=7;

	/* TP-DT */
	SmsUtilDecodeTimeStamp( &pTPDU[position], & DischargeTime );
	position+=7;

	/* TP-ST */
	status_report.status = pTPDU[position];
	position ++;

	/* TP-PI */
	status_report.paraInd = pTPDU[position];

	mask = status_report.paraInd  ;

	if ( ( mask != 0xFF ) && ( mask != 0 ) )
	{
		/* Increment only if mask is valid */
		position ++;

		/* TP-PID */
		if ( mask & 0x01)
		{
			status_report.pId= pTPDU[position];
			position ++;
		}
		/* TP-DCS */
		if ( mask & 0x02)
		{
			SmsUtilDecodeDCS( &status_report.dcs, pTPDU[position] );
			position ++;
		}
		if ( mask & 0x04)
		{
			/* TP-UDL */
			status_report.udl = pTPDU[position];
			position++;

			/* TP-UD */

			status_report.userData = malloc( sizeof ( unsigned char ) * ( TAPI_NETTEXT_SMDATA_SIZE_MAX + 1 ) );

			memset(status_report.userData, 0, sizeof ( unsigned char ) * ( TAPI_NETTEXT_SMDATA_SIZE_MAX + 1 ) );

			if (status_report.dcs.alphabetType == TAPI_NETTEXT_ALPHABET_DEFAULT )
			{
				BYTE	inData[TAPI_NETTEXT_SMDATA_SIZE_MAX+1];

				memset( inData, 0x00, TAPI_NETTEXT_SMDATA_SIZE_MAX + 1 );

				memcpy( (void*) inData, (void*) &pTPDU[position], status_report.udl );

				SmsUtilUnpackGSMCode(status_report.userData, inData, (UINT8) status_report.udl);
			}

		}
	}

	fprintf(stderr, "*************************************\n");
	fprintf(stderr, "Msg Type: STATUS_REPORT\n");
	if(status_report.mms)
		fprintf(stderr, "MMS: No more messages are waiting for the MS in this SC\n");
	else if(!status_report.mms)
		fprintf(stderr, "MMS: More messages are waiting for the MS in this SC\n");

	if(status_report.srq)
		fprintf(stderr, "SRQ: the result of an SMS COMMAND \n");
	else if(!status_report.srq)
		fprintf(stderr, "SRQ: the result of a SMS SUBMIT.\n");

	if(status_report.udhi)
		fprintf(stderr, "UDHI: Contains a Header in addition to the short message.\n");
	else if(!status_report.udhi)
		fprintf(stderr, "UDHI: Only Short Message\n");

	if(status_report.status==0x00)
		fprintf(stderr, "STATUS : Short message received by the SME\n");
	else if(status_report.status==0x01)
		fprintf(stderr, "STATUS : Short message forwarded by the SC to the SME\nbut the SC is unable to confirm delivery\n");
	else if(status_report.status==0x02)
		fprintf(stderr, "STATUS : Short message replaced by the SC\n");

	fprintf(stderr, "Recipient Number : %s\n" ,rcpAddr );
	fprintf(stderr, "SC Time Stamp : %d-%d-%d  , %d:%d:%d\n",
			SCTimeStamp.year, SCTimeStamp.month, SCTimeStamp.day, SCTimeStamp.hour, SCTimeStamp.minute ,SCTimeStamp.second);
	fprintf(stderr, "Discharge Time : %d-%d-%d  , %d:%d:%d\n",
			DischargeTime.year, DischargeTime.month, DischargeTime.day, DischargeTime.hour, DischargeTime.minute ,DischargeTime.second);

	if(mask & 0x04)
		fprintf(stderr, "Message : %s \n" , status_report.userData);

	fprintf(stderr, "*************************************\n");

	return TRUE;
}

void ActionStatusNotification(int length, char *pData)
{
	BYTE  actionCmd;
	BYTE  result;
	int  index;

	actionCmd = pData[0];
	result = pData[1];
	index = (int) pData[2];

	fprintf(stderr, "=====================================\n");
	if(actionCmd == 0x04)
	{
		if(result==0x00)
			fprintf(stderr, "\nSAVE MSESSAGE!! - FAIL\n");
		else if(result ==0x01)
			fprintf(stderr, "\nSAVE MSESSAGE:  INDEX = %d SUCCESS!!!\n",index);
	}
	else if(actionCmd == 0x05)
	{
		if(result==0x00)
			fprintf(stderr, "\nDELETE MSESSAGE!! - FAIL\n");
		else if(result ==0x01)
			fprintf(stderr, "\nDELETE MSESSAGE: INDEX = %d SUCCESS!!!\n",index);
	}

	fprintf(stderr, "=====================================\n");
}

static int tapi_callback(TapiEventClass eventClass, TapiEventType eventType, int param1, void* param2)
{
	int i;
	fprintf(stderr, ">>>>> sms tapi_callback() is called\n");
	char *pData ;
	int value;

	if(param2 !=NULL)
	{
		pData = malloc(sizeof((char *)param2));
		if(pData!=NULL)
		{
			pData= (char *)param2;
			value = (int)pData[0];
		}
	}
	fprintf(stderr, "eventClass=%x, eventType=%x, param1 = %x \n", eventClass, eventType, param1);

	switch(eventClass)
	{
		case TAPI_NETTEXT:
		case TAPI_NETTEXT_SMS:
			{
				switch(eventType)
				{
					case TAPI_EVENT_NETTEXT_INCOM :
						{
							fprintf(stderr, "\n$$$$$$ INCOMING MESSAGE NOTIFICATION $$$$$$ \n");
							ReceiveMessage(param1,pData);
							//system("cat sent.wav > /dev/dsp");
							break;
						}

					case TAPI_EVENT_NETTEXT_SENTSTATUS:
						{
							fprintf(stderr, "\n$$$$$$ RECEIVE  ACK NOTIFICATION $$$$$$ \n");
							DecodeSmsSubmitReportTpdu(param1,pData);
							system("cat chimeup.wav > /dev/dsp");
							break;
						}

					case TAPI_EVENT_NETTEXT_CB_INCOM:
						{
							fprintf(stderr, "\n$$$$$$ INCOMING CB NOTIFICATION $$$$$$ \n");
							DecodeCellBroadcastMsg(param1, pData);
							system("cat sent.wav > /dev/dsp");
							break;
						}
					case TAPI_EVENT_NETTEXT_ACTION_STATUS:
						{
							fprintf(stderr, "\n$$$$$$ ACTION STATUS  NOTIFICATION $$$$$$ \n");
							ActionStatusNotification(param1,pData);
							break;
						}
					case TAPI_EVENT_NETTEXT_MEMORY_STATUS_IND:
						{
							fprintf(stderr, "\n$$$$$$ MEMORYF  FULL NOTIFICATION $$$$$$ \n");
							break;
						}
					default:
						break;
				}
			}
		default:
			break;
	}
	return 0;
}

int  SendMessage()
{
	int 		ret;
	int 		len=0,msg_len=0;
	char 		buf[512];
	char 		message[512];
	char  	diallingNum[TAPI_NETTEXT_ADDRESS_LEN_MAX];
	char  	ScaAddr[TAPI_NETTEXT_SCADDRESS_LEN_MAX];
	int     	diallingNum_len=0;

	memset(buf,0, sizeof(buf));
	memset(diallingNum,0,sizeof(diallingNum));
	diallingNum_len=0;

	sfprintf(stderr, diallingNum,"82103001");

	fprintf(stderr, "Enter destination Number:\n");

	ret = read(0, buf, sizeof(buf));
	if (ret < 0)
	{
		if (errno == EINTR)
			perror("read(1)");
		return -1;
	}
	else if (ret == 0)
		return ret;

	diallingNum_len=strlen(diallingNum);
	memcpy(&diallingNum[diallingNum_len], buf,4);

	diallingNum_len = strlen(diallingNum); //recalculate
	diallingNum[diallingNum_len]=0;
	diallingNum_len-1;

	fprintf(stderr, "Enter Message\n");
	memset(message,0,sizeof(message));

	ret = read(0, message, sizeof(message));

	if (ret <= 0) {
		fprintf(stderr, " NULL msg can NOT be sent \n");
		return -1;
	}

	msg_len=strlen(message);

	fprintf(stderr, "==========================\n");
	fprintf(stderr, "To :%s\n", diallingNum);
	fprintf(stderr, "Message: %s\nMsg Length:%d\n",message, msg_len);
	fprintf(stderr, "Dialling number Length : %d\n" ,diallingNum_len);
	fprintf(stderr, "==========================\n\n");

	//TapiGetServiceCenterAddress(&ScaAddr);
	EncodeSmsSubmitTpdu(diallingNum, diallingNum_len,message, msg_len) ;
	return 1;

}

int ReadMessage()
{
	int 		menu;
	int 		ret;
	BYTE 	buf[100];
	int i;

	/************** Count Info *************/
	StoredMsgCountInfo msgCountInfo;
	/************* memory type************/
	int  * memoryType;
	int  * totalSize;
	int  * usedSize;
	/************read Msg****************/
	int 	index=0;
	int 	* msgStatus;
	TapiNetTextDataPackage DataPackage;

	int                scaAddr_len =0;
	char		    * pTPDU;
	int		    tpdu_len =0;
	BYTE		    diallingNum[TAPI_NETTEXT_ADDRESS_LEN_MAX];
	BYTE             scaAddr[TAPI_NETTEXT_SCADDRESS_LEN_MAX]; //service center address
	int 		    sca_ton, sca_npi;
	int 		    position;

	memset(buf,0, sizeof(buf));

	fprintf(stderr, "*************************************\n");
	fprintf(stderr, "1. SIM Message\n");
	//fprintf(stderr, "2. Incoming Msg\n");
	//fprintf(stderr, "3. Sent Msg\n");
	//fprintf(stderr, "4. Status Report\n");
	//fprintf(stderr, "5. Go back\n");
	//fprintf(stderr, "*************************************\n");
	fprintf(stderr, "Select Number:\n");

	ret = read(0, buf, sizeof(buf));

	if (ret < 0)
	{
		if (errno == EINTR)
			perror("read(1)");
		return -1;
	}
	else if (ret == 0)
		return ret;

	menu = atoi(buf);

	switch(menu)
	{
		case 1:  //SIM Message
			{
				TapiSetSelectMemory(0x02);  // 0x02: SIM memory

				TapiGetSelectMemory(&memoryType,&totalSize,&usedSize);

				if(memoryType == 0x02)
				{
					TapiGetNetTextCount(&msgCountInfo);

					fprintf(stderr, "totalCount =%d , usedCount=%d\n", msgCountInfo.totalCount,msgCountInfo.usedCount);

					if(msgCountInfo.usedCount!=0x00)
					{	fprintf(stderr, "Index LIST..........\n");
						for(i  = 0; i < msgCountInfo.usedCount ;  i++)
							fprintf(stderr, "[%02x]", msgCountInfo.index_list[i]);
					}

					fprintf(stderr, "\n=====================================\n");

					for (i = 0 ; i < msgCountInfo.usedCount ; i++)
					{
						TapiReadNetText(msgCountInfo.index_list[i], &msgStatus,&DataPackage);

						fprintf(stderr, "\n=======READ MESSAGE(index = %d)=======\n", msgCountInfo.index_list[i]);


						if(msgStatus == 0x00)
							fprintf(stderr, "Msg Staus : unknown msg\n");
						else if(msgStatus == 0x01)
							fprintf(stderr, "Msg Staus : received unread msg\n");
						else if(msgStatus ==0x02)
							fprintf(stderr, "Msg Staus : received read msg\n");
						else if(msgStatus ==0x03)
							fprintf(stderr, "Msg Staus : unsent msg\n");
						else if(msgStatus ==0x04)
							fprintf(stderr, "Msg Staus : stored sent msg\n");

						if(msgStatus==0x01 || msgStatus ==0x02)
						{
							position=0;

							/* SCA_ADDR */

							memset(diallingNum, 0, sizeof(diallingNum));

							SmsUtilDecodeAddrField(diallingNum, &DataPackage.szData[position],&sca_ton, &sca_npi);

							position+=2;  //include Address-Length, Type of Address

							scaAddr_len = strlen( diallingNum );

							if ( scaAddr_len % 2 )
								position += scaAddr_len / 2 + 1;
							else
								position += scaAddr_len / 2;


							if ( sca_ton == SIM_TON_INTERNATIONAL )
							{
								scaAddr[0] = '+';
								memcpy( &scaAddr[1], diallingNum, scaAddr_len );
								scaAddr[scaAddr_len+1] = '\0';
							}
							else
							{
								memcpy( scaAddr, diallingNum, scaAddr_len );
								scaAddr[scaAddr_len] = '\0';
							}

							tpdu_len = DataPackage.msgLength -position;

							fprintf(stderr, "SCA Number : %s\n",scaAddr);

							pTPDU = malloc(sizeof(BYTE) * tpdu_len);

							memcpy(pTPDU, &DataPackage.szData[position], tpdu_len);

							DecodeSmsDeliverTpdu(tpdu_len, pTPDU);
						}

						fprintf(stderr, "=====================================\n");

					}
				}

				break;
			}
			/*
			   case 2: //Incoming Msg
			   {
			   TapiSetSelectMemory(0x01);  // 0x01: Phone Flash memory
			   TapiGetSelectMemory(&memoryType,&totalSize,&usedSize);
			   if(memoryType ==0x01)
			   {

			//DecodeSmsDeliverTpdu(int pdu_len, char * pPDU, BOOL bRead);
			}
			break;
			}
			case 3: //Sent Msg
			{
			TapiSetSelectMemory(0x01);  // 0x01: Phone Flash memory
			TapiGetSelectMemory(&memoryType,&totalSize,&usedSize);
			if(memoryType ==0x01)
			{
			//DecodeSmsSubmitTpdu(int length, char * data);
			}
			break;
			}
			case 4: //Status Report Msg
			{
			TapiSetSelectMemory(0x01);  // 0x01: Phone Flash memory
			TapiGetSelectMemory(&memoryType,&totalSize,&usedSize);
			if(memoryType ==0x01)
			{
			//DecodeSmsStatusReportTpdu(int pdu_len, char * pPDU);
			}

			break;
			}
			 */
		case 5:
			return;

	}
	return 1;
}

int DeleteMessage()
{
	int 	ret;
	int  	index;
	BYTE buf[100];

	StoredMsgCountInfo msgCountInfo;

	int 	i;

	memset(buf, 0, sizeof(buf));

	fprintf(stderr, "Enter delete index >>\n");

	ret = read(0, buf, sizeof(buf));
	if (ret < 0)
	{
		if (errno == EINTR)
			perror("read(1)");
		return -1;
	}
	else if (ret == 0)
		return ret;

	index=atoi(buf);

	TapiDeleteNetText(index);
	TapiGetNetTextCount(&msgCountInfo);

	fprintf(stderr, "======Current Memory Infomation======\n");

	fprintf(stderr, "totalCount =%d , usedCount=%d\n", msgCountInfo.totalCount,msgCountInfo.usedCount);

	if(msgCountInfo.usedCount!=0x00)
	{	fprintf(stderr, "Index LIST..........\n");
		for(i  = 0; i < msgCountInfo.usedCount ;  i++)
			fprintf(stderr, "[%02x]", msgCountInfo.index_list[i]);
	}
	fprintf(stderr, "\n=====================================\n");
	return 1;

}

int ReceiveMessage(int pdu_len, char * pPDU)
{

	int                scaAddr_len =0;
	char		    * pTPDU;
	int		    tpdu_len =0;
	BYTE		    diallingNum[TAPI_NETTEXT_ADDRESS_LEN_MAX];
	BYTE             scaAddr[TAPI_NETTEXT_SCADDRESS_LEN_MAX]; //service center address
	int 		    sca_ton, sca_npi;
	int 		    position;
	int		    dcs_pos =0;
	BYTE 	    mti; //message type
	BYTE 	    sri; //status report indication
	int i;
	TapiNetTextCodingScheme dcs;

	position=0;

	/* SCA_ADDR */

	memset(diallingNum, 0, sizeof(diallingNum));

	SmsUtilDecodeAddrField(diallingNum, &pPDU[position],&sca_ton, &sca_npi);

	position+=2;  //include Address-Length, Type of Address

	scaAddr_len = strlen( diallingNum );

	if ( scaAddr_len % 2 )
		position += scaAddr_len / 2 + 1;
	else
		position += scaAddr_len / 2;


	if ( sca_ton == SIM_TON_INTERNATIONAL )
	{
		scaAddr[0] = '+';
		memcpy( &scaAddr[1], diallingNum, scaAddr_len );
		scaAddr[scaAddr_len+1] = '\0';
	}
	else
	{
		memcpy( scaAddr, diallingNum, scaAddr_len );
		scaAddr[scaAddr_len] = '\0';
	}

	mti =pPDU[position] & 0x03;

	tpdu_len = pdu_len -position;

	fprintf(stderr, "SCA Number : %s\n",scaAddr);

	pTPDU = malloc(sizeof(BYTE) * tpdu_len);

	memcpy(pTPDU, &pPDU[position], tpdu_len);

	if(mti ==SMS_TPDU_DELIVER)
	{
		DecodeSmsDeliverTpdu(tpdu_len, pTPDU);

		fprintf(stderr, "dcs_pos : %d\n", dcs_pos);

		SmsUtilDecodeDCS(&dcs, pPDU[position+dcs_pos]);

		/*
		   if(dcs.classType == TAPI_NETTEXT_CLASS_2)
		   {
		   TapiNetTextDataPackage * pDataPackage;

		   pDataPackage = malloc(sizeof(TapiNetTextDataPackage));

		   pDataPackage->msgLength = pdu_len ;
		   memcpy(pDataPackage->szData, pPDU, pDataPackage->msgLength);

		   TapiWriteNetText(0x01, pDataPackage);

		   free(pDataPackage);
		   }
		 */

	}
	else if(mti ==SMS_TPDU_STATUS_REPORT)
	{
		DecodeSmsStatusReportTpdu(tpdu_len, pTPDU);
	}


	free(pTPDU);


	return 1;

}

int GetInformation()
{
	//SysDebug(MID_TAPI | DBG_TAPI_NETTEXT, "====Information\n");

	int 		menu;
	int 		ret;
	BYTE 	buf[100];
	int i,k;

	/* Get Service Center Number */
	//BYTE  	ScaAddr[TAPI_NETTEXT_SCADDRESS_LEN_MAX];
	SmsAddressInfo  szSCAddr;

	BYTE  	decodeScaAddr[TAPI_NETTEXT_SCADDRESS_LEN_MAX];
	BYTE 	realScaAddr[TAPI_NETTEXT_SCADDRESS_LEN_MAX];
	int            sca_len =0;
	//int 		sca_ton, sca_npi;

	/* Get Beare Type */
	int  * 	svc;

	/* Get Stored Msg Count */
	int * pMemoryType;
	int * pTotalSize;
	int * pUsedSize;

	StoredMsgCountInfo msgCountInfo;

	/* Get CB Setting Information */
	TapiNetTextCBConfig  CBConfig;
	UINT16 CBChannel[MAX_GSM_SMS_CBMI_LIST_SIZE];

	memset(buf,0, sizeof(buf));

	fprintf(stderr, "*************************************\n");
	fprintf(stderr, "1.Get Service Center Number\n");
	fprintf(stderr, "2.Get Bearer Type\n");
	fprintf(stderr, "3.Get Current Memory Information\n");
	fprintf(stderr, "4.Get CB Setting Information\n");
	//fprintf(stderr, "5.Go back\n");
	fprintf(stderr, "*************************************\n");
	fprintf(stderr, "Select Number:\n");

	ret = read(0, buf, sizeof(buf));
	//fprintf(stderr, "ret :%d\n",ret);
	if (ret < 0)
	{
		if (errno == EINTR)
			perror("read(1)");
		return -1;
	}
	else if (ret == 0)
		return ret;

	menu = atoi(buf);

	switch(menu)
	{
		case 1:
			{
				TapiGetServiceCenterAddress(&szSCAddr);

				SmsUtilDecodeAddrField(decodeScaAddr, &szSCAddr.diallingNum[0],&szSCAddr.ton, &szSCAddr.npi);

				sca_len = strlen( decodeScaAddr );

				if ( szSCAddr.ton == SIM_TON_INTERNATIONAL )
				{
					realScaAddr[0] = '+';
					memcpy( &realScaAddr[1], decodeScaAddr, sca_len );
					realScaAddr[sca_len+1] = '\0';
				}
				else
				{
					memcpy( realScaAddr, decodeScaAddr, sca_len );
					realScaAddr[sca_len] = '\0';
				}

				fprintf(stderr, "===== Decode SCA Address =====\n");
				fprintf(stderr, "SCA Number :%s \n",realScaAddr);
				fprintf(stderr, "==============================\n");

				break;
			}
		case 2:
			{
				TapiGetPreferredBearer(&svc);

				fprintf(stderr, "====== Current Bearer ======\n");

				if(svc== TAPI_NETTEXT_BEARER_PS_ONLY)
					fprintf(stderr, "Packet Domain\n");
				else if(svc== TAPI_NETTEXT_BEARER_CS_ONLY)
					fprintf(stderr, "Circuit Switched\n");
				else if(svc== TAPI_NETTEXT_BEARER_PS_PREFERRED)
					fprintf(stderr, "Packet Domain preferred \n");
				else if(svc== TAPI_NETTEXT_BEARER_CS_PREFERRED)
					fprintf(stderr, "Circuit Switched preferred\n");


				fprintf(stderr, "===========================\n");

				break;
			}
		case 3:
			{
				TapiGetSelectMemory(&pMemoryType, &pTotalSize, & pUsedSize);

				fprintf(stderr, "========Current Memory ==========\n");

				if(pMemoryType==0x01)
				{
					fprintf(stderr, "Select Memroy: Phone Flash\n");
				}
				else if(pMemoryType==0x02)
				{
					fprintf(stderr, "Select Memroy: SIM\n");
				}
				TapiGetNetTextCount(&msgCountInfo);
				fprintf(stderr, "Total Count : %d , Used Count :%d\n",msgCountInfo.totalCount ,msgCountInfo.usedCount);
				fprintf(stderr, "===========================\n");

				break;
			}
		case 4:
			{
				TapiGetCBConfig(&CBConfig);

				fprintf(stderr, "=========CB Configuration=========\n");

				/*** CB Enable/Diable ***/
				if(CBConfig.CBEnabled==0x01)
					fprintf(stderr, "Cell Broadcast Msg Enabled...\n");
				else if(CBConfig.CBEnabled==0x02)
					fprintf(stderr, "Cell Broadcast Msg Disabled...\n");

				/*** Selected ID ***/
				if(CBConfig.selectedId == 0x01)
					fprintf(stderr, "All CBMI is selected... \n");
				else if(CBConfig.selectedId == 0x02)
					fprintf(stderr, "There are some selected CBMI...\n");

				/*** CBMI  Count ***/
				fprintf(stderr, "CBMI Count: %d \n\n",CBConfig.nMsgIdCount);

				/*** CBMI  List ***/
				if(CBConfig.nMsgIdCount !=0)
				{
					fprintf(stderr, "----- CBMI List -----\n");

					for(i =0 ; i < CBConfig.nMsgIdCount; i++)
					{
						fprintf(stderr, "No.%d - [0x%04x]\n", i,CBConfig.MsgIDs[i]);
					}

				}
				fprintf(stderr, "\n==================================\n");
				break;
			}
		case 5:
			return;

	}

	return 1;
}

int Setting()
{
	//SysDebug(MID_TAPI | DBG_TAPI_NETTEXT, "====Setting\n");
	int 				settingMenu;
	int 				ret;
	BYTE 			buf[512];

	BYTE     			memoryType[2]; //2006/8/8
	BYTE 			bearerType[2];  //2006/8/8
	BYTE     			cbEnable[2];//2006/8/8

	BYTE				scabuf[512];
	BYTE				scaNum[TAPI_NETTEXT_SCADDRESS_LEN_MAX];
	BYTE				scaNum_len =0;
	SmsAddressInfo       * szSCAddr;

	TapiNetTextCBConfig   *pCBConfig;

	int 				memType;
	int 				index;
	int 				msgStauts;
	int *      	  		CurMsgStauts;
	TapiNetTextDataPackage DataPackage;

	int 				idx=0;
	int				i =0;
	BYTE 			packet[500];

	memset(buf,0, sizeof(buf));

	fprintf(stderr, "*************************************\n");
	fprintf(stderr, "1.Set Service Center Number\n");
	fprintf(stderr, "2.Set Meomry Type\n");
	fprintf(stderr, "3.Set Bearer Type\n");
	fprintf(stderr, "4.Set Memory Full Notification\n");
	fprintf(stderr, "5.Set Deliver Report\n");
	fprintf(stderr, "6.Set CB Enable/Disable\n");
	fprintf(stderr, "7.Set Stored MsgStaus\n");
	//fprintf(stderr, "8.Go back\n");
	fprintf(stderr, "*************************************\n");
	fprintf(stderr, "Select Number:\n");

	ret = read(0, buf, sizeof(buf));

	if (ret < 0)
	{
		if (errno == EINTR)
			perror("read(1)");
		return -1;
	}
	else if (ret == 0)
		return ret;

	settingMenu = atoi(buf);


	switch(settingMenu)
	{
		case 1:  //Set Service Center Number
			{
				szSCAddr =malloc(sizeof(SmsAddressInfo));
				memset(szSCAddr,0, sizeof(szSCAddr));
				memset(scaNum, 0, sizeof(scaNum));
				memset(scabuf,0,sizeof(scabuf));

				sfprintf(stderr, scaNum,"821091");

				fprintf(stderr, "Enter SCA Number Last 4 Digit :\n");

				ret = read(0, scabuf, sizeof(scabuf));
				if (ret < 0)
				{
					if (errno == EINTR)
						perror("read(1)");
					return -1;
				}
				else if (ret == 0)
					return ret;

				scaNum_len=strlen(scaNum);
				memcpy(&scaNum[scaNum_len], scabuf, 4);

				scaNum_len = strlen(scaNum);
				scaNum[scaNum_len] = 0;
				scaNum_len-1;

				fprintf(stderr, "==========================\n");
				fprintf(stderr, "Sca Number :%s\n", scaNum);
				fprintf(stderr, "sca  number length : %d \n",  scaNum_len);
				fprintf(stderr, "==========================\n");

				idx = SmsUtilEncodeAddrField( packet, scaNum, scaNum_len, 0x01, 0x01);

				fprintf(stderr, "idx =%d\n",idx);

				for(i = 0; i < idx ; i++)
					fprintf(stderr, "[%02x]",packet[i]);

				szSCAddr->dialnumLen = idx ;

				memcpy(szSCAddr->diallingNum , packet, idx);

				TapiSetServiceCenterAddress(szSCAddr);

				free(szSCAddr);

				break;
			}
		case 2: //Set Meomry Type
			{
				fprintf(stderr, "Enter set memory type(1:Phone Flash,2:SIM):\n");

				memset(memoryType,0 ,sizeof(memoryType));

				ret = read(0, memoryType, sizeof(memoryType));

				if (ret <= 0) {
					fprintf(stderr, " NULL msg can NOT be sent \n");
					return -1;
				}

				fprintf(stderr, "MemoryType: %d\n", atoi(memoryType));

				TapiSetSelectMemory(atoi(memoryType));

				break;
			}
		case 3: //Set Bearer Type
			{
				fprintf(stderr, "Enter Bearer Type(1:PD, 2:CS, 3:PDP 4:CSP):\n");

				memset(bearerType,0 ,sizeof(bearerType));

				ret = read(0, bearerType, sizeof(bearerType));
				if (ret <= 0) {
					fprintf(stderr, " NULL msg can NOT be sent \n");
					return -1;
				}
				fprintf(stderr, "Set Preferred Bearer : bearerType =%d \n", atoi(bearerType));
				TapiSetPreferredBearer(atoi(bearerType));

				break;
			}
		case 4: //Set Memory Full Notification
			{
				fprintf(stderr, "Request  Set Memory Full  Notification:\n");
				TapiSetNetTextMemoryFull();
				break;
			}
		case 5: //Set Deliver Report
			{
				fprintf(stderr, "Set Deliver Report\n");
				EncodeSmsDeliverReportTpdu();
				break;
			}
		case 6: //Set CB Enable/Disable
			{
				fprintf(stderr, "Enter CB ON/OFF (1: Enable, 2:Disable):\n");

				memset(cbEnable,0 ,sizeof(cbEnable));
				pCBConfig = malloc(sizeof(TapiNetTextCBConfig));

				ret = read(0, cbEnable, sizeof(cbEnable));

				if (ret <= 0) {
					fprintf(stderr, " NULL msg can NOT be sent \n");
					return -1;
				}

				pCBConfig->CBEnabled = atoi(cbEnable);
				pCBConfig->selectedId = 0x01;
				pCBConfig->nMsgIdCount = 0x02;

				pCBConfig->MsgIDs[0] = 0x0c;
				pCBConfig->MsgIDs[1] = 0x0c;
				pCBConfig->MsgIDs[2] = 0xdf;
				pCBConfig->MsgIDs[3] = 0x00;

				TapiSetCBConfig(pCBConfig);

				free(pCBConfig);
				break;
			}
		case 7: //Set Stored MsgStaus
			{
				memType=0x02;
				index=0x02;
				msgStauts =0x02;

				TapiReadNetText(index, &CurMsgStauts, &DataPackage);

				fprintf(stderr, "Before set status : index = %d, CurMsgStauts=%d\n");

				TapiSetMsgStatus(memoryType,  index,  msgStauts);

				TapiReadNetText(index, &CurMsgStauts, &DataPackage);

				fprintf(stderr, "After set status :index = %d, CurMsgStauts=%d\n");

				break;
			}
		case 8:
			return;
	}
	return 1;
}

int get_handle()
{
	_handle = TapiRegisterClient( TAPI_NETTEXT_SMS , tapi_callback, TRUE);
	if(!TapiRegisterNetText()) fprintf(stderr, "register Failure!\n");

	if(_handle == NULL)
	{
		fprintf(stderr, "handle get error\n");
		return -1;
	}
}

int read_phoneserver_pkt(void)
{
	lxt_callback(_handle);
	return 0;
}
int read_key_input(void)
{

	int 	ret;
	char 	buf[512];

	//initialize value
	memset(buf,0, sizeof(buf));

	ret = read(0, buf, sizeof(buf));
	if (ret < 0)
	{
		if (errno == EINTR)
			perror("read(1)");
		return -1;
	}
	else if (ret == 0)
		return ret;

	fprintf(stderr, "*************************************\n");
	fprintf(stderr, "1.Send New Message\n");
	fprintf(stderr, "2.Read Message\n");
	fprintf(stderr, "3.Delete Message\n");
	fprintf(stderr, "4.Delete All Message\n");
	fprintf(stderr, "5.Setting\n");
	fprintf(stderr, "6.SMS Information\n");
	fprintf(stderr, "7.SMS Exit\n");
	fprintf(stderr, "*************************************\n");
	fprintf(stderr, "Select Number : %s\n", buf);

	if( memcmp(buf,"1",sizeof("1")-1) == 0 ){ SendMessage();}
	if( memcmp(buf,"2",sizeof("2")-1) == 0 ){ ReadMessage();}
	if( memcmp(buf,"3",sizeof("3")-1) == 0 ){ DeleteMessage();}
	if( memcmp(buf,"4",sizeof("4")-1) == 0 ){ TapiDeleteNetText(0xFF);}
	if( memcmp(buf,"5",sizeof("5")-1) == 0 ){ Setting();}
	if( memcmp(buf,"6",sizeof("6")-1) == 0 ){ GetInformation();}

	if( memcmp(buf,"7",sizeof("7")-1) == 0 ){
		fprintf(stderr, "SMS Program is terminated.\n");
		exit(0);
	}

	return 0;
}

void select_loop(void)
{
	int ret;
	fd_set readfds;
	int fd=_handle->fd;
	char buf[512];

	while (1) {
		FD_ZERO(&readfds);
		FD_SET(fd, &readfds);
		FD_SET(0, &readfds);


		ret = select(fd + 1, &readfds, NULL, NULL, NULL);
		if (ret) {
			if (FD_ISSET(fd, &readfds)) {
				read_phoneserver_pkt();
			}

			if (FD_ISSET(0, &readfds)) {
				read_key_input();

			}

		}
	}
}

int main(int argc, char *argv[])
{

	char buf[1000];
	int ret=0;

	ret=get_handle();

	select_loop();


}

#endif /* 0 */
