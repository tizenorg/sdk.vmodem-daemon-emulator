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

#ifndef _vgsm_pb_h_
#define _vgsm_pb_h_

#ifdef __cplusplus
extern "C"
{
#endif


typedef enum{
  GSM_PB_ST_DC=0x01,  	/* 0x01 : ME dialed calls list */
  GSM_PB_ST_EN,  		/* 0x02 : SIM ( or ME ) emergency number */
  GSM_PB_ST_FD,  		/* 0x03 : SIM fixed-dialing phonebook EF[FDN] */
  GSM_PB_ST_LD,  		/* 0x04 : SIM last-dialing phonebook */
  GSM_PB_ST_MC,  		/* 0x05 : ME missed calls list */
  GSM_PB_ST_ME,  		/* 0x06 : ME phonebook */
  GSM_PB_ST_MT,  		/* 0x07 : Combined ME and SIM phonebook */
  GSM_PB_ST_ON,  		/* 0x08 : SIM ( or ME ) own numbers ( MSISDNs ) list EF[MSISDN], */
  GSM_PB_ST_RC,  		/* 0x09 : ME received calls list */
  GSM_PB_ST_SIM,  		/* 0x0A : SIM phonebook EF[ADN], DF[PHONEBOOK] */
  GSM_PB_ST_SDN,        /* 0x0B : Service Dialing Number */
  GSM_PB_ST_3GSIM,		/*0x0C : 3G SIM Phonebook EF[ADN]*/
  GSM_PB_ST_ICI,			/*0x0D : Incoming Call Information*/
  GSM_PB_ST_OCI,		/*0x0E : Outgoing Call Information*/
  GSM_PB_ST_AAS,		/*0x0F : Additional Number Alpha String*/
  GSM_PB_ST_GAS,		/*0x10 : Grouping Information String*/
  GSM_PB_ST_MAX
} gsm_pb_storage_type_e_type;


/* MODE  field */
typedef enum{
	GSM_PB_MODE_READ	= 0x00,	/* 0x00 */
	GSM_PB_MODE_ADD,			/* 0x01 */
	GSM_PB_MODE_DEL,			/* 0x02 */
	GSM_PB_MODE_EDIT,			/* 0x03 */
	GSM_PB_MODE_DELETE_ALL,	/* 0x04 */
	GSM_PB_MODE_MAX
} gsm_pb_acc_mode_e_type;


typedef struct m_PB PB_;
struct m_PB {
	gsm_pb_storage_type_e_type db_st_type;
	char name[30];
	char number[20];
	char address[30];
};

/*********************************************************************************

                                            Sub Command

**********************************************************************************/
typedef enum{
  GSM_PB_ACCESS=0x01,     /* 0x01 : PhoneBook Access Message */
  GSM_PB_STORAGE,            /* 0x02 : PhoneBook Storage Message */
  GSM_PB_STORAGE_LIST,  /* 0x03 : PhoneBook Storage List Message */
  GSM_PB_ENTRY_INFO,      /* 0x04 : PhoneBook Entry Information Message */
  GSM_PB_CAPABILITY_INFO,      /* 0x05 : PhoneBook Grouping List Message */
  GSM_PB_MAX
} gsm_pb_sub_cmd_type;


#if 0 
/*
typedef struct tagPB2G {
	unsigned char num_type;
	unsigned char num_len;
	unsigned char txt_len;
	char number[256];
	char text[256];
} GSM_PhoneBook2G;

typedef struct tagPB3GGrp {
	GSM_PB_GRP_TYPE grp_type;
	unsigned char grp_len;
	unsigned char num_type;
	char data[256];
} GSM_PhoneBook3GGrp;

typedef struct tagPB3G {
	unsigned char hidden_entry;
	unsigned char num_pb_grp;
	GSM_PhoneBook3GGrp pb_grp[10];
} GSM_PhoneBook3G;

typedef struct tagPBdata {
	GSM_PB_ACCESS_MODE mode;
	unsigned short index;
	GSM_PB_CARD pb_type;
	union {
		GSM_PhoneBook2G* ptr_pb2g;
		GSM_PhoneBook3G* ptr_pb3g;
	} pb;
} GSM_PhoneBookData;
*/

typedef struct {
	GSM_PB_ACCESS_MODE mode;
	unsigned short index;
	unsigned short next_idx; 
	GSM_PB_CARD pb_type;
	unsigned char len_number;
	unsigned char record_type;
	char number[256];
	unsigned char len_text;
	unsigned char text_encode;
	char text[256];
} GSM_PhoneBookData2G;

typedef struct tagPBStorageInfo {
	PB_STORAGE_TYPE storage_type;
	unsigned short total_count;
	unsigned short used_count;
} GSM_PhoneBookStorageInfo;

typedef struct tagPBEntryInfo {
	unsigned short pb_index_min;
	unsigned short pb_index_max;
	unsigned short pb_num_max;
	unsigned short pb_text_max;
} GSM_PhoneBookEntryInfo;

typedef struct tagPBGrpName {
	unsigned char grp_name_len;
	char grp_name[256];
} GSM_PhoneBookGroupName;

typedef struct tagPBGrpNameList {
	unsigned char num_grp;

	GSM_PhoneBookGroupName* grp_name_list;
} GSM_PhoneBookGroupNameList;

#endif 

#ifdef __cplusplus
}
#endif


#endif // __vgsm_pb_h_
