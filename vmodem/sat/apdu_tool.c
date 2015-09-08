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

/******************************************************************************
 *	Includes
 *****************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "sat_values.h"
#include "sat_util.h"
#include "logmsg.h"

//
//#include <alp/telephony.h>
//#include <alp/textmgr.h>
//#include <alp/pddk/phone_driver.h>
//#include <gtk/gtk.h>


//#include "phone_driver_debug.h"
//


struct _VGSMApduFragment;


// SIMPLE-TLV data objects encoding function type
typedef int(VGSMApduEncodeSimpleTLVObjectFunc)(struct _VGSMApduFragment* encoderP, struct _VGSMApduFragment* fragmentP);


//////////////////////////////////////////////////////////////////////////
// Temporary (to remove when the converters are available

#define	chrAsterisk					0x002A
#define	chrPlusSign					0x002B
#define	chrCapital_W				0x0057
#define	chrQuestionMark				0x003F
#define	chrCircumflexAccent			0x005E
#define	chrDigitZero				0x0030
#define	chrNumberSign				0x0023


#define U8_LENGTH(c) \
    ((int)(c)<=0x7f ? 1 : \
     ((int)(c)<=0x7ff ? 2 : \
      ((int)(c)<=0xd7ff ? 3 : \
       ((int)(c)<=0xdfff || (int)(c)>0x10ffff ? 0 : \
	((int)(c)<=0xffff ? 3 : 4)\
       ) \
      ) \
     ) \
    )


/**
 * Append a code point to a string, overwriting 1 to 4 bytes.
 * The offset points to the current end of the string contents
 * and is advanced (post-increment).
 * "Unsafe" macro, assumes a valid code point and sufficient space in the string.
 * Otherwise, the result is undefined.
 *
 * @param s const unsigned char * string buffer
 * @param i string offset
 * @param c code point to append
 * @see U8_APPEND
 * @stable ICU 2.4
 */
#define U8_APPEND_UNSAFE(s, i, c) { \
    if((int)(c)<=0x7f) { \
	(s)[(i)++]=(unsigned char)(c); \
    } else { \
	if((int)(c)<=0x7ff) { \
	    (s)[(i)++]=(unsigned char)(((c)>>6)|0xc0); \
	} else { \
	    if((int)(c)<=0xffff) { \
		(s)[(i)++]=(unsigned char)(((c)>>12)|0xe0); \
	    } else { \
		(s)[(i)++]=(unsigned char)(((c)>>18)|0xf0); \
		(s)[(i)++]=(unsigned char)((((c)>>12)&0x3f)|0x80); \
	    } \
	    (s)[(i)++]=(unsigned char)((((c)>>6)&0x3f)|0x80); \
	} \
	(s)[(i)++]=(unsigned char)(((c)&0x3f)|0x80); \
    } \
}







///////////////////////////////////////////////////////////////////////////
// APDU Simple object encoders
// See chapter 12 of the 11.11 documentation

// DCS: Data Coding Scheme for UD
#define VGSM_SMS_DCS_MODE_MASK							0x0C	// 00001100
#define VGSM_SMS_DCS_MODE_7BIT							0x00	// 00000000
#define VGSM_SMS_DCS_MODE_8BIT							0x04	// 00000100
#define VGSM_SMS_DCS_MODE_UCS2BIT						0x08	// 00001000
#define VGSM_SMS_DCS_MODE_RESERVEDBIT					0x0C	// 00001100

#define VGSM_APDU_COMPREHENSION_REQUIRED_FLAG				0x80


// SIMPLE-TLV data objects count // TODO: enum last
#define VGSM_ADPU_SIMPLE_TLV_COUNT				0x3F

// say an object buffer is dynamically allocated (even if its size <= 4)
#define VGSM_APDU_DYNAMIC_BUFFER_FLAG			0x01

#define VGSM_MAX_DECODED_SIZE					256


// BER-TLV data objects: { count of simple objects, simple-tlv object1, simple-tlv object 2, ... }
typedef struct _VGSMApduFragment
{
    unsigned char					flags;			// internal flags
    unsigned char					tag;			// BER or SIMPLE object tag
    int							length;			// data length in bytes
    struct _VGSMApduFragment*	son;			// pointer to the first son
    struct _VGSMApduFragment*	lson;			// pointer to the last son
    struct _VGSMApduFragment*	next;			// pointer to the next brother
    union buffer
    {
	char*						dataP;			// data buffer
	unsigned char				data[4];		// for data size <= 4
    } buffer;
} VGSMApduFragment;

typedef struct _VGSMApduEncoder
{
    SatApduEncoderCallback			HLGetDataF;
    void*									HLDataP;
    char*									BERFormat;
    VGSMApduFragment						BERObject;
    int									BERFormatLength;
} VGSMApduEncoder;

#define prv_apdu_get_length_of_length(L) \
    ((L) >= 128 ? 2 : 1)


///////
static int alp_pdrv_charset_convert(const char* srcTextP, size_t srcBytes, unsigned char srcEncoding, char* dstTextP, size_t* dstBytes, unsigned char dstEncoding, const char* substitutionStr, size_t substitutionLen)
{
    int	err		= 0;
    /*
       size_t			srcSize	= srcBytes;
       int	err		= ALP_STATUS_OK;

       if (dstEncoding == srcEncoding)
       {

       memcpy(dstTextP, srcTextP, srcBytes);
     * dstBytes= srcBytes;
     return ALP_STATUS_OK;
     }

    // check if UTF8 charset correspond to the local charset
    if ((srcEncoding == VGSM_CHARSET_UTF8) || (dstEncoding == VGSM_CHARSET_UTF8))
    {
    const gchar *charset;

    // Get the current character encoding
    if (g_get_charset (&charset)==TRUE)  // g_get_charset seems to not work properly, always returns ANSI X3.4-1968
    {
    TRACE( "alp_pdrv_charset_convert: [%s]:", charset);
    }
    else
    TRACE("alp_pdrv_charset_convert: [%s] not UTF-8 ! :", charset);
    }


    // Converting charset
    if ((srcEncoding==VGSM_CHARSET_UCS2)&&(dstEncoding==VGSM_CHARSET_UTF8))
    {
    err=alp_txt_utf8_from_ucs2((unsigned char*)srcTextP, srcBytes, (unsigned char *)dstTextP, *dstBytes, dstBytes, 0);
    }

    else if ((srcEncoding==VGSM_CHARSET_UTF8)&&(dstEncoding==VGSM_CHARSET_UCS2))
    {
    err=alp_txt_utf8_to_ucs2((unsigned char*)srcTextP, srcBytes, (unsigned char *)dstTextP, *dstBytes, dstBytes, 0);
    }

    else if ((srcEncoding==VGSM_CHARSET_UTF8)&&(dstEncoding==VGSM_CHARSET_GSM))
    {
    alp_txt_utf8_to_gsm((char*)srcTextP, srcBytes, (char*)dstTextP, *dstBytes, dstBytes,0);

    }

    else if ((srcEncoding==VGSM_CHARSET_GSM)&&(dstEncoding==VGSM_CHARSET_UTF8))
    {
    err=alp_txt_utf8_from_gsm((char*)srcTextP, srcBytes, (char*)dstTextP, *dstBytes, dstBytes,0);
    }

    else if ((srcEncoding==VGSM_CHARSET_GSM)&&(dstEncoding==VGSM_CHARSET_UCS2))
    {

    size_t dstSize = *dstBytes;
    unsigned char * intermediateTextP= malloc(dstSize);

    //first convert GSM character into UTF8 character
    err=alp_txt_utf8_from_gsm((char*)srcTextP, srcBytes, intermediateTextP, dstSize, &dstSize,0);

    //then convert UTF8 character into UCS2 character
    err=alp_txt_utf8_to_ucs2(intermediateTextP, dstSize, (unsigned char *)dstTextP, *dstBytes, dstBytes, 0);

    if (intermediateTextP) free (intermediateTextP);

    }

    else
    {
    TRACE( "alp_pdrv_charset_convert: conversion not supported !");
    return 	ALP_STATUS_TEL_FEATURE_NOT_SUPPORTED;
    }

    TRACE( "alp_pdrv_charset_convert: Buffer after charset conversion, err[%lX], size[%lu]:", err, *dstBytes);
    */
	return err;
}

///////



#if 0
/*******************************************************************
 * PURPOSE:
 *
 * PROTOTYPE:
 * static int PrvGsmEncodeDCS7(unsigned char* Data,
 *			unsigned char* Text, int TextLen)
 *
 * PARAMETERS:
 * Data			IN		The 7bits packed buffer
 * Text			OUT		The 8bits unpacked buffer
 * TextLen		IN		The bytes count of Text
 *
 *******************************************************************/

static void prv_gsm_encode_dcs7(unsigned char* bufferP, int* bufferSize)
{
    unsigned char*	readP;
    unsigned char*	writeP;
    unsigned short	theWord = 0;
    unsigned char	n;
    int			i;
    int			newSize = 0;

    for (readP = bufferP, writeP = bufferP, i = 0; i < *bufferSize; i++, readP++)
    {
	n = i & 7;

	if (n == 0)
	{
	    *writeP = *readP;
	    *writeP &= 0x7F;
	}
	else
	{
	    theWord = (*readP & 0x7F) << (8 - n);
	    *writeP |= (theWord & 0x00FF);
	    *++writeP = theWord >> 8;
	}

	if (n != 7)
	    newSize++;
    }

    *bufferSize = newSize;
}
#endif

/*******************************************************************
 * PURPOSE:
 * Convert UTF8 Text to GSM/USCS2 etc
 *
 * PROTOTYPE:
 * static int prv_convert_utf8_text(PrvApduEncoderType* encoderP,
 *	PrvApduFragmentType* fragmentP)
 *
 * PARAMETERS:
 *
 * RETURNED:
 *	  error code
 *
 *******************************************************************/

static int prv_convert_utf8_text(VGSMApduEncoder* encoderP, VGSMApduFragment* fragmentP)
{
    char*			dcsP = fragmentP->buffer.dataP;
    unsigned char	destEncoding = VGSM_CHARSET_UTF8;
    int			packed = 1;
    //	int			bufferSize=0;
    //	char*			bufferP=0;
    int		err = 0;

    /*
       switch(*dcsP)
       {
       case SAT_RESP_TYPE_YES_NO_OPTION:
     *dcsP = VGSM_SMS_DCS_MODE_7BIT;
     break;

     case SAT_RESP_TYPE_UNPACKED_DIGITS_ONLY:
     case SAT_RESP_TYPE_UNPACKED_SMS_ALPHABET:
     *dcsP = VGSM_SMS_DCS_MODE_7BIT;
     destEncoding = VGSM_CHARSET_GSM;
     break;

     case SAT_RESP_TYPE_PACKED_DIGITS_ONLY:
     case SAT_RESP_TYPE_PACKED_SMS_ALPHABET:
     *dcsP = VGSM_SMS_DCS_MODE_8BIT;
     destEncoding = VGSM_CHARSET_GSM;
     packed = TRUE;
     break;

     case SAT_RESP_TYPE_UCS2_DIGITS_ONLY:
     case SAT_RESP_TYPE_UCS2_ALPHABET:
     *dcsP = VGSM_SMS_DCS_MODE_UCS2BIT;
     destEncoding = VGSM_CHARSET_UCS2;
     break;
     }

     if (destEncoding != VGSM_CHARSET_UTF8)
     {
     bufferSize = VGSM_APDU_CONVERTION_BUFFER_SIZE;
     bufferP = (char*) malloc(bufferSize);

     if (!bufferP)
     {
     err = -1;
     goto finalize;
     }

    // the coding scheme
    bufferP[0] = *dcsP;

    // convert from utf8 to GSM or UCS2
    if ((err = alp_pdrv_charset_convert((char*) (fragmentP->buffer.dataP + 1), (int) fragmentP->length - 1, VGSM_CHARSET_UTF8, (char*) bufferP + 1, &bufferSize, destEncoding, NULL, 0)) != TRUE)
    {
    free(bufferP);
    goto finalize;
    }

    // replace the old buffer
    free(fragmentP->buffer.dataP);
    fragmentP->length = bufferSize + 1;
    bufferP = realloc(bufferP, fragmentP->length);
    fragmentP->buffer.dataP = bufferP;

    // pack GSM
    if (packed)
    prv_gsm_encode_dcs7((unsigned char*)bufferP, &fragmentP->length);
    }
     */

    //temp..  only use UCS2BIT....
    *dcsP = VGSM_SMS_DCS_MODE_8BIT;
    destEncoding = VGSM_CHARSET_UTF8;
    packed = 0;

    //finalize:
    return err;
}


/*******************************************************************
 * PURPOSE:
 * Encode an apdu BER. Creates al TLV Objects and populate them.
 *
 * PROTOTYPE:
 *
 * PARAMETERS:
 *
 * RETURNED:
 *	  error code
 *
 *******************************************************************/

static int prv_apdu_encode_ber(VGSMApduEncoder* encoderP)
{
    int			err = 0;
    VGSMApduFragment*	fragmentP = &encoderP->BERObject;
    unsigned char			i;

    for (i = 0; i < encoderP->BERFormatLength && err == 0; i++)
    {
	// create a new fragment
	VGSMApduFragment* newFragmentP = (VGSMApduFragment*) malloc(sizeof(VGSMApduFragment));

	if (newFragmentP == NULL)
	{
	    TRACE(MSGL_VGSM_ERR, "newFragmentP is NULL\n");
	    err = -1;
	    goto finalize;
	}


	memset(newFragmentP, 0, sizeof(VGSMApduFragment));

	newFragmentP->tag = encoderP->BERFormat[i];

	// insert it as the last son of fragmentP
	if (!fragmentP->lson)
	    fragmentP->son = fragmentP->lson = newFragmentP;
	else
	    fragmentP->lson = fragmentP->lson->next = newFragmentP;



	// encode the simple-TLV object
	if (newFragmentP->tag < VGSM_ADPU_SIMPLE_TLV_COUNT)
	{
	    if (newFragmentP->tag == SAT_TEXT_STRING || newFragmentP->tag == SAT_ITEM ||newFragmentP->tag == SAT_ALPHA_IDENTIFIER ||newFragmentP->tag == SAT_SMS_TPDU ||newFragmentP->tag == SAT_ADDRESS)
		newFragmentP->flags |= VGSM_APDU_DYNAMIC_BUFFER_FLAG;

	    err = encoderP->HLGetDataF(newFragmentP->tag, (char *)&(newFragmentP->buffer.data[0]), &newFragmentP->length, encoderP->HLDataP);

	    //
	    TRACE(MSGL_VGSM_INFO, "Cnt is %d Tag is %x and len is %dand data is %s\n",i,newFragmentP->tag,newFragmentP->length,&(newFragmentP->buffer.data[0]));
	    //

	    switch(newFragmentP->tag)
	    {
		case SAT_COMMAND_DETAILS:
		    newFragmentP->tag = newFragmentP->buffer.data[3];	// get the CR flag
		    break;

		case SAT_TEXT_STRING: //temp .... only use UTF8
		    prv_convert_utf8_text(encoderP, newFragmentP);
		    break;

	    }
	}
	else
	{
	    TRACE(MSGL_VGSM_ERR, "newFragmentP->tag > VGSM_ADPU_SIMPLE_TLV_COUNT\n");
	    err = -1;
	}
    }

finalize:
    return err;
}

/*******************************************************************
 * PURPOSE:
 * Encode an object tag and length in hexadecimal string format
 *
 * PROTOTYPE:
 * static char* AdpuSPrintHeader(char* destP, PrvApduFragmentType* objectP)
 *
 * PARAMETERS:
 *
 * RETURNED:
 *	  error code
 *
 *******************************************************************/

static char* prv_adpu_sprint_header(char* destP, VGSMApduFragment* objectP)
{
    char*			writeP = destP;
    unsigned char	len = (unsigned char) objectP->length;

    writeP += sprintf(writeP, "%02X", objectP->tag | VGSM_APDU_COMPREHENSION_REQUIRED_FLAG);

    if (len >= 128)
	writeP += sprintf(writeP, "81");

    writeP += sprintf(writeP, "%02X", len);

    return writeP;
}


/*******************************************************************
 * PURPOSE:
 * Encode a TLV object body (bytes are converted to hexadecimal string)
 *
 * PROTOTYPE:
 * static char* AdpuSPrintBody(char* destP, PrvApduFragmentType* objectP)
 *
 * PARAMETERS:
 *
 * RETURNED:
 *	  error code
 *
 *******************************************************************/

static char* prv_adpu_sprint_body(char* destP, VGSMApduFragment* objectP)
{
    char*	writeP = destP;
    unsigned char*	readP;
    unsigned char i;
    unsigned char len = (unsigned char) objectP->length;


    if (objectP->length > 4 || (objectP->flags & VGSM_APDU_DYNAMIC_BUFFER_FLAG) != 0)
	readP = (unsigned char *)objectP->buffer.dataP;
    else
	readP = objectP->buffer.data;



    for(i = 0; i < len; i++)
	writeP += sprintf(writeP, "%02X", readP[i]);

    return writeP;
}

/*******************************************************************
 * PURPOSE:
 * Create a new Apdu encoder object
 *
 * PROTOTYPE:
 * extern int PDrvCatApduEncode(unsigned char tag, PDrvCatApduEncoderCallbackType* getDataF,
 *			void* HLDataP, char** stringPP)
 *
 * PARAMETERS:
 * tag				IN			The tag of the BER object to encode (example: SAT_MENU_SELECTION)
 * getDataF			IN			A callback called when the encoder needs data.
 *								The encoder will ask for data using kAlpPDrvCatGetDataXXXX constants.
 *								ex. SAT_ITEMIdentifier
 * HLDataP			IN			structure that will be passed to the callback. Generally, it's
 *								a telephony structure related to the BER object to encode
 *								(ex. a TelCatMenuSelectionType structure for the SAT_MENU_SELECTION
 *								BER object)
 * stringPP			OUT			The decoder returns a newly allocated string. This string is the
 *								encoded object. This string must be freed with 'free' by the caller
 *								of PDrvCatApduEncode.
 *
 * RETURNED:
 *	  error code
 *
 *******************************************************************/

int sat_apdu_encode(unsigned char tag, SatApduEncoderCallback getDataF, void* HLDataP, char** stringPP)
{
    int			err = 0;
    VGSMApduEncoder		encoder;
    VGSMApduFragment 	*TLVObjectP, *nextP;
    char					*hexP, *writeP;
    int					size;

    TRACE(MSGL_VGSM_INFO, "\n");

    // init the encoder
    memset(&encoder, 0, sizeof(VGSMApduEncoder));

    encoder.BERObject.tag = tag;   // ex)SAT_DISPLAY_TEXT
    encoder.HLGetDataF = getDataF; //ex)cmd_display_text_callback
    encoder.HLDataP = HLDataP; //ex)dp_text

    // get the format for the BER command (depending on the tag)
    err = (*(encoder.HLGetDataF))(SAT_GET_DATA_FORMAT, (char*) (&encoder.BERFormat), &encoder.BERFormatLength, encoder.HLDataP);

    if (err)
    {
	TRACE(MSGL_VGSM_ERR, "get the format for the BER command\n");
	goto finalize;
    }

    TRACE(MSGL_VGSM_INFO, "BERFormatLength=%d\n", encoder.BERFormatLength);




    /////////////////////////////////////////////////////////////////


    // encode BER object and TVL objects. TLV objects are assembled into a list
    err = prv_apdu_encode_ber(&encoder);
    if (err)
    {
	TRACE(MSGL_VGSM_ERR, "encode BER object and TVL objects. \n");
	goto finalize;
    }



    // compute length of the BER object   //get length
    encoder.BERObject.length = 0;
    for (TLVObjectP = encoder.BERObject.son; TLVObjectP != NULL; TLVObjectP = TLVObjectP->next)
	encoder.BERObject.length += TLVObjectP->length /* length of the object */+ prv_apdu_get_length_of_length(TLVObjectP->length) /* 1 or 2 */ + 1 /* tag */;


    // alocate room for the whole object in ascii hex format (2 chars / byte) and a terminal 0.
    if (tag != SAT_RESPONSE)
	size=((encoder.BERObject.length + prv_apdu_get_length_of_length(encoder.BERObject.length) /* 1 or 2 */ + 1 /* BER tag */) * 2 + 1);
    else size=((encoder.BERObject.length) * 2 + 1); //no BERObject header for SAT_RESPONSE

    hexP = (char*) malloc(size);

    if (!hexP)
    {
	err = -1;
	TRACE(MSGL_VGSM_ERR, "\n!hexP\n");
	goto finalize;
    }

    //////////////////////////////////////////////////////////

    // print the BER object
    writeP = hexP;
    if (tag != SAT_RESPONSE)
	writeP = prv_adpu_sprint_header(writeP, &encoder.BERObject);

    // print TLV Objects
    for (TLVObjectP = encoder.BERObject.son; TLVObjectP != NULL; TLVObjectP = TLVObjectP->next)
    {
	TRACE(MSGL_VGSM_INFO, "TLVObjectP->length=%d\n", TLVObjectP->length);

	writeP = prv_adpu_sprint_header(writeP, TLVObjectP);
	writeP = prv_adpu_sprint_body(writeP, TLVObjectP);

    }

    //*writeP = 0;	// terminal 0  not useful since the sprintf terminate the string with '/0'
    *stringPP = hexP;


finalize:
    // delete temporary objects

    for (TLVObjectP = encoder.BERObject.son; TLVObjectP != NULL; )
    {

	if (TLVObjectP->length > 4 || (TLVObjectP->flags & VGSM_APDU_DYNAMIC_BUFFER_FLAG) != 0)
	{

	    if(TLVObjectP->tag == SAT_TEXT_STRING)
		if(TLVObjectP->buffer.dataP)
		    free(TLVObjectP->buffer.dataP);
	}

	nextP = TLVObjectP->next;

	if(TLVObjectP)
	    free(TLVObjectP);

	TLVObjectP = nextP;

    }


    return err;
}

/*******************************************************************
 * PURPOSE:
 * Read one or more bytes in an hex ascii stream
 *
 * PROTOTYPE:
 * int PrvReadApduByte(PDrvApduDecoderType* decoderP, unsigned char length,
 *					unsigned char* bufferToFillP)
 *
 * PARAMETERS:
 *	decoderP			IN/OUT	the apdu encoder object. It's updated to
 *								reflect read progress.
 *
 *	length				IN		Count of bytes to read
 *
 *	bufferToFillP		OUT		Buffer in which copy the data
 *
 * RETURNED:
 *	  error code
 *
 *******************************************************************/

static int prv_read_apdu_byte(SatApduDecoder* decoderP, unsigned char length, char* bufferToFillP)
{
    if (length > decoderP->apduSizeInBytes)
	length = decoderP->apduSizeInBytes;
    memcpy(bufferToFillP, decoderP->apduReadP, length);
    decoderP->apduReadP += length;
    decoderP->apduSizeInBytes -= length;
    return 0;
}


/*******************************************************************
 * PURPOSE:
 * Convert SMS 7bits packed to 8bits
 *
 * PROTOTYPE:
 * static int PrvGsmDecodeDCS7(unsigned char* Data, int DataTextLen,
 *			unsigned char* Text, int TextLen)
 *
 * PARAMETERS:
 * Data			IN		The 7bits packed buffer
 * Text			OUT		The 8bits unpacked buffer
 * TextLen		IN		The bytes count of Text
 *
 *******************************************************************/

static int prv_gsm_decode_dcs7(unsigned char* Data, char* Text, int TextLen)
{
    unsigned short	theIndex;
    unsigned short	theByte; // to hold left shifting
    unsigned short	value_for_test;

    TRACE(MSGL_VGSM_INFO, "\n");

    while (TextLen > 0)
    {
	theByte	= 0;
	for (theIndex	= 0; theIndex < 7 && TextLen > 0; theIndex++)	// there is 8 characters in seven bytes
	{

	    value_for_test= (unsigned short)(Data[theIndex] & 0x00FF);
	    theByte |= (value_for_test << theIndex);	// nextbyte most_significant to rest
	    *Text++ = (unsigned char) (theByte & 0x7F);							// take least_significant 7-bit
	    TRACE(MSGL_VGSM_INFO, "byte before=%x  byte after=%x, value_for_test=%x", (char)Data[theIndex],(char) (theByte & 0x7F), value_for_test);
	    TextLen--;
	    theByte >>= 7;												// rest shift-7-bit to least_significant
	}

	if (TextLen > 0)
	{
	    *Text++ = (char) theByte;
	    TextLen--;
	}
	Data += 7;
    }

    return 0;
}

/*******************************************************************
 * PURPOSE:
 * Implement the TS11.11 Annex B UCS2 text conversion
 *
 * PROTOTYPE:
 * static int PrvAlphaFieldsUCS2Convert(char* srcP,
 *			int srcSize, char* dstP, int* dstSize)
 *
 * PARAMETERS:
 *	srcP			IN		The source string
 *  srcSize			IN		The source string size
 *  dstP			OUT		The destination string
 *  dstSize			IN/OUT	The destination string size/real size
 *
 *******************************************************************/

static int prv_alpha_fields_ucs2_convert(char* srcP, int srcSize, char* dstP, int* dstSize)
{
    int	err			= 0;
    char		UCS2Type	= (char) (*srcP);
    char*		tmpString	= NULL;			// ucs2 temp string

    TRACE(MSGL_VGSM_INFO, "prv_alpha_fields_ucs2_convert: UCS2Type=%x",UCS2Type);

    if (UCS2Type == (char) 0x80)
    {
	// jump over the 0x80 code
	srcP++;
	srcSize--;

	TRACE(MSGL_VGSM_INFO, "no  GSM default alphabet characters");


	// remove terminal 0xFF
	while (srcSize > 0 && srcP[srcSize - 1] == (char) 0xFF)
	    srcSize--;

    }
    else
    {
	// we  make a valid ucs2 string in tmpString

	char			charsToProcess = srcP[1];			// length
	unsigned short	ucs2Base;							// ucs2 base to add to each char
	char*			writeP;								// the write pointer on tmpString

	if ((unsigned char) UCS2Type == (unsigned char) 0x81)
	{
	    ucs2Base = ((char) srcP[2]) << 7;					// bit 15-8 of the base
	    srcP += 3;									// jump over code, length, base
	}
	else // UCS2Type = 0x82
	{
	    ucs2Base = (srcP[2] << 8) + srcP[3];		// the base is coded on 2 bytes, MSB first
	    srcP += 4;									// jump over code, length, base
	}

	srcSize = charsToProcess * 2;					// allocate tmpString
	writeP = tmpString = (char*) malloc(srcSize);
	if (tmpString == NULL)
	{
	    err = -1;
	    goto finalize;
	}

	while (charsToProcess > 0)
	{
	    if ((*srcP & 0x80) == 0)
	    {
		// GSM alphabet: convert the character to an ucs2 one and add it to tmpString
		int ucs2Size = 2;

		err = alp_pdrv_charset_convert(srcP, 1, VGSM_CHARSET_GSM, writeP, (size_t *)&ucs2Size, VGSM_CHARSET_UCS2, NULL, 0);
		writeP += 2;
		TRACE(MSGL_VGSM_INFO, "convert GSM default alphabet characters (%x) into UCS2 character",srcP[0]);
	    }
	    else
	    {
		// UCS2 char: compute the ucs2 char and add it
		unsigned short ucs2Char = ucs2Base + (((unsigned short) (*srcP)) & 0x007F);
		*writeP++ = (char) ((ucs2Char & 0xFF00) >> 8);
		*writeP++ = (char) (ucs2Char & 0x00FF);
	    }
	    charsToProcess--;
	    srcP++;
	}
	srcP = tmpString;
    }

    err = alp_pdrv_charset_convert(srcP, srcSize, VGSM_CHARSET_UCS2, dstP, (size_t *)dstSize, VGSM_CHARSET_UTF8, NULL, 0);

    if (tmpString)
	free(tmpString);

finalize:
    return err;
}

/*******************************************************************
 * PURPOSE:
 * Make text conversion (typically, 7bits->8bits, GSM to current)
 *
 * PROTOTYPE:
 * void PrvConvertTextData(PDrvApduDecoderType* decoderP, unsigned char **dataP)
 *
 * PARAMETERS:
 *	decoderP			IN/OUT	the apdu encoder object
 *  dataP				IN/OUT	The initial buffer. In return, the converted buffer
 *
 *******************************************************************/

static void prv_convert_text_data(SatApduDecoder* decoderP, char** dataP)
{
    int	err = 0;
    char			DCS = (*dataP)[0] & VGSM_SMS_DCS_MODE_MASK;				// the codind scheme
    char*			encodedStringP = (*dataP) + 1;								// the encoded string
    int			encodedStringSize = decoderP->currentObjectLength - 1;		// the length of the encoded string
    int			decodedStringSize;
    char*			decodedStringP;
    int			bufferSize = 1024;	// we have a max of 274 chars. But we think about multibytes locale charset encoding
    char*			bufferP;


    if (DCS == VGSM_SMS_DCS_MODE_7BIT)
    {
	TRACE(MSGL_VGSM_INFO, "DCS = VGSM_SMS_DCS_MODE_7BIT");

	decodedStringSize = (encodedStringSize * 8) / 7;

	TRACE(MSGL_VGSM_INFO,  "decodedStringSize=%x encodedStringSize=%x",decodedStringSize,encodedStringSize);

	// Handle the edge case where 8n-1 chars were transmited (in this case, decodedStringSize is now 8n):
	// If the last char is a <cr>, it's just padding and we have to ignore it
	if ((decodedStringSize & 0x07) == 0 && (encodedStringP[encodedStringSize - 1] & 0xFE) == (0x0D << 1))
	    decodedStringSize--;	// ignore the last padding cr
	decodedStringP = malloc(decodedStringSize);
	err = prv_gsm_decode_dcs7((unsigned char*)encodedStringP, decodedStringP, decodedStringSize);
	free(*dataP);
	*dataP = decodedStringP;
    }
    else
    {
	memmove(*dataP, encodedStringP, encodedStringSize);
	decodedStringSize = encodedStringSize;
	decodedStringP = *dataP;
    }

    // convert from GSM or UCS2 to utf8 charset
    bufferP = (char*) malloc(bufferSize);
    if (!bufferP)
	bufferSize = 0;
    else
    {
	err = alp_pdrv_charset_convert((char*) decodedStringP, decodedStringSize, (DCS == VGSM_SMS_DCS_MODE_UCS2BIT) ? VGSM_CHARSET_UCS2 : VGSM_CHARSET_GSM, (char*) bufferP, (size_t *)&bufferSize, VGSM_CHARSET_UTF8, NULL, 0);
    }

    if (!err)
    {
	bufferP = (char*) realloc(bufferP, bufferSize + 1);
	bufferP[bufferSize] = 0;
	free(*dataP);
	*dataP = bufferP;
    }
    else
    {
	free(bufferP);
    }
}

/*******************************************************************
 * PURPOSE:
 * Make alpha identifier  conversion (typically, GSM to current)
 *
 * PROTOTYPE:
 * static void PrvConvertAlphaData(unsigned char tag, PDrvApduDecoderType* decoderP,
 *						unsigned char **dataP)
 *
 * PARAMETERS:
 *  tag					IN		the object TLV tag
 *	decoderP			IN/OUT	the apdu encoder object
 *  dataP				IN/OUT	The initial buffer. In return, the converted buffer
 *
 *******************************************************************/

static void prv_convert_alpha_data(unsigned char tag, SatApduDecoder* decoderP, char** dataP)
{
    int	err = 0;
    int		bufferSize = 1024;	// we have a max of 274 chars. But we think about multibytes locale charset encoding
    char*		bufferP;
    char*		encodedStringP = *dataP;								// the encoded string
    int		encodedStringSize = decoderP->currentObjectLength;		// the length of the encoded string

    bufferP = (char*) malloc(bufferSize);

    if (tag == SAT_ITEM)
    {
	// there is 1 byte for itemId
	*bufferP++ = *encodedStringP++;
	encodedStringSize--;
	bufferSize--;
    }

    if (((unsigned char)encodedStringP[0] >= (unsigned char) 0x80) && ((unsigned char) encodedStringP[0] <= (unsigned char) 0x82))
    {
	// UCS2 Annex B TS 11.11
	err = prv_alpha_fields_ucs2_convert((char*) encodedStringP, encodedStringSize, (char*) bufferP, &bufferSize);
    }
    else
    {
	// GSM charset
	err = alp_pdrv_charset_convert((char*) encodedStringP, encodedStringSize, VGSM_CHARSET_GSM, (char*) bufferP,(size_t *)&bufferSize, VGSM_CHARSET_UTF8, NULL, 0);
    }

    if (tag == SAT_ITEM)
    {
	bufferP--;
	bufferSize++;
    }

    if (!err)
    {
	bufferP = (char*) realloc(bufferP, bufferSize + 1);
	bufferP[bufferSize] = 0;

	free(*dataP);
	*dataP = bufferP;
    }
    else
    {
	free(bufferP);
    }
    TRACE(MSGL_VGSM_INFO, "done");

}

/*******************************************************************
 * PURPOSE:
 * Make dialing number/address conversion
 *
 * PROTOTYPE:
 * int PrvConvertDialingNumberData(unsigned char tag,
 *					PDrvApduDecoderType* decoderP, unsigned char **dataP)
 *
 * PARAMETERS:
 *  tag					IN		the object TLV tag
 *	decoderP			IN/OUT	the apdu encoder object
 *  dataP				IN/OUT	The initial buffer. In return, the converted buffer
 *
 *******************************************************************/


static void prv_convert_dialing_number_data(unsigned char tag, SatApduDecoder* decoderP, char** dataP)
{
    //int	err = 0;
    int		bufferSize = 0;
    char*		bufferP;
    char*		encodedStringP = *dataP;								// the encoded string
    int		encodedStringSize = decoderP->currentObjectLength;		// the length of the encoded string
    int	oneChar;
    char		oneNibble;
    char		nibbleIndex;

    bufferP = (char*) malloc(VGSM_MAX_DECODED_SIZE);

    if ((tag == SAT_ADDRESS)|| (tag==SAT_SS_STRING) || (tag == SAT_SUBADDRESS))
    {
	// handle the TON and NPI
	if ((*encodedStringP & 0x70) == 0x10)
	    U8_APPEND_UNSAFE(bufferP, bufferSize, chrPlusSign);

	encodedStringP++;
	encodedStringSize--;
    }

    if (tag == SAT_USSD_STRING)
    {
	//bypass the data coding scheme
	encodedStringP++;
	encodedStringSize--;
    }

    // read the address
    for(; (encodedStringSize > 0) && (bufferSize < VGSM_MAX_DECODED_SIZE); encodedStringSize--, encodedStringP++)
    {
	// read the digit (BCD nibble)
	for(nibbleIndex = 0; nibbleIndex < 2; nibbleIndex++)
	{
	    if (nibbleIndex == 0)
		oneNibble = (unsigned char)(*encodedStringP & 0x0F);
	    else
		oneNibble = (unsigned char)((*encodedStringP >> 4)& 0x0F);

	    // Convert the digit to Platform character
	    switch(oneNibble)
	    {
		case 0x0A:
		    oneChar = chrAsterisk;
		    break;
		case 0x0B:
		    oneChar = chrNumberSign;
		    break;
		case 0x0C:
		    // DTMF Control Digits Separator
		    oneChar = chrCapital_W;
		    break;
		case 0x0D:
		    // Wild character
		    oneChar = chrQuestionMark;
		    break;
		case 0x0E:
		    // Expansion digit
		    oneChar = chrCircumflexAccent;
		    break;
		case 0x0F:
		    // Endmark
		    goto END;
		default:
		    oneChar = chrDigitZero + (int)oneNibble;
		    break;
	    }
	    // Append the new character to result
	    if (bufferSize + U8_LENGTH(oneChar) < VGSM_MAX_DECODED_SIZE)
		U8_APPEND_UNSAFE(bufferP, bufferSize, oneChar);
	}
    }

END:
    //if (!err)
    {
	bufferP = (char*) realloc(bufferP, bufferSize + 1);
	bufferP[bufferSize] = '\0';
	free(*dataP);
	*dataP = bufferP;
	TRACE(MSGL_VGSM_INFO, "address decoded");


	decoderP->currentObjectLength= bufferSize+1;

    }
#if 0
    else
    {
	free(bufferP);
    }
#endif

}



/*******************************************************************
 * PURPOSE:
 * decode a BRV or TLV object
 *
 * PROTOTYPE:
 * static int	PrvDecodeObject(PDrvApduDecoderType* decoderP)
 *
 * PARAMETERS:
 *	decoderP			IN/OUT	the apdu encoder object. It's updated to
 *								reflect read progress.
 * RETURNED:
 *	  error code
 *
 *******************************************************************/

/* UNUSED */ int prv_decode_object(SatApduDecoder* decoderP);
int prv_decode_object(SatApduDecoder* decoderP)
{
    unsigned char	tag;
    char*			dataP;
    int		err = 0;

    tag = decoderP->currentObjectTag;
    if (tag >= 0x81 && tag <= 0xBE)		// take care about the CR flag
	tag &= 0x7F;


    // 0x01< TLV tag < 0x70              0xD0 < BER tag < 0xDF
    if ((tag >= 0x01 && tag <= 0x70) || ((tag >= 0xD0 && tag <= 0xDF)))
    {
	// supported tags
	if (decoderP->formatP == NULL || strchr((char*)decoderP->formatP, (char)tag))
	{
	    // if tag is referenced: call the callback



	    if ((tag >= 0xD0 && tag <= 0xDF) || decoderP->currentObjectLength == 0)
	    {
		// BER Object: No data, or empty TLV object
		dataP = NULL;
	    }
	    else
	    {
		// non empty TLV Object: allocate the data buffer
		dataP = (char*) malloc(decoderP->currentObjectLength);
		if (!dataP)
		{
		    err = -1;
		    goto finalize;
		}
		// get the object bytes
		prv_read_apdu_byte(decoderP, decoderP->currentObjectLength, dataP);
		TRACE(MSGL_VGSM_INFO, "read apdu byte done");
	    }

	    if (dataP)
	    {
		// decode text string
		if (tag == SAT_TEXT_STRING || tag == SAT_DEFAULT_TEXT)
		    prv_convert_text_data(decoderP, &dataP);
		else if (tag == SAT_ALPHA_IDENTIFIER || tag == SAT_ITEM || tag == SAT_URL)
		    prv_convert_alpha_data(tag, decoderP, &dataP);
		else if (tag == SAT_DTMF_STRING || tag == SAT_ADDRESS || tag == SAT_SUBADDRESS || tag == SAT_SS_STRING || tag == SAT_USSD_STRING )
		    prv_convert_dialing_number_data(tag, decoderP, &dataP);
	    }

	    TRACE(MSGL_VGSM_INFO, "call the callback");

	    if (dataP!=NULL)
		TRACE(MSGL_VGSM_INFO, "%s %d\n", dataP , decoderP->currentObjectLength);

	    // call the callback
	    err = decoderP->HLSetDataF(tag, decoderP, dataP);

	    // free allocated data
	    if (dataP!=NULL) free(dataP);

	}
	else
	{
	    // pass the object (BER objects are not passed because we go into them to search for TLV objects)
	    if (tag >= 0x01 && tag <= 0x3E)		// Simple TLV Object
	    {
		decoderP->apduReadP += decoderP->currentObjectLength;
		decoderP->apduSizeInBytes -= decoderP->currentObjectLength;
	    }
	}
    }
    else
    {
	// unsupported tags
	err = -1;		// TODO: error "unsupported tag"
	goto finalize;
    }

finalize:
    return err;
}

/*******************************************************************
 * PURPOSE:
 * Create a new Apdu decoder object
 *
 * PROTOTYPE:
 * extern int PDrvCatApduDecode(unsigned char* apduP, int apduSizeInBytes,
 *			unsigned char* tags, PDrvCatApduDecoderCallbackType* setDataF, void* dataP)
 *
 * PARAMETERS:
 * apduP		IN		apdu in binary format
 *
 * apduSizeInBytes	IN	count of apdu bytes
 *
 * tags			IN		0 terminated array of tags. It's the set of tags of objects
 *						the caller cares about. The callback will be called each time
 *						an object tag in the adpu matches one of the tags in the array
 *
 * setDataF		IN		callback to call when a matching tag is found
 *
 * dataP		IN		The caller provides a structure that will be passed to
 *						the callback. The callback normally fills this structure
 *						with data found in objects.
 *
 * RETURNED:
 *	  error code
 *
 *******************************************************************/
/*
   int sat_apdu_decode(char* apduP, int apduSizeInBytes, char* tags, SatApduDecoderCallback setDataF, void* dataP)
   {
   SatApduDecoder	decoder;
   int			err = 0;

// init the decoder object
decoder.apduP = decoder.apduReadP = apduP;
decoder.apduSizeInBytes = apduSizeInBytes;
decoder.formatP = tags;
decoder.HLDataP = dataP;
decoder.HLSetDataF = setDataF;

// read objects one after one
while (decoder.apduSizeInBytes > 0 && err == 0)
{
// read the tag byte
if ((err = prv_read_apdu_byte(&decoder, 1, &decoder.currentObjectTag)) != 0)
goto finalize;

TRACE( "sat_apdu_decode: decoder.currentObjectTag=%d", decoder.currentObjectTag);
//TRACE( (char*)TLVObjectP->buffer.data , TLVObjectP->length));

// read the object size
if ((err = prv_read_apdu_byte(&decoder, 1, &decoder.currentObjectLength)) != 0)
goto finalize;

TRACE( "sat_apdu_decode: decoder.currentObjectLength=%ld", decoder.currentObjectLength);

if (decoder.currentObjectLength == 0x81)
{
if ((err = prv_read_apdu_byte(&decoder, 1, &decoder.currentObjectLength)) != 0)
goto finalize;
TRACE( "sat_apdu_decode: decoder.currentObjectLength=%d", decoder.currentObjectLength);

}

// decode the object
err = prv_decode_object(&decoder);
}

finalize:
TRACE( "sat_apdu_decode: error=%x",err);

return err;
}
 */
