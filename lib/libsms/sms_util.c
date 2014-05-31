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

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sms_header.h"
#include "sms_util.h"


void  reverse(char* x, int len)
{
    int i, j = len - 1;

    for (i = 0; i < j; i++)
    {
	int t = x[i];
	x[i] = x[j];
	x[j--] = t;
    }
}

char*  AcItoa(int n, char* str, int b)
{
    int i = 0;

    do
	str[i++] = "0123456789ABCDEF"[n%b];

    while ((n /= b) > 0);
    reverse(str, i);
    str[i] = '\0';

    return str;
}

int  AcToupper(int ch)
{
    return (('a' <= (ch) && (ch) <= 'z')? ((ch) - ('a'-'A')) : (ch));
}

char* SmsUtilUnpackGSM8Code( char* szData, const BYTE* pIn, int in_len )
{
    int i;

    for (i = 0; i < in_len; i ++) {  
	szData [i] = pIn [i];
    }  

    return szData;	
}


int SmsUtilUnpackGSMCode( char* szData, const BYTE* pIn, int in_len )
{
    int i;
    int pos = 0;
    int shift = 0;

    /* If the number of fill bits != 0, then it would cause an additional shift */
    /*
       if ( shift != 0 )
       pos = pos + 1;

       if ( shift ==7 )
       shift = 0;
     */

    for ( i = 0; i < in_len; i++, pos++ )
    {
	szData[i] = ( pIn[pos] << shift ) & 0x7F;

	if ( pos != 0 )
	{
	    /* except the first byte, a character contains some bits
	     ** from the previous byte.
	     */
	    szData[i] |= pIn[pos-1] >> (8-shift);
	}

	shift ++;

	if ( shift == 7 )
	{
	    shift = 0;

	    /* a possible extra complete character is available */
	    i++;
	    szData[i] = pIn[pos] >> 1;

	    if( szData[i] == 0 )
	    {
		/* this is the end of the input, quit */
		//		break;
	    }
	}
    }

    return i;
}

int SmsUtilpackGSM8Code( BYTE* pOut, const char* szData, int in_len )
{
    int i;
    int pos = 0;

    for (i = 0; i < in_len; i ++) {  
	pOut[i] = szData [i];
	pos++;
    }  

    return pos;	
}

int SmsUtilPackGSMCode( BYTE* pOut, const char* szData, int in_len )
{
    int i;
    int pos;
    int shift = 0;
    //shift = fill_bits;

    //  memset( out, 0, out_len_max );

    /* pack the ASCII characters
     */
    /*
       if ( shift == 7 )
       shift = 0;
     */

    for( pos = 0, i = 0; /*pos < out_len_max &&*/ i < in_len; pos++, i++ )
    {
	/* pack the low bits */
	pOut[pos] = szData[i] >> shift;

	if ( i + 1 < in_len )
	{
	    /* pack the high bits using the low bits of the next character */
	    pOut[pos] |= szData[i+1] << ( 7 - shift );

	    shift++;

	    if( shift == 7 )
	    {
		shift = 0;
		i++;
	    }
	}
    }

    /* done */
    return pos;
}


void SmsUtilConvertBCD2Digit( char* pDigits, char* pBCD, int digitLen )
{
    int		i, bcdLen;
    char	c[2];
    unsigned char	higher, lower;

    //SysDebug( ( MID_SMS, "__SmsConvertBCD2Digit: start.\n" ) );

    if ( pBCD == NULL || pDigits == NULL )
    {
	printf("__SmsConvertBCD2Digit: pBCD == NULL || pDigits == NULL. return.\n"  );
	return;
    }

    if ( digitLen == 0 )
    {
	//printf("__SmsConvertBCD2Digit: digitLen == 0. return.\n" );

	pDigits[0] = 0x00;

	return;
    }

    if ( digitLen % 2 )
	bcdLen = digitLen / 2 + 1;
    else
	bcdLen = digitLen / 2;

    memset( pDigits, 0, bcdLen * 2 );

    for ( i = 0; i < bcdLen; i++ )
    {
	lower = pBCD[i] & 0x0F;					// get low nibble

	if ( lower == 0x0A )
	    lower = '*';
	else if ( lower == 0x0B )
	    lower = '#';
	else if ( lower == 0x0C )
	    lower = 'p';			//DTMF Control pDigits seperator
	else if ( lower == 0x0F )
	    lower = 0;
	else
	{
	    AcItoa( lower, c, 16 );
	    lower = (char) AcToupper(c[0]);
	}

	higher = ( pBCD[i] >> 4 ) & 0x0F;			// get high nibble

	if ( higher == 0x0A )
	    higher = '*';		// =0x2A
	else if ( higher == 0x0B )
	    higher = '#';		// =0x23
	else if ( higher == 0x0C )
	    higher = 'p';		// =0x70, DTMF Control pDigits seperator
	else if ( higher == 0x0F ) // if higher semi-octet is 0x0F, filled bits.
	{
	    //higher = 0;
	    sprintf(pDigits + strlen(pDigits), "%c", lower);
	    pDigits[/*digitLen-1*/bcdLen*2-1] = '\0';

	    //printf("__SmsConvertBCD2Digit: pDigits [%s].\n", pDigits  );

	    return;
	}
	else
	{
	    AcItoa(higher, c, 16);
	    higher = (char) AcToupper(c[0]);
	}

	//sprintf(pDigits, "%s%c%c", pDigits, lower, higher);
	sprintf(pDigits + strlen(pDigits), "%c%c", lower, higher);
    }

    pDigits[digitLen] = '\0';

    //printf("__SmsConvertBCD2Digit: pDigits [%s].\n", pDigits  );
}


void SmsUtilConvertDigit2BCD( char* pBCD, char* pDigits, int digitLen )
{
    int	i, j, digit;

    unsigned char	higher, lower;

    if ( pBCD == NULL || pDigits == NULL )
	return;

    // 0123456789 -> 1032547698
    for ( i = 0, j = 0; i < digitLen; i = i + 2, j++ )
    {
	if ( pDigits[i] == '*' )
	    digit = 0x0A;
	else if ( pDigits[i] == '#' )
	    digit = 0x0B;
	else if ( AcToupper( pDigits[i] ) == 'P' )
	    digit = 0x0C;
	else
	    digit = (int) ( pDigits[i] - '0' );

	lower = digit & 0x0F;

	if ( digitLen != i + 1 )
	{
	    if ( pDigits[i+1] == '*' )
		digit = 0x0A;
	    else if ( pDigits[i+1] == '#' )
		digit = 0x0B;
	    else if ( AcToupper( pDigits[i+1] ) == 'P' )
		digit = 0x0C;
	    else
		digit = (int) ( pDigits[i+1] - '0' );

	    higher = digit & 0x0F;
	}
	else
	{
	    higher = 0xFF;
	}

	pBCD[j] = ( higher << 4 ) | lower;
    }
}


TmDateTime* SmsUtilDecodeTimeStamp(unsigned char * pTimeStamp, TmDateTime *tmDateTime )
{
    //TmDateTime tmDateTime;
    char szBuf[3];
    //UINT32	time;

    if ( pTimeStamp == NULL )
	return NULL;

    SmsUtilConvertBCD2Digit( szBuf, (char*) &pTimeStamp[0], 2 );
    tmDateTime->year = atoi( szBuf ) + 2000;
    if ( ( tmDateTime->year >= 1900 + MAX_YEAR )/* && ( tmDateTime->year < 2000 + BASE_YEAR )*/ )
	tmDateTime->year -= 100;
    SmsUtilConvertBCD2Digit( szBuf, (char*) &pTimeStamp[1], 2 );
    tmDateTime->month = atoi( szBuf );
    SmsUtilConvertBCD2Digit( szBuf, (char*) &pTimeStamp[2], 2 );
    tmDateTime->day = atoi( szBuf );
    SmsUtilConvertBCD2Digit( szBuf, (char*) &pTimeStamp[3], 2 );
    tmDateTime->hour = atoi( szBuf );
    SmsUtilConvertBCD2Digit( szBuf, (char*) &pTimeStamp[4], 2 );
    tmDateTime->minute = atoi( szBuf );
    SmsUtilConvertBCD2Digit( szBuf, (char*) &pTimeStamp[5], 2 );
    tmDateTime->second = atoi( szBuf );

    if ( ( tmDateTime->year < 1900 + BASE_YEAR ) || ( tmDateTime->year > 1900 + MAX_YEAR ) )
	tmDateTime->year = 1900 + BASE_YEAR;

    /*
       time = TmDateTimeToSeconds( &tmDateTime );
       if ( time > MAX_SECONDS )
       time = MAX_SECONDS;

     */

    return tmDateTime;
}

unsigned char* SmsUtilEncodeTimeStamp( TmDateTime* tmDateTime, unsigned char* pTimeStamp )
{
    char szBuf[3];
    int	year;

    if ( pTimeStamp == NULL )
	return NULL;

    memset( (void*) pTimeStamp, 0x00, sizeof ( unsigned char ) * 7 );

    year = tmDateTime->year - 2000;
    if ( year < 0 )
	year += 100;

    sprintf( szBuf, "%02d", year );
    SmsUtilConvertDigit2BCD( (char*) &pTimeStamp[0], szBuf, 2 );
    sprintf( szBuf, "%02d", tmDateTime->month );
    SmsUtilConvertDigit2BCD( (char*) &pTimeStamp[1], szBuf, 2 );
    sprintf( szBuf, "%02d", tmDateTime->day );
    SmsUtilConvertDigit2BCD( (char*) &pTimeStamp[2], szBuf, 2 );
    sprintf( szBuf, "%02d", tmDateTime->hour );
    SmsUtilConvertDigit2BCD( (char*) &pTimeStamp[3], szBuf, 2 );
    sprintf( szBuf, "%02d", tmDateTime->minute );
    SmsUtilConvertDigit2BCD( (char*) &pTimeStamp[4], szBuf, 2 );
    sprintf( szBuf, "%02d", tmDateTime->second );
    SmsUtilConvertDigit2BCD( (char*) &pTimeStamp[5], szBuf, 2 );

    sprintf( szBuf, "%02d", 0x00 );
    SmsUtilConvertDigit2BCD( (char*) &pTimeStamp[6], szBuf, 2 );

    return pTimeStamp;
}

void  SmsUtilDecodeAddrField_sca(char *diallingNum, unsigned char* pAddrField, int *result_ton, int *result_npi )
{
    int index = 0;
    int  ton,npi;
    int dialnumLen=0;
    int length = 0;


    ton = ( pAddrField[index+1] & 0x70 ) >> 4;
    npi = pAddrField[index+1] & 0x0F;

    if ( ton != SIM_TON_ALPHA_NUMERIC )
    {
	// Origination/Destination address field's length is the real address length.
	// The number type/plan field is '0xFF' even the origination/destination address field's length is zero.
	//dialnumLen = pAddrField[index++];
	length = pAddrField[index];
	// the service center address could be null when the message doesn't be sent.
	// In this case,
	// when the length is 0, there is no number type and plan,
	// when the length is 1, there are the only type and plan.
	if ( length > 1 )
	{
	    dialnumLen = (int)( ( pAddrField[index++] - 1 ) * 2); // -1 is for TON/API field.
	}
    }
    else
    {
	dialnumLen = (int)( ( ( ( pAddrField[index++] + 1 ) / 2 ) * 8 ) / 7);
    }

    if ( dialnumLen > SIM_SMSP_ADDRESS_LEN )
    {
	dialnumLen = SIM_SMSP_ADDRESS_LEN;
    } else if ( dialnumLen < 0 ) {
	dialnumLen = 0;
    }

    //printf(" dialnumLen = %d\n", dialnumLen  );

    index++; /* ignore Type of Address field */

    if (ton != SIM_TON_ALPHA_NUMERIC )
    {
	SmsUtilConvertBCD2Digit( (char*)diallingNum, (char*) &pAddrField[index],dialnumLen );
    }
    else
    {
	SmsUtilUnpackGSMCode( diallingNum, &pAddrField[index],dialnumLen );

    }


    //printf(  "__SmsDecodeAddrField: diallingNum [%s].\n", (char*) diallingNum  );

    *result_ton=ton;
    *result_npi=npi;
}


void  SmsUtilDecodeAddrField_dst(char *diallingNum, unsigned char* pAddrField, int *result_ton, int *result_npi )
{
    int index = 0;
    int  ton,npi;
    int dialnumLen=0;

    ton = ( pAddrField[index+1] & 0x70 ) >> 4;
    npi = pAddrField[index+1] & 0x0F;

    if ( ton != SIM_TON_ALPHA_NUMERIC )
    {
	dialnumLen = pAddrField[index++];
    }
    else
    {
	dialnumLen = ( ( ( pAddrField[index++] + 1 ) / 2 ) * 8 ) / 7;
    }

    if ( dialnumLen > SIM_SMSP_ADDRESS_LEN )
    {
	dialnumLen = SIM_SMSP_ADDRESS_LEN;
    }

    //printf(" dialnumLen = %d\n", dialnumLen  );

    index++; /* ignore Type of Address field */

    if (ton != SIM_TON_ALPHA_NUMERIC )
    {
	SmsUtilConvertBCD2Digit( (char*)diallingNum, (char*) &pAddrField[index],dialnumLen );
    }
    else
    {
	SmsUtilUnpackGSMCode( diallingNum, &pAddrField[index],dialnumLen );

    }

    *result_ton=ton;
    *result_npi=npi;
}

int SmsReadSMSCfromSIM(unsigned char* pAddrFiled)
{
    int index = 0;
    int i;

    pAddrFiled[index++] = 0x7; //there are 7 octets following
    pAddrFiled[index++] = 0x91; //internatial format
    for(i=0; i<6; i++)
	pAddrFiled[index++]=0x11; // SMSC number stored in SIM is 111111111111

    return index;	
}

int  SmsUtilEncodeAddrField_sca(unsigned char* pAddrField, unsigned char* diallingNum, int dialnumLen, int ton, int npi)
{
    int index = 0;

    if ( diallingNum == NULL || pAddrField == NULL )
	return -1;

    if ( ton != SIM_TON_ALPHA_NUMERIC )
    {
	if ( dialnumLen % 2 )
	{
	    pAddrField[index++] = dialnumLen / 2 + 1 + 1; 
	}
	else
	{
	    pAddrField[index++] = dialnumLen / 2 + 1; 
	}
    }
    else
    {
	pAddrField[index] = (unsigned char) ( ( ( dialnumLen * 7 + 7 ) / 8 ) * 2 );

	if ( ( ( dialnumLen * 7 ) % 8 ) <= 4 )
	    pAddrField[index]--;

	printf(" addr len packet: %d out of SIM_TON_ALPAHA\n", pAddrField[index]);

	index++;
    }

    SET_TON_NPI( pAddrField[index], ton, npi );

    index++; 

    if ( ton != SIM_TON_ALPHA_NUMERIC )
    {
	SmsUtilConvertDigit2BCD( (char*) &pAddrField[index], (char*) diallingNum, dialnumLen );

	if ( dialnumLen % 2 )
	    index += dialnumLen / 2 + 1;
	else
	    index += dialnumLen / 2;
    }
    else
    {
	index += SmsUtilPackGSMCode( &pAddrField[index], (char *)diallingNum, (int) dialnumLen );
    }

    return index;
}

int  SmsUtilEncodeAddrField_dst(unsigned char* pAddrField, unsigned char* diallingNum, int dialnumLen, int ton, int npi )
{
    int index = 0;

    if ( diallingNum == NULL || pAddrField == NULL )
	return -1;

    if ( ton != SIM_TON_ALPHA_NUMERIC )
    {
	pAddrField[index++] = (unsigned char)dialnumLen;
	//printf(" addr len packet: %d\n", pAddrField[index]);
    }
    else
    {
	pAddrField[index] = (unsigned char) ( ( ( dialnumLen * 7 + 7 ) / 8 ) * 2 );

	// On last byte, if the high 4 bit doesn't be used, then do -1 to the length field.
	if ( ( ( dialnumLen * 7 ) % 8 ) <= 4 )
	    pAddrField[index]--;

	printf(" addr len packet: %d out of SIM_TON_ALPAHA\n", pAddrField[index]);

	index++;



    }

    SET_TON_NPI( pAddrField[index], ton, npi );
    index++; // SET_TON_NPI is a MACRO, so we couldn't use like 'pAddrField[index++]'.

    if ( ton != SIM_TON_ALPHA_NUMERIC )
    {
	SmsUtilConvertDigit2BCD( (char*) &pAddrField[index], (char*) diallingNum, dialnumLen );

	if ( dialnumLen % 2 )
	    index += dialnumLen / 2 + 1;
	else
	    index += dialnumLen / 2;
    }
    else
    {
	index += SmsUtilPackGSMCode( &pAddrField[index], (char *)diallingNum, (int) dialnumLen );
    }

    return index;
}

int  SmsUtilEncodeAddrField(unsigned char* pAddrField, unsigned char* diallingNum, int dialnumLen, int ton, int npi )
{
    int index = 0;

    if ( diallingNum == NULL || pAddrField == NULL )
	return -1;

    if ( ton != SIM_TON_ALPHA_NUMERIC )
    {
	pAddrField[index++] = (unsigned char)dialnumLen;
	//printf(" addr len packet: %d\n", pAddrField[index]);
    }
    else
    {
	pAddrField[index] = (unsigned char) ( ( ( dialnumLen * 7 + 7 ) / 8 ) * 2 );

	if ( ( ( dialnumLen * 7 ) % 8 ) <= 4 )
	    pAddrField[index]--;

	printf(" addr len packet: %d out of SIM_TON_ALPAHA\n", pAddrField[index]);

	index++;



    }

    SET_TON_NPI( pAddrField[index], ton, npi );
    index++;

    if ( ton != SIM_TON_ALPHA_NUMERIC )
    {
	SmsUtilConvertDigit2BCD( (char*) &pAddrField[index], (char*) diallingNum, dialnumLen );

	if ( dialnumLen % 2 )
	    index += dialnumLen / 2 + 1;
	else
	    index += dialnumLen / 2;
    }
    else
    {
	index += SmsUtilPackGSMCode( &pAddrField[index], (char *)diallingNum, (int) dialnumLen );
    }

    return index;
}
int SmsUtilDecodeScAddrField( SmsAddressInfo* pSmsAddrField, BYTE* pAddrField )
{
    printf("SmsUtilDecodeScAddrField\n");
    int index = 0;
    int length = 0;

    //SysDebug( ( MID_SMS, "__SmsDecodeScAddrField: start.\n" ) );

    if ( pSmsAddrField == NULL || pAddrField == NULL )
    {
	printf( "SmsUtilDecodeScAddrField: pSimAddrField or pAddrField is NULL.\n"  );

	return 0;
    }

    length = pAddrField[index];

    if ( length > 1 )
    {
	pSmsAddrField->dialnumLen = ( pAddrField[index++] - 1 ) * 2; // -1Àis for TON/API field

	if ( pSmsAddrField->dialnumLen > SIM_SMSP_ADDRESS_LEN )
	{
	    pSmsAddrField->dialnumLen = SIM_SMSP_ADDRESS_LEN;
	}

	pSmsAddrField->ton = ( pAddrField[index] & 0x70 ) >> 4;
	pSmsAddrField->npi = pAddrField[index] & 0x0F;

	index++; /* ignore Type of Address field */

	SmsUtilConvertBCD2Digit( (char*) pSmsAddrField->diallingNum, (char*) &pAddrField[index], pSmsAddrField->dialnumLen );

	printf( "SmsUtilDecodeScAddrField: diallingNum [%s].\n", (char*) pSmsAddrField->diallingNum  );

	printf( "length=%d , ton %d, npi =%d\n",pSmsAddrField->dialnumLen, pSmsAddrField->ton,pSmsAddrField->npi );
    }

    return ++length;
}

int  SmsUtilEncodeScAddrField( BYTE* pAddrField, SmsAddressInfo * pSmsAddrField )
{
    int index = 0;

    if ( pSmsAddrField == NULL || pAddrField == NULL )
	return -1;

    if ( pSmsAddrField->dialnumLen % 2 )
    {
	pAddrField[index++] = pSmsAddrField->dialnumLen / 2 + 1 + 1; // +1 is for TON/NPI field, and if it's odd then +1 again.
    }
    else
    {
	pAddrField[index++] = pSmsAddrField->dialnumLen / 2 + 1; // +1 is for TON/NPI field
    }

    SET_TON_NPI( pAddrField[index], pSmsAddrField->ton, pSmsAddrField->npi );
    index++;

    SmsUtilConvertDigit2BCD( (char*) &pAddrField[index], (char*) pSmsAddrField->diallingNum, pSmsAddrField->dialnumLen );

    if ( pSmsAddrField->dialnumLen % 2 )
	index += pSmsAddrField->dialnumLen / 2 + 1;
    else
	index += pSmsAddrField->dialnumLen / 2;

    return index;
}

void SmsUtilDecodeDCS( TapiNetTextCodingScheme* pCodingScheme,   unsigned char dcs )
{
    assert( pCodingScheme != NULL );

    memset( pCodingScheme, 0, sizeof ( TapiNetTextCodingScheme ) );

    if ( dcs < 0x40 ) // bits 7..4 = 00xx : general data coding indication
    {
	pCodingScheme->codingGroupType = TAPI_NETTEXT_CODGRP_SM_GENERAL_DCS;

	if ( dcs & 0x20 ) // bit 5 = 1 : indicates the text is compressed
	    pCodingScheme->bCompressed = TRUE;

	if ( dcs & 0x10 ) // bit 4 = 1 : indicates that bits  1 to 0 have a message class meaning
	{
	    pCodingScheme->bMsgClassSet = TRUE;

	    switch ( dcs & 0x03 ) // bits 1 to 0 : message class
	    {
		case 0x00:
		    pCodingScheme->classType = TAPI_NETTEXT_CLASS_0;
		    break;
		case 0x01:
		    pCodingScheme->classType = TAPI_NETTEXT_CLASS_1;
		    break;
		case 0x02:
		    pCodingScheme->classType = TAPI_NETTEXT_CLASS_2;
		    break;
		case 0x03:
		    pCodingScheme->classType = TAPI_NETTEXT_CLASS_3;
		    break;
	    }
	}
	else // bit 4 = 0 : indicates that bits 1 to 0 are reserved and have no message class meaning
	    pCodingScheme->classType = TAPI_NETTEXT_CLASS_NONE;

	switch ( dcs & 0x0C ) // bits 4 to 3 : character set
	{
	    case 0x00:
		pCodingScheme->alphabetType = TAPI_NETTEXT_ALPHABET_DEFAULT;
		break;
	    case 0x04:
		pCodingScheme->alphabetType = TAPI_NETTEXT_ALPHABET_8BIT;
		break;
	    case 0x08:
		pCodingScheme->alphabetType = TAPI_NETTEXT_ALPHABET_UCS2;
		break;
	    case 0x0C:
		pCodingScheme->alphabetType = TAPI_NETTEXT_ALPHABET_MAX;
		break;
	}
    }
    else if ( dcs >= 0x40 && dcs < 0x80 ) // bits 7..4 = 01xx : message marked for automatic deletion group. bits 5..0 are coded exactly the same as group 00xx
    {
	pCodingScheme->codingGroupType = TAPI_NETTEXT_CODGRP_SM_AUTO_DELETION;

	if ( dcs & 0x20 ) // bit 5 = 1 : indicates the text is compressed
	    pCodingScheme->bCompressed = TRUE;

	if ( dcs & 0x10 ) // bit 4 = 1 : indicates that bits  1 to 0 have a message class meaning
	{
	    pCodingScheme->bMsgClassSet = TRUE;

	    switch ( dcs & 0x03 ) // bits 1 to 0 : message class
	    {
		case 0x00:
		    pCodingScheme->classType = TAPI_NETTEXT_CLASS_0;
		    break;
		case 0x01:
		    pCodingScheme->classType = TAPI_NETTEXT_CLASS_1;
		    break;
		case 0x02:
		    pCodingScheme->classType = TAPI_NETTEXT_CLASS_2;
		    break;
		case 0x03:
		    pCodingScheme->classType = TAPI_NETTEXT_CLASS_3;
		    break;
	    }
	}
	else // bit 4 = 0 : indicates that bits 1 to 0 are reserved and have no message class meaning
	    pCodingScheme->classType = TAPI_NETTEXT_CLASS_NONE;

	switch ( dcs & 0x0C ) // bits 4 to 3 : character set
	{
	    case 0x00:
		pCodingScheme->alphabetType = TAPI_NETTEXT_ALPHABET_DEFAULT;
		break;
	    case 0x04:
		pCodingScheme->alphabetType = TAPI_NETTEXT_ALPHABET_8BIT;
		break;
	    case 0x08:
		pCodingScheme->alphabetType = TAPI_NETTEXT_ALPHABET_UCS2;
		break;
	    case 0x0C:
		pCodingScheme->alphabetType = TAPI_NETTEXT_ALPHABET_MAX;
		break;
	}
    }
    // bits 7..4 = 1000 ~ 1011 : reserved
    else if ( dcs == 0xC0 ) // bits 7..4 = 1100 : message waiting indication group, discard message
    {
	pCodingScheme->codingGroupType = TAPI_NETTEXT_CODGRP_SM_WAITING_DISCARD;
    }
    else if ( dcs < 0xE0 )
    {
	pCodingScheme->codingGroupType = TAPI_NETTEXT_CODGRP_SM_WAITING_STORE;

	if ( dcs & 0x08 )
	    pCodingScheme->bMsgIndActive = TRUE;

	switch ( dcs & 0x03 )
	{
	    case 0x00:
		pCodingScheme->waitingType = TAPI_NETTEXT_WAITING_VOICE_MSG;
		break;
	    case 0x01:
		pCodingScheme->waitingType = TAPI_NETTEXT_WAITING_FAX_MSG;
		break;
	    case 0x02:
		pCodingScheme->waitingType = TAPI_NETTEXT_WAITING_EMAIL_MSG;
		break;
	    case 0x03:
		pCodingScheme->waitingType = TAPI_NETTEXT_WAITING_OTHER_MSG;
		break;
	}
    }
    else if ( dcs < 0xF0 )
    {
	pCodingScheme->codingGroupType = TAPI_NETTEXT_CODGRP_SM_WAITING_STORE_UCS2;

	if ( dcs & 0x08 )
	    pCodingScheme->bMsgIndActive = TRUE;

	switch ( dcs & 0x03 )
	{
	    case 0x00:
		pCodingScheme->waitingType = TAPI_NETTEXT_WAITING_VOICE_MSG;
		break;
	    case 0x01:
		pCodingScheme->waitingType = TAPI_NETTEXT_WAITING_FAX_MSG;
		break;
	    case 0x02:
		pCodingScheme->waitingType = TAPI_NETTEXT_WAITING_EMAIL_MSG;
		break;
	    case 0x03:
		pCodingScheme->waitingType = TAPI_NETTEXT_WAITING_OTHER_MSG;
		break;
	}
    }
    else
    {
	pCodingScheme->codingGroupType = TAPI_NETTEXT_CODGRP_SM_CLASS_CODING;

	if ( dcs & 0x04 )
	    pCodingScheme->alphabetType = TAPI_NETTEXT_ALPHABET_8BIT;

	switch ( dcs & 0x03 )
	{
	    case 0x00:
		pCodingScheme->classType = TAPI_NETTEXT_CLASS_0;
		break;
	    case 0x01:
		pCodingScheme->classType = TAPI_NETTEXT_CLASS_1;
		break;
	    case 0x02:
		pCodingScheme->classType = TAPI_NETTEXT_CLASS_2;
		break;
	    case 0x03:
		pCodingScheme->classType = TAPI_NETTEXT_CLASS_3;
		break;
	}
    }
}

void SmsUtilEncodeDCS( BYTE* pDCS, TapiNetTextCodingScheme* pCodingScheme )
{
    BYTE dcs = 0x00;

    assert( pCodingScheme != NULL );

    switch ( pCodingScheme->codingGroupType )
    {
	case TAPI_NETTEXT_CODGRP_SM_GENERAL_DCS: // bit 7..4 is 00xx
	    {
		if ( pCodingScheme->bCompressed )
		    dcs |= 0x20; // bit 5 is 1

		if ( pCodingScheme->bMsgClassSet )
		{
		    dcs |= 0x10; // bit 4 is 1

		    switch ( pCodingScheme->classType )
		    {
			case TAPI_NETTEXT_CLASS_0: // bit 1..0 is 00
			    {
				dcs |= 0x00;
				break;
			    }
			case TAPI_NETTEXT_CLASS_1: // bit 1..0 is 01
			    {
				dcs |= 0x01;
				break;
			    }
			case TAPI_NETTEXT_CLASS_2: // bit 1..0 is 10
			    {
				dcs |= 0x02;
				break;
			    }
			case TAPI_NETTEXT_CLASS_3: // bit 1..0 is 11
			    {
				dcs |= 0x03;
				break;
			    }
			default:
			    break;
		    }
		}

		switch ( pCodingScheme->alphabetType )
		{
		    case TAPI_NETTEXT_ALPHABET_DEFAULT: // bit 3..2 is 00
			{
			    dcs |= 0x00;
			    break;
			}
		    case TAPI_NETTEXT_ALPHABET_8BIT: // bit 3..2 is 01
			{
			    dcs |= 0x04;
			    break;
			}
		    case TAPI_NETTEXT_ALPHABET_UCS2: // bit 3..2 is 10
			{
			    dcs |= 0x08;
			    break;
			}
		    default: // bit 3..2 is 11
			{
			    dcs |= 0x0C;
			    break;
			}
		}

		break;
	    }
	case TAPI_NETTEXT_CODGRP_SM_WAITING_DISCARD: // bit 7..4 is 1100
	    {
		dcs |= 0xC0;

		break;
	    }
	case TAPI_NETTEXT_CODGRP_SM_WAITING_STORE: // bit 7..4 is 1101
	    {
		dcs |= 0xD0;

		if ( pCodingScheme->bMsgIndActive ) // bit 3..2 is 10
		    dcs |= 0x08;

		switch ( pCodingScheme->waitingType )
		{
		    case TAPI_NETTEXT_WAITING_VOICE_MSG: // bit 1..0 is 00
			{
			    dcs |= 0x00;
			    break;
			}
		    case TAPI_NETTEXT_WAITING_FAX_MSG: // bit 1..0 is 01
			{
			    dcs |= 0x01;
			    break;
			}
		    case TAPI_NETTEXT_WAITING_EMAIL_MSG: // bit 1..0 is 10
			{
			    dcs |= 0x02;
			    break;
			}
		    case TAPI_NETTEXT_WAITING_OTHER_MSG: // bit 1..0 is 11
			{
			    dcs |= 0x03;
			    break;
			}
		    default:
			break;
		}

		break;
	    }
	case TAPI_NETTEXT_CODGRP_SM_WAITING_STORE_UCS2: // bit 7..4 is 1110
	    {
		dcs |= 0xE0;

		if ( pCodingScheme->bMsgIndActive ) // bit 3..2 is 10
		    dcs |= 0x08;

		switch ( pCodingScheme->waitingType )
		{
		    case TAPI_NETTEXT_WAITING_VOICE_MSG: // bit 1..0 is 00
			{
			    dcs |= 0x00;
			    break;
			}
		    case TAPI_NETTEXT_WAITING_FAX_MSG: // bit 1..0 is 01
			{
			    dcs |= 0x01;
			    break;
			}
		    case TAPI_NETTEXT_WAITING_EMAIL_MSG: // bit 1..0 is 10
			{
			    dcs |= 0x02;
			    break;
			}
		    case TAPI_NETTEXT_WAITING_OTHER_MSG: // bit 1..0 is 11
			{
			    dcs |= 0x03;
			    break;
			}
		    default:
			break;
		}

		break;
	    }
	case TAPI_NETTEXT_CODGRP_SM_CLASS_CODING: // bit 7..4 is 1111
	    {
		dcs |= 0xF0;

		switch ( pCodingScheme->alphabetType )
		{
		    case TAPI_NETTEXT_ALPHABET_DEFAULT: // bit 2 is 0
			{
			    dcs |= 0x00;
			    break;
			}
		    case TAPI_NETTEXT_ALPHABET_8BIT: // bit 2 is 1
			{
			    dcs |= 0x04;
			    break;
			}
		    default:
			break;
		}

		switch ( pCodingScheme->classType )
		{
		    case TAPI_NETTEXT_CLASS_0: // bit 1..0 is 00
			{
			    break;
			}
		    case TAPI_NETTEXT_CLASS_1: // bit 1..0 is 01
			{
			    dcs |= 0x01;
			    break;
			}
		    case TAPI_NETTEXT_CLASS_2: // bit 1..0 is 10
			{
			    dcs |= 0x02;
			    break;
			}
		    case TAPI_NETTEXT_CLASS_3: // bit 1..0 is 11
			{
			    dcs |= 0x03;
			    break;
			}
		    default:
			break;
		}

		break;
	    }
	case TAPI_NETTEXT_CODGRP_SM_RESERVED: // bit 7..4 is 1111
	    {
		dcs = (pCodingScheme->codingGroup << 4) & 0xF0;
		dcs |= (pCodingScheme->code & 0x0F);
		break;
	    }
	default:
	    break;
    }

    memcpy( pDCS, &dcs, sizeof ( BYTE ) );
}

UINT8 SmsUtilEncodeValidity( BYTE* pValidity, TapiNetTextVP* pVP )
{
    UINT8	pos = 0;

    switch( pVP->vpType )
    {
	case TAPI_NETTEXT_VP_NOT_USED:
	    break;

	case TAPI_NETTEXT_VP_RELATIVE:
	    pValidity[pos] = (UINT8) pVP->vpValue;
	    pos ++;
	    break;

	case TAPI_NETTEXT_VP_ABSOLUTE:
	    //TO DO
	    //SmsUtilEncodeTimeStamp( pValidity, pVP->vpValue );
	    pos += 7;
	    break;

	case TAPI_NETTEXT_VP_ENHANCED:
	    break;
    }

    return pos;
}

