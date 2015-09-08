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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <sys/time.h>
#include "at_tx_func.h"

#include "sat_util.h"
#include "vgsm_sat.h"


char sMbiz_getInkeyRespType[4] =
{
    SAT_RESP_TYPE_UNPACKED_DIGITS_ONLY,
    SAT_RESP_TYPE_UNPACKED_SMS_ALPHABET,
    SAT_RESP_TYPE_UCS2_DIGITS_ONLY,
    SAT_RESP_TYPE_UCS2_ALPHABET
};

typedef struct MbizSetCmdResponseParamsTag
{
    SatCmdResponse*	responseP;
    void*					proactiveCmdP;
} MbizSetCmdResponseParamsType;

typedef struct MbizSetCmdResponseParamsTagInternal
{
    SatCmdResponseInternal*	responseP;
    void*					proactiveCmdP;
} MbizSetCmdResponseParamsTypeInternal;

char g_item_cnt;

char MbizPrvSetCmdResponseFormat[16] 	= { SAT_COMMAND_DETAILS, SAT_DEVICE_IDENTITIES, SAT_RESULT };

char	MenSelectionFormat[] 	= { SAT_DEVICE_IDENTITIES, SAT_ITEM_IDENTIFIER, SAT_HELP_REQUEST };

char	DisplayTextFormat[8] 	= { SAT_COMMAND_DETAILS, SAT_DEVICE_IDENTITIES, SAT_TEXT_STRING/*,SAT_ICON_IDENTIFIER,SAT_IMMEDIATE_RESPONSE, SAT_DURATION*/};

char	GetInkeyFormat[8] 	= { SAT_COMMAND_DETAILS, SAT_DEVICE_IDENTITIES, SAT_TEXT_STRING/*,SAT_ICON_IDENTIFIER,SAT_IMMEDIATE_RESPONSE, SAT_DURATION*/};

char	GetInputFormat[8] 	= { SAT_COMMAND_DETAILS, SAT_DEVICE_IDENTITIES, SAT_TEXT_STRING/*,SAT_ICON_IDENTIFIER,SAT_IMMEDIATE_RESPONSE, SAT_DURATION*/};

char	SetUpMenuFormat[8] 	= { SAT_COMMAND_DETAILS, SAT_DEVICE_IDENTITIES, SAT_ALPHA_IDENTIFIER,SAT_ITEM};

char	SendSMSFormat[8] 	= { SAT_COMMAND_DETAILS, SAT_DEVICE_IDENTITIES, SAT_ALPHA_IDENTIFIER,SAT_ADDRESS,SAT_SMS_TPDU};

char	SetUpCallFormat[8] 	= { SAT_COMMAND_DETAILS, SAT_DEVICE_IDENTITIES};

char	MbizPrvEventDownloadFormat[] 	= { SAT_EVENT_LIST, SAT_DEVICE_IDENTITIES, 0 };





/******************************************************************************
 * Function:	str_to_bin
 *
 * Description:	convert ASCII char to binary form(HEX)
 *
 * Parameters:	char* iStringP		input; string format
 *				char *oBinP		output; binary format
 ******************************************************************************/
static int str_to_bin(char* iStringP, char **oBinPP, int *oBinLenP)
{
    int 		stringLen, binLen;
    char *		binP = NULL;
    unsigned int	i;
    char * 		tmp = NULL;



    stringLen 	= (int)strlen((char *)iStringP);
    binLen 		= stringLen / 2;


    binP= (char *)malloc(binLen);
    if(binP == NULL)
    {
	TRACE(MSGL_VGSM_INFO, "memory allocation faillure");
	return -1;
    }
    memset(binP, 0, binLen);

    tmp = malloc(2 * sizeof(char));
    for(i=0;i<binLen;i++){
	memset(tmp, 0, 2);
	memcpy(tmp, &iStringP[i*2], 2);
	binP[i] = (char)strtol(tmp, NULL, 16);
    }

    if(tmp)
	free(tmp);

    // oBinPP's memory will be freed by the function which called str_to_bin().
    *oBinPP = (char *)binP;
    *oBinLenP = binLen;

    TRACE(MSGL_VGSM_INFO, "output binary is");


    return 0;
}


#if 0
/******************************************************************************
 * Function:	cmd_param_init
 *
 * Description:	Initialize a memory block for the sub-allocator
 *
 * Parameters:	paramsP		a param block, defining the location
 *							and the size of the block
 ******************************************************************************/

static int cmd_param_init(SatCmdParams* paramsP)
{
    char*		blockP;
    int		blockSize;
    int*		queueP;
    int*		headP;



    blockP		= (char*) paramsP->cmdParamP;
    TRACE( "blockP paramsP->cmdParamP = %s", blockP);

    blockSize	= (int) (paramsP->cmdParamSize & ~3);
    TRACE( "blockSize = %d", blockSize);

    if (blockSize < 2 * sizeof(int))
	return -1;

    queueP	= (int*) (blockP + blockSize - sizeof(int));
    headP	= queueP - 1;
    *headP	= 0;
    *queueP	= blockSize - 2 * sizeof(int);

    return TRUE;
}


/******************************************************************************
 * Function:	get_cmd_param_size
 *
 * Description:	give the real size of the proactive command parameter
 *
 * Parameters:	paramsP		a param block, defining the location
 *							and the size of the block
 * Return	:	the real size used in the param block by the proactive command parameter
 ******************************************************************************/

static int get_cmd_param_size(SatCmdParams* paramsP)
{
    char*		blockP;
    int		blockSize;
    int*		queueP;
    int*		headP;


    blockP		= (char*) paramsP->cmdParamP;
    blockSize		= (int) (paramsP->cmdParamSize & ~3);
    queueP		= (int*) (blockP + blockSize - sizeof(int));
    headP		= queueP - 1;

    return (*headP);
}

/******************************************************************************
 * Function:	cmd_param_alloc_head
 *
 * Description:	Allocate a sub-block at the begining of the block
 *
 * Parameters:	paramsP		a param block, defining the location
 *							and the size of the block
 *				size		the size of the sub block to allocate
 ******************************************************************************/

static void* cmd_param_alloc_head(SatCmdParams* paramsP, int size)
{
    char*		blockP;
    int		blockSize;
    int*		queueP;
    int*		headP;
    int		p;

    blockP		= (char*) paramsP->cmdParamP;
    blockSize		= (int) (paramsP->cmdParamSize & ~3);
    queueP		= (int*) (blockP + blockSize - sizeof(int));
    headP		= queueP - 1;
    p			= *headP;

    if (*headP + size >= *queueP)
	return NULL;

    *headP += size;

    if (size & 3)
	*headP += (int) (4 - (size & 3));

    return blockP + p;
}

/******************************************************************************
 * Function:	prv_cmd_param_alloc_queue
 *
 * Description:	Allocate a sub-block at the end of the block
 *
 * Parameters:	paramsP		a param block, defining the location
 *							and the size of the block
 *				size		the size of the sub block to allocate
 ******************************************************************************/

static void* prv_cmd_param_alloc_queue(SatCmdParams* paramsP, int size)
{
    char*	blockP;
    int	blockSize;
    int*	queueP;
    int*	headP;

    alp_prv_mbiz_begin_proc(prv_cmd_param_alloc_queue);

    blockP		= (char*) paramsP->cmdParamP;
    blockSize	= (int) (paramsP->cmdParamSize & ~3);
    queueP		= (int*) (blockP + blockSize - sizeof(int));
    headP		= queueP - 1;

    if (*queueP - size < *headP)
	return NULL;

    *queueP -= size;

    if (size & 3)
	*queueP -= (int) (4 - (size & 3));

    return blockP + *queueP;
}

/******************************************************************************
 * Function:	get_cmd_buffer
 *
 * Description:	Get bytes from a buffer and copy them in a sub-block
 *
 * Parameters:	paramsP			a param block, defining the location
 *								and the size of the block
 *				srcP			The source buffer
 *				size			the size of the sub block to allocate and fill
 *				destPP			a reference to the sub block to allocate and fill
 ******************************************************************************/

static int get_cmd_buffer(SatCmdParams* paramsP, char* srcP, char size, char** destPP)
{


    *destPP = cmd_param_alloc_head(paramsP, size);

    if (*destPP == NULL)
	return -1;

    memcpy(*destPP, srcP, size);

    return TRUE;
}



static int  menu_selection_callback(int request, char* bufferP, int* bufferSize, SatMenuSelection* menuSelectionP)
{
    int err = TRUE;

    switch (request)
    {
	case SAT_GET_DATA_FORMAT:
	    *((char**) bufferP) = MenSelectionFormat;
	    if (menuSelectionP->evtCode == SAT_MENU_SELECTION_HELP_REQUESTED)
		*bufferSize = 3;
	    else
		*bufferSize = 2;
	    break;

	case SAT_DEVICE_IDENTITIES:
	    *bufferSize = 2;
	    bufferP[0] = SAT_DEVICE_KEYPAD;
	    bufferP[1] = SAT_DEVICE_SIM;
	    break;

	case SAT_ITEM_IDENTIFIER:
	    *bufferSize = 1;
	    *bufferP = menuSelectionP->appId;
	    break;

	case SAT_HELP_REQUEST:
	    *bufferSize = 0;
	    break;

	default:
	    err = -1;
    }

    return err;
}

int   menu_selection( SatMenuSelection* iSelectionP , char *		apduStrP)
{
    char*		apduP;
    int			apduLen;
    int	err = TRUE;

    //	char* 		raw_packet = NULL;
    //	int 			raw_packet_size=0;



    // make and send
    TRACE(MSGL_VGSM_INFO, "selection by CAT Browser");
    TRACE(MSGL_VGSM_INFO, "need help = %x, selected Item identifier = %x", iSelectionP->evtCode, iSelectionP->appId);

    if ((err = sat_apdu_encode(SAT_MENU_SELECTION, (SatApduEncoderCallback) menu_selection_callback, iSelectionP, &apduStrP)) != TRUE)
    {
	TRACE(MSGL_VGSM_INFO, "sat_apdu_encode failed with error (0x%08x).", err);
	return err;

    }

    if ((err = str_to_bin(apduStrP, &apduP, &apduLen)) != TRUE)
    {
	TRACE(MSGL_VGSM_INFO, "str_to_bin failed with error (0x%08x).", err);

	return err;

    }

    return err;


}
#endif

/*
//////
static int set_cmd_response_callback(int request, char* bufferP, int16_t* bufferSize, MbizSetCmdResponseParamsType* paramsP)
{
int err = TRUE;

switch (request)
{
case SAT_GET_DATA_FORMAT:
{
char* writeP = MbizPrvSetCmdResponseFormat + 3;
switch (paramsP->responseP->cmdId)
{
case SAT_CMD_GET_INKEY:
case SAT_CMD_GET_INPUT:
if ((paramsP->responseP->resCode & 0xF0) == 0)
 *writeP++ = SAT_TEXT_STRING;
 break;

 case SAT_CMD_SELECT_ITEM:
 if ((paramsP->responseP->resCode & 0xF0) == 0)
 *writeP++ = SAT_ITEM_IDENTIFIER;
 break;
 }
 *((char**) bufferP) = MbizPrvSetCmdResponseFormat;
 *bufferSize = writeP - MbizPrvSetCmdResponseFormat;
 break;
 }

 case SAT_COMMAND_DETAILS:
 {
 char* objectP = paramsP->proactiveCmdP;
 char tag;

 *bufferSize = 3;

 if (objectP == NULL)
 err = -1;
 else
 {
// get the original
objectP += 4 + (objectP[1] == 0x81);
tag = objectP[-2];
 *bufferP++ = *objectP++;
 *bufferP++ = *objectP++;
 *bufferP++ = *objectP;
// The CR tag must be send back to the encoder in 4th byte
 *bufferP++ = tag;
 }
 break;
 }

 case SAT_DEVICE_IDENTITIES:
 *bufferSize = 2;
 bufferP[0] = SAT_DEVICE_ME;
 bufferP[1] = SAT_DEVICE_SIM;
 break;

 case SAT_RESULT:
 *bufferSize = 2;
 bufferP[0] = paramsP->responseP->resCode;
 bufferP[1] = paramsP->responseP->addInfo;
 break;

 case SAT_TEXT_STRING:
 {
 char** dataPP = (char**) bufferP;
 char* dataP;

// Here the buffer MUST be allocated with malloc.
// The APDU encoder will free it
*bufferSize = paramsP->responseP->respSize + 1;
dataP = *dataPP = malloc(*bufferSize);
if (dataP)
{
    dataP[0] = paramsP->responseP->respType;
    memcpy(dataP + 1, paramsP->responseP->respP, paramsP->responseP->respSize);
}
else
err = -1;
break;
}

case SAT_ITEM_IDENTIFIER:
*bufferSize = 1;
*bufferP = paramsP->responseP->itemIdentifier;
break;

default:
err = -1;
break;
}

return err;
}
*/

#if 0
void prtbin(unsigned char dt,char * bin)
{
    int i;

    for (i=7; i>=0; i--)
    {
	if (dt&0x80) 	*bin++ = '1';
	else *bin++ = '0';

	dt<<=1;
    }
}

void bin2dec(unsigned char bin,int *dec)
{
    int i,j;
    int k;

    for (i=8; i>0; i--)
    {
	k=1;

	if (bin&0x80)
	{
	    if(i==1)
		dec +=1;

	    else{
		for(j=i;i>0;j--)
		{
		    k = k*2;
		}
		dec += k;
	    }
	}

	bin<<=1;
    }
}
#endif



//////
static int cmd_display_text_callback(int request, char* bufferP, int* bufferSize, DP_TEXT* dp_text)
{
    //char	DisplayTextFormat[] 	= { SAT_COMMAND_DETAILS, SAT_DEVICE_IDENTITIES, SAT_TEXT_STRING,SAT_ICON_IDENTIFIER,SAT_IMMEDIATE_RESPONSE, SAT_DURATION};

    int err = 0;
    int str_len=0;

    char* writeP;
    char** dataPP;
    char* dataP;

    str_len = strlen(dp_text->text_string);

    switch (request)
    {
	case SAT_GET_DATA_FORMAT:

	    writeP = DisplayTextFormat + 3;

	    if (dp_text->icon_identifier == 1)
		*writeP++ = SAT_ICON_IDENTIFIER;

	    if (dp_text->immediate_response == 1)
		*writeP++ = SAT_IMMEDIATE_RESPONSE;

	    if (dp_text->duration == 1)
		*writeP++ = SAT_DURATION;


	    *((char**) bufferP) = DisplayTextFormat;
	    *bufferSize = writeP - DisplayTextFormat;



	    break;

	case SAT_COMMAND_DETAILS:

	    *bufferSize = 3 ;//

	    bufferP[0] = 0x01;//commnad number
	    bufferP[1] = 0x21;//typr of command

	    if(dp_text->priority == 0 && dp_text->clear_msg == 0)
		bufferP[2] = 0x00;//command qualifier 0/0
	    else if(dp_text->priority == 0 && dp_text->clear_msg == 1)
		bufferP[2] = 0x01;//command qualifier 0/1
	    else if(dp_text->priority == 1 && dp_text->clear_msg == 0)
		bufferP[2] = 0x80;//command qualifier 1/0
	    else if(dp_text->priority == 1 && dp_text->clear_msg == 1)
		bufferP[2] = 0x81;//command qualifier 1/1

	    bufferP[3] = 0x81 ; //tag - for  CR  ..The CR tag must be send back to the encoder in 4th byte
	    break;

	case SAT_DEVICE_IDENTITIES:
	    *bufferSize = 2;

	    bufferP[0] = SAT_DEVICE_SIM;//source device identities
	    bufferP[1] = SAT_DEVICE_DISPLAY;//destination device identities

	    break;

	case SAT_TEXT_STRING:

	    dataPP = (char**) bufferP;


	    // Here the buffer MUST be allocated with malloc.
	    // The APDU encoder will free it
	    *bufferSize = str_len + 1;

	    dataP = *dataPP = malloc(*bufferSize);

	    if (dataP)
	    {
		dataP[0] = dp_text->coding_scheme;
		memcpy(dataP + 1, dp_text->text_string, str_len);
	    }

	    else
		err = -1;
	    break;

	case SAT_ICON_IDENTIFIER:
	    *bufferSize = 2;

	    if(dp_text->icon_qualifier == 0)
		bufferP[0] = 0x00;//icon is self explanatory
	    else
		bufferP[0] = 0x01;//icon is not self explanatory

	    bufferP[1] = 0x4f;//dp_text->icon_identifier;//icon_identifier address EF-img

	    break;

	case SAT_IMMEDIATE_RESPONSE:
	    *bufferSize = 0;
	    break;

	case SAT_DURATION:
	    *bufferSize = 2;
	    if(dp_text->duration_unit == 0)
		bufferP[0] = 0x00;//time unit
	    else if(dp_text->duration_unit == 1)
		bufferP[0] = 0x01;//time unit
	    else if(dp_text->duration_unit == 2)
		bufferP[0] = 0x02;//time unit

	    bufferP[1] = 0x02;//time interval

	    break;

	default:
	    err = -1;
    }

    return err;

}

///////
//////
static int cmd_set_menu_callback(int request, char* bufferP, int* bufferSize, void* data)
{
    int err = 0;
    int str_len=0;
    int dec=0;

    char* writeP;
    char** dataPP;
    char* dataP;
    //	char* bits[8];
    int i=0;
    //	char *alpha_id="test setmenu";

    unsigned char cmd_detail=0;
    unsigned char num=0;
    //	unsigned char * t_num;

    //	GSM_SatProactiveCmd cmd;

    SET_MENU* set_menu;
    SET_MENU *temp_set_menu;

    //
    //	int str_size=0;
    unsigned char * pdata = (unsigned char *)(data);
    /////////////////////////
    num = *pdata++;
    cmd_detail = *pdata++;

    set_menu = (SET_MENU *)(pdata);

    switch (request)
    {
	case SAT_GET_DATA_FORMAT:

	    writeP = SetUpMenuFormat + 4;

	    g_item_cnt = num;

	    for(i=0;i<num-1;i++)  // one is mandaotory
		*writeP++ = SAT_ITEM;

	    *((char**) bufferP) = SetUpMenuFormat;
	    *bufferSize = writeP - SetUpMenuFormat;

	    TRACE(MSGL_VGSM_INFO, "cmd detail is <%x>\n",cmd_detail);
	    TRACE(MSGL_VGSM_INFO, "num is <%d>\n",g_item_cnt);
	    TRACE(MSGL_VGSM_INFO, "bufferSize is <%d>",*bufferSize);
	    break;

	case SAT_COMMAND_DETAILS:

	    *bufferSize = 3 ;//

	    bufferP[0] = 0x02;//commnad number
	    bufferP[1] = 0x25;//type of command

	    dec= cmd_detail;
	    bufferP[2] =dec;

	    bufferP[3] = 0x81 ; //tag - for  CR  ..The CR tag must be send back to the encoder in 4th byte
	    break;

	case SAT_DEVICE_IDENTITIES:
	    *bufferSize = 2;

	    bufferP[0] = SAT_DEVICE_SIM;//source device identities
	    bufferP[1] = SAT_DEVICE_ME;//destination device identities

	    break;

	case SAT_ALPHA_IDENTIFIER:

	    dataPP = (char**) bufferP;
	    dataP = *dataPP = malloc(*bufferSize);

	    temp_set_menu = &(set_menu[0]);

	    if (dataP)
		memcpy(dataP, temp_set_menu->alpah_string, strlen(temp_set_menu->alpah_string));

	    TRACE(MSGL_VGSM_INFO, "dataP is <%s>",dataP);

	    *bufferSize = strlen(temp_set_menu->alpah_string);
	    break;

	case SAT_ITEM:
	    //			TRACE("\CNT NUMBER is <%d>",g_item_cnt);

	    temp_set_menu = &(set_menu[num-g_item_cnt]);
	    g_item_cnt = g_item_cnt-1;

	    str_len = strlen(temp_set_menu->item_string);

	    *bufferSize = str_len + 1;

	    dataPP = (char**) bufferP;
	    dataP = *dataPP = malloc(*bufferSize);

	    TRACE(MSGL_VGSM_INFO, "Id number is <%d>\n",temp_set_menu->id_num);
	    TRACE(MSGL_VGSM_INFO, "text string is <%s>\n",temp_set_menu->item_string);
	    TRACE(MSGL_VGSM_INFO, "strlen is <%d>\n",str_len);


	    if (dataP)
	    {
		dataP[0] = temp_set_menu->id_num;
		memcpy(dataP + 1, temp_set_menu->item_string, str_len);
		TRACE(MSGL_VGSM_INFO, "Id number is <%d>\n",dataP[0]);
		TRACE(MSGL_VGSM_INFO, "text string is <%s>\n",dataP + 1);
	    }

	    else
		err = -1;

	    break;

	default:
	    err = -1;
    }

    return err;

}

//////
static int cmd_select_item_callback(int request, char* bufferP, int* bufferSize, void* data)
{
    int err = 0;
    int str_len=0;
    int dec=0;

    char* writeP;
    char** dataPP;
    char* dataP;
    //	char* bits[8];
    int i=0;

    unsigned char cmd_detail=0;
    unsigned char num=0;
    //	unsigned char * t_num;
    char *alpha_id = "test Select";
    //	GSM_SatProactiveCmd cmd;

    SET_MENU* set_menu;
    SET_MENU *temp_set_menu;

    //
    //	int str_size=0;
    unsigned char * pdata = (unsigned char *)(data);
    /////////////////////////
    num = *pdata++;
    cmd_detail = *pdata++;

    set_menu = (SET_MENU *)(pdata);

    switch (request)
    {
	case SAT_GET_DATA_FORMAT:

	    writeP = SetUpMenuFormat + 4;

	    g_item_cnt = num;

	    for(i=0;i<num-1;i++)  // one is mandaotory
		*writeP++ = SAT_ITEM;

	    *((char**) bufferP) = SetUpMenuFormat;
	    *bufferSize = writeP - SetUpMenuFormat;

	    TRACE(MSGL_VGSM_INFO, "cmd detail is <%x>\n",cmd_detail);
	    TRACE(MSGL_VGSM_INFO, "num is <%d>\n",g_item_cnt);
	    TRACE(MSGL_VGSM_INFO, "\bufferSize is <%d>",*bufferSize);
	    break;

	case SAT_COMMAND_DETAILS:

	    *bufferSize = 3 ;//

	    bufferP[0] = 0x01;//commnad number
	    bufferP[1] = 0x24;//type of command

	    dec= cmd_detail;
	    bufferP[2] =dec;

	    bufferP[3] = 0x81 ; //tag - for  CR  ..The CR tag must be send back to the encoder in 4th byte
	    break;

	case SAT_DEVICE_IDENTITIES:
	    *bufferSize = 2;

	    bufferP[0] = SAT_DEVICE_SIM;//source device identities
	    bufferP[1] = SAT_DEVICE_ME;//destination device identities

	    break;

	case SAT_ALPHA_IDENTIFIER:
	    *bufferSize = 4;

	    dataPP = (char**) bufferP;
	    dataP = *dataPP = malloc(*bufferSize);

	    TRACE(MSGL_VGSM_INFO, "alpha_id is <%s>",alpha_id);

	    if (dataP)
		memcpy(dataP, alpha_id,strlen(alpha_id));

	    TRACE(MSGL_VGSM_INFO, "dataP is <%s>",dataP);

	    break;

	case SAT_ITEM:
	    //			TRACE(MSGL_VGSM_INFO, "\CNT NUMBER is <%d>",g_item_cnt);

	    temp_set_menu = &(set_menu[num-g_item_cnt]);
	    g_item_cnt = g_item_cnt-1;

	    str_len = strlen(temp_set_menu->item_string);

	    *bufferSize = str_len + 1;

	    dataPP = (char**) bufferP;
	    dataP = *dataPP = malloc(*bufferSize);

	    TRACE(MSGL_VGSM_INFO, "Id number is <%d>\n",temp_set_menu->id_num);
	    TRACE(MSGL_VGSM_INFO, "text string is <%s>\n",temp_set_menu->item_string);
	    TRACE(MSGL_VGSM_INFO, "strlen is <%d>\n",str_len);


	    if (dataP)
	    {
		dataP[0] = temp_set_menu->id_num;
		memcpy(dataP + 1, temp_set_menu->item_string, str_len);

		TRACE(MSGL_VGSM_INFO, "Id number is <%d>\n",dataP[0]);
		TRACE(MSGL_VGSM_INFO, "text string is <%s>\n",dataP + 1);
	    }

	    else
		err = -1;

	    break;

	default:
	    err = -1;
    }

    return err;

}
//////
//////
static int cmd_send_sms_callback(int request, char* bufferP, int* bufferSize, void* data)
{
    int err = 0;
    //	int str_len=0;
    int dec=0;

    char* writeP;
    char** dataPP;
    char* dataP;
    char** tpduPP;
    char* tpduP;
    //	char* bits[8];
    //	int i=0;
    char *alpha_id="test Send SMS";
    unsigned char address;

    //	GSM_SatProactiveCmd cmd;
    SEND_SMS *send_sms_packet;
    //
    //	int str_size=0;

    send_sms_packet = (SEND_SMS *)(data);

    switch (request)
    {
	case SAT_GET_DATA_FORMAT:

	    writeP = SendSMSFormat + 5;

	    *((char**) bufferP) = SendSMSFormat;
	    *bufferSize = writeP - SendSMSFormat;

	    TRACE(MSGL_VGSM_INFO, "bufferSize is <%d>",*bufferSize);
	    break;

	case SAT_COMMAND_DETAILS:

	    *bufferSize = 3 ;//

	    bufferP[0] = 0x01;//commnad number
	    bufferP[1] = 0x13;//type of command

	    dec= send_sms_packet->cmd_detail;
	    bufferP[2] =dec;

	    bufferP[3] = 0x81 ; //tag - for  CR  ..The CR tag must be send back to the encoder in 4th byte
	    break;

	case SAT_DEVICE_IDENTITIES:
	    *bufferSize = 2;

	    // ???????????????????
	    bufferP[1] = SAT_DEVICE_SIM;//source device identities
	    bufferP[0] = SAT_DEVICE_ME;//destination device identities

	    break;

	case SAT_ALPHA_IDENTIFIER:

	    dataPP = (char**) bufferP;
	    dataP = *dataPP = malloc(*bufferSize);

	    if (dataP)
		memcpy(dataP, alpha_id, strlen(alpha_id));

	    *bufferSize = strlen(alpha_id);

	    break;

	case SAT_ADDRESS:
	    *bufferSize = strlen(send_sms_packet->dial_num) + 1;

	    dataPP = (char**) bufferP;
	    dataP = *dataPP = malloc(*bufferSize);

	    if (dataP)
	    {
		address = send_sms_packet->address;
		dataP[0] =(char)address;
		memcpy(dataP + 1, send_sms_packet->dial_num, strlen(send_sms_packet->dial_num));
	    }

	    else
	    {
		TRACE(MSGL_VGSM_INFO, "dataP is NULL \n");
		err = -1;
	    }

	    break;

	case SAT_SMS_TPDU:
	    *bufferSize = strlen(send_sms_packet->tpdu_sms) ;

	    tpduPP = (char**) bufferP;
	    tpduP = *tpduPP = malloc(*bufferSize);

	    TRACE(MSGL_VGSM_INFO, "TPDU-send_sms_packet->tpdu_sms is <%s>\n",send_sms_packet->tpdu_sms);

	    if (tpduP)
		memcpy(tpduP , send_sms_packet->tpdu_sms, strlen(send_sms_packet->tpdu_sms));
	    else
	    {
		TRACE(MSGL_VGSM_INFO, "dataP is NULL \n");
		err = -1;
	    }

	    TRACE(MSGL_VGSM_INFO, "TPDU-dataP is <%s>\n",tpduP);

	    break;

	default:
	    err = -1;
    }

    return err;

}
//////
static int cmd_set_up_call_callback(int request, char* bufferP, int* bufferSize, void* data)
{
    int err = 0;
    //	int str_len=0;
    int dec=0;

    char* writeP;
    char** dataPP;
    char* dataP;
    //     char* bits[8];
    int i=0;

    //	GSM_SatProactiveCmd cmd;

    //int			cnt=0;

    static unsigned char cmd_detail;
    static unsigned char options;
    static char dial_len;
    static char *dial_num;
    static unsigned char address;
    static char alpha_user_len;
    static char *alpha_user;
    //	static char alpha_call_len;
    //	static char *alpha_call;
    static char duration_unit;
    //	static char alpha_state;


    //
    //	int str_size;
    int tmp_address=0;
    unsigned char * pdata = (unsigned char *)(data);
    //////////////

    switch (request)
    {
	case SAT_GET_DATA_FORMAT:

	    writeP = SetUpCallFormat + 2;

	    options = *pdata++;
	    cmd_detail = *pdata++;
	    dial_len = *pdata++;
	    dial_num = malloc(dial_len);
	    memset(dial_num,0,dial_len);
	    memcpy(dial_num,pdata,dial_len);
	    //
	    TRACE(MSGL_VGSM_INFO,  " *-options is %x\n",options);
	    TRACE(MSGL_VGSM_INFO,  " *-dial_num is %s \n ",dial_num);
	    TRACE(MSGL_VGSM_INFO,  " *-dial_len is %d \n ",dial_len);


	    pdata+=dial_len;

	    //	alpha_state=0;

	    ///////////////////////////////
	    switch(options)
	    {
		case 0x00 : // not options

		    address = *pdata;
		    //
		    TRACE(MSGL_VGSM_INFO," *-address is %x \n ",address);
		    *writeP++ = SAT_ADDRESS;
		    break;

		case 0x01: // alpha_

		    alpha_user_len= *pdata++;
		    alpha_user = malloc(alpha_user_len);
		    memset(alpha_user,0,alpha_user_len);
		    memcpy(alpha_user,pdata,alpha_user_len);
		    pdata += alpha_user_len;
		    /*
		       alpha_call_len= *pdata++;
		       alpha_call = malloc(alpha_call_len);
		       memset(alpha_call,0,alpha_call_len);
		       memcpy(alpha_call,pdata,alpha_call_len);
		       pdata += alpha_call_len;
		    //
		     */
		    TRACE(MSGL_VGSM_INFO,"*-alpha_user is %s \n ",alpha_user);
		    //					TRACE(MSGL_VGSM_INFO,"*-alpha_call is %s \n ",alpha_call);
		    TRACE(MSGL_VGSM_INFO,"*-alpha_user_len is %d \n ",alpha_user_len);
		    //					TRACE(MSGL_VGSM_INFO,"*-alpha_call_len is %d \n ",alpha_call_len);

		    address = *pdata;
		    //
		    TRACE(MSGL_VGSM_INFO,"*-address is %x \n ",address);
		    *writeP++ = SAT_ALPHA_IDENTIFIER;
		    *writeP++ = SAT_ADDRESS;
		    //	*writeP++ = SAT_ALPHA_IDENTIFIER;

		    break;

		case 0x02: // DURATION
		    address = *pdata++;
		    //
		    TRACE(MSGL_VGSM_INFO,"*-address is %x \n ",address);

		    duration_unit = *pdata;
		    //
		    TRACE(MSGL_VGSM_INFO,"*-duration_unit is %x \n ",duration_unit);

		    *writeP++ = SAT_ADDRESS;
		    *writeP++ = SAT_DURATION;
		    break;
		case 0x03: //  alpha_ & DURATION
		    alpha_user_len= *pdata++;
		    alpha_user = malloc(alpha_user_len);
		    memset(alpha_user,0,alpha_user_len);
		    memcpy(alpha_user,pdata,alpha_user_len);
		    pdata += alpha_user_len;
		    /*
		       alpha_call_len= *pdata++;
		       alpha_call = malloc(alpha_call_len);
		       memset(alpha_call,0,alpha_user_len);
		       memcpy(alpha_call,pdata,alpha_call_len);
		       pdata += alpha_call_len;
		     */
		    //
		    TRACE(MSGL_VGSM_INFO,"*-alpha_user is %s \n ",alpha_user);
		    //	TRACE(MSGL_VGSM_INFO,"*-alpha_call is %s \n ",alpha_call);
		    TRACE(MSGL_VGSM_INFO,"*-alpha_user_len is %d \n ",alpha_user_len);
		    //	TRACE(MSGL_VGSM_INFO,"*-alpha_user_len is %d \n ",alpha_user_len);

		    address = *pdata++;
		    duration_unit = *pdata;

		    //
		    TRACE(MSGL_VGSM_INFO,"*-address is %x \n ",address);
		    TRACE(MSGL_VGSM_INFO,"*-duration_unit is %x \n ",duration_unit);

		    *writeP++ = SAT_ALPHA_IDENTIFIER;
		    *writeP++ = SAT_ADDRESS;
		    *writeP++ = SAT_DURATION;
		    //	*writeP++ = SAT_ALPHA_IDENTIFIER;
		    break;

	    }

	    *((char**) bufferP) = SetUpCallFormat;
	    *bufferSize = writeP - SetUpCallFormat;

	    TRACE(MSGL_VGSM_INFO,"cmd detail is <%x>\n",cmd_detail);
	    TRACE(MSGL_VGSM_INFO,"\bufferSize is <%d>",*bufferSize);

	    for(i=0;i<*bufferSize;i++)
		TRACE(MSGL_VGSM_INFO,"*- SetUpCallFormat is <%x>",SetUpCallFormat[i]);

	    break;

	case SAT_COMMAND_DETAILS:

	    *bufferSize = 3 ;//

	    bufferP[0] = 0x01;//commnad number
	    bufferP[1] = 0x10;//SAT_CMD_SETUP_CALL;//type of command

	    dec= cmd_detail;
	    bufferP[2] =dec;

	    bufferP[3] = 0x81 ; //tag - for  CR  ..The CR tag must be send back to the encoder in 4th byte
	    break;

	case SAT_DEVICE_IDENTITIES:
	    *bufferSize = 2;

	    bufferP[0] = SAT_DEVICE_SIM;//source device identities
	    bufferP[1] = SAT_DEVICE_EARPIECE;//destination device identities

	    break;

	case SAT_ALPHA_IDENTIFIER:


	    dataPP = (char**) bufferP;
	    dataP = *dataPP = malloc(*bufferSize);

	    //	if(alpha_state == 0)
	    //		{
	    //			alpha_state =1;
	    TRACE(MSGL_VGSM_INFO,"alpha_user is <%s>",alpha_user);

	    if (dataP)
		memcpy(dataP, alpha_user,alpha_user_len);

	    *bufferSize = alpha_user_len;
	    TRACE(MSGL_VGSM_INFO,"dataP is <%s>",dataP);
	    //		}
	    /*
	       else
	       {
	       alpha_state =0;
	       TRACE(MSGL_VGSM_INFO,"alpha_call is <%s>",alpha_call);

	       if (dataP)
	       memcpy(dataP, alpha_call,alpha_call_len);

	     *bufferSize = alpha_call_len;
	     TRACE(MSGL_VGSM_INFO,"dataP is <%s>",dataP);
	     }
	     */
	    break;

	case SAT_DURATION:
	    *bufferSize = 2;
	    TRACE(MSGL_VGSM_INFO,"duration_unit is <%x>",duration_unit);
	    if(duration_unit == 0)
		bufferP[0] = 0x00;//time unit
	    else if(duration_unit == 1)
		bufferP[0] = 0x01;//time unit
	    else if(duration_unit == 2)
		bufferP[0] = 0x02;//time unit

	    bufferP[1] = 0x02;//time interval

	    break;

	case SAT_ADDRESS:
	    *bufferSize = dial_len + 1;

	    dataPP = (char**) bufferP;
	    dataP = *dataPP = malloc(*bufferSize);

	    if (dataP)
	    {
		tmp_address = address;
		dataP[0] =(char)tmp_address;
		memcpy(dataP + 1, dial_num, dial_len);
		TRACE(MSGL_VGSM_INFO,"Id number is <%#x>\n",dataP[0]);
		TRACE(MSGL_VGSM_INFO,"text string is <%s>\n",dataP + 1);
	    }

	    else
	    {
		TRACE(MSGL_VGSM_INFO,"dataP is NULL \n");
		err = -1;
	    }

	    break;


	default:
	    {
		TRACE(MSGL_VGSM_INFO,"Defalut \n");
		err = -1;
	    }
    }

    return err;

}
//////
//////
static int cmd_get_input_callback(int request, char* bufferP, int* bufferSize, GET_INPUT* get_input)
{
    int err = 0;
    int str_len=0;
    int dec=0;

    char* writeP;
    char** dataPP;
    char* dataP;

    //	char* bits[8];

    str_len = strlen(get_input->text_string);

    switch (request)
    {
	case SAT_GET_DATA_FORMAT:

	    writeP = GetInputFormat + 3;

	    if (get_input->icon_identifier == 1)
		*writeP++ = SAT_ICON_IDENTIFIER;

	    if (get_input->immediate_response == 1)
		*writeP++ = SAT_IMMEDIATE_RESPONSE;

	    if (get_input->duration == 1)
		*writeP++ = SAT_DURATION;


	    *((char**) bufferP) = GetInputFormat;
	    *bufferSize = writeP - GetInputFormat;

	    break;

	case SAT_COMMAND_DETAILS:

	    *bufferSize = 3 ;//

	    bufferP[0] = 0x01;//commnad number
	    bufferP[1] = 0x23;//type of command

	    //bin2dec(get_inkey->cmd_detail,&dec);  //  different - signed,unsigned....
	    dec= get_input->cmd_detail;
	    bufferP[2] =dec;

	    bufferP[3] = 0x81 ; //tag - for  CR  ..The CR tag must be send back to the encoder in 4th byte
	    break;

	case SAT_DEVICE_IDENTITIES:
	    *bufferSize = 2;

	    bufferP[0] = SAT_DEVICE_SIM;//source device identities
	    bufferP[1] = SAT_DEVICE_DISPLAY;//destination device identities

	    break;

	case SAT_TEXT_STRING:

	    dataPP = (char**) bufferP;


	    // Here the buffer MUST be allocated with malloc.
	    // The APDU encoder will free it
	    *bufferSize = str_len + 1;

	    dataP = *dataPP = malloc(*bufferSize);



	    if (dataP)
	    {
		dataP[0] = get_input->coding_scheme;
		memcpy(dataP + 1, get_input->text_string, str_len);
	    }

	    else
		err = -1;
	    break;

	case SAT_ICON_IDENTIFIER:
	    *bufferSize = 2;

	    if(get_input->icon_qualifier == 0)
		bufferP[0] = 0x00;//icon is self explanatory
	    else
		bufferP[0] = 0x01;//icon is not self explanatory

	    bufferP[1] = 0x4f;//dp_text->icon_identifier;//icon_identifier address EF-img

	    break;

	case SAT_IMMEDIATE_RESPONSE:
	    *bufferSize = 0;
	    break;

	case SAT_DURATION:
	    *bufferSize = 2;
	    if(get_input->duration_unit == 0)
		bufferP[0] = 0x00;//time unit
	    else if(get_input->duration_unit == 1)
		bufferP[0] = 0x01;//time unit
	    else if(get_input->duration_unit == 2)
		bufferP[0] = 0x02;//time unit

	    bufferP[1] = 0x02;//time interval

	    break;

	default:
	    err = -1;
    }

    return err;

}

//////
static int cmd_get_inkey_callback(int request, char* bufferP, int* bufferSize, GET_INKEY* get_inkey)
{
    int err = 0;
    int str_len=0;
    int dec=0;

    char* writeP;
    char** dataPP;
    char* dataP;

    //	char* bits[8];

    str_len = strlen(get_inkey->text_string);

    switch (request)
    {
	case SAT_GET_DATA_FORMAT:

	    writeP = GetInkeyFormat + 3;

	    if (get_inkey->icon_identifier == 1)
		*writeP++ = SAT_ICON_IDENTIFIER;

	    if (get_inkey->immediate_response == 1)
		*writeP++ = SAT_IMMEDIATE_RESPONSE;

	    if (get_inkey->duration == 1)
		*writeP++ = SAT_DURATION;


	    *((char**) bufferP) = GetInkeyFormat;
	    *bufferSize = writeP - GetInkeyFormat;

	    break;

	case SAT_COMMAND_DETAILS:

	    *bufferSize = 3 ;//

	    bufferP[0] = 0x01;//commnad number
	    bufferP[1] = 0x22;//type of command

	    //bin2dec(get_inkey->cmd_detail,&dec);  //  different - signed,unsigned....
	    dec= get_inkey->cmd_detail;
	    bufferP[2] =dec;

	    bufferP[3] = 0x81 ; //tag - for  CR  ..The CR tag must be send back to the encoder in 4th byte
	    break;

	case SAT_DEVICE_IDENTITIES:
	    *bufferSize = 2;

	    bufferP[0] = SAT_DEVICE_SIM;//source device identities
	    bufferP[1] = SAT_DEVICE_DISPLAY;//destination device identities

	    break;

	case SAT_TEXT_STRING:

	    dataPP = (char**) bufferP;


	    // Here the buffer MUST be allocated with malloc.
	    // The APDU encoder will free it
	    *bufferSize = str_len + 1;

	    dataP = *dataPP = malloc(*bufferSize);

	    if (dataP)
	    {
		dataP[0] = get_inkey->coding_scheme;
		memcpy(dataP + 1, get_inkey->text_string, str_len);
	    }

	    else
		err = -1;
	    break;

	case SAT_ICON_IDENTIFIER:
	    *bufferSize = 2;

	    if(get_inkey->icon_qualifier == 0)
		bufferP[0] = 0x00;//icon is self explanatory
	    else
		bufferP[0] = 0x01;//icon is not self explanatory

	    bufferP[1] = 0x4f;//dp_text->icon_identifier;//icon_identifier address EF-img

	    break;

	case SAT_IMMEDIATE_RESPONSE:
	    *bufferSize = 0;
	    break;

	case SAT_DURATION:
	    *bufferSize = 2;
	    if(get_inkey->duration_unit == 0)
		bufferP[0] = 0x00;//time unit
	    else if(get_inkey->duration_unit == 1)
		bufferP[0] = 0x01;//time unit
	    else if(get_inkey->duration_unit == 2)
		bufferP[0] = 0x02;//time unit

	    bufferP[1] = 0x02;//time interval

	    break;

	default:
	    err = -1;
    }

    return err;

}

int   Display_Text( DP_TEXT* dp_text)
{
    char *		apduStrP=NULL;
    char*		apduP;
    int			apduLen;
    int			err = 1;
    //	int 			i;
    GSM_SatProactiveCmd cmd;

    int str_size=0;

    // make and send
    TRACE(MSGL_VGSM_INFO, "Display_Text: selection by CAT Browser");
    //	TRACE(MSGL_VGSM_INFO, "Display_Text: priority is %x   clear msg is %d text is %s",dp_text->priority,dp_text->priority,dp_text->text_string);

    if ((err = sat_apdu_encode(SAT_PROACTIVE_SIM_COMMAND, (SatApduEncoderCallback) cmd_display_text_callback, dp_text, &apduStrP)) != 0)
    {
	TRACE(MSGL_VGSM_INFO, "Display_Text: sat_apdu_encode failed with error (0x%08x).", err);
	return err;
    }

    if(apduStrP==NULL){
	TRACE(MSGL_VGSM_INFO,"NULL\n");
	return -1;
    }

    str_size = strlen(apduStrP);

    if ((err = str_to_bin(apduStrP, &apduP, &apduLen)) != 0)
    {
	TRACE(MSGL_VGSM_INFO, "Display_Text: str_to_bin failed with error (0x%08x).", err);
	if(apduStrP)
	    free(apduStrP);
	return err;
    }

    cmd.length = str_size/2;
    cmd.cmd = (unsigned char*)apduP;
    TxSAT_ATGetProactiveCommand(&cmd);

    if(apduStrP)
	free(apduStrP);
    return 1;
}

int   Get_Inkey( GET_INKEY* get_inkey)
{
    char *		apduStrP=NULL;
    char*		apduP;
    int			apduLen;
    int			err = 1;
    //	int 			i;
    GSM_SatProactiveCmd cmd;

    //
    int str_size=0;

    // make and send
    TRACE(MSGL_VGSM_INFO, "selection by CAT Browser");

    TRACE(MSGL_VGSM_INFO,"In VGSM : cmd detail - %d\n",get_inkey->cmd_detail);

    if ((err = sat_apdu_encode(SAT_PROACTIVE_SIM_COMMAND, (SatApduEncoderCallback) cmd_get_inkey_callback, get_inkey, &apduStrP)) != 0)
    {
	TRACE(MSGL_VGSM_INFO, "sat_apdu_encode failed with error (0x%08x).", err);
	return err;

    }

    assert(apduStrP);

    str_size = strlen(apduStrP);

    if ((err = str_to_bin(apduStrP, &apduP, &apduLen)) != 0)
    {
	TRACE(MSGL_VGSM_INFO, "str_to_bin failed with error (0x%08x).", err);

	if(apduStrP)
	    free(apduStrP);
	return err;

    }

    cmd.length = str_size/2;
    cmd.cmd = (unsigned char*)apduP;
    TxSAT_ATGetProactiveCommand(&cmd);

    if(apduStrP)
	free(apduStrP);
    return 1;

}

int   Get_Input( GET_INPUT* get_input)
{
    char *		apduStrP=NULL;
    char*		apduP;
    int			apduLen;
    int			err = 1;
    //	int 			i;
    GSM_SatProactiveCmd cmd;

    //
    int str_size=0;

    // make and send
    TRACE(MSGL_VGSM_INFO, "selection by CAT Browser");

    if ((err = sat_apdu_encode(SAT_PROACTIVE_SIM_COMMAND, (SatApduEncoderCallback) cmd_get_input_callback, get_input, &apduStrP)) != 0)
    {
	TRACE(MSGL_VGSM_ERR, "sat_apdu_encode failed with error (0x%08x).", err);
	return err;

    }

    assert(apduStrP);

    str_size = strlen(apduStrP);

    if ((err = str_to_bin(apduStrP, &apduP, &apduLen)) != 0)
    {
	TRACE(MSGL_VGSM_ERR, "str_to_bin failed with error (0x%08x).", err);

	if(apduStrP)
	    free(apduStrP);
	return err;

    }

    cmd.length = str_size/2;
    cmd.cmd = (unsigned char*)apduP;
    TxSAT_ATGetProactiveCommand(&cmd);

    if(apduStrP)
	free(apduStrP);
    return 1;

}
/////////////////
int   Set_Menu(void *data)
{
    char *		apduStrP=NULL;
    char*		apduP;
    int			apduLen;
    int			err = 1;

    GSM_SatProactiveCmd cmd;

    int str_size=0;

    // make and send
    TRACE(MSGL_VGSM_INFO, "selection by CAT Browser");


    if ((err = sat_apdu_encode(SAT_PROACTIVE_SIM_COMMAND, (SatApduEncoderCallback) cmd_set_menu_callback, data, &apduStrP)) != 0)
    {
	TRACE(MSGL_VGSM_ERR, "sat_apdu_encode failed with error (0x%08x).", err);
	return err;

    }

    assert(apduStrP!=NULL);

    str_size = strlen(apduStrP);

    if ((err = str_to_bin(apduStrP, &apduP, &apduLen)) != 0)
    {
	TRACE(MSGL_VGSM_ERR, "str_to_bin failed with error (0x%08x).", err);
	if(apduStrP)
	    free(apduStrP);
	return err;
    }

    cmd.length = str_size/2;
    cmd.cmd = (unsigned char*)apduP;
    TxSAT_ATGetProactiveCommand(&cmd);

    if(apduStrP)
	free(apduStrP);
    return 1;

}

int   Select_Item(void *data)
{
    char *		apduStrP=NULL;
    char*		apduP;
    int			apduLen;
    int			err = 1;

    GSM_SatProactiveCmd cmd;

    int str_size=0;

    // make and send
    TRACE(MSGL_VGSM_INFO, "selection by CAT Browser");


    if ((err = sat_apdu_encode(SAT_PROACTIVE_SIM_COMMAND, (SatApduEncoderCallback) cmd_select_item_callback, data, &apduStrP)) != 0)
    {
	TRACE(MSGL_VGSM_ERR, "sat_apdu_encode failed with error (0x%08x).", err);
	return err;

    }

    assert(apduStrP);

    str_size = strlen(apduStrP);


    if ((err = str_to_bin(apduStrP, &apduP, &apduLen)) != 0)
    {
	TRACE(MSGL_VGSM_ERR, "str_to_bin failed with error (0x%08x).", err);
	if(apduStrP)
	    free(apduStrP);
	return err;
    }

    cmd.length = str_size/2;
    cmd.cmd = (unsigned char*)apduP;
    TxSAT_ATGetProactiveCommand(&cmd);

    if(apduStrP)
	free(apduStrP);
    return 1;

}

////////
int   Send_SMS(void *data)
{
    char *		apduStrP=NULL;
    char*		apduP;
    int			apduLen;
    int			err = 1;
    //	int 			i=0;

    GSM_SatProactiveCmd cmd;

    int str_size=0;

    // make and send
    TRACE(MSGL_VGSM_INFO, "selection by CAT Browser\n");

    if ((err = sat_apdu_encode(SAT_PROACTIVE_SIM_COMMAND, (SatApduEncoderCallback) cmd_send_sms_callback, data, &apduStrP)) != 0)
    {
	TRACE(MSGL_VGSM_ERR, "sat_apdu_encode failed with error (0x%08x).", err);
	return err;
    }

    assert(apduStrP!=NULL);

    str_size = strlen(apduStrP);

    if ((err = str_to_bin(apduStrP, &apduP, &apduLen)) != 0)
    {
	TRACE(MSGL_VGSM_ERR, "str_to_bin failed with error (0x%08x).", err);

	if(apduStrP)
	    free(apduStrP);
	return err;

    }

    cmd.length = str_size/2;
    cmd.cmd = (unsigned char*)apduP;
    TxSAT_ATGetProactiveCommand(&cmd);

    if(apduStrP)
	free(apduStrP);
    return 1;

}

////
int   Set_Up_Call(void *data)
{
    char *		apduStrP=NULL;
    char*		apduP;
    int			apduLen;
    int			err = 1;
    //	int 			i=0;

    GSM_SatProactiveCmd cmd;

    int str_size=0;

    // make and send
    TRACE(MSGL_VGSM_INFO, "selection by CAT Browser\n");

    if ((err = sat_apdu_encode(SAT_PROACTIVE_SIM_COMMAND, (SatApduEncoderCallback) cmd_set_up_call_callback, data, &apduStrP)) != 0)
    {
	TRACE(MSGL_VGSM_ERR, "sat_apdu_encode failed with error (0x%08x).", err);
	return err;
    }
    if(!apduStrP)
	return -1;
    str_size = strlen(apduStrP);


    if ((err = str_to_bin(apduStrP, &apduP, &apduLen)) != 0)
    {
	TRACE(MSGL_VGSM_ERR, "str_to_bin failed with error (0x%08x).", err);

	if(apduStrP)
	    free(apduStrP);
	return err;

    }


    cmd.length = str_size/2;
    cmd.cmd = (unsigned char*)apduP;
    TxSAT_ATGetProactiveCommand(&cmd);

    if(apduStrP)
	free(apduStrP);
    return 1;

}

#if 0
////
/***********************************************************************
 * FUNCTION:	cmd_generic_callback
 ***********************************************************************/

static int cmd_generic_callback(char tag, SatApduDecoder* decoderP, char* objectP)
{
    SatCmdParams* paramsP = (SatCmdParams*) decoderP->HLDataP;


    switch (tag)
    {
	case SAT_ALPHA_IDENTIFIER:
	case SAT_TEXT_STRING:
	    {
		if (objectP)
		{
		    if (paramsP->textP!=NULL) free(paramsP->textP);
		    paramsP->textSize = strlen((char*) objectP) + 1;
		    paramsP->textP= (char*)malloc(paramsP->textSize);

		    strncpy(paramsP->textP, (char*) objectP, paramsP->textSize - 1);
		    paramsP->textP[paramsP->textSize - 1] = 0;
		    TRACE( "cmd_generic_callback: paramsP->textP=%s", paramsP->textP);


		}
		else
		{
		    //if the alpha identifier is provided by the UICC and is a null data object,
		    //this is an indication that the terminal should not give any information
		    //to the user concerning what is happening
		    paramsP->textSize = 1;
		    paramsP->textP= (char*)malloc(paramsP->textSize);
		    paramsP->textP[0] = 0;

		}

		break;
	    }
	case SAT_ICON_IDENTIFIER:
	    {
		if (objectP)
		{
		    paramsP->explicitIcon = (objectP[0] & NON_EXPLICIT_ICON) == 0;
		    paramsP->iconId = objectP[1];
		}
		break;
	    }

	default:
	    break;
    }

    return TRUE;
}
//////////////////////////////////////////////////////

/***********************************************************************
 * FUNCTION:	cmd_display_text_callback
 ***********************************************************************/

int cmd_display_text_callback(char tag, SatApduDecoder* decoderP, char* objectP)
{
    SatCmdParams*	paramsP				= (SatCmdParams*) decoderP->HLDataP;
    SatDisplayText*	displayTextParamsP	= (SatDisplayText*) paramsP->cmdParamP;

    alp_prv_mbiz_begin_proc(cmd_display_text_callback);

    cmd_generic_callback(tag, decoderP, objectP);

    switch (tag)
    {
	case SAT_COMMAND_DETAILS:
	    if (objectP)
	    {
		displayTextParamsP->clearAfterDelay = (COMMAND_QUALIFIER & DISPLAY_TEXT_DETAILS_CLEAR_AFTER_DELAY) == 0;
		displayTextParamsP->priority = COMMAND_QUALIFIER & DISPLAY_TEXT_DETAILS_PRIORITY;
	    }
	    break;

	case SAT_IMMEDIATE_RESPONSE:
	    displayTextParamsP->immediateResponse = TRUE;
	    break;

	case SAT_DURATION:
	    {
		switch (objectP[0])	// switch on time unit
		{
		    case 0: // minutes
			displayTextParamsP->duration = (int) objectP[1] * 60000;
			break;

		    case 1: // seconds
			displayTextParamsP->duration = (int) objectP[1] * 1000;
			break;

		    case 2: // tenths of seconds
			displayTextParamsP->duration = (int) objectP[1] * 100;
			break;

		    default:
			break;
		}

		break;
	    }


	default:
	    break;
    }

    return TRUE;
}

/***********************************************************************
 * FUNCTION:	cmd_get_inkey_callback
 ***********************************************************************/

int cmd_get_inkey_callback(char tag, SatApduDecoder* decoderP, char* objectP)
{

    SatCmdParams*	paramsP			= (SatCmdParams*) decoderP->HLDataP;
    SatGetInkey*	getInkeyParamsP	= (SatGetInkey*) paramsP->cmdParamP;

    alp_prv_mbiz_begin_proc(cmd_get_inkey_callback);

    cmd_generic_callback(tag, decoderP, objectP);

    switch (tag)
    {
	case SAT_COMMAND_DETAILS:
	    if (objectP)
	    {
		getInkeyParamsP->helpInfo = (COMMAND_QUALIFIER & GET_DETAILS_HELP_INFO) == GET_DETAILS_HELP_INFO;
		getInkeyParamsP->respType = COMMAND_QUALIFIER & GETINKEY_DETAILS_YES_NO ?
		    SAT_RESP_TYPE_YES_NO_OPTION :
		    sMbiz_getInkeyRespType[COMMAND_QUALIFIER & GETINKEY_DETAILS_DCS_MASK];
	    }
	    break;

	case SAT_DURATION:
	    {
		switch (objectP[0])	// switch on time unit
		{
		    case 0: // minutes
			getInkeyParamsP->duration = (int) objectP[1] * 60000;
			break;

		    case 1: // seconds
			getInkeyParamsP->duration = (int) objectP[1] * 1000;
			break;

		    case 2: // tenths of seconds
			getInkeyParamsP->duration = (int) objectP[1] * 100;
			break;

		    default:
			break;
		}
		break;
	    }


	default:
	    break;
    }

    return TRUE;
}

/***********************************************************************
 * FUNCTION:	cmd_get_input_callback
 ***********************************************************************/

int cmd_get_input_callback(char tag, SatApduDecoder* decoderP, char* objectP)
{

    SatCmdParams*	paramsP			= (SatCmdParams*) decoderP->HLDataP;
    SatGetInput*	getInputParamsP	= (SatGetInput*)	paramsP->cmdParamP;
    int			err 				= TRUE;

    alp_prv_mbiz_begin_proc(cmd_get_input_callback);

    cmd_generic_callback(tag, decoderP, objectP);

    if (objectP == NULL)
	return TRUE;

    switch (tag)
    {
	case SAT_COMMAND_DETAILS:
	    getInputParamsP->helpInfo		= (COMMAND_QUALIFIER & GET_DETAILS_HELP_INFO) == GET_DETAILS_HELP_INFO;
	    getInputParamsP->hideUserInput	= (COMMAND_QUALIFIER & GETINPUT_DETAILS_HIDE_INPUT) == GETINPUT_DETAILS_HIDE_INPUT;
	    getInputParamsP->respType		= sMbiz_getInkeyRespType[COMMAND_QUALIFIER & GETINKEY_DETAILS_DCS_MASK];

	    if (COMMAND_QUALIFIER & GETINPUT_DETAILS_PACKED_BIT)
		getInputParamsP->respType++;	// packed

	    break;

	case SAT_DEFAULT_TEXT:
	    getInputParamsP->defRespSize = (char) strlen((char*) objectP) + 1;

	    if ((err = get_cmd_buffer(paramsP, objectP, getInputParamsP->defRespSize, (char**) &getInputParamsP->defRespP)) != TRUE)
		return err;

	    break;

	case SAT_RESPONSE_LENGTH:
	    getInputParamsP->minRespLength = objectP[0];
	    getInputParamsP->maxRespLength = objectP[1];
	    break;

	default:
	    break;
    }

    return TRUE;
}



/***********************************************************************
 * FUNCTION:	cmd_select_item_callback
 ***********************************************************************/

int cmd_select_item_callback(char tag, SatApduDecoder* decoderP, char* objectP)
{


    SatCmdParams*	paramsP		= (SatCmdParams*) decoderP->HLDataP;
    SatItemList*		itemListP	= (SatItemList*)	paramsP->cmdParamP;
    SatItem* 		itemPtr;
    int 			err 			= TRUE;

    alp_prv_mbiz_begin_proc(cmd_select_item_callback);

    cmd_generic_callback(tag, decoderP, objectP);

    if (objectP == NULL)
	return TRUE;

    switch (tag)
    {
	case SAT_COMMAND_DETAILS:
	    {
		char softKeyFlag;

		if (tag == SAT_CMD_SETUP_MENU)
		    softKeyFlag = SETUP_MENU_DETAILS_SOFTKEY;
		else
		    softKeyFlag = SELECT_ITEM_DETAILS_SOFTKEY;

		itemListP->helpInfo	= (COMMAND_QUALIFIER & GET_DETAILS_HELP_INFO) == GET_DETAILS_HELP_INFO;
		itemListP->softKey	= (COMMAND_QUALIFIER & softKeyFlag) == softKeyFlag;

		break;
	    }

	case SAT_ITEM:
	    {
		SatItem* itemP = cmd_param_alloc_head(paramsP, sizeof(SatItem));

		TRACE(MSGL_VGSM_INFO, "cmd_select_item_callback: itemP=%x", itemP));


		if (!itemP)
		    return -1;

		itemP->nameSize	= strlen(((char*) (objectP)) + 1) + 1;
		itemP->nameP	= cmd_param_alloc_head(paramsP, itemP->nameSize);
		itemP->nextItemP= NULL;

		if (!itemP->nameP)
		    return -1;

		memcpy(itemP->nameP, objectP + 1, itemP->nameSize - 1);
		itemP->nameP[itemP->nameSize - 1] = 0;
		itemP->id = *objectP;


		TRACE(MSGL_VGSM_INFO, "cmd_select_item_callback: itemP->nameP=%s, itemP->nameSize=%d",itemP->nameP,itemP->nameSize));


		itemListP->itemCount++;

		if (itemListP->itemCount==1)
		    itemListP->firstItemP=itemP;

		else {

		    TRACE(MSGL_VGSM_INFO, "cmd_select_item_callback: itemListP->itemCount=%d", itemListP->itemCount));

		    itemPtr = itemListP->firstItemP;

		    while(itemPtr->nextItemP!=NULL){
			itemPtr=itemPtr->nextItemP;
		    }
		    itemPtr->nextItemP=itemP;

		    TRACE(MSGL_VGSM_INFO, "cmd_select_item_callback: itemPtr=%x", itemPtr));
		    TRACE(MSGL_VGSM_INFO, "cmd_select_item_callback: itemPtr->nextItemP=%x", itemPtr->nextItemP));

		}


		break;
	    }

	case SAT_ITEMS_NEXT_ACTION_INDICATOR:
	    if (decoderP->currentObjectLength == itemListP->itemCount)
	    {
		SatItem*	itemP;
		char			i;

		for (i=0, itemP = itemListP->firstItemP; itemP!=NULL ; i++, itemP=itemP->nextItemP)
		    itemP->nextActionInd = objectP[i];
	    }
	    break;

	case SAT_ITEM_ICON_IDENTIFIER_LIST:
	    if (decoderP->currentObjectLength == itemListP->itemCount + 1)
	    {
		SatItem*	itemP;
		char			i;

		for (i=0, itemP = itemListP->firstItemP; itemP!=NULL ; i++, itemP=itemP->nextItemP)
		{
		    itemP->expIcon	= (objectP[0] & NON_EXPLICIT_ICON) == 0;
		    itemP->iconId	= objectP[i + 1];
		}
	    }

	    break;

	    // for select item only: the Item identifier gives an indication of the default item
	case SAT_ITEM_IDENTIFIER:
	    itemListP->defItemId = objectP[0];
	    break;

	default:
	    break;
    }

    return err;
}



/***********************************************************************
 * FUNCTION:	cmd_refresh_callback
 ***********************************************************************/

int cmd_refresh_callback(char tag, SatApduDecoder* decoderP, char* objectP)
{

    SatCmdParams*		paramsP		= (SatCmdParams*) decoderP->HLDataP;
    SatRefreshFileList*	refreshP		= (SatRefreshFileList*)paramsP->cmdParamP;
    int				err			= TRUE;

    alp_prv_mbiz_begin_proc(cmd_refresh_callback);

    cmd_generic_callback(tag, decoderP, objectP);

    if (objectP == NULL)
	return TRUE;

    switch (tag)
    {
	case SAT_COMMAND_DETAILS:
	    refreshP->opCode = COMMAND_QUALIFIER;
	    break;

	case SAT_FILE_LIST:
	    refreshP->fileIdCount = objectP[0]; //number of files in the file list
	    refreshP->filePathCount = (decoderP->currentObjectLength - 1)/2; // number of path in the file list, a path is composed of two bytes
	    if ((err = get_cmd_buffer(paramsP, objectP + 1, decoderP->currentObjectLength - 1, (char**) &refreshP->filePathP)) != TRUE)
		return err;
	    break;

	default:
	    break;
    }

    return TRUE;
}


/***********************************************************************
 * FUNCTION:	cmd_setup_event_list_callback
 ***********************************************************************/

int cmd_setup_event_list_callback(char tag, SatApduDecoder* decoderP, char* objectP)
{

    SatCmdParams*		paramsP			= (SatCmdParams*) decoderP->HLDataP;
    SatSetUpEventList*	setUpEventListP	= (SatSetUpEventList*) paramsP->cmdParamP;
    int				err				= TRUE;

    alp_prv_mbiz_begin_proc(cmd_setup_event_list_callback);

    if (objectP == NULL)
	return TRUE;

    switch (tag)
    {
	case SAT_EVENT_LIST:
	    setUpEventListP->eventCount = decoderP->currentObjectLength;
	    if ((err = get_cmd_buffer(paramsP, objectP, setUpEventListP->eventCount, &setUpEventListP->eventP)) != TRUE)
		return err;
	    break;

	default:
	    break;
    }

    return TRUE;
}

/***********************************************************************
 * FUNCTION:	cmd_setup_call_callback
 ***********************************************************************/

int cmd_setup_call_callback(char tag, SatApduDecoder* decoderP, char* objectP)
{

    SatCmdParams*	paramsP		= (SatCmdParams*) decoderP->HLDataP;
    SatSetUpCall*	setUpCallP	= (SatSetUpCall*) paramsP->cmdParamP;

    static char		text;
    static char		icon;
    int		err;

    alp_prv_mbiz_begin_proc(cmd_setup_call_callback);

    if (objectP == NULL)
	return TRUE;

    switch (tag)
    {
	case SAT_COMMAND_DETAILS:
	    setUpCallP->condition = COMMAND_QUALIFIER;
	    text = icon = 0;
	    break;

	case SAT_ALPHA_IDENTIFIER:
	    {
		char size = (char) (strlen((char*) objectP) + 1);

		if (text == 0)
		{
		    setUpCallP->userConfTextSize = size;
		    err = get_cmd_buffer(paramsP, objectP, size, (char**) &setUpCallP->userConfTextP);
		    text = 1;
		}
		else
		{
		    setUpCallP->callEstaTextSize = size;
		    err = get_cmd_buffer(paramsP, objectP, size, (char**) &setUpCallP->callEstaTextP);
		    icon = 1;
		}

		if (err)
		    return err;

		break;
	    }

	case SAT_ICON_IDENTIFIER:
	    if (icon == 0)
	    {
		setUpCallP->userConfExplicitIcon = (objectP[0] & NON_EXPLICIT_ICON) == 0;
		setUpCallP->userConfIconId = objectP[1];
		icon = 1;
	    }
	    else
	    {
		setUpCallP->callEstaExplicitIcon = (objectP[0] & NON_EXPLICIT_ICON) == 0;
		setUpCallP->callEstaIconId = objectP[1];
	    }
	    break;

	case SAT_ADDRESS:
	    setUpCallP->numberSize = strlen(((char*) (objectP)) + 1) + 1; // bypass the TON/NPI byte
	    if ((err = get_cmd_buffer(paramsP, objectP, setUpCallP->numberSize, (char**) &setUpCallP->numberP)) != TRUE)
		return err;
	    text = 1;
	    break;

	case SAT_CAPABILILITY_CONFIGURATION_PARAMETERS:
	    setUpCallP->bearerCapSize = (char) (strlen((char*) objectP) + 1);
	    if ((err = get_cmd_buffer(paramsP, objectP, setUpCallP->bearerCapSize, &setUpCallP->bearerCapP)) != TRUE)
		return err;
	    break;

	case SAT_SUBADDRESS:
	    setUpCallP->subAddressSize = (char) (strlen((char*) objectP) + 1);
	    if ((err = get_cmd_buffer(paramsP, objectP, setUpCallP->subAddressSize, (char**) &setUpCallP->subAddressP)) != TRUE)
		return err;
	    break;

	case SAT_DURATION:
	    {
		switch (objectP[0])	// switch on time unit
		{
		    case 0: // minutes
			setUpCallP->duration = (int) objectP[1] * 60000;
			break;

		    case 1: // seconds
			setUpCallP->duration = (int) objectP[1] * 1000;
			break;

		    case 2: // tenths of seconds
			setUpCallP->duration = (int) objectP[1] * 100;
			break;

		    default:
			break;
		}
		break;
	    }

	default:
	    break;
    }

    return TRUE;
}

/***********************************************************************
 * FUNCTION:	cmd_launch_browser_callback
 ***********************************************************************/

int cmd_launch_browser_callback(char tag, SatApduDecoder* decoderP, char* objectP)
{

    SatCmdParams*		paramsP			= (SatCmdParams*) decoderP->HLDataP;
    SatLaunchBrowser*	launchBrowserP	= (SatLaunchBrowser*) paramsP->cmdParamP;
    static char*			filePathCursorP;
    int				err = TRUE;

    alp_prv_mbiz_begin_proc(cmd_launch_browser_callback);

    if (objectP == NULL)
	return TRUE;

    switch (tag)
    {
	case SAT_COMMAND_DETAILS:
	    launchBrowserP->condition = COMMAND_QUALIFIER;
	    break;

	case SAT_BROWSER_IDENTITY:
	    launchBrowserP->browserId = objectP[0];
	    break;

	case SAT_URL:
	    launchBrowserP->urlSize = (char) strlen((char*) objectP) + 1;
	    err = get_cmd_buffer(paramsP, objectP, launchBrowserP->urlSize, (char**) &launchBrowserP->urlP);
	    break;

	case SAT_BEARER:
	    launchBrowserP->prefBearerCount = decoderP->currentObjectLength;
	    err = get_cmd_buffer(paramsP, objectP, launchBrowserP->prefBearerCount, &launchBrowserP->prefBearersP);
	    break;

	case SAT_PROVISIONING_FILE_REFERENCE:
	    launchBrowserP->fileIdCount++;
	    if (launchBrowserP->fileIdCount == 1)
	    {
		// allocate a max size for file paths
		launchBrowserP->filePathP = cmd_param_alloc_head(paramsP, 256);
		if (launchBrowserP->filePathP == NULL)
		{
		    err = -1;
		    break;
		}
		filePathCursorP = (char*) launchBrowserP->filePathP;
	    }
	    memcpy(filePathCursorP, objectP, decoderP->currentObjectLength);
	    filePathCursorP += decoderP->currentObjectLength;
	    break;

	case SAT_TEXT_STRING:
	    launchBrowserP->gatewaySize = strlen((char*) objectP + 1);
	    err = get_cmd_buffer(paramsP, objectP, launchBrowserP->gatewaySize, (char**) &launchBrowserP->gatewayP);
	    break;

	case SAT_ALPHA_IDENTIFIER:
	case SAT_ICON_IDENTIFIER:
	    err = cmd_generic_callback(tag, decoderP, objectP);
	    break;

	default:
	    break;
    }

    return err;
}

/***********************************************************************
 * FUNCTION:	cmd_play_tone_callback
 ***********************************************************************/

int cmd_play_tone_callback(char tag, SatApduDecoder* decoderP, char* objectP)
{

    SatCmdParams*	paramsP		= (SatCmdParams*) decoderP->HLDataP;
    SatPlayTone*	playToneP	= (SatPlayTone*) paramsP->cmdParamP;
    int			err			= TRUE;

    alp_prv_mbiz_begin_proc(cmd_play_tone_callback);

    err = cmd_generic_callback(tag, decoderP, objectP);

    if (objectP == NULL)
	return TRUE;

    switch (tag)
    {
	case SAT_COMMAND_DETAILS:
	    playToneP->vibrateAlert = COMMAND_QUALIFIER;
	    break;

	case SAT_TONE:
	    playToneP->sndCode = objectP[0];
	    break;

	case SAT_DURATION:
	    switch (objectP[0])	// switch on time unit
	    {
		case 0: // minutes
		    playToneP->sndDuration = (int) objectP[1] * 60000;
		    break;

		case 1: // seconds
		    playToneP->sndDuration = (int) objectP[1] * 1000;
		    break;

		case 2: // tenths of seconds
		    playToneP->sndDuration = (int) objectP[1] * 100;
		    break;

		default:
		    break;
	    }
	    break;

	default:
	    break;
    }

    return err;
}

/***********************************************************************
 * FUNCTION:	cmd_send_short_message_callback
 ***********************************************************************/

int cmd_send_short_message_callback(char tag, SatApduDecoder* decoderP, char* objectP)
{

    SatCmdParams*			paramsP				= (SatCmdParams*) decoderP->HLDataP;
    SatSendShortMessage*	sendShortMessageP	= (SatSendShortMessage*) paramsP->cmdParamP;
    int					err					= TRUE;

    alp_prv_mbiz_begin_proc(cmd_send_short_message_callback);

    err = cmd_generic_callback(tag, decoderP, objectP);

    if (objectP == NULL)
	return TRUE;

    switch (tag)
    {
	case SAT_COMMAND_DETAILS:
	    sendShortMessageP->packingRequired = (COMMAND_QUALIFIER & 1) == 1;
	    break;

	case SAT_ADDRESS:
	    sendShortMessageP->addressSize = strlen((char*) objectP) + 1;
	    if ((err = get_cmd_buffer(paramsP, objectP, sendShortMessageP->addressSize, (char**) &sendShortMessageP->addressP)) != TRUE)
		return err;
	    break;

	case SAT_SMS_TPDU:
	    sendShortMessageP->TPDUSize = decoderP->currentObjectLength;
	    if ((err = get_cmd_buffer(paramsP, objectP, sendShortMessageP->TPDUSize, &sendShortMessageP->TPDUP)) != TRUE)
		return err;
	    break;

	default:
	    break;
    }

    return err;
}

/***********************************************************************
 * FUNCTION:	cmd_send_callback
 ***********************************************************************/

int cmd_send_callback(char tag, SatApduDecoder* decoderP, char* objectP)
{

    SatCmdParams*	paramsP	= (SatCmdParams*) decoderP->HLDataP;
    SatBuffer*		bufferP	= (SatBuffer*) paramsP->cmdParamP;
    int			err		= TRUE;

    alp_prv_mbiz_begin_proc(cmd_send_callback);

    err = cmd_generic_callback(tag, decoderP, objectP);

    if (objectP == NULL)
	return TRUE;

    switch (tag)
    {
	case SAT_SS_STRING:
	case SAT_USSD_STRING:
	case SAT_DTMF_STRING:
	case SAT_AT_COMMAND:
	    bufferP->bufferSize = decoderP->currentObjectLength;
	    if ((err = get_cmd_buffer(paramsP, objectP, bufferP->bufferSize, &bufferP->bufferP)) != TRUE)
		return err;
	    break;
    }

    return err;
}


/***********************************************************************
 *
 * FUNCTION:	get_cmd_parameters
 *
 * DESCRIPTION: Retrieve the parameters of the currently running
 *				proactive command.
 *				Analyse parameters of received Proactive Command
 *
 * PARAMETERS:		=> ioDriverP	: Pointer to a driver
 *					=> ioParamsP	:
 *
 * RETURNED:	Error if any
 *
 ***********************************************************************/

int get_cmd_parameters(AlpPhoneDriver* ioDriverP, SatCmdParams* ioParamsP)
{
    SatApduDecoderCallback	commandCallback	= NULL;
    char*						commandTags	= NULL;
    int					err				= TRUE;

    if (ioDriverP->catCmdDataP == NULL)
    {
	TRACE(MSGL_VGSM_INFO, "get_cmd_parameters: ERROR: Invalid pointer, check why we got this."));
	err = -1;
	goto ERR;
    }

    switch (ioParamsP->cmdId)
    {
	//case SAT_CMD_SETUP_IDLE_MODE_TEXT:
	//commandCallback = cmd_generic_callback;
	//break;

	case SAT_CMD_DISPLAY_TEXT:
	    err = cmd_param_init(ioParamsP);
	    if (err)
		goto ERR;

	    if (cmd_param_alloc_head(ioParamsP, sizeof(SatDisplayText)) == NULL)
	    {
		err = -1;
		goto ERR;
	    }

	    commandCallback = cmd_display_text_callback;

	    break;

	case SAT_CMD_GET_INKEY:
	    err = cmd_param_init(ioParamsP);
	    if (err)
		goto ERR;

	    if (cmd_param_alloc_head(ioParamsP, sizeof(SatGetInkey)) == NULL)
	    {
		err = -1;
		goto ERR;
	    }

	    commandCallback = cmd_get_inkey_callback;

	    break;

	case SAT_CMD_PLAY_TONE:
	    err = cmd_param_init(ioParamsP);
	    if (err)
		goto ERR;

	    if (cmd_param_alloc_head(ioParamsP, sizeof(SatPlayTone)) == NULL)
	    {
		err = -1;
		goto ERR;
	    }

	    commandCallback = cmd_play_tone_callback;

	    break;


	case SAT_CMD_GET_INPUT:
	    err = cmd_param_init(ioParamsP);
	    if (err)
		goto ERR;

	    if (cmd_param_alloc_head(ioParamsP, sizeof(SatGetInput)) == NULL)
	    {
		err = -1;
		goto ERR;
	    }

	    commandCallback = cmd_get_input_callback;

	    break;

	case SAT_CMD_SELECT_ITEM:
	case SAT_CMD_SETUP_MENU:
	    {
		SatItemList* itemListP;

		if ((err = cmd_param_init(ioParamsP)) != TRUE)
		    goto ERR;

		if ((itemListP = cmd_param_alloc_head(ioParamsP, sizeof(SatItemList))) == NULL)
		{
		    err = -1;
		    goto ERR;
		}

		itemListP->itemCount=0;

		commandCallback = cmd_select_item_callback;

		break;
	    }

	case SAT_CMD_REFRESH:

	    err = cmd_param_init(ioParamsP);
	    if (err)
		goto ERR;
	    if (cmd_param_alloc_head(ioParamsP, sizeof(SatRefreshFileList)) == NULL)
	    {
		err = -1;
		goto ERR;
	    }

	    commandCallback = cmd_refresh_callback;

	    break;

	case SAT_CMD_SETUP_EVENTLIST:

	    err = cmd_param_init(ioParamsP);
	    if (err)
		goto ERR;

	    if (cmd_param_alloc_head(ioParamsP, sizeof(SatSetUpEventList)) == NULL)
	    {
		err = -1;
		goto ERR;
	    }

	    commandCallback = cmd_setup_event_list_callback;

	    break;

	case SAT_CMD_SETUP_CALL:

	    err = cmd_param_init(ioParamsP);
	    if (err)
		goto ERR;

	    if (cmd_param_alloc_head(ioParamsP, sizeof(SatSetUpCall)) == NULL)
	    {
		err = -1;
		goto ERR;
	    }

	    commandCallback = cmd_setup_call_callback;


	    break;

	case SAT_CMD_LAUNCH_BROWSER:

	    err = cmd_param_init(ioParamsP);
	    if (err)
		goto ERR;

	    if (cmd_param_alloc_head(ioParamsP, sizeof(SatLaunchBrowser)) == NULL)
	    {
		err = -1;
		goto ERR;
	    }

	    commandCallback = cmd_launch_browser_callback;

	    break;

	case SAT_CMD_SEND_SHORT_MESSAGE:

	    err = cmd_param_init(ioParamsP);
	    if (err)
		goto ERR;

	    if (cmd_param_alloc_head(ioParamsP, sizeof(SatSendShortMessage)) == NULL)
	    {
		err = -1;
		goto ERR;
	    }

	    commandCallback = cmd_send_short_message_callback;

	    break;

	case SAT_CMD_SEND_USSD:
	case SAT_CMD_SEND_DTMF:
	case SAT_CMD_RUN_AT_COMMAND:
	case SAT_CMD_SEND_SS:

	    err = cmd_param_init(ioParamsP);
	    if (err)
		goto ERR;

	    if (cmd_param_alloc_head(ioParamsP, sizeof(SatBuffer)) == NULL)
	    {
		err = -1;
		goto ERR;
	    }

	    commandCallback = cmd_send_callback;

	    break;


	default:
	    TRACE(MSGL_VGSM_INFO, "get_cmd_parameters: Unsupported proactive command (%hu).", (uint16_t) ioParamsP->cmdId));
	    err = -1;
	    goto ERR;
    }

    err = sat_apdu_decode(((AlpPDrvMbizCatData*) ioDriverP->catCmdDataP)->buffer, ((AlpPDrvMbizCatData*) ioDriverP->catCmdDataP)->bufferSize, commandTags, commandCallback, ioParamsP);

    //get the real size used by the proactive command parameters
    ioParamsP->cmdParamSize	= get_cmd_param_size(ioParamsP);
    ioParamsP->cmdParamP	= realloc(ioParamsP->cmdParamP,ioParamsP->cmdParamSize);

    return err;

ERR:
    TRACE(MSGL_VGSM_INFO, "get_cmd_parameters: state %u failed with error (0x%08lX).", ioDriverP->state, err));
    TRACE(MSGL_VGSM_INFO, "get_cmd_parameters: ioParamsP->cmdParamP=%lx).",ioParamsP->cmdParamP ));

    return err;
}


/***********************************************************************
 *
 * FUNCTION:	set_cmd_response_callback
 *
 * DESCRIPTION: Make a specific response according to the currently
 *				running proactive command to send it to UICC.
 *				that is, Send Terminal Response in 3GPP spec
 *
 *
 * PARAMETERS:		=> ioDriverP	: Pointer to a driver
 *					=> iResponseP	:
 *
 * RETURNED:	Error if any
 ***********************************************************************/

static int set_cmd_response_callback(int request, char* bufferP, int16_t* bufferSize, MbizSetCmdResponseParamsType* paramsP)
{
    int err = TRUE;

    switch (request)
    {
	case SAT_GET_DATA_FORMAT:
	    {
		char* writeP = MbizPrvSetCmdResponseFormat + 3;
		switch (paramsP->responseP->cmdId)
		{
		    case SAT_CMD_GET_INKEY:
		    case SAT_CMD_GET_INPUT:
			if ((paramsP->responseP->resCode & 0xF0) == 0)
			    *writeP++ = SAT_TEXT_STRING;
			break;

		    case SAT_CMD_SELECT_ITEM:
			if ((paramsP->responseP->resCode & 0xF0) == 0)
			    *writeP++ = SAT_ITEM_IDENTIFIER;
			break;
		}
		*((char**) bufferP) = MbizPrvSetCmdResponseFormat;
		*bufferSize = writeP - MbizPrvSetCmdResponseFormat;
		break;
	    }

	case SAT_COMMAND_DETAILS:
	    {
		char* objectP = paramsP->proactiveCmdP;
		char tag;

		*bufferSize = 3;

		if (objectP == NULL)
		    err = -1;
		else
		{
		    // get the original
		    objectP += 4 + (objectP[1] == 0x81);
		    tag = objectP[-2];
		    *bufferP++ = *objectP++;
		    *bufferP++ = *objectP++;
		    *bufferP++ = *objectP;
		    // The CR tag must be send back to the encoder in 4th byte
		    *bufferP++ = tag;
		}
		break;
	    }

	case SAT_DEVICE_IDENTITIES:
	    *bufferSize = 2;
	    bufferP[0] = SAT_DEVICE_ME;
	    bufferP[1] = SAT_DEVICE_SIM;
	    break;

	case SAT_RESULT:
	    *bufferSize = 2;
	    bufferP[0] = paramsP->responseP->resCode;
	    bufferP[1] = paramsP->responseP->addInfo;
	    break;

	case SAT_TEXT_STRING:
	    {
		char** dataPP = (char**) bufferP;
		char* dataP;

		// Here the buffer MUST be allocated with malloc.
		// The APDU encoder will free it
		*bufferSize = paramsP->responseP->respSize + 1;
		dataP = *dataPP = malloc(*bufferSize);
		if (dataP)
		{
		    dataP[0] = paramsP->responseP->respType;
		    memcpy(dataP + 1, paramsP->responseP->respP, paramsP->responseP->respSize);
		}
		else
		    err = -1;
		break;
	    }

	case SAT_ITEM_IDENTIFIER:
	    *bufferSize = 1;
	    *bufferP = paramsP->responseP->itemIdentifier;
	    break;

	default:
	    err = -1;
	    break;
    }

    return err;
}

/***********************************************************************
 *
 * FUNCTION:	set_cmd_response
 *
 * DESCRIPTION: Send a specific response according to the currently
 *				running proactive command.
 *				that is, Send Terminal Response in 3GPP spec
 *				This fuction is only for calling by CAT Brower
 *
 * PARAMETERS:		=> ioDriverP	: Pointer to a driver
 *					=> iResponseP:
 *
 *
 * RETURNED:	Error if any
 ***********************************************************************/

int set_cmd_response(AlpPhoneDriver* ioDriverP, SatCmdResponse* iResponseP)
{
    MbizSetCmdResponseParamsType		params;

    char *		apduStrP;
    char *		apduP;
    int		apduLen;
    int	err = TRUE;

    char* 		raw_packet = NULL;
    int 			raw_packet_size=0;

    alp_prv_mbiz_begin_proc(set_cmd_response);

    TRACE(MSGL_VGSM_INFO, "set_cmd_response: State (%u).", ioDriverP->state);


    // Send a Terminal Response to UICC
    switch (ioDriverP->state)
    {
	case 0: //  send the command
	    if (ioDriverP->catCmdDataP == NULL)
	    {
		TRACE(MSGL_VGSM_INFO, "set_cmd_response: ERR!! There is no SAT payload.");
		return ALP_STATUS_TEL_INVALID_PARAMETER;
	    }

	    params.proactiveCmdP	= ((AlpPDrvMbizCatData*) ioDriverP->catCmdDataP)->buffer;
	    params.responseP		= iResponseP;

	    TRACE(MSGL_VGSM_INFO, "set_cmd_response: reponse");

	    ALP_TM(B(printf_CLASS, (char*)iResponseP , sizeof(SatCmdResponse)));

	    TRACE(MSGL_VGSM_INFO, "set_cmd_response: %x %lx %x %x %x %x",iResponseP->itemIdentifier,iResponseP->respSize,iResponseP->cmdId,iResponseP->respType,iResponseP->resCode,iResponseP->addInfo);

	    if ((err = sat_apdu_encode(SAT_RESPONSE, (SatApduEncoderCallback) set_cmd_response_callback, &params, &apduStrP)) != TRUE)
	    {
		TRACE(MSGL_VGSM_INFO, "set_cmd_response: sat_apdu_encode failed with error (0x%08lX).", err);

		return err;
		break;
	    }

	    if ((err = str_to_bin(apduStrP, &apduP, &apduLen)) != TRUE)
	    {
		TRACE(MSGL_VGSM_INFO, "set_cmd_response: str_to_bin failed with error (0x%08lX).", err));

		return err;
		break;
	    }


	    // Make packet with encoded apdu
	    if ((err = TxSAT_GetProactiveCommand(&raw_packet,&raw_packet_size, apduP, apduLen)) != TRUE)
	    {
		TRACE(MSGL_VGSM_INFO, "set_cmd_response: TxSAT_GetProactiveCommand failed with error (0x%08lX).", err));

		return err;
		break;
	    }

	    // Send the command
	    if ((err = alp_prv_comm_send_at_cmd(ioDriverP, ioDriverP->receiveTimeoutMs, raw_packet,raw_packet_size)) != TRUE)
	    {
		TRACE(MSGL_VGSM_INFO, "set_cmd_response: alp_prv_comm_send_at_cmd failed with error (0x%08lX).", err));

		return err;
		break;
	    }
	    // Set internal to 0
	    ioDriverP->internal = 0;

	    if(apduP)		// This variable is malloc on str_to_bin()
		free(apduP);
	    if(apduStrP)	// This variable is malloc on sat_apdu_encode()
		free(apduStrP);

	    // Remove previous SAT payload because sending TR for it is successfull.
	    if (ioDriverP->catCmdDataP)
	    {
		free(ioDriverP->catCmdDataP);
		ioDriverP->catCmdDataP=NULL;
		TRACE(MSGL_VGSM_INFO, "set_cmd_response: free ioDriverP->catCmdDataP, %lx",ioDriverP->catCmdDataP));
	    }

	    ioDriverP->state++;


	case 1: // waiting for response for each cmdId
	    // 0x0E SIM Toolkit Commands
	    // 0x03 Proactive Command
	    if( (err = alp_prv_analyse_response(ioDriverP,0x0E, 0x03,NULL, NULL))  != TRUE)
		TRACE(MSGL_VGSM_INFO, "set_cmd_response: response fail!"));
	    else
		TRACE(MSGL_VGSM_INFO, "set_cmd_response: response O.K."));

	    // Command is finished
	    ioDriverP->state = 0;

	    return err;
	    break;

	default:
	    TRACE(MSGL_VGSM_INFO, "set_cmd_response: Invalid state."));
	    return -1;
    }


    return err;

}




/***********************************************************************
 *
 * FUNCTION:	menu_selection
 *
 * DESCRIPTION: Notify the card to launch an application or to provide
 *				its help information if any.
 *
 * PARAMETERS:		=> ioDriverP	: Pointer to a driver
 *					=> iSelectionP	:
 *
 * RETURNED:	Error if any
 ***********************************************************************/


/***********************************************************************
 *
 * FUNCTION:	call_action
 *
 * DESCRIPTION: Inform the card if the user accepted/rejected to set up
 *				the call according to the given action.
 *				Terminal Response for SETUP CALL proactive cmd
 *
 * PARAMETERS:		=> ioDriverP	: Pointer to a driver
 *					=> iAction		:
 *
 * RETURNED:	Error if any
 ***********************************************************************/

int call_action(AlpPhoneDriver* ioDriverP, char iAction)
{
    int	err = TRUE;

    alp_prv_mbiz_begin_proc(call_action);

    TRACE(MSGL_VGSM_INFO, "call_action: State (%u).", ioDriverP->state));


    if (iAction == SAT_CALL_ACCEPT)
	TRACE(MSGL_VGSM_INFO, "call_action: should call, not yet"));
    else
	TRACE(MSGL_VGSM_INFO, "call_action: release call, not yet"));

    return err;
}

static int event_download_callback(int request, char* bufferP, int16_t* bufferSize, SatEventToCard* eventP)
{
    int err = TRUE;

    alp_prv_mbiz_begin_proc(event_download_callback);

    switch (request)
    {
	case SAT_GET_DATA_FORMAT:
	    *((char**) bufferP) = MbizPrvEventDownloadFormat;
	    switch (eventP->evtCode)
	    {
		case SAT_EVENT_USER_ACTIVITY:
		    *bufferSize = 2;
		    break;

		case SAT_EVENT_IDLE_SCREEN_AVAILABLE:
		    *bufferSize = 2;
		    break;

		case SAT_EVENT_LANGUAGE_SELECTION:
		    *bufferSize = 3;
		    MenSelectionFormat[2] = SAT_LANGUAGE;
		    break;

		case SAT_EVENT_BROWSER_TERMINATION:
		    *bufferSize = 3;
		    MenSelectionFormat[2] = SAT_BROWSER_TERMINATION_CAUSE;
		    break;
	    }
	    break;

	case SAT_DEVICE_IDENTITIES:
	    *bufferSize = 2;
	    if (eventP->evtCode == SAT_EVENT_IDLE_SCREEN_AVAILABLE)
		bufferP[0] = SAT_DEVICE_DISPLAY;
	    else
		bufferP[0] = SAT_DEVICE_ME;
	    bufferP[1] = SAT_DEVICE_SIM;
	    break;

	case SAT_EVENT_LIST:
	    bufferP[0] = eventP->evtCode;
	    *bufferSize = 1;
	    break;

	case SAT_LANGUAGE:
	    *bufferSize = 2;
	    bufferP[0] = eventP->lanCode[0];
	    bufferP[1] = eventP->lanCode[1];
	    break;

	case SAT_BROWSER_TERMINATION_CAUSE:
	    *bufferSize = 1;
	    bufferP[0] = eventP->browserTerminationCause;
	    break;

	default:
	    err = -1;
    }

    return err;
}


/***********************************************************************
 *
 * FUNCTION:	notify_card_of_event
 *
 * DESCRIPTION: Notify the card of an event that has occurred on the
 *				PDA according to the event selected
 *
 * PARAMETERS:		=> ioDriverP	: Pointer to a driver
 *					=> iEventP		:
 *
 * RETURNED:	Error if any
 ***********************************************************************/

int notify_card_of_event(AlpPhoneDriver* ioDriverP, SatEventToCard* iEventP)
{
    char*	bufferP;
    int	bufferSize;
    char*	apduP;
    char*	apduStrP;
    int	apduLen;

    char* 	raw_packet = NULL;
    int 		raw_packet_size=0;

    int	err = TRUE;

    alp_prv_mbiz_begin_proc(notify_card_of_event);

    TRACE(MSGL_VGSM_INFO, "notify_card_of_event: received event by CAT Browser"));

    ALP_TM(B(printf_CLASS, (char*)iEventP , sizeof(SatEventToCard)));

    switch (ioDriverP->state)
    {
	case 0:
	    if ((err = sat_apdu_encode(SAT_EVENT_DOWNLOAD, (SatApduEncoderCallback) event_download_callback, iEventP, &apduStrP)) != TRUE)
	    {
		TRACE(MSGL_VGSM_INFO, "notify_card_of_event: sat_apdu_encode failed with error (0x%08lX).", err));
		return err;
		break;
	    }

	    if ((err = str_to_bin(apduStrP, &apduP, &apduLen)) != TRUE)
	    {
		TRACE(MSGL_VGSM_INFO, "notify_card_of_event: str_to_bin failed with error (0x%08lX).", err));
		return err;
		break;
	    }

	    // Make packet with encoded apdu
	    if ((err = TxSAT_SetEventDownload(&raw_packet,&raw_packet_size, apduP, apduLen)) != TRUE)
	    {
		TRACE(MSGL_VGSM_INFO, "notify_card_of_event: TxSAT_SetEventDownload failed with error (0x%08lX).", err));
		return err;
		break;
	    }

	    ALP_TM(B(printf_CLASS, raw_packet,raw_packet_size));

	    // Send the command
	    if ((err = alp_prv_comm_send_at_cmd(ioDriverP, ioDriverP->receiveTimeoutMs, raw_packet,raw_packet_size)) != TRUE)
	    {
		TRACE(MSGL_VGSM_INFO, "notify_card_of_event: alp_prv_comm_send_at_cmd failed with error (0x%08lX).", err));
		return err;
		break;
	    }
	    // Set internal to 0
	    ioDriverP->internal = 0;

	    if(apduP)		// This variable is malloc on str_to_bin()
		free(apduP);
	    if(apduStrP)	// This variable is malloc on sat_apdu_encode()
		free(apduStrP);

	    ioDriverP->state++;


	case 1: // waiting for general response
	    // 0x0E : SIM Toolkit Commands
	    // 0x05 : Event Download
	    if( (err = alp_prv_analyse_response(ioDriverP,0x0E, 0x05,NULL, NULL))  != TRUE)
		TRACE(MSGL_VGSM_INFO, "notify_card_of_event: response fail!"));
	    else
		TRACE(MSGL_VGSM_INFO, "notify_card_of_event: response O.K."));

	    // Command is finished
	    ioDriverP->state = 0;

	    return err;
	    break;

	default:
	    TRACE(MSGL_VGSM_INFO, "notify_card_of_event: Invalid state."));
	    return -1;
	    break;
    }

    return err;

}

// lucky29
/***********************************************************************
 *
 * FUNCTION:	prv_card_analyse_crsm
 *
 * DESCRIPTION:
 *
 * PARAMETERS:		=> ioDriverP	: Pointer to a driver
 *					=> ioFileP		:
 *
 * RETURNED:	Error if any
 ***********************************************************************/

int prv_card_analyse_crsm(AlpPhoneDriver* ioDriverP, AlpTelCardFile* ioFileP)
{
    int	err;

    char			*bufP, *tmpBufP;
    char			cmdType=0;
    int			bufSize, tmpBufSize, val;
    int		value;

    alp_prv_mbiz_begin_proc(prv_card_analyse_crsm);

    // Analyse the data
    // +CRSM: <sw1>,<sw2>[,<response>]

    /*
       if ((err = alp_pdrv_analyse_buffer(ioDriverP, &bufP, &bufSize, kCRSM, prv_analyse_line)) != TRUE)
       {
       TRACE(MSGL_VGSM_INFO, "prv_card_analyse_crsm: alp_pdrv_analyse_buffer failed with error (0x%08lX).", err));
       return err;
       }
     */
    // 0x05 : Security - SIM control Commands
    // 0x05 : +CRSM, Restricted SIM Access Message
    if ((err = alp_prv_analyse_response(ioDriverP, 0x05, 0x05,RxSEC_ResRSIMAccess,ioFileP)) != TRUE)
    {
	TRACE(MSGL_VGSM_INFO, "alp_prv_mbiz_spc_initiate_call: alp_prv_analyse_response() failed with error (0x%08lX).", err));
	break;
    }

    TRACE(MSGL_VGSM_INFO, "IN prv_card_analyse_crsm "));

    switch (ioFileP->fileStruct)
    {
	case SAT_CARD_FILE_STRUCT_TRANSPARENT:	cmdType = VGSM_CARD_COMMAND_INS_READ_BINARY; break;
	case SAT_CARD_FILE_STRUCT_LINEAR_FIXED:
	case SAT_CARD_FILE_STRUCT_CYCLIC:		cmdType = VGSM_CARD_COMMAND_INS_READ_RECORD; break;
	case (char) -1:								cmdType = VGSM_CARD_COMMAND_INS_GET_RESPONSE; break;
	default:										return ALP_STATUS_TEL_INVALID_INDEX;
    }

    // Get <sw1>
    if ((err = alp_pdrv_get_int(bufP, bufSize, &value)) != TRUE)
    {
	TRACE(MSGL_VGSM_INFO, "prv_card_analyse_crsm: alp_pdrv_get_int failed with error (0x%08lX).", err));
	goto failed;
    }

    TRACE(MSGL_VGSM_INFO, "prv_card_analyse_crsm: Response code (%lu).", value));

    if (value != VGSM_CARD_RESPONSE_NORMAL_ENDING)
    {
	TRACE(MSGL_VGSM_INFO, "prv_card_analyse_crsm: Get info failed."));

	if (value == VGSM_CARD_RESPONSE_REFERENCING_ERROR)
	    return ALP_STATUS_TEL_CARD_FILE_NOT_FOUND;
	else
	    return -1;
    }

    // Remove the ','
    if ((err = alp_pdrv_search_char(&bufP, &bufSize, ',')) != TRUE)
    {
	TRACE(MSGL_VGSM_INFO, "prv_card_analyse_crsm: alp_pdrv_search_char failed with error (0x%08lX).", err));
	goto failed;
    }
    bufP++;
    bufSize--;

    // Skip <sw2>, remove the ','
    if ((err = alp_pdrv_search_char(&bufP, &bufSize, ',')) != TRUE)
    {
	TRACE(MSGL_VGSM_INFO, "prv_card_analyse_crsm: alp_pdrv_search_char failed with error (0x%08lX).", err));
	goto failed;
    }
    bufP++;
    bufSize--;

    // Get <response>
    tmpBufP = bufP;
    tmpBufSize = bufSize;
    // Copy the response into the buffer and return
    if ((err = alp_pdrv_search_char(&tmpBufP, &tmpBufSize, '\x0D')) != TRUE)
    {
	TRACE(MSGL_VGSM_INFO, "prv_card_analyse_crsm: alp_pdrv_search_char failed with error (0x%08lX).", err));
	goto failed;
    }

    tmpBufSize = (tmpBufP - bufP);
    if ( (tmpBufSize/2) > ioFileP->bufSize )
    {
	err = ALP_STATUS_TEL_BUFFER_SIZE;
	TRACE(MSGL_VGSM_INFO, "prv_card_analyse_crsm: failed because ioFileP->bufSize=%ld and tmpBufSize=%ld ", ioFileP->bufSize,tmpBufSize/2));

	goto failed;
    }

    val = ioFileP->bufSize;
    if ((err = alp_pdrv_hexadecimal_string_to_bytes((char*)bufP, bufSize, ioFileP->bufP+ioFileP->byteCount, &val)) != TRUE)
    {
	TRACE( "prv_card_analyse_crsm: alp_pdrv_hexadecimal_string_to_bytes failed with error (0x%08lX).", err));
	goto failed;
    }

    if (cmdType == VGSM_CARD_COMMAND_INS_GET_RESPONSE)
    {
	ioFileP->fileSize = (ioFileP->bufP[2]*256) + ioFileP->bufP[3];
	ioFileP->fileStruct = ioFileP->bufP[13];
	if (ioFileP->fileStruct != SAT_CARD_FILE_STRUCT_TRANSPARENT)
	    ioFileP->recSize  = ioFileP->bufP[14];
    }
    else if( (cmdType == VGSM_CARD_COMMAND_INS_READ_RECORD) && (ioFileP->mode == SAT_CARD_MODE_READ_FILE) )
    {
	ioFileP->recId++;
    }

    // Update byte counter
    ioFileP->byteCount += val;
    ioFileP->bufSize-=val;

    // Remove the line
    alp_pdrv_comm_remove_line(ioDriverP);

failed:

    return err;
}






#endif



