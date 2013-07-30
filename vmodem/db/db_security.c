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

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <glib.h>
#include <glib/gstdio.h>

#include "state.h"
#include "server_common_security.h"
#include "vgsm_sim.h"
#include "server_tx_security.h"
#include "db_security.h"
#include "fileio.h"

#define ALP_NOTIFY_DATABASE_DIR  "./db"
#define ALP_NOTIFY_DATABASE_FILE "sim.db"
#define ALP_NOTIFY_DATABASE_FILE_INFO "sim_info.db"
#define ALP_NOTIFY_DATABASE_FILE_DATA "sim_data.db"
#define ALP_NOTIFY_DATABASE_MODE (S_ISGID|S_ISVTX|S_IRWXU|S_IRWXG)

// ----------------------------------------------------------------------------
// sqlite3 specific code
// ----------------------------------------------------------------------------

#include <sqlite3.h>


struct db_sim_sql_callback_info
{
	unsigned char *data;
	int len;
};

// for boot
static int db_sim_info_restore_callback(void * ref, int ncol, char ** cols, char ** name)
{
	struct db_sim_sql_callback_info *info = ref;
	int i;

	info->len = atoi(cols[0]);

	SIM_DEBUG("db_sim_info_restore_callback ncol = %d len = %d \n", ncol, info->len);
	for(i=1;i<ncol;i++)
	{
		//SIM_DEBUG("%s, %x\n", name[i], atoi(cols[i]));
		info->data[i-1] = atoi(cols[i]);
		//SIM_DEBUG(" @@@%x\n", info->data[i-1]);
	}

	return 0;
}

static const char *db_table_from_fileid(gsm_sec_sim_file_id_t file_id)
{
	switch(file_id)
	{
	case GSM_SEC_EFILE_ICCID : return "Ticcid";
	case GSM_SEC_EFILE_USIM_LI : return "Tusim_li";
	case GSM_SEC_EFILE_ECC : return "Tecc";
	case GSM_SEC_EFILE_SST : return "Tsst";
	case GSM_SEC_EFILE_EST : return "Test";
	case GSM_SEC_EFILE_CPHS_CPHS_INFO : return "Tcphs";
	case GSM_SEC_EFILE_SPN : return "Tspn";
	case GSM_SEC_EFILE_IMSI : return "Timsi";
	default:
		return NULL;
	}
}

static int db_read_from_database(
		const char *filename, gsm_sec_sim_file_id_t file_id,
		unsigned char* data, int *len)
{
    sqlite3 * db = 0;
	int err;
	char * mesg;
	char dbname[256];
	char query[256];
	char *bin_path = get_bin_path();
	struct db_sim_sql_callback_info info;
	const char *table;

	sprintf(dbname,"%s/%s/%s", bin_path, ALP_NOTIFY_DATABASE_DIR, filename);

	if (bin_path)
		g_free(bin_path);

	SIM_DEBUG("\n");

	// open the database
	err = sqlite3_open(dbname,&db);
	if (err != SQLITE_OK)
	{
		SIM_DEBUG("sqlite3_open err=%d\n",err);
	    goto Done;
	}

	info.data = data;
	info.len = 0;

	// dump all the rows

	table = db_table_from_fileid(file_id);
	if (!table)
		goto Done;

	sprintf(query, "SELECT ALL * FROM %s", table);

	SIM_DEBUG("file_id = %04x  query = %s\n", file_id, query);

	err = sqlite3_exec(db, query, db_sim_info_restore_callback, &info, &mesg);

	*len = info.len;

    if (err != SQLITE_OK)
	{
		SIM_DEBUG("sqlite3_exec err=%d\n",err);

		if (mesg) {
			SIM_DEBUG("sqlite3_exec: %s\n",mesg);
			sqlite3_free(mesg);
		}
		goto Done;
	}

Done:
    if (db)
		sqlite3_close(db);

	return err == SQLITE_OK ? SIM_SUCCESS : err;
}

int _sim_get_file_info_tmp(gsm_sec_sim_file_id_t file_id,/* in */ unsigned char* pfile_info, /* out */ int *file_info_len/* out */)
{
	return db_read_from_database( ALP_NOTIFY_DATABASE_FILE_INFO, file_id, pfile_info, file_info_len);
}

int _sim_get_file_data_tmp(gsm_sec_sim_file_id_t file_id,/* in */ unsigned char* pfile_data, /* out */ int* file_data_len/* out */)
{
	return db_read_from_database( ALP_NOTIFY_DATABASE_FILE_DATA, file_id, pfile_data, file_data_len);
}

static int db_sim_info_callback(void * ref, int ncol, char ** cols, char ** name)		//called do_sim()
{
	int i, len;
	unsigned char *sim_info;

	SIM_DEBUG("\n\n1. in db_sim_info__callback \n\n");

	len = atoi(*(cols));

	sim_info = malloc(sizeof(unsigned char)*len);
	memset(sim_info, 0, len);

	SIM_DEBUG("ncol : %d \n", ncol);
	SIM_DEBUG("len : %d \n", len);
	for(i=1;i<ncol;i++)
	{
		SIM_DEBUG(" %s, %x\n", name[i], atoi(cols[i]));
		sim_info[i-1] = (char)atoi(cols[i]);
		SIM_DEBUG(" @@@%x\n", sim_info[i-1]);
	}

	server_tx_sim_info_get_db(sim_info, len);

	return 0;
}
///////////////////////////////////////////////////////////////////////////////////


// after 'select' sql statement,  put sec info(db info) into SimSecurity.
int db_sim_sec_restore_callback(void * ref, int ncol, char ** cols, char ** name)
{
	VGSM_SIMType * type = (VGSM_SIMType*)ref;

	SimSecurity *sim_sec;

	SIM_DEBUG("\n");

	sim_sec = malloc(sizeof(SimSecurity));
	memset(sim_sec,0,sizeof(SimSecurity));

	SIM_DEBUG("ncol : %d \n", ncol);
	SIM_DEBUG("pin_val =%s, puk_val = %s, pin2_val = %s, puk2_val = %s,pin_cnt = %d,pin2_cnt = %d,puk_cnt = %d,puk2_cnt = %d,sim_state = %d,facility_pin = %d,facility_fdn = %d lock_sc2_state = %d \n",
		              *cols,*(cols+1), *(cols+2),*(cols+3),atoi(*(cols+4)),atoi(*(cols+5)),atoi(*(cols+6)),atoi(*(cols+7)),atoi(*(cols+8)),atoi(*(cols+9)),atoi(*(cols+10)),atoi(*(cols+11)));

	memcpy(sim_sec->pin_value, *(cols), strlen(*(cols)));
	memcpy(sim_sec->puk_value, *(cols+1), strlen(*(cols+1)));
	memcpy(sim_sec->pin2_value, *(cols+2), strlen(*(cols+2)));
	memcpy(sim_sec->puk2_value, *(cols+3), strlen(*(cols+3)));

	sim_sec->pin_cnt			=atoi(*(cols+4));
	sim_sec->pin2_cnt			=atoi(*(cols+5));
	sim_sec->puk_cnt			=atoi(*(cols+6));
	sim_sec->puk2_cnt			=atoi(*(cols+7));
	sim_sec->sim_state			=atoi(*(cols+8));
	sim_sec->facility_pin		=atoi(*(cols+9));
	sim_sec->facility_fdn		=atoi(*(cols+10));
	sim_sec->lock_sc2_state		=atoi(*(cols+11));

	/*
	mem free is done in each case func
	*/
	switch(*type)
	{
		case VGSM_SIMTypeLoad:
			server_sec_set_sec_db_info(sim_sec, ncol);
			break;

		// it`s called by EI
		case VGSM_SIMTypeget:
			server_tx_sim_sec_get_db(sim_sec);
			break;
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////

//090502
// create the SIM info TABLE
int db_sim_info_create(void)
{
	sqlite3 * db = 0;
	int err;
	char * mesg;

	TRACE(MSGL_VGSM_INFO, "\n");

	char directory[256];
	char dbname[256];

	//char *dbname = ALP_NOTIFY_DATABASE_DIR "/" ALP_NOTIFY_DATABASE_FILE;
	//char *directory = ALP_NOTIFY_DATABASE_DIR;
	char *bin_path = get_bin_path();
	sprintf(directory,"%s/%s", bin_path, ALP_NOTIFY_DATABASE_DIR);
	sprintf(dbname,"%s/%s/%s", bin_path, ALP_NOTIFY_DATABASE_DIR, ALP_NOTIFY_DATABASE_FILE_INFO);

	if (bin_path)
		g_free(bin_path);

	/* make sure database directory exists */
	if (!g_file_test (directory, G_FILE_TEST_IS_DIR))
	{
		SIM_DEBUG("directory %s does not exit or is not directory\n", directory);

		/* check if exists and if so, attempt to remove non-directory */
		if (g_file_test (directory, G_FILE_TEST_EXISTS))
		{
			if (g_remove (directory) < 0)
			{
				SIM_DEBUG("unable to remove %s: %d %s\n",directory, errno, strerror(errno));

				err = errno;
				goto Done;
			}
		}

		/* create directory */
		if (g_mkdir_with_parents (directory, ALP_NOTIFY_DATABASE_MODE) < 0)
		{
			/* unable to create directory */
			SIM_DEBUG("unable to create %s: %d %s\n", directory, errno, strerror(errno));

			err = errno;
			goto Done;
		}
	}

	// open the database
	err = sqlite3_open(dbname,&db);

	if (err != SQLITE_OK)
	{
		SIM_DEBUG("db_sim_sec_create: sqlite3_open err=%d\n",err);
		goto Done;
	}

	// create the table
	err = sqlite3_exec(db,"CREATE TABLE Tusim_li ( len INT, usim_li1 INT, usim_li2 INT, usim_li3 INT, usim_li4 INT, usim_li5 INT, usim_li6 INT, usim_li7 INT, usim_li8 INT, usim_li9 INT, usim_li10 INT, usim_li11 INT, usim_li12 INT, usim_li13 INT, usim_li14 INT, usim_li15 INT, usim_li16 INT, usim_li17 INT, usim_li18 INT, usim_li19 INT, usim_li20 INT, usim_li21 INT, usim_li22 INT, usim_li23 INT, usim_li24 INT, usim_li25 INT, usim_li26 INT, usim_li27 INT, usim_li28 INT, usim_li29 INT, usim_li30 INT, usim_li31 INT, usim_li32 INT, usim_li33 INT)"
			,0,0,&mesg);

	err = sqlite3_exec(db,"CREATE TABLE Tecc ( len INT, ecc1 INT, ecc2 INT, ecc3 INT, ecc4 INT, ecc5 INT, ecc6 INT, ecc7 INT, ecc8 INT, ecc9 INT, ecc10 INT, ecc11 INT, ecc12 INT, ecc13 INT, ecc14 INT, ecc15 INT, ecc16 INT, ecc17 INT, ecc18 INT, ecc19 INT, ecc20 INT, ecc21 INT, ecc22 INT, ecc23 INT, ecc24 INT, ecc25 INT, ecc26 INT, ecc27 INT, ecc28 INT, ecc29 INT, ecc30 INT, ecc31 INT, ecc32 INT, ecc33 INT, ecc34 INT, ecc35 INT, ecc36 INT)"
			,0,0,&mesg);

	err = sqlite3_exec(db,"CREATE TABLE Tsst ( len INT,	sst1 INT, sst2 INT, sst3 INT, sst4 INT, sst5 INT, sst6 INT, sst7 INT, sst8 INT, sst9 INT, sst10 INT, sst11 INT, sst12 INT, sst13 INT, sst14 INT, sst15 INT, sst16 INT, sst17 INT, sst18 INT, sst19 INT, sst20 INT, sst21 INT, sst22 INT, sst23 INT, sst24 INT, sst25 INT, sst26 INT, sst27 INT, sst28 INT, sst29 INT, sst30 INT, sst31 INT, sst32 INT, sst33 INT)"
			,0,0,&mesg);

	err = sqlite3_exec(db,"CREATE TABLE Test ( len INT, est1 INT, est2 INT, est3 INT, est4 INT, est5 INT, est6 INT, est7 INT, est8 INT, est9 INT, est10 INT, est11 INT, est12 INT, est13 INT, est14 INT, est15 INT, est16 INT, est17 INT, est18 INT, est19 INT, est20 INT, est21 INT, est22 INT, est23 INT, est24 INT, est25 INT, est26 INT, est27 INT, est28 INT, est29 INT, est30 INT, est31 INT, est32 INT, est33 INT)"
			,0,0,&mesg);

	err = sqlite3_exec(db,"CREATE TABLE Tcphs ( len INT, cphs_info1 INT, cphs_info2 INT, cphs_info3 INT)",0,0,&mesg);

	err = sqlite3_exec(db,"CREATE TABLE Tspn ( len INT,	spn1 INT, spn2 INT, spn3 INT, spn4 INT, spn5 INT, spn6 INT, spn7 INT, spn8 INT, spn9 INT, spn10 INT, spn11 INT, spn12 INT, spn13 INT, spn14 INT, spn15 INT, spn16 INT, spn17 INT, spn18 INT, spn19 INT, spn20 INT, spn21 INT, spn22 INT, spn23 INT, spn24 INT, spn25 INT, spn26 INT, spn27 INT, spn28 INT, spn29 INT, spn30 INT, spn31 INT, spn32 INT)"
			,0,0,&mesg);

	err = sqlite3_exec(db,"CREATE TABLE Timsi ( len INT, imsi1 INT, imsi2 INT, imsi3 INT, imsi4 INT, imsi5 INT, imsi6 INT, imsi7 INT, imsi8 INT, imsi9 INT, imsi10 INT, imsi11 INT, imsi12 INT, imsi13 INT, imsi14 INT, imsi15 INT, imsi16 INT, imsi17 INT, imsi18 INT, imsi19 INT, imsi20 INT, imsi21 INT, imsi22 INT, imsi23 INT, imsi24 INT, imsi25 INT, imsi26 INT, imsi27 INT, imsi28 INT, imsi29 INT, imsi30 INT, imsi31 INT, imsi32 INT, imsi33 INT, imsi34 INT, imsi35 INT, imsi36 INT, imsi37 INT, imsi38 INT, imsi39 INT, imsi40 INT, imsi41 INT, imsi42 INT, imsi43 INT)"
			,0,0,&mesg);

	//err = sqlite3_exec(db,"CREATE TABLE sim_info ( usim_li VARCHAR(33), ecc VARCHAR(36), sst VARCHAR(33),est VARCHAR(33),cphs_cphs_info VARCHAR(3), spn VARCHAR(32), imsi VARCHAR(43))",0,0,&mesg);

	if (err != SQLITE_OK)
	{
		TRACE(MSGL_VGSM_ERR, "sqlite3_exec err=%d\n",err);

		if (mesg)
		{
			TRACE(MSGL_VGSM_ERR, "sqlite3_exec: %s\n",mesg);
			sqlite3_free(mesg);
		}
		goto Done;
	}
Done:
	// close the database
	if (db) sqlite3_close(db);

	// return status
	return err == SQLITE_OK ? SIM_SUCCESS: err;
}

//090502
// create the SIM data TABLE
int db_sim_data_create(void)
{
	sqlite3 * db = 0;
	int err;
	char * mesg;

	TRACE(MSGL_VGSM_INFO, "\n");

	char directory[256];
	char dbname[256];

	//char *dbname = ALP_NOTIFY_DATABASE_DIR "/" ALP_NOTIFY_DATABASE_FILE;
	//char *directory = ALP_NOTIFY_DATABASE_DIR;
	char *bin_path = get_bin_path();
	sprintf(directory,"%s/%s", bin_path, ALP_NOTIFY_DATABASE_DIR);
	sprintf(dbname,"%s/%s/%s", bin_path, ALP_NOTIFY_DATABASE_DIR, ALP_NOTIFY_DATABASE_FILE_DATA);

	if (bin_path)
		g_free(bin_path);

	/* make sure database directory exists */
	if (!g_file_test (directory, G_FILE_TEST_IS_DIR))
	{
		SIM_DEBUG("directory %s does not exit or is not directory\n", directory);

		/* check if exists and if so, attempt to remove non-directory */
		if (g_file_test (directory, G_FILE_TEST_EXISTS))
		{
			if (g_remove (directory) < 0)
			{
				SIM_DEBUG("unable to remove %s: %d %s\n",directory, errno, strerror(errno));

				err = errno;
				goto Done;
			}
		}

		/* create directory */
		if (g_mkdir_with_parents (directory, ALP_NOTIFY_DATABASE_MODE) < 0)
		{
			/* unable to create directory */
			SIM_DEBUG("unable to create %s: %d %s\n", directory, errno, strerror(errno));

			err = errno;
			goto Done;
		}
	}

	// open the database
	err = sqlite3_open(dbname,&db);

	if (err != SQLITE_OK)
	{
		SIM_DEBUG("db_sim_sec_create: sqlite3_open err=%d\n",err);
		goto Done;
	}

	// create the table
	err = sqlite3_exec(db,"CREATE TABLE Ticcid ( len INT, iccid1 INT, iccid2 INT, iccid3 INT, iccid4 INT, iccid5 INT, iccid6 INT, iccid7 INT, iccid8 INT, iccid9 INT, iccid10 INT, iccid11 INT, iccid12 INT, iccid13 INT)"
			,0,0,&mesg);

	err = sqlite3_exec(db,"CREATE TABLE Tusim_li ( len INT,	usim_li1 INT, usim_li2 INT, usim_li3 INT, usim_li4 INT, usim_li5 INT, usim_li6 INT, usim_li7 INT, usim_li8 INT, usim_li9 INT, usim_li10 INT, usim_li11)"
			,0,0,&mesg);

	err = sqlite3_exec(db,"CREATE TABLE Tecc ( len INT, ecc1 INT, ecc2 INT, ecc3 INT, ecc4 INT, ecc5 INT, ecc6 INT, ecc7 INT, ecc8 INT, ecc9 INT, ecc10 INT, ecc11 INT, ecc12 INT, ecc13 INT, ecc14 INT, ecc15 INT, ecc16 INT, ecc17 INT, ecc18 INT, ecc19 INT, ecc20 INT, ecc21 INT, ecc22 INT, ecc23 INT)"
			,0,0,&mesg);

	err = sqlite3_exec(db,"CREATE TABLE Tsst ( len INT, sst1 INT, sst2 INT, sst3 INT, sst4 INT, sst5 INT, sst6 INT, sst7 INT, sst8 INT, sst9 INT, sst10 INT)"
			,0,0,&mesg);

	err = sqlite3_exec(db,"CREATE TABLE Test ( len INT, est1 INT, est2 INT, est3 INT, est4 INT)"
			,0,0,&mesg);

	err = sqlite3_exec(db,"CREATE TABLE Tspn ( len INT, spn1 INT, spn2 INT, spn3 INT, spn4 INT, spn5 INT, spn6 INT, spn7 INT, spn8 INT, spn9 INT, spn10 INT, spn11 INT, spn12 INT, spn13 INT, spn14 INT, spn15 INT, spn16 INT, spn17 INT, spn18 INT, spn19 INT)"
			,0,0,&mesg);

	err = sqlite3_exec(db,"CREATE TABLE Timsi ( len INT, imsi1 INT, imsi2 INT, imsi3 INT, imsi4 INT, imsi5 INT, imsi6 INT, imsi7 INT, imsi8 INT, imsi9 INT, imsi10 INT, imsi11 INT, imsi12 INT)"
			,0,0,&mesg);

	//err = sqlite3_exec(db,"CREATE TABLE sim_data ( iccid VARCHAR(13), usim_li VARCHAR(11), ecc VARCHAR(23),sst VARCHAR(10),est VARCHAR(4), spn VARCHAR(19), imsi VARCHAR(12))",0,0,&mesg);

	if (err != SQLITE_OK)
	{
		TRACE(MSGL_VGSM_ERR, "sqlite3_exec err=%d\n", err);

		if (mesg)
		{
			TRACE(MSGL_VGSM_ERR, "sqlite3_exec: %s\n", mesg);
			sqlite3_free(mesg);
		}
		goto Done;
	}
Done:
	// close the database
	if (db) sqlite3_close(db);

	// return status
	return err == SQLITE_OK ? SIM_SUCCESS: err;
}


//090502
// set up SIMinfo init
int db_sim_info_add(int index, _SIMD_t *simd_t)
{
	int err;
	sqlite3 * db = 0;
	char * mesg;
	char str[1000];
	char str1[1000];
	int count, i;

	unsigned char usim_li[33]=
	{   0x90, 0x00, 0x1e, 0x62, 0x1c, 0x82, 0x02, 0x41, 0x21, 0x83,
	    0x02, 0x6f, 0x05, 0xa5, 0x03, 0xda, 0x01, 0x06, 0x8a, 0x01,
	    0x05, 0x8b, 0x03, 0x6f, 0x06, 0x05, 0x80, 0x02, 0x00, 0x08,
	    0x88, 0x01, 0x10};
	unsigned char ecc[36]  =
    {   0x90, 0x00, 0x21, 0x62, 0x1f, 0x82, 0x05, 0x42, 0x21, 0x00,
	    0x14, 0x05, 0x83, 0x02, 0x6f, 0xb7, 0xa5, 0x03, 0xda, 0x01,
   	    0x04, 0x8a, 0x01, 0x05, 0x8b, 0x03, 0x6f, 0x06, 0x0d, 0x80,
       	0x02, 0x00, 0x64, 0x88, 0x01, 0x08};
  	unsigned char sst[33]  =
   	{   0x90, 0x00, 0x1e, 0x62, 0x1c, 0x82, 0x02, 0x41, 0x21, 0x83,
		0x02, 0x6f, 0x38, 0xa5, 0x03, 0xda, 0x01, 0x04, 0x8a, 0x01,
		0x05, 0x8b, 0x03, 0x6f, 0x06, 0x08, 0x80, 0x02, 0x00, 0x07,
  	    0x88, 0x01, 0x20};
   	unsigned char est[33]  =
   	{   0x90, 0x00, 0x1e, 0x62, 0x1c, 0x82, 0x02, 0x41, 0x21, 0x83,
 		0x02, 0x6f, 0x56, 0xa5, 0x03, 0xda, 0x01, 0x04, 0x8a, 0x01,
		0x05, 0x8b, 0x03, 0x6f, 0x06, 0x07, 0x80, 0x02, 0x00, 0x01,
   		0x88, 0x01, 0x28 };
	unsigned char cphs_cphs_info[3]   = {   0x6a, 0x82, 0x00};
	unsigned char spn[32]  =
   	{   0x90, 0x00, 0x1d, 0x62, 0x1b, 0x82, 0x02, 0x41, 0x21, 0x83,
        0x02, 0x6f, 0x46, 0xa5, 0x03, 0xda, 0x01, 0x06, 0x8a, 0x01,
        0x05, 0x8b, 0x03, 0x6f, 0x06, 0x0d, 0x80, 0x02, 0x00, 0x11,
        0x88, 0x00};
	unsigned char imsi[43]  =
 	{   0x90, 0x00, 0x28, 0x62, 0x26, 0x82, 0x02, 0x41, 0x21, 0x83,
        0x02, 0x6f, 0x07, 0xa5, 0x06, 0x80, 0x01, 0x31, 0xc0, 0x01,
        0x00, 0x8a, 0x01, 0x05, 0x8b, 0x06, 0x6f, 0x06, 0x01, 0x04,
        0x00, 0x04, 0x80, 0x02, 0x00, 0x09, 0x81, 0x02, 0x00, 0x1d,
        0x88, 0x01, 0x38
	};

	if(simd_t != NULL)
	{

		switch(simd_t->type2)
		{
			case 0:
				for(i=0;i<33;i++)
					usim_li[i] = simd_t->tmp[i];
				count = simd_t->size;
			break;
			case 1:
				for(i=0;i<36;i++)
					ecc[i] = simd_t->tmp[i];
				count = simd_t->size;
			break;
			case 2:
				for(i=0;i<33;i++)
					sst[i] = simd_t->tmp[i];
				count = simd_t->size;
			break;
			case 3:
				for(i=0;i<33;i++)
					est[i] = simd_t->tmp[i];
				count = simd_t->size;
			break;
			case 4:
				for(i=0;i<3;i++)
					cphs_cphs_info[i] = simd_t->tmp[i];
				count = simd_t->size;
			break;
			case 5:
				for(i=0;i<32;i++)
					spn[i] = simd_t->tmp[i];
				count = simd_t->size;
			break;
			case 6:
				for(i=0;i<43;i++)
					imsi[i] = simd_t->tmp[i];
				count = simd_t->size;
			break;

		}
	}
							
	SIM_DEBUG("\n");

	char dbname[256];
	char *bin_path = get_bin_path();

	sprintf(dbname,"%s/%s/%s", bin_path, ALP_NOTIFY_DATABASE_DIR, ALP_NOTIFY_DATABASE_FILE_INFO);

	if (bin_path)
		g_free(bin_path);

	// open the database
	err = sqlite3_open(dbname,&db);
	if (err != SQLITE_OK)
	{
		SIM_DEBUG("sqlite3_open err=%d\n",err);
		goto Done;
	}



	// create the sql string
	switch(index)
	{
		case	0:
			sprintf(str1,"DELETE FROM Tusim_li");
			count = sizeof(usim_li);
			sprintf(str,"INSERT INTO Tusim_li VALUES (%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)",
													33,
													usim_li[0],usim_li[1],usim_li[2],usim_li[3],usim_li[4],
													usim_li[5],usim_li[6],usim_li[7],usim_li[8],usim_li[9],
													usim_li[10],usim_li[11],usim_li[12],usim_li[13],usim_li[14],
													usim_li[15],usim_li[16],usim_li[17],usim_li[18],usim_li[19],
													usim_li[20],usim_li[21],usim_li[22],usim_li[23],usim_li[24],
													usim_li[25],usim_li[26],usim_li[27],usim_li[28],usim_li[29],
													usim_li[30],usim_li[31],usim_li[32]);
		break;
		case	1:
			sprintf(str1,"DELETE FROM Tecc");
			count = sizeof(ecc);
			sprintf(str,"INSERT INTO Tecc VALUES (%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)",
												36,
												ecc[0],ecc[1],ecc[2],ecc[3],ecc[4],
												ecc[5],ecc[6],ecc[7],ecc[8],ecc[9],
												ecc[10],ecc[11],ecc[12],ecc[13],ecc[14],
												ecc[15],ecc[16],ecc[17],ecc[18],ecc[19],
												ecc[20],ecc[21],ecc[22],ecc[23],ecc[24],
												ecc[25],ecc[26],ecc[27],ecc[28],ecc[29],
												ecc[30],ecc[31],ecc[32],ecc[33],ecc[34],ecc[35]);
		break;
		case	2:
			sprintf(str1,"DELETE FROM Tsst");
			count = sizeof(sst);
			sprintf(str,"INSERT INTO Tsst VALUES (%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)",
												33,
												sst[0],sst[1],sst[2],sst[3],sst[4],
												sst[5],sst[6],sst[7],sst[8],sst[9],
												sst[10],sst[11],sst[12],sst[13],sst[14],
												sst[15],sst[16],sst[17],sst[18],sst[19],
												sst[20],sst[21],sst[22],sst[23],sst[24],
												sst[25],sst[26],sst[27],sst[28],sst[29],
												sst[30],sst[31],sst[32]);
		break;
		case	3:
			sprintf(str1,"DELETE FROM Test");
			count = sizeof(est);
			sprintf(str,"INSERT INTO Test VALUES (%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)",
												33,
												est[0],est[1],est[2],est[3],est[4],
												est[5],est[6],est[7],est[8],est[9],
												est[10],est[11],est[12],est[13],est[14],
												est[15],est[16],est[17],est[18],est[19],
												est[20],est[21],est[22],est[23],est[24],
												est[25],est[26],est[27],est[28],est[29],
												est[30],est[31],est[32]);
		break;
		case	4:
			sprintf(str1,"DELETE FROM Tcphs");
			count = sizeof(cphs_cphs_info);
			sprintf(str,"INSERT INTO Tcphs VALUES (%d,%d,%d,%d)",
												3,
												cphs_cphs_info[0], cphs_cphs_info[1], cphs_cphs_info[2]);
		break;
		case	5:
			sprintf(str1,"DELETE FROM Tspn");
			count = sizeof(spn);
			sprintf(str,"INSERT INTO Tspn VALUES (%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)",
												32,
												spn[0],spn[1],spn[2],spn[3],spn[4],
												spn[5],spn[6],spn[7],spn[8],spn[9],
												spn[10],spn[11],spn[12],spn[13],spn[14],
												spn[15],spn[16],spn[17],spn[18],spn[19],
												spn[20],spn[21],spn[22],spn[23],spn[24],
												spn[25],spn[26],spn[27],spn[28],spn[29],
												spn[30],spn[31]);
		break;
		case	6:
			sprintf(str1,"DELETE FROM Timsi");
			count = sizeof(imsi);
			sprintf(str,"INSERT INTO Timsi VALUES (%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)",
												43,
												imsi[0],imsi[1],imsi[2],imsi[3],imsi[4],
												imsi[5],imsi[6],imsi[7],imsi[8],imsi[9],
												imsi[10],imsi[11],imsi[12],imsi[13],imsi[14],
												imsi[15],imsi[16],imsi[17],imsi[18],imsi[19],
												imsi[20],imsi[21],imsi[22],imsi[23],imsi[24],
												imsi[25],imsi[26],imsi[27],imsi[28],imsi[29],
												imsi[30],imsi[31],imsi[32],imsi[33],imsi[34],
												imsi[35],imsi[36],imsi[37],imsi[38],imsi[39],
												imsi[40],imsi[41],imsi[42]);
		break;
	}
/*
	sprintf(str,"INSERT INTO sim_info VALUES ('%s','%s','%s','%s','%s','%s','%s')",
				usim_li,
				ecc,
				sst,
				est,
				cphs_cphs_info,
				spn,
				imsi);
*/

	SIM_DEBUG("SQL statement : %s\n", str);

	// add a row
	err = sqlite3_exec(db,str1,0,0,&mesg);

	if (err != SQLITE_OK)
	{
		SIM_DEBUG("sqlite3_exec err=%d\n",err);

		if (mesg)
		{
			SIM_DEBUG("sqlite3_exec: %s\n",mesg);
			sqlite3_free(mesg);
		}
		goto Done;
	}

	err = sqlite3_exec(db,str,0,0,&mesg);

	if (err != SQLITE_OK)
	{
		SIM_DEBUG("sqlite3_exec err=%d\n",err);

		if (mesg)
		{
			SIM_DEBUG("sqlite3_exec: %s\n",mesg);
			sqlite3_free(mesg);
		}
		goto Done;
	}
Done:
	// close the database
	if (db) sqlite3_close(db);

	// return status
	return err == SQLITE_OK ? SIM_SUCCESS : err;
}

//090502
// set up SIMdata init
int db_sim_data_add(int index, _SIMD_t *simd_t)
{
	int err;
	sqlite3 * db = 0;
	char * mesg;
	char str[1000];
	char str1[1000];
	int count,i;

	unsigned char iccid[13]  =
    { 0x90, 0x00, 0x0a, 0x98, 0x94, 0x22, 0x06, 0x45, 0x61, 0x53,
	  0x49, 0x38, 0xf6 };
	unsigned char usim_li[11]  =
	{ 0x90, 0x00, 0x08, 0x65, 0x6e, 0x65, 0x6e, 0x65, 0x6e, 0x64,
	  0x65 };
	unsigned char ecc[23]  =
	{ 0x90, 0x00, 0x14, 0x11, 0xf2, 0xff, 0x4e, 0x6f, 0x74, 0x72,
   	  0x75, 0x66, 0x20, 0x46, 0x65, 0x75, 0x65, 0x72, 0x77, 0x65,
  	  0x68, 0x72, 0x00    };
	unsigned char sst[10]  =
 	{ 0x90, 0x00, 0x07, 0x9e, 0xdf, 0xb7, 0x1d, 0xe7, 0xfe, 0x07  };
	unsigned char est[4]   =
	{ 0x90, 0x00, 0x01, 0x00  };
	unsigned char spn[19]  =
   	{ 0x90, 0x00, 0x10, 0x6f, 0x32, 0x20, 0x2d, 0x20, 0x64, 0x65,
      0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff    };
	unsigned char imsi[12]  =
   	{ 0x90, 0x00, 0x09, 0x08, 0x49, 0x05, 0x10, 0x30, 0x00, 0x61,
	  0x66, 0x31};

	if(simd_t != NULL)
	{

		switch(simd_t->type2)
		{
			case 0:
				for(i=0;i<13;i++)
					iccid[i] = simd_t->tmp[i];
				count = simd_t->size;
			break;
			case 1:
				for(i=0;i<11;i++)
					usim_li[i] = simd_t->tmp[i];
				count = simd_t->size;
			break;
			case 2:
				for(i=0;i<23;i++)
					ecc[i] = simd_t->tmp[i];
				count = simd_t->size;
			break;
			case 3:
				for(i=0;i<10;i++)
					sst[i] = simd_t->tmp[i];
				count = simd_t->size;
			break;
			case 4:
				for(i=0;i<4;i++)
					est[i] = simd_t->tmp[i];
				count = simd_t->size;
			break;
			case 5:
				for(i=0;i<19;i++)
					spn[i] = simd_t->tmp[i];
				count = simd_t->size;
			break;
			case 6:
				for(i=0;i<12;i++)
					imsi[i] = simd_t->tmp[i];
				count = simd_t->size;
			break;

		}

	}

	SIM_DEBUG("\n");

	char dbname[256];

	char *bin_path = get_bin_path();
	sprintf(dbname,"%s/%s/%s", bin_path, ALP_NOTIFY_DATABASE_DIR, ALP_NOTIFY_DATABASE_FILE_DATA);

	if (bin_path)
		g_free(bin_path);

	// open the database
	err = sqlite3_open(dbname,&db);
	if (err != SQLITE_OK)
	{
		SIM_DEBUG("sqlite3_open err=%d\n",err);
		goto Done;
	}

	// create the sql string
	switch(index)
	{
		case 0:
			sprintf(str1,"DELETE FROM Ticcid");
			sprintf(str,"INSERT INTO Ticcid VALUES (%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)",
													13,
													iccid[0],iccid[1],iccid[2],iccid[3],iccid[4],
													iccid[5],iccid[6],iccid[7],iccid[8],iccid[9],
													iccid[10],iccid[11],iccid[12]
													);
		break;
		case 1:
			sprintf(str1,"DELETE FROM Tusim_li");
			sprintf(str,"INSERT INTO Tusim_li VALUES (%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)",
													11,
													usim_li[0],usim_li[1],usim_li[2],usim_li[3],usim_li[4],
													usim_li[5],usim_li[6],usim_li[7],usim_li[8],usim_li[9],
													usim_li[10]
													);
		break;
		case 2:
			sprintf(str1,"DELETE FROM Tecc");
			sprintf(str,"INSERT INTO Tecc VALUES (%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)",
													23,
													ecc[0],ecc[1],ecc[2],ecc[3],ecc[4],
													ecc[5],ecc[6],ecc[7],ecc[8],ecc[9],
													ecc[10],ecc[11],ecc[12],ecc[13],ecc[14],
													ecc[15],ecc[16],ecc[17],ecc[18],ecc[19],
													ecc[20],ecc[21],ecc[22]
													);
		break;
		case 3:
			sprintf(str1,"DELETE FROM Tsst");
			sprintf(str,"INSERT INTO Tsst VALUES (%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)",
													10,
													sst[0],sst[1],sst[2],sst[3],sst[4],
													sst[5],sst[6],sst[7],sst[8],sst[9]
													);
		break;
		case 4:
			sprintf(str1,"DELETE FROM Test");
			sprintf(str,"INSERT INTO Test VALUES (%d,%d,%d,%d,%d)",
													4,
													est[0],est[1],est[2],est[3]
													);
		break;
		case 5:
			sprintf(str1,"DELETE FROM Tspn");
			sprintf(str,"INSERT INTO Tspn VALUES (%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)",
													19,
													spn[0],spn[1],spn[2],spn[3],spn[4],
													spn[5],spn[6],spn[7],spn[8],spn[9],
													spn[10],spn[11],spn[12],spn[13],spn[14],
													spn[15],spn[16],spn[17],spn[18]
													);
		break;
		case 6:
			sprintf(str1,"DELETE FROM Timsi");
			sprintf(str,"INSERT INTO Timsi VALUES (%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)",
													12,
													imsi[0],imsi[1],imsi[2],imsi[3],imsi[4],
													imsi[5],imsi[6],imsi[7],imsi[8],imsi[9],
													imsi[10],imsi[11]
													);
		break;
	}
/*
	sprintf(str,"INSERT INTO sim_data VALUES ('%s','%s','%s','%s','%s','%s','%s')",
				iccid,
				usim_li,
				ecc,
				sst,
				est,
				spn,
				imsi);
*/

	SIM_DEBUG("SQL statement : %s\n", str);

	err = sqlite3_exec(db,str1,0,0,&mesg);

	if (err != SQLITE_OK)
	{
		SIM_DEBUG("sqlite3_exec err=%d\n",err);

		if (mesg)
		{
			SIM_DEBUG("sqlite3_exec: %s\n",mesg);
			sqlite3_free(mesg);
		}
		goto Done;
	}

	// add a row
	err = sqlite3_exec(db,str,0,0,&mesg);

	if (err != SQLITE_OK)
	{
		SIM_DEBUG("sqlite3_exec err=%d\n",err);

		if (mesg)
		{
			SIM_DEBUG("sqlite3_exec: %s\n",mesg);
			sqlite3_free(mesg);
		}
		goto Done;
	}
Done:
	// close the database
	if (db) sqlite3_close(db);

	// return status
	return err == SQLITE_OK ? SIM_SUCCESS : err;
}

///////090502/////////////////////////////////////////////////////////////////////

// create the SIM SECURITY TABLE
int db_sim_sec_create(void)
{
	sqlite3 * db = 0;
	int err;
	char * mesg;

	SIM_DEBUG("\n");

	char directory[256];
	char dbname[256];

	//char *dbname = ALP_NOTIFY_DATABASE_DIR "/" ALP_NOTIFY_DATABASE_FILE;
	//char *directory = ALP_NOTIFY_DATABASE_DIR;
	char *bin_path = get_bin_path();
	sprintf(directory,"%s/%s", bin_path, ALP_NOTIFY_DATABASE_DIR);
	sprintf(dbname,"%s/%s/%s", bin_path, ALP_NOTIFY_DATABASE_DIR, ALP_NOTIFY_DATABASE_FILE);

	if (bin_path)
		g_free(bin_path);

	/* make sure database directory exists */
	if (!g_file_test (directory, G_FILE_TEST_IS_DIR))
	{
		SIM_DEBUG("directory %s does not exit or is not directory\n", directory);

		/* check if exists and if so, attempt to remove non-directory */
		if (g_file_test (directory, G_FILE_TEST_EXISTS))
		{
			if (g_remove (directory) < 0)
			{
				SIM_DEBUG("unable to remove %s: %d %s\n",directory, errno, strerror(errno));

				err = errno;
				goto Done;
			}
		}

		/* create directory */
		if (g_mkdir_with_parents (directory, ALP_NOTIFY_DATABASE_MODE) < 0)
		{
			/* unable to create directory */
			SIM_DEBUG("unable to create %s: %d %s\n", directory, errno, strerror(errno));

			err = errno;
			goto Done;
		}
	}

	// open the database
	err = sqlite3_open(dbname,&db);

	if (err != SQLITE_OK)
	{
		SIM_DEBUG("db_sim_sec_create: sqlite3_open err=%d\n",err);
		goto Done;
	}

	// create the table
	err = sqlite3_exec(db,"CREATE TABLE security ( pin_value VARCHAR(8), puk_value VARCHAR(8),pin2_value VARCHAR(8),puk2_value VARCHAR(8),pin_cnt INT,pin2_cnt INT,puk_cnt INT,puk2_cnt INT,sim_state INT,facility_pin INT,facility_fdn INT,lock_sc2_state INT)",0,0,&mesg);

	if (err != SQLITE_OK)
	{
		SIM_DEBUG("db_sim_sec_create: sqlite3_exec err=%d\n",err);

		if (mesg)
		{
			SIM_DEBUG("db_sim_sec_create: sqlite3_exec: %s\n",mesg);
			sqlite3_free(mesg);
		}
		goto Done;
	}
Done:
	// close the database
	if (db) sqlite3_close(db);

	// return status
	return err == SQLITE_OK ? SIM_SUCCESS: err;
}



// set up SIM init data (default security data)
int db_sim_sec_add(void)
{
	int err;
	sqlite3 * db = 0;
	char * mesg;
	char str[1000];

	char *pin	="1234";
	char *pin2	="5678";
	char *puk	="11111111";
	char *puk2	="22222222";

	SimSecurity *sim_sec;
	SIM_DEBUG("\n");

	sim_sec = malloc(sizeof(SimSecurity));
	if(!sim_sec)
		return -1;

	memset(sim_sec,0,sizeof(SimSecurity));

	memcpy(sim_sec->pin_value,pin,strlen(pin)+1);
	memcpy(sim_sec->pin2_value,pin2,strlen(pin2)+1);
	memcpy(sim_sec->puk_value,puk,strlen(puk)+1);
	memcpy(sim_sec->puk2_value,puk2,strlen(puk2)+1);

	sim_sec->pin_cnt		=	3;
	sim_sec->pin2_cnt		=	3;
	sim_sec->puk_cnt		=	10;
	sim_sec->puk2_cnt	=	10;
	sim_sec->sim_state	=	SIM_STATE_READY;
	sim_sec->facility_pin	=	GSM_SEC_LOCK_MODE_UNLOCK;	// 0x00;
	sim_sec->facility_fdn	=	GSM_SEC_LOCK_MODE_UNLOCK;
	sim_sec->lock_sc2_state = SIM_STATE_PIN2_REQ;

	char dbname[256];

	char *bin_path = get_bin_path();
	sprintf(dbname,"%s/%s/%s", bin_path, ALP_NOTIFY_DATABASE_DIR, ALP_NOTIFY_DATABASE_FILE);

	if (bin_path)
		g_free(bin_path);

	SIM_DEBUG("puk %s len %d\n\n\n", 	puk, strlen(puk));

	// open the database
	err = sqlite3_open(dbname,&db);
	if (err != SQLITE_OK)
	{
		SIM_DEBUG("sqlite3_open err=%d\n",err);
		goto Done;
	}

	// create the sql string
	sprintf(str,"INSERT INTO security VALUES ('%s','%s','%s','%s',%d,%d,%d,%d,%d,%d,%d,%d)",
				sim_sec->pin_value,
				sim_sec->puk_value,
				sim_sec->pin2_value,
				sim_sec->puk2_value,
				sim_sec->pin_cnt,
				sim_sec->pin2_cnt,
				sim_sec->puk_cnt,
				sim_sec->puk2_cnt,
				sim_sec->sim_state,
				sim_sec->facility_pin,
				sim_sec->facility_fdn,
				sim_sec->lock_sc2_state);

	SIM_DEBUG("SQL statement : %s\n", str);

	// add a row
	err = sqlite3_exec(db,str,0,0,&mesg);

	if (err != SQLITE_OK)
	{
		SIM_DEBUG("sqlite3_exec err=%d\n",err);

		if (mesg)
		{
			SIM_DEBUG("sqlite3_exec: %s\n",mesg);
			sqlite3_free(mesg);
		}
		goto Done;
	}
Done:
	// close the database
	if (db) sqlite3_close(db);

	if(sim_sec) free(sim_sec);
	// return status
	return err == SQLITE_OK ? SIM_SUCCESS : err;
}


// remove a registration from the database
// delete a row from the database which contains the registration information
int db_sim_sec_remove(void)
{
/*
	sqlite3 * db = 0;
	int err;
	char * mesg;
	char str[1000];


	char dbname[256];

	char *bin_path = get_bin_path();
	sprintf(dbname,"%s/%s/%s", bin_path, ALP_NOTIFY_DATABASE_DIR, ALP_NOTIFY_DATABASE_FILE);

	// open the database
	err = sqlite3_open(dbname,&db);
	if (err != SQLITE_OK) {

		TRACE(MSGL_VGSM_ERR, "sqlite3_open err=%d\n",err);

		goto Done;
	}

	// create the sql string
	sprintf(str,"DELETE FROM network WHERE (number_PLMN == %d)",number_PLMN);

	// remove the row
	err = sqlite3_exec(db,str,0,0,&mesg);
	if (err != SQLITE_OK) {

		TRACE(MSGL_VGSM_ERR, "sqlite3_exec err=%d\n",err);

		if (mesg) {

			TRACE(MSGL_VGSM_ERR, "sqlite3_exec: %s\n",mesg);

			sqlite3_free(mesg);
		}
		goto Done;
	}
Done:
	// close the database
	if (db) sqlite3_close(db);

	// return status
	return err == SQLITE_OK ? TRUE : err;
*/
return 0;
}

//////////////////////////////////////////////////////////////////////////
int db_sim_info_restore(char* table)
{
	sqlite3 * db = 0;
    int err;
    char * mesg;
    char dbname[256];
    char str[1000];

    char *bin_path = get_bin_path();
    sprintf(dbname,"%s/%s/%s", bin_path, ALP_NOTIFY_DATABASE_DIR, ALP_NOTIFY_DATABASE_FILE_INFO);

    if (bin_path)
	    g_free(bin_path);

    SIM_DEBUG("\n");

    // open the database
    err = sqlite3_open(dbname,&db);
    if (err != SQLITE_OK) {
	    SIM_DEBUG("sqlite3_open err=%d\n",err);

    goto Done;
    }

    sprintf(str,"SELECT ALL * FROM %s",table);

    // dump all the rows
    err = sqlite3_exec(db, str, db_sim_info_callback, 0, &mesg);
    if (err != SQLITE_OK) {
	    SIM_DEBUG("sqlite3_exec err=%d\n",err);

	    if (mesg) {
		    SIM_DEBUG("sqlite3_exec: %s\n",mesg);

   		    sqlite3_free(mesg);
    	}
    goto Done;
    }
Done:
   	// close the database
    if (db) sqlite3_close(db);

    // return status
	return err == SQLITE_OK ? SIM_SUCCESS : err;
}


int db_sim_data_restore(char* table)
{
	sqlite3 * db = 0;
    int err;
    char * mesg;
    char dbname[256];
    char str[1000];

    char *bin_path = get_bin_path();
    sprintf(dbname,"%s/%s/%s", bin_path, ALP_NOTIFY_DATABASE_DIR, ALP_NOTIFY_DATABASE_FILE_DATA);

    if (bin_path)
	    g_free(bin_path);

    SIM_DEBUG("\n");

    // open the database
    err = sqlite3_open(dbname,&db);
    if (err != SQLITE_OK) {
	    SIM_DEBUG("sqlite3_open err=%d\n",err);

    goto Done;
    }

    sprintf(str,"SELECT ALL * FROM %s",table);

    // dump all the rows
    err = sqlite3_exec(db, str, db_sim_info_callback, 0, &mesg);
    if (err != SQLITE_OK) {
	    SIM_DEBUG("sqlite3_exec err=%d\n",err);

	    if (mesg) {
		    SIM_DEBUG("sqlite3_exec: %s\n",mesg);

   		    sqlite3_free(mesg);
    	}
    goto Done;
    }
Done:
   	// close the database
    if (db) sqlite3_close(db);

    // return status
	return err == SQLITE_OK ? SIM_SUCCESS : err;
}
//////////////////////////////////////////////////////////////////////////


// restore all previously save registrations
// select all rows and process each one in the callback
int db_sim_sec_restore(VGSM_SIMType type)
{
	sqlite3 * db = 0;
	int err;
	char * mesg;
	char dbname[256];

	char *bin_path = get_bin_path();
	sprintf(dbname,"%s/%s/%s", bin_path, ALP_NOTIFY_DATABASE_DIR, ALP_NOTIFY_DATABASE_FILE);

	if (bin_path)
		g_free(bin_path);

	SIM_DEBUG("\n");

	// open the database
	err = sqlite3_open(dbname,&db);
	if (err != SQLITE_OK) {
		SIM_DEBUG("sqlite3_open err=%d\n",err);

		goto Done;
	}

	// dump all the rows
	err = sqlite3_exec(db,"SELECT ALL * FROM security",db_sim_sec_restore_callback,&type,&mesg);
	if (err != SQLITE_OK) {
		SIM_DEBUG("sqlite3_exec err=%d\n",err);

		if (mesg) {
			SIM_DEBUG("sqlite3_exec: %s\n",mesg);

			sqlite3_free(mesg);
		}
		goto Done;
	}
	Done:
	// close the database
	if (db) sqlite3_close(db);

	// return status
	return err == SQLITE_OK ? SIM_SUCCESS : err;
}

int db_sim_sec_edit(int db_type,char *data,int data2)
{
	sqlite3 * db = 0;
	int err;
	char * mesg;
	char str[1000];

	SIM_DEBUG("\n");

	char dbname[256];

	char *bin_path = get_bin_path();
	sprintf(dbname,"%s/%s/%s", bin_path, ALP_NOTIFY_DATABASE_DIR, ALP_NOTIFY_DATABASE_FILE);

	if (bin_path)
		g_free(bin_path);

	// open the database
	err = sqlite3_open(dbname,&db);

	if (err != SQLITE_OK)
	{
		SIM_DEBUG("sqlite3_open err=%d\n",err);

		goto Done;
	}

	// create the sql string
	switch(db_type)
	{
		case DB_PIN:
			sprintf(str,"UPDATE security SET pin_value = %s",data);
			break;
		case DB_PIN2:
			sprintf(str,"UPDATE security SET pin2_value = %s",data);
			break;
		case DB_PUK:
			sprintf(str,"UPDATE security SET puk_value = %s",data);
			break;
		case DB_PUK2:
			sprintf(str,"UPDATE security SET puk2_value = %s",data);
			break;
		case DB_PIN_CNT:
			sprintf(str,"UPDATE security SET pin_cnt = %d",data2);
			break;
		case DB_PIN2_CNT:
			sprintf(str,"UPDATE security SET pin2_cnt = %d",data2);
			break;
		case DB_PUK_CNT:
			sprintf(str,"UPDATE security SET puk_cnt = %d",data2);
			break;
		case DB_PUK2_CNT:
			sprintf(str,"UPDATE security SET puk2_cnt = %d",data2);
			break;
		case DB_SIM_STATE:
			sprintf(str,"UPDATE security SET sim_state = %d",data2);
			break;
		case DB_F_PIN:
			sprintf(str,"UPDATE security SET facility_pin = %d",data2);
			break;
		case DB_F_FDN:
			sprintf(str,"UPDATE security SET facility_fdn = %d",data2);
			break;
		case DB_LOCK_SC2_STATE:
			sprintf(str,"UPDATE security SET lock_sc2_state = %d",data2);
			break;
	}


	// dump all the rows
	err = sqlite3_exec(db,str,0,0,&mesg);
	if (err != SQLITE_OK)
	{
		SIM_DEBUG("sqlite3_exec err=%d\n",err);

		if (mesg)
		{
			SIM_DEBUG("sqlite3_exec: %s\n",mesg);

			sqlite3_free(mesg);
		}
		goto Done;
	}

Done:
	// close the database
	if (db) sqlite3_close(db);

	// return status
	return err == SQLITE_OK ? SIM_SUCCESS : err;
}

// ----------------------------------------------------------------------------
// end of sqlite3 specific code
// ----------------------------------------------------------------------------

