/*
 *  telephony-emulator
 *
 * Copyright (c) 2000 - 2011 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact: 
 * Sooyoung Ha <yoosah.ha@samsung.com>
 * Sungmin Ha <sungmin82.ha@samsung.com>
 * YeongKyoon Lee <yeongkyoon.lee@samsung.com>
 * HyunGoo Kang <hyungoo1.kang@samsung.com>
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
#include <errno.h>
#include <string.h>

#include <glib.h>
#include <glib/gstdio.h>

#include "vgsm_network.h"
#include "state.h"
#include "db_network.h"
#include "server_common_network.h"
#include "logmsg.h"
#include "fileio.h"

#define ALP_NOTIFY_DATABASE_DIR  "./db"
#define ALP_NOTIFY_DATABASE_FILE "network.db"
#define ALP_NOTIFY_DATABASE_MODE (S_ISGID|S_ISVTX|S_IRWXU|S_IRWXG)


// ----------------------------------------------------------------------------
// sqlite3 specific code
// ----------------------------------------------------------------------------

#include <sqlite3.h>


gsm_network_plmn_list_t g_tmp_plmn_list = {0, NULL};
extern gsm_network_plmn_list_t g_plmn_list;



// callback to handle each row from "SELECT ALL * FROM network"
// where each row is a persistent registration
static int vgsm_network_sqlite_restore_callback(void * ref, int ncol, char ** cols, char ** name)
{
	gsm_network_plmn_record_t *rec;
	int num_rec;
		
	log_msg(MSGL_VGSM_INFO,"vgsm_network_sqlite_restore callback PLMN_status=%i PLMN_id=%s ACT=%d\n",
		                                     atoi(*cols),*(cols+1), atoi(*(cols+2)));

	g_tmp_plmn_list.num_record++;
	num_rec = g_tmp_plmn_list.num_record;
	
	g_tmp_plmn_list.precord = realloc(g_tmp_plmn_list.precord, num_rec * sizeof(gsm_network_plmn_record_t));

	if (g_tmp_plmn_list.precord == NULL) {
		log_msg(MSGL_VGSM_ERR,"vgsm_network_sqlite_restore callback error\n");
		return -1;
	}

	rec = (g_tmp_plmn_list.precord + (num_rec - 1));

	rec->status = atoi(*cols);
	memcpy(rec->plmn, *(cols+1), sizeof(rec->plmn));
	rec->act = atoi(*(cols+2));
	
	// should always return zero from the callback
	return 0;
}

// initialize the database
// create the network table
static int vgsm_network_sqlite_init(void)
{
	sqlite3 * db = 0;
	int err;
	char * mesg;
	char directory[256];
	char dbname[256];

	log_msg(MSGL_VGSM_INFO,"vgsm_network_sqlite_init\n");


	//char *dbname = ALP_NOTIFY_DATABASE_DIR "/" ALP_NOTIFY_DATABASE_FILE;
	//char *directory = ALP_NOTIFY_DATABASE_DIR;
	char *bin_path = get_bin_path();
	sprintf(directory,"%s/%s", bin_path, ALP_NOTIFY_DATABASE_DIR);
	sprintf(dbname,"%s/%s/%s", bin_path, ALP_NOTIFY_DATABASE_DIR, ALP_NOTIFY_DATABASE_FILE);

	if (bin_path)
		g_free(bin_path);
	
	/* make sure database directory exists */
	if (!g_file_test (directory, G_FILE_TEST_IS_DIR)) {
		log_msg(MSGL_VGSM_INFO,"vgsm_network_sqlite_init: "
			"directory %s does not exit or is not directory\n",
			directory);

		/* check if exists and if so, attempt to remove non-directory */
		if (g_file_test (directory, G_FILE_TEST_EXISTS)) {
			if (g_remove (directory) < 0) {
				log_msg(MSGL_VGSM_INFO,"vgsm_network_sqlite_init: "
					"unable to remove %s: %d %s\n",
					directory, errno, strerror(errno));

				err = errno;
				goto Done;
			}
		}

        /* create directory */
        if (g_mkdir_with_parents (directory, ALP_NOTIFY_DATABASE_MODE) < 0) {
            /* unable to create directory */
            log_msg(MSGL_VGSM_INFO,"vgsm_network_sqlite_init: "
                   "unable to create %s: %d %s\n",
                   directory, errno, strerror(errno));

            err = errno;
            goto Done;
        }
    }

	// open the database
	err = sqlite3_open(dbname,&db);
	if (err != SQLITE_OK) {
		log_msg(MSGL_VGSM_ERR,"vgsm_network_sqlite_init: sqlite3_open err=%d\n",err);

		goto Done;
	}

	// create the table
	err = sqlite3_exec(db,"CREATE TABLE network (PLMN_status INT, PLMN_id TEXT, ACT INT)",0,0,&mesg);
	if (err != SQLITE_OK) {
		log_msg(MSGL_VGSM_ERR,"sqlite3_exec err=%d\n",err);

		if (mesg) {
			log_msg(MSGL_VGSM_INFO,"sqlite3_exec: %s\n",mesg);
			sqlite3_exec(db,"TRUNCATE TABLE network",0,0,&mesg);
			sqlite3_free(mesg);
		}
		goto Done;
	}
	
Done:
	// close the database
	if (db)
		sqlite3_close(db);

	// return status
	return err == SQLITE_OK ? TRUE: err;
}

// add a new registration to the database
// insert a row into the database which contains the registration information
static int vgsm_network_sqlite_add(int  id, int  PLMN_status, char *number_PLMN, int ACT)
{
	sqlite3 * db = 0;
	int err;
	char * mesg;
	char str[1000];
	char temp[100];

	log_msg(MSGL_VGSM_INFO,"vgsm_network_sqlite_add\n");

	char *dbname = ALP_NOTIFY_DATABASE_DIR "/" ALP_NOTIFY_DATABASE_FILE;

	// open the database
	err = sqlite3_open(dbname,&db);
	if(err != SQLITE_OK) {
		log_msg(MSGL_VGSM_ERR,"vgsm_network_sqlite_add: sqlite3_open err=%d\n",err);

		goto Done;
	}

	memcpy(temp, number_PLMN, 6);
	temp[6] = 0;
	
	// create the sql string
	sprintf(str,"INSERT INTO network VALUES (%d,'%s',%d)", PLMN_status, temp, ACT);

	log_msg(MSGL_VGSM_INFO," SQL statement : %s\n", str);

	// add a row 
	err = sqlite3_exec(db, str, 0, 0, &mesg);
	if(err != SQLITE_OK) {
		log_msg(MSGL_VGSM_ERR,"vgsm_network_sqlite_add: sqlite3_exec err=%d\n",err);

		if(mesg) {
			log_msg(MSGL_VGSM_INFO,"vgsm_network_sqlite_add: sqlite3_exec: %s\n",mesg);
			
			sqlite3_free(mesg);
		}
		goto Done;
	}

Done:
	// close the database
	if (db)
		sqlite3_close(db);

	// return status
	return err == SQLITE_OK ? TRUE : err;
}

// remove a registration from the database
// delete a row from the database which contains the registration information
static int vgsm_network_sqlite_remove(int id, char *number_PLMN)
{
	sqlite3 * db = 0;
	int err;
	char * mesg;
	char str[1000];
	char temp[100];


	log_msg(MSGL_VGSM_INFO,"vgsm_network_sqlite_remove\n");

	char *dbname = ALP_NOTIFY_DATABASE_DIR "/" ALP_NOTIFY_DATABASE_FILE;

	// open the database
	err = sqlite3_open(dbname,&db);
	if (err != SQLITE_OK) {

		log_msg(MSGL_VGSM_ERR,"vgsm_network_sqlite_remove: sqlite3_open err=%d\n",err);

		goto Done;
	}

	memcpy(temp, number_PLMN, 6);
	temp[6] = 0;

	// create the sql string
	sprintf(str, "DELETE FROM network WHERE (PLMN_id == '%s')", temp);

	// remove the row
	err = sqlite3_exec(db, str, 0, 0, &mesg);
	if (err != SQLITE_OK) {

		log_msg(MSGL_VGSM_ERR,"vgsm_network_sqlite_remove: sqlite3_exec err=%d\n",err);

		if (mesg) {

			log_msg(MSGL_VGSM_INFO,"vgsm_network_sqlite_remove: sqlite3_exec: %s\n",mesg);

			sqlite3_free(mesg);
		}
		goto Done;
	}
	
Done:
	// close the database
	if (db)
		sqlite3_close(db);

	// return status
	return err == SQLITE_OK ? TRUE : err;
}

// restore all previously save registrations
// select all rows and process each one in the callback
static int vgsm_network_sqlite_restore(void)
{
	sqlite3 * db = 0;
	int err;
	char * mesg;
	int num_rec;

	log_msg(MSGL_VGSM_INFO,"vgsm_network_sqlite_restore\n");

	char *dbname = ALP_NOTIFY_DATABASE_DIR "/" ALP_NOTIFY_DATABASE_FILE;

	if (g_tmp_plmn_list.precord) {
		free(g_tmp_plmn_list.precord);
		g_tmp_plmn_list.precord = NULL;
		g_tmp_plmn_list.num_record = 0;
	}
	
	// open the database
	err = sqlite3_open(dbname,&db);
	if (err != SQLITE_OK) {

		log_msg(MSGL_VGSM_ERR,"vgsm_network_sqlite_restore: sqlite3_open err=%d\n",err);

		goto Done;
	}

	// dump all the rows
	err = sqlite3_exec(db, "SELECT ALL * FROM network", vgsm_network_sqlite_restore_callback, 0, &mesg);
	if (err != SQLITE_OK) {

		log_msg(MSGL_VGSM_ERR,"vgsm_network_sqlite_restore: sqlite3_exec err=%d\n",err);

		if (mesg) {
			log_msg(MSGL_VGSM_INFO,"vgsm_network_sqlite_restore: sqlite3_exec: %s\n",mesg);

			sqlite3_free(mesg);
		}
		
		goto Done;
	}

	if (g_plmn_list.num_record > 0) {
		free(g_plmn_list.precord);

		g_plmn_list.precord = NULL;
		g_plmn_list.num_record = 0;
	}

	num_rec = g_tmp_plmn_list.num_record;
	g_plmn_list.num_record = num_rec;
	g_plmn_list.precord = malloc(num_rec * sizeof(gsm_network_plmn_record_t));
	memcpy(g_plmn_list.precord, g_tmp_plmn_list.precord, sizeof(gsm_network_plmn_record_t) * num_rec);
	
Done:

	if (g_tmp_plmn_list.precord) {
		free(g_tmp_plmn_list.precord);
		
		g_tmp_plmn_list.precord = NULL;
		g_tmp_plmn_list.num_record = 0;
	}
		
	// close the database
	if (db)
		sqlite3_close(db);

	// return status
	return err == SQLITE_OK ? TRUE : err;
}

// ----------------------------------------------------------------------------
// end of sqlite3 specific code
// ----------------------------------------------------------------------------

// initialize the database
int vgsm_network_database_init(void)
{
	log_msg(MSGL_VGSM_INFO,"vgsm_network_database_init\n");

	// initialize the database
	return vgsm_network_sqlite_init();
}

// add a registration
int vgsm_network_database_add(NetworkEntry * entry)
{
	log_msg(MSGL_VGSM_INFO,"vgsm_network_database_add\n");

/*
	// don't save server registrations
	if (entry->callback) {
		log_msg(MSGL_VGSM_INFO," there is NO callback function, do NOTHING !!!!|n");
		return TRUE;
		}
*/
	// add the registration to the database
	return vgsm_network_sqlite_add(entry->id, entry->PLMN_status, entry->number_PLMN, entry->ACT);
}

// remove a registration
int vgsm_network_database_remove(NetworkEntry * entry)
{
	log_msg(MSGL_VGSM_INFO,"vgsm_network_database_remove\n");

	// remove the registration from the database
	return vgsm_network_sqlite_remove(entry->id, entry->number_PLMN);
}

// restore all previously save registrations
int vgsm_network_database_restore(void)
{
	log_msg(MSGL_VGSM_INFO,"vgsm_network_database_restore\n");
	return vgsm_network_sqlite_restore();
}

#if 0
void main(){

	NetworkEntry  entry;
	char  act[10]="CDMA";
	
	
	

	vgsm_network_sqlite_init();


	entry.id=9;
	entry.number_PLMN=8888;
	entry.PLMN_status=5;
	entry.ACT=act;
	entry.callback=NULL;



	vgsm_network_database_add(&entry);

	vgsm_network_database_restore();
	
	vgsm_network_database_remove(&entry);

	vgsm_network_database_restore();


}
#endif

