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
#include <errno.h>
#include <string.h>

#include <glib.h>
#include <glib/gstdio.h>

#include "vgsm_call.h"
#include "vgsm_ss.h"
#include "db_ss.h"
#include "logmsg.h"
#include "fileio.h"
//#include "state.h"
#include "at_func.h"

#define ALP_NOTIFY_DATABASE_DIR  "./db"
#define ALP_NOTIFY_DATABASE_FILE "ss.db"
#define ALP_NOTIFY_DATABASE_MODE (S_ISGID|S_ISVTX|S_IRWXU|S_IRWXG)

// ----------------------------------------------------------------------------
// sqlite3 specific code
// ----------------------------------------------------------------------------

#include <sqlite3.h>

//090225
static int cw_count=0;

unsigned int 	g_cw_entry_count;
unsigned int 	g_cf_entry_count;
unsigned int 	g_cb_entry_count;

#ifndef	TRUE
#define TRUE	1
#endif /* TRUE */

#ifndef FALSE
#define FALSE	0
#endif /* FALSE */

call_waiting_entry_t		g_cw_entry[20];
call_barring_entry_t		g_cb_entry[20];
call_forwarding_entry_t		g_cf_entry[20];
int				g_mo_voice_is_barred = FALSE;
int				g_mt_voice_is_barred = FALSE;
int                             g_mo_video_is_barred = FALSE;
int                             g_mt_video_is_barred = FALSE;
int				g_cf_is_activated = FALSE;
char* g_cb_pwd="1111";
static int g_cb_pwd_fail_count = 0;

// end

// callback to handle each row from "SELECT ALL * FROM ss"
// where each row is a persistent registration
static int vgsm_ss_sqlite_restore_callback(void * ref, int ncol, char ** cols, char ** name)
{
    ss_cmd_e_type * type = (ss_cmd_e_type*)ref;

    log_msg(MSGL_VGSM_INFO,"ncol : %d \n", ncol);
    switch(*type)
    {
	case SS_CMD_CF:
	    {
		call_forwarding_entry_t		entry;
		memset(entry.number, 0, sizeof(entry.number));

		log_msg(MSGL_VGSM_INFO,"class = %d, type = %d, number = %s(%d), reply time = %d, ss_mode = %d \n", atoi(*cols), atoi(*(cols+1)), *(cols+2), strlen(*(cols+2)), atoi(*(cols+3)), atoi(*(cols+4)));
		entry.tel_class = atoi(*cols);
		entry.type = atoi(*(cols+1));
		memcpy(&entry.number, *(cols+2), strlen(*(cols+2)));
		entry.replyTime = atoi(*(cols+3));
		entry.ss_mode = atoi(*(cols+4));
		g_cf_entry_count++;

		set_call_forwarding_entry(&entry, g_cf_entry_count);
	    }
	    break;
	case SS_CMD_CB:
	    {
		call_barring_entry_t	entry;

		log_msg(MSGL_VGSM_INFO,"class = %d, type = %d, ss_mode = %d \n", atoi(*cols), atoi(*(cols+1)), atoi(*(cols+2)));
		entry.tel_class = atoi(*cols);
		entry.type = atoi(*(cols+1));
		entry.ss_mode = atoi(*(cols+2));

		g_cb_entry_count++;
		set_call_barring_entry(&entry, g_cb_entry_count);

		if(entry.tel_class == AT_CALL_SS_CLASS_VIDEO)	// video call
		{
		    if(entry.type == SS_CB_TYPE_AB)
		    {
			set_outgoing_video_call_barring_state(entry.ss_mode);
			set_incoming_video_call_barring_state(entry.ss_mode);
		    }
		    else if(entry.type == SS_CB_TYPE_BAOC)
		    {
			set_outgoing_video_call_barring_state(entry.ss_mode);
		    }
		    else if(entry.type == SS_CB_TYPE_BAIC)
		    {
			set_incoming_video_call_barring_state(entry.ss_mode);
		    }
		}
		else		// voice call
		{
		    if(entry.type == SS_CB_TYPE_AB)
		    {
			set_outgoing_voice_call_barring_state(entry.ss_mode);
			set_incoming_voice_call_barring_state(entry.ss_mode);
		    }
		    else if(entry.type == SS_CB_TYPE_BAOC)
		    {
			set_outgoing_voice_call_barring_state(entry.ss_mode);
		    }
		    else if(entry.type == SS_CB_TYPE_BAIC)
		    {
			set_incoming_voice_call_barring_state(entry.ss_mode);
		    }
		}
	    }
	    break;
	case SS_CMD_CW:
	    {
		call_waiting_entry_t	entry;
		memset(entry.number, 0, sizeof(entry.number));

		entry.tel_class = atoi(*cols);
		entry.ss_mode = atoi(*(cols+1));
		memcpy(&entry.number, *(cols+2), strlen(*(cols+2)));
		log_msg(MSGL_VGSM_INFO,"class = %d, ss_mode = %d\n", entry.tel_class, entry.ss_mode);
		g_cw_entry_count++;

		set_call_waiting_entry(&entry, g_cw_entry_count);
	    }
	    break;
    }

    // should always return zero from the callback
    return 0;

}

// initialize the database
// create the ss table
static int vgsm_ss_sqlite_init(void)
{
    sqlite3 * db = 0;
    int err;
    char * mesg;


    log_msg(MSGL_VGSM_INFO,"vgsm_ss_sqlite_init\n");

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
    if (!g_file_test (directory, G_FILE_TEST_IS_DIR)) {

	log_msg(MSGL_VGSM_INFO,"vgsm_ss_sqlite_init: directory %s does not exit or is not directory\n", directory);

	/* check if exists and if so, attempt to remove non-directory */
	if (g_file_test (directory, G_FILE_TEST_EXISTS)) {
	    if (g_remove (directory) < 0) {

		log_msg(MSGL_VGSM_INFO,"vgsm_ss_sqlite_init: "
			"unable to remove %s: %d %s\n",
			directory, errno, strerror(errno));

		err = errno;
		goto Done;
	    }
	}

	/* create directory */
	if (g_mkdir_with_parents (directory, ALP_NOTIFY_DATABASE_MODE) < 0) {
	    /* unable to create directory */

	    log_msg(MSGL_VGSM_INFO,"vgsm_ss_sqlite_init: "
		    "unable to create %s: %d %s\n",
		    directory, errno, strerror(errno));

	    err = errno;
	    goto Done;
	}
    }

    // open the database
    err = sqlite3_open(dbname, &db);
    if (err != SQLITE_OK) {
	log_msg(MSGL_VGSM_INFO,"vgsm_ss_sqlite_init: sqlite3_open err=%d\n",err);

	goto Done;
    }

    // create the call forwarding  table
    err = sqlite3_exec(db,"CREATE TABLE forwarding ( class INT , type INT, number TEXT, replytime  INT, ss_mode INT)", 0, 0, &mesg);
    if (err != SQLITE_OK) {
	//log_msg(MSGL_VGSM_INFO,"vgsm_ss_sqlite_init(forwarding): sqlite3_exec err=%d\n",err);
	sqlite3_exec(db,"DELETE FROM forwarding",0,0,&mesg);
	if (mesg) {
	    log_msg(MSGL_VGSM_INFO,"vgsm_ss_sqlite_init: sqlite3_exec: %s\n",mesg);
	    sqlite3_free(mesg);
	}
	//goto Done; // if it exist, next tables could be generated. so comment out.
    }

    // create the call barring  table
    err = sqlite3_exec(db,"CREATE TABLE barring ( class INT , type INT, ss_mode INT)", 0, 0, &mesg);
    if (err != SQLITE_OK) {
	//log_msg(MSGL_VGSM_INFO,"vgsm_ss_sqlite_init(barring): sqlite3_exec err=%d\n",err);
	sqlite3_exec(db,"DELETE FROM barring",0,0,&mesg);
	if (mesg) {
	    log_msg(MSGL_VGSM_INFO,"vgsm_ss_sqlite_init: sqlite3_exec: %s\n",mesg);
	    sqlite3_free(mesg);
	}
	//goto Done;
    }

    // create the call waiting  table
    err = sqlite3_exec(db,"CREATE TABLE waiting ( class INT , ss_mode INT, number TEXT)", 0, 0, &mesg);
    if (err != SQLITE_OK) {
	//log_msg(MSGL_VGSM_INFO,"vgsm_ss_sqlite_init(waiting): sqlite3_exec err=%d\n",err);
	sqlite3_exec(db,"DELETE FROM waiting",0,0,&mesg);
	if (mesg) {
	    log_msg(MSGL_VGSM_INFO,"vgsm_ss_sqlite_init: sqlite3_exec: %s\n",mesg);
	    sqlite3_free(mesg);
	}
	//goto Done;
    }

Done:
    // close the database
    if (db) sqlite3_close(db);

    // return status
    return err == SQLITE_OK ? TRUE: err;
}

static int count_callback(void *some_entry, int ncol, char **cols, char **azColName)
{
    unsigned char * count;

    count = (unsigned char *)(some_entry);

    *count += 1;
    //090225
    cw_count = *count;

    return 0;
}

int  vgsm_ss_sqlite_cw_check_exist (int tel_class)
{
    log_msg(MSGL_VGSM_INFO,"vgsm_ss_sqlite_cw_check_exist\n");

    char str[512];
    unsigned char counter = 0;
    sqlite3 * db = 0;
    int err;
    char * mesg;

    char dbname[256];

    char *bin_path = get_bin_path();
    sprintf(dbname,"%s/%s/%s", bin_path, ALP_NOTIFY_DATABASE_DIR, ALP_NOTIFY_DATABASE_FILE);

    if (bin_path)
	g_free(bin_path);

    err = sqlite3_open(dbname, &db);
    if (err != SQLITE_OK) {

	log_msg(MSGL_VGSM_INFO,"vgsm_ss_sqlite_cw_check_exist: sqlite3_open err=%d\n", err);

	goto Done;
    }

    sprintf(str, "SELECT * FROM waiting WHERE class = '%d'", tel_class);

    log_msg(MSGL_VGSM_INFO,"count sql = <%s> \n", str);

    err = sqlite3_exec(db, str, count_callback, &counter, &mesg);
    if (err != SQLITE_OK) {

	log_msg(MSGL_VGSM_INFO,"vgsm_ss_sqlite_cw_check_exist: sqlite3_exec err=%d\n", err);

	if (mesg) {

	    log_msg(MSGL_VGSM_INFO,"vgsm_ss_sqlite_cw_check_exist: sqlite3_exec: %s\n", mesg);

	    sqlite3_free(mesg);
	}
	goto Done;
    }

    if(counter > 0)
	return TRUE;

Done:
    // close the database
    if (db) sqlite3_close(db);

    return FALSE;
}

static int  vgsm_ss_sqlite_cb_check_exist (int tel_class, int type)
{
    log_msg(MSGL_VGSM_INFO,"vgsm_ss_sqlite_cb_check_exist\n");

    char str[512];
    unsigned char counter = 0;
    sqlite3 * db = 0;
    int err;
    char * mesg;


    char dbname[256];

    char *bin_path = get_bin_path();
    sprintf(dbname,"%s/%s/%s", bin_path, ALP_NOTIFY_DATABASE_DIR, ALP_NOTIFY_DATABASE_FILE);

    if (bin_path)
	g_free(bin_path);

    err = sqlite3_open(dbname, &db);
    if (err != SQLITE_OK) {

	log_msg(MSGL_VGSM_INFO,"vgsm_ss_sqlite_cb_check_exist: sqlite3_open err=%d\n", err);

	goto Done;
    }

    sprintf(str, "SELECT * FROM barring WHERE (class = '%d' AND type = '%d')", tel_class, type);

    log_msg(MSGL_VGSM_INFO,"count sql = <%s> \n", str);

    err = sqlite3_exec(db, str, count_callback, &counter, &mesg);
    if (err != SQLITE_OK) {

	log_msg(MSGL_VGSM_INFO,"vgsm_ss_sqlite_cb_check_exist: sqlite3_exec err=%d\n", err);

	if (mesg) {

	    log_msg(MSGL_VGSM_INFO,"vgsm_ss_sqlite_cb_check_exist: sqlite3_exec: %s\n", mesg);

	    sqlite3_free(mesg);
	}
	goto Done;
    }

    if(counter > 0)
	return TRUE;

Done:
    // close the database
    if (db) sqlite3_close(db);

    return FALSE;
}

static int  vgsm_ss_sqlite_cf_check_exist (int tel_class, int type)
{
    log_msg(MSGL_VGSM_INFO,"vgsm_ss_sqlite_cf_check_exist\n");

    char str[512];
    unsigned char counter = 0;
    sqlite3 * db = 0;
    int err;
    char * mesg;

    char dbname[256];

    char *bin_path = get_bin_path();
    sprintf(dbname,"%s/%s/%s", bin_path, ALP_NOTIFY_DATABASE_DIR, ALP_NOTIFY_DATABASE_FILE);

    if (bin_path)
	g_free(bin_path);

    log_msg(MSGL_VGSM_INFO,"check 1 in vgsm_ss_sqlite_cf_check_exist\n");
    err = sqlite3_open(dbname, &db);
    log_msg(MSGL_VGSM_INFO,"check 2 in vgsm_ss_sqlite_cf_check_exist\n");
    if (err != SQLITE_OK) {

	log_msg(MSGL_VGSM_INFO,"vgsm_ss_sqlite_cf_check_exist: sqlite3_open err=%d\n", err);

	goto Done;
    }

    sprintf(str, "SELECT * FROM forwarding WHERE (class = '%d' AND type = '%d')", tel_class, type);

    log_msg(MSGL_VGSM_INFO,"count sql = <%s> \n", str);

    err = sqlite3_exec(db, str, count_callback, &counter, &mesg);
    if (err != SQLITE_OK) {

	log_msg(MSGL_VGSM_INFO,"vgsm_ss_sqlite_cf_check_exist: sqlite3_exec err=%d\n", err);

	if (mesg) {

	    log_msg(MSGL_VGSM_INFO,"vgsm_ss_sqlite_cf_check_exist: sqlite3_exec: %s\n", mesg);

	    sqlite3_free(mesg);
	}
	goto Done;
    }

    if(counter > 0)
	return TRUE;

Done:
    // close the database
    if (db) sqlite3_close(db);

    return FALSE;
}

// add a new registration to the database
// insert a row into the database which contains the registration information
static int vgsm_ss_sqlite_add(char * sqlString)
{
    sqlite3 * db = 0;
    int err;
    char * mesg;

    char str[512];
    //	char str[1000];
    //
#if 0							// - this is for checking sqlString
    int fd;

    if(fd = open("/root/sql_tmp",O_WRONLY|O_CREAT) == -1)
    {
	log_msg(MSGL_VGSM_INFO,"err=%d\n","Error opening the file");
	log_msg(MSGL_VGSM_INFO,"err=%d\n","Error opening the file");
	log_msg(MSGL_VGSM_INFO,"err=%d\n","Error opening the file");
    }
    else
    {
	write(fd, sqlString, strlen(*sqlString));
    }
#endif

#if 1
    sprintf(str, "<ADD>SELECT * FROM waiting WHERE class = '%s'", sqlString);
    log_msg(MSGL_VGSM_INFO,"count sql = <%s> \n", str);
#endif


    log_msg(MSGL_VGSM_INFO,"vgsm_ss_sqlite_add\n");

    char dbname[256];

    char *bin_path = get_bin_path();
    sprintf(dbname,"%s/%s/%s", bin_path, ALP_NOTIFY_DATABASE_DIR, ALP_NOTIFY_DATABASE_FILE);

    if (bin_path)
	g_free(bin_path);

    // open the database
    err = sqlite3_open(dbname,&db);
    if (err != SQLITE_OK) {

	log_msg(MSGL_VGSM_INFO,"vgsm_ss_sqlite_add: sqlite3_open err=%d\n",err);

	goto Done;
    }

    // create the sql string
    //	sprintf(str,"INSERT INTO ss VALUES (%d,%d,%d,'%s')",id, PLMN_status, number_PLMN, ACT);

    if(sqlString == NULL)
	goto Done;

    log_msg(MSGL_VGSM_INFO," SQL statement : %s\n", sqlString);

    // add a row
    err = sqlite3_exec(db, sqlString, 0, 0, &mesg);
    if (err != SQLITE_OK) {

	log_msg(MSGL_VGSM_INFO,"vgsm_ss_sqlite_add: sqlite3_exec err=%d\n",err);

	if (mesg) {

	    log_msg(MSGL_VGSM_INFO,"vgsm_ss_sqlite_add: sqlite3_exec: %s\n",mesg);

	    sqlite3_free(mesg);
	}
	goto Done;
    }
Done:
    // close the database
    if (db) sqlite3_close(db);

    // return status
    //return err == SQLITE_OK ? TRUE: err;
    return err;
}

// restore all previously save registrations
// select all rows and process each one in the callback
static int vgsm_ss_sqlite_restore(ss_cmd_e_type type)
{
    char str[128];
    sqlite3 * db = 0;
    int err;
    char * mesg;

    char dbname[256];

    char *bin_path = get_bin_path();
    sprintf(dbname,"%s/%s/%s", bin_path, ALP_NOTIFY_DATABASE_DIR, ALP_NOTIFY_DATABASE_FILE);

    if (bin_path)
	g_free(bin_path);

    // open the database
    err = sqlite3_open(dbname, &db);
    if (err != SQLITE_OK) {

	log_msg(MSGL_VGSM_INFO,"vgsm_ss_sqlite_restore: sqlite3_open err=%d\n", err);

	goto Done;
    }

    switch(type)
    {
	case SS_CMD_CF:
	    sprintf(str, "SELECT * FROM %s", "forwarding");
	    break;
	case SS_CMD_CB:
	    sprintf(str, "SELECT * FROM %s", "barring");
	    break;
	case SS_CMD_CW:
	    sprintf(str, "SELECT * FROM %s", "waiting");
	    break;
    }

    g_cw_entry_count = 0;
    g_cf_entry_count = 0;
    g_cb_entry_count = 0;

    log_msg(MSGL_VGSM_INFO, "%s\n", str);
    err = sqlite3_exec(db,str, vgsm_ss_sqlite_restore_callback, &type, &mesg);
    if(err)
    {
    }

    log_msg(MSGL_VGSM_INFO,"<<<<>>>>>cw(%d) cf(%d) cb(%d)\n", g_cw_entry_count, g_cf_entry_count, g_cb_entry_count);
    if(type == SS_CMD_CW && g_cw_entry_count == 0)
	set_call_waiting_entry(NULL, 0);
    if(type ==  SS_CMD_CF && g_cf_entry_count == 0)
	set_call_forwarding_entry(NULL, 0);
    if(type == SS_CMD_CB && g_cb_entry_count == 0)
	set_call_barring_entry(NULL, 0);

    if (err != SQLITE_OK) {
	log_msg(MSGL_VGSM_INFO,"vgsm_ss_sqlite_restore: sqlite3_exec err=%d\n", err);

	if (mesg) {
	    log_msg(MSGL_VGSM_INFO,"vgsm_ss_sqlite_restore: sqlite3_exec: %s\n", mesg);

	    sqlite3_free(mesg);
	}
	goto Done;
    }

Done:
    // close the database
    if (db) sqlite3_close(db);

    // return status
    return err;
}

// remove a registration from the database
// delete a row from the database which contains the registration information
static int vgsm_ss_sqlite_remove(ss_cmd_e_type type, int tel_class, int ss_type)
{
    sqlite3 * db = 0;
    int err;
    char * mesg;
    char str[1000];


    log_msg(MSGL_VGSM_INFO,"vgsm_ss_sqlite_remove\n");

    char dbname[256];

    char *bin_path = get_bin_path();
    sprintf(dbname,"%s/%s/%s", bin_path, ALP_NOTIFY_DATABASE_DIR, ALP_NOTIFY_DATABASE_FILE);

    if (bin_path)
	g_free(bin_path);

    // open the database
    err = sqlite3_open(dbname,&db);
    if (err != SQLITE_OK) {

	log_msg(MSGL_VGSM_INFO,"vgsm_ss_sqlite_remove: sqlite3_open err=%d\n",err);

	goto Done;
    }

    // create the sql string
    switch(type)
    {
	case SS_CMD_CF:
	    //090325
	    //sprintf(str,"DELETE FROM forwarding WHERE (class = '%d' AND type = '%d')", tel_class, ss_type);
	    if(ss_type == SS_CF_TYPE_CF_ALL) // SS_CF_TYPE_CF_ALL
		sprintf(str,"DELETE FROM forwarding");				// for defect X200001252, set Cancel all in Simulator
	    else if(tel_class == -1 || tel_class == 0x10) // All teleservices
		sprintf(str,"DELETE FROM forwarding WHERE type = '%d'", ss_type);
	    else
		//	sprintf(str,"DELETE FROM forwarding WHERE type = '%d'", ss_type);
		sprintf(str,"DELETE FROM forwarding WHERE (class = '%d' AND type = '%d')", tel_class, ss_type);

	    break;
	case SS_CMD_CB:
	    //090325
	    //sprintf(str,"DELETE FROM barring WHERE (class = '%d' AND type = '%d')", tel_class, ss_type);
	    sprintf(str,"DELETE FROM barring WHERE type = '%d'", ss_type);
	    break;
	case SS_CMD_CW:
	    sprintf(str,"DELETE FROM waiting WHERE (class = '%d')", tel_class);
	    break;
    }

    log_msg(MSGL_VGSM_INFO,"delete sql = <%s> \n", str);

    // remove the row
    err = sqlite3_exec(db, str, 0, 0, &mesg);
    if (err != SQLITE_OK) {

	log_msg(MSGL_VGSM_INFO,"vgsm_ss_sqlite_remove: sqlite3_exec err=%d\n",err);

	if (mesg) {

	    log_msg(MSGL_VGSM_INFO,"vgsm_ss_sqlite_remove: sqlite3_exec: %s\n",mesg);

	    sqlite3_free(mesg);
	}
	goto Done;
    }

    return vgsm_ss_sqlite_restore(type);

Done:
    // close the database
    if (db) sqlite3_close(db);

    // return status
    return err;
}


static int vgsm_ss_sqlite_make_sql(ss_cmd_e_type type, void * entry)
{
    char	 str[1024];
    int err = 0;

    switch(type)
    {
	case SS_CMD_CF:
	    {
		call_forwarding_entry_t * cf_entry = (call_forwarding_entry_t*)entry;
		log_msg(MSGL_VGSM_INFO, "ss_mode = %d\n", cf_entry->ss_mode);
		if(vgsm_ss_sqlite_cf_check_exist(cf_entry->tel_class, cf_entry->type))
		{
		    if(cf_entry->ss_mode == SS_MODE_DEREG) // GSM_SS_MODE_DEREG
		    {
			if(cf_entry->type == SS_CF_TYPE_CFU)
			{
			    log_msg(MSGL_VGSM_INFO, "g_cf_is_activated = FALSE\n");
			    g_cf_is_activated = FALSE;
			}

			return vgsm_ss_sqlite_remove(SS_CMD_CF, cf_entry->tel_class, cf_entry->type);
		    }
		    else if(cf_entry->ss_mode == SS_MODE_REG && cf_entry->type == SS_CF_TYPE_CFU)
		    {
			log_msg(MSGL_VGSM_INFO, "g_cf_is_activated = TRUE\n");
			g_cf_is_activated = TRUE;
			vgsm_ss_sqlite_remove(SS_CMD_CF, cf_entry->tel_class, SS_CF_TYPE_CFB);
			vgsm_ss_sqlite_remove(SS_CMD_CF, cf_entry->tel_class, SS_CF_TYPE_CFNRy);
			vgsm_ss_sqlite_remove(SS_CMD_CF, cf_entry->tel_class, SS_CF_TYPE_CFNRc);

			memset(str, '\0', sizeof(str));
			sprintf(str, "UPDATE forwarding SET number = '%s', replytime = '%d', ss_mode  = '%d' "
				"WHERE (class = '%d' AND type = '%d')", cf_entry->number, cf_entry->replyTime, cf_entry->ss_mode, cf_entry->tel_class, cf_entry->type);
			log_msg(MSGL_VGSM_INFO, "\n");
		    }
		    else
		    {
			if(g_cf_is_activated == TRUE)
			{
			    vgsm_ss_sqlite_remove(SS_CMD_CF, cf_entry->tel_class, SS_CF_TYPE_CFB);
			    vgsm_ss_sqlite_remove(SS_CMD_CF, cf_entry->tel_class, SS_CF_TYPE_CFNRy);
			    vgsm_ss_sqlite_remove(SS_CMD_CF, cf_entry->tel_class, SS_CF_TYPE_CFNRc);

			    return vgsm_ss_sqlite_restore(type);
			}

			memset(str, '\0', sizeof(str));
			sprintf(str, "UPDATE forwarding SET number = '%s', replytime = '%d', ss_mode  = '%d' "
				"WHERE (class = '%d' AND type = '%d')", cf_entry->number, cf_entry->replyTime, cf_entry->ss_mode, cf_entry->tel_class, cf_entry->type);
			log_msg(MSGL_VGSM_INFO, "\n");
		    }
		}
		else if(cf_entry->ss_mode == SS_MODE_DEREG)
		{
		    if(cf_entry->type == SS_CF_TYPE_CFU)
		    {
			log_msg(MSGL_VGSM_INFO, "g_cf_is_activated = FALSE\n");
			g_cf_is_activated = FALSE;
		    }

		    log_msg(MSGL_VGSM_INFO,"###%d, %d, '%s',%d, %d\n",cf_entry->tel_class, cf_entry->type, cf_entry->number, cf_entry->replyTime, cf_entry->ss_mode);
		    vgsm_ss_sqlite_remove(SS_CMD_CF, cf_entry->tel_class, cf_entry->type);
		}
		else
		{
		    if(cf_entry->ss_mode == SS_MODE_REG && cf_entry->type == SS_CF_TYPE_CFU)
		    {
			log_msg(MSGL_VGSM_INFO, "g_cf_is_activated = TRUE\n");
			g_cf_is_activated = TRUE;
			vgsm_ss_sqlite_remove(SS_CMD_CF, cf_entry->tel_class, SS_CF_TYPE_CFB);
			vgsm_ss_sqlite_remove(SS_CMD_CF, cf_entry->tel_class, SS_CF_TYPE_CFNRy);
			vgsm_ss_sqlite_remove(SS_CMD_CF, cf_entry->tel_class, SS_CF_TYPE_CFNRc);
		    }
		    else if(g_cf_is_activated == TRUE)
		    {
			vgsm_ss_sqlite_remove(SS_CMD_CF, cf_entry->tel_class, SS_CF_TYPE_CFB);
			vgsm_ss_sqlite_remove(SS_CMD_CF, cf_entry->tel_class, SS_CF_TYPE_CFNRy);
			vgsm_ss_sqlite_remove(SS_CMD_CF, cf_entry->tel_class, SS_CF_TYPE_CFNRc);

			return vgsm_ss_sqlite_restore(type);
		    }

		    memset(str, '\0', sizeof(str));
		    sprintf(str,"INSERT INTO forwarding VALUES (%d, %d, '%s',%d, %d)", cf_entry->tel_class, cf_entry->type, cf_entry->number, cf_entry->replyTime, cf_entry->ss_mode);
		    log_msg(MSGL_VGSM_INFO,"####%d, %d, '%s',%d, %d\n",cf_entry->tel_class, cf_entry->type, cf_entry->number, cf_entry->replyTime, cf_entry->ss_mode);
		}
	    }
	    break;
	case SS_CMD_CB:
	    {
		call_barring_entry_t * cb_entry = (call_barring_entry_t*)entry;
		log_msg(MSGL_VGSM_INFO,"tel_class : %x, ssmode : %x, sstype : %x\n",cb_entry->tel_class, cb_entry->ss_mode, cb_entry->type);
		if(vgsm_ss_sqlite_cb_check_exist(cb_entry->tel_class, cb_entry->type))
		{
		    if(cb_entry->ss_mode == SS_MODE_DEREG) // GSM_SS_MODE_DEREG
			return vgsm_ss_sqlite_remove(SS_CMD_CB, cb_entry->tel_class, cb_entry->type);
		    else
		    {
			if(cb_entry->type == SS_CB_TYPE_BAOC && cb_entry->ss_mode == SS_MODE_ACT)
			{
			    memset(str, '\0', sizeof(str));
			    sprintf(str, "UPDATE barring SET ss_mode  = '%d' WHERE (class = '%d' AND type = '%d')", cb_entry->ss_mode, cb_entry->tel_class, SS_CB_TYPE_BAOC);
			    if(vgsm_ss_sqlite_add((char*)&str))
				return err;
			    memset(str, '\0', sizeof(str));
			    sprintf(str, "UPDATE barring SET ss_mode  = '%d' WHERE (class = '%d' AND type = '%d')", cb_entry->ss_mode, cb_entry->tel_class, SS_CB_TYPE_BOIC);
			    if(vgsm_ss_sqlite_add((char*)&str))
				return err;
			    memset(str, '\0', sizeof(str));
			    sprintf(str, "UPDATE barring SET ss_mode  = '%d' WHERE (class = '%d' AND type = '%d')", cb_entry->ss_mode, cb_entry->tel_class, SS_CB_TYPE_BOIC_NOT_HC);
			}
			else if(cb_entry->type == SS_CB_TYPE_BAIC && cb_entry->ss_mode == SS_MODE_ACT)
			{
			    memset(str, '\0', sizeof(str));
			    sprintf(str, "UPDATE barring SET ss_mode  = '%d' WHERE (class = '%d' AND type = '%d')", cb_entry->ss_mode, cb_entry->tel_class, SS_CB_TYPE_BAIC);
			    if(vgsm_ss_sqlite_add((char*)&str))
				return err;
			    memset(str, '\0', sizeof(str));
			    sprintf(str, "UPDATE barring SET ss_mode  = '%d' WHERE (class = '%d' AND type = '%d')", cb_entry->ss_mode, cb_entry->tel_class, SS_CB_TYPE_BIC_ROAM);
			}
			else
			{	
			    memset(str, '\0', sizeof(str));
			    sprintf(str, "UPDATE barring SET ss_mode  = '%d' WHERE (class = '%d' AND type = '%d')", cb_entry->ss_mode, cb_entry->tel_class, cb_entry->type);
			}
		    }
		}
		//090327	// this is point of preventing deregisteration data in cb
		else if(cb_entry->ss_mode != SS_MODE_DEACT)	// GSM_SS_MODE_DEACTIVATE
		{
		    if(cb_entry->type == SS_CB_TYPE_BAOC && cb_entry->ss_mode == SS_MODE_ACT)
		    {
			memset(str, '\0', sizeof(str));
			sprintf(str,"INSERT INTO barring VALUES (%d, %d, %d)", cb_entry->tel_class, SS_CB_TYPE_BAOC, cb_entry->ss_mode);
			log_msg(MSGL_VGSM_INFO, "%s\n", str);
			if(vgsm_ss_sqlite_add((char*)&str))
			    return err;
			memset(str, '\0', sizeof(str));
			sprintf(str,"INSERT INTO barring VALUES (%d, %d, %d)", cb_entry->tel_class, SS_CB_TYPE_BOIC, cb_entry->ss_mode);	
			log_msg(MSGL_VGSM_INFO, "%s\n", str);
			if(vgsm_ss_sqlite_add((char*)&str))
			    return err;
			memset(str, '\0', sizeof(str));
			sprintf(str,"INSERT INTO barring VALUES (%d, %d, %d)", cb_entry->tel_class, SS_CB_TYPE_BOIC_NOT_HC, cb_entry->ss_mode);
			log_msg(MSGL_VGSM_INFO, "%s\n", str);
		    }
		    else if(cb_entry->type == SS_CB_TYPE_BAIC && cb_entry->ss_mode == SS_MODE_ACT)
		    {
			memset(str, '\0', sizeof(str));
			sprintf(str,"INSERT INTO barring VALUES (%d, %d, %d)", cb_entry->tel_class, SS_CB_TYPE_BAIC, cb_entry->ss_mode);
			if(vgsm_ss_sqlite_add((char*)&str))
			    return err;
			memset(str, '\0', sizeof(str));
			sprintf(str,"INSERT INTO barring VALUES (%d, %d, %d)", cb_entry->tel_class, SS_CB_TYPE_BIC_ROAM, cb_entry->ss_mode);
		    }
		    else
		    {
			memset(str, '\0', sizeof(str));
			sprintf(str,"INSERT INTO barring VALUES (%d, %d, %d)", cb_entry->tel_class, cb_entry->type, cb_entry->ss_mode);
		    }
		}
	    }
	    break;
	case SS_CMD_CW:
	    {
		call_waiting_entry_t * cw_entry = (call_waiting_entry_t*)entry;
		/*******090225*****************************************************************************************************/
		if(cw_entry->tel_class == -1 || cw_entry->tel_class == 0x10)		// for defect X200001045, inconsistency problem
		{
		    if(cw_entry->ss_mode == SS_MODE_DEREG || cw_entry->ss_mode == SS_MODE_DEACT) // GSM_SS_MODE_DEREG or GSM_SS_MODE_DEACTIVATE
			//						return vgsm_ss_sqlite_remove(SS_CMD_CW, cw_entry->tel_class, 0);
		    {
			memset(str, '\0', sizeof(str));
			sprintf(str, "UPDATE waiting SET number = '%s', ss_mode  = '%d' WHERE class = '%d'", cw_entry->number, cw_entry->ss_mode, 1);
			if(vgsm_ss_sqlite_add((char*)&str))
			    return err;
			memset(str, '\0', sizeof(str));
			sprintf(str, "UPDATE waiting SET number = '%s', ss_mode  = '%d' WHERE class = '%d'", cw_entry->number, cw_entry->ss_mode, 4);
			if(vgsm_ss_sqlite_add((char*)&str))
			    return err;
			memset(str, '\0', sizeof(str));
			sprintf(str, "UPDATE waiting SET number = '%s', ss_mode  = '%d' WHERE class = '%d'", cw_entry->number, cw_entry->ss_mode, 8);
			if(vgsm_ss_sqlite_add((char*)&str))
			    return err;
			memset(str, '\0', sizeof(str));
			sprintf(str, "UPDATE waiting SET number = '%s', ss_mode  = '%d' WHERE class = '%d'", cw_entry->number, cw_entry->ss_mode, 32);
		    }
		    else
		    {
			memset(str, '\0', sizeof(str));
			sprintf(str, "UPDATE waiting SET number = '%s', ss_mode  = '%d' WHERE class = '%d'", cw_entry->number, cw_entry->ss_mode, 1);
			if(vgsm_ss_sqlite_add((char*)&str))
			    return err;
			memset(str, '\0', sizeof(str));
			sprintf(str, "UPDATE waiting SET number = '%s', ss_mode  = '%d' WHERE class = '%d'", cw_entry->number, cw_entry->ss_mode, 4);
			if(vgsm_ss_sqlite_add((char*)&str))
			    return err;
			memset(str, '\0', sizeof(str));
			sprintf(str, "UPDATE waiting SET number = '%s', ss_mode  = '%d' WHERE class = '%d'", cw_entry->number, cw_entry->ss_mode, 8);
			if(vgsm_ss_sqlite_add((char*)&str))
			    return err;
			memset(str, '\0', sizeof(str));
			sprintf(str, "UPDATE waiting SET number = '%s', ss_mode  = '%d' WHERE class = '%d'", cw_entry->number, cw_entry->ss_mode, 32);
		    }
		}
		else
		{
		    if(vgsm_ss_sqlite_cw_check_exist(cw_entry->tel_class))
		    {
			/*	if(cw_entry->ss_mode == SS_MODE_DEREG || cw_entry->ss_mode == SS_MODE_DEACT) // GSM_SS_MODE_DEREG or GSM_SS_MODE_DEACT
				return  vgsm_ss_sqlite_remove(SS_CMD_CW, cw_entry->tel_class, 0);
				else*/
			memset(str, '\0', sizeof(str));
			sprintf(str, "UPDATE waiting SET number = '%s', ss_mode  = '%d' WHERE class = '%d'", cw_entry->number, cw_entry->ss_mode, 1);
			if(vgsm_ss_sqlite_add((char*)&str))
			    return err;
			memset(str, '\0', sizeof(str));
			sprintf(str, "UPDATE waiting SET number = '%s', ss_mode  = '%d' WHERE class = '%d'", cw_entry->number, cw_entry->ss_mode, 4);
			if(vgsm_ss_sqlite_add((char*)&str))
			    return err;
			memset(str, '\0', sizeof(str));
			sprintf(str, "UPDATE waiting SET number = '%s', ss_mode  = '%d' WHERE class = '%d'", cw_entry->number, cw_entry->ss_mode, 8);
			if(vgsm_ss_sqlite_add((char*)&str))
			    return err;
			memset(str, '\0', sizeof(str));
			sprintf(str, "UPDATE waiting SET number = '%s', ss_mode  = '%d' WHERE class = '%d'", cw_entry->number, cw_entry->ss_mode, 32);
		    }
		    else
		    {
			memset(str, '\0', sizeof(str));
			sprintf(str,"INSERT INTO waiting VALUES (%d, %d, '%s')", cw_entry->tel_class, cw_entry->ss_mode, cw_entry->number);
			log_msg(MSGL_VGSM_INFO, "%s\n", str);
		    }
		}
	    }
	    break;
    }

    if(vgsm_ss_sqlite_add((char*)&str))
	return err;

    return vgsm_ss_sqlite_restore(type);

}

// ----------------------------------------------------------------------------
// end of sqlite3 specific code
// ----------------------------------------------------------------------------

// initialize the database
int vgsm_ss_database_init()
{
    log_msg(MSGL_VGSM_INFO,"vgsm_ss_database_init\n");

    // initialize the database
    return vgsm_ss_sqlite_init();
}

// add a registration
int vgsm_ss_database_add(ss_cmd_e_type type, void * entry)
{
    log_msg(MSGL_VGSM_INFO,"vgsm_ss_database_add\n");

    return vgsm_ss_sqlite_make_sql(type, entry);
}

// remove a registration
int vgsm_ss_database_remove(ss_cmd_e_type type, int tel_class, int ss_type)
{

    log_msg(MSGL_VGSM_INFO,"vgsm_ss_database_remove\n");

    if(ss_type == SS_CF_TYPE_CFU)
    {
	log_msg(MSGL_VGSM_INFO, "g_cf_is_activated = FALSE\n");
	g_cf_is_activated = FALSE;
    }
    // remove the registration from the database
    return vgsm_ss_sqlite_remove(type, tel_class, ss_type);
}

// restore all previously save registrations
int vgsm_ss_database_restore(ss_cmd_e_type type)
{
    log_msg(MSGL_VGSM_INFO,"vgsm_ss_database_restore : ss_cmd=%d \n", type);

    return vgsm_ss_sqlite_restore(type);
}

/*========== voice =================*/

int get_outgoing_voice_call_barring_state(void)
{
    log_msg(MSGL_VGSM_INFO,"%d\n", g_mo_voice_is_barred);
    return g_mo_voice_is_barred;
}

int get_incoming_voice_call_barring_state(void)
{
    log_msg(MSGL_VGSM_INFO,"%d\n", g_mt_voice_is_barred);
    return g_mt_voice_is_barred;
}

void set_outgoing_voice_call_barring_state(int state)
{
    log_msg(MSGL_VGSM_INFO,"%d\n", state);
    g_mo_voice_is_barred = state == 0x03 ? TRUE : FALSE;
}

void set_incoming_voice_call_barring_state(int state)
{
    log_msg(MSGL_VGSM_INFO,"%d\n", state);
    g_mt_voice_is_barred = state == 0x03 ? TRUE : FALSE;
}

/*========== video =================*/

int get_outgoing_video_call_barring_state(void)
{
    log_msg(MSGL_VGSM_INFO,"%d\n", g_mo_video_is_barred);
    return g_mo_video_is_barred;
}

int get_incoming_video_call_barring_state(void)
{
    log_msg(MSGL_VGSM_INFO,"%d\n", g_mt_video_is_barred);
    return g_mt_video_is_barred;
}

void set_outgoing_video_call_barring_state(int state)
{
    log_msg(MSGL_VGSM_INFO,"%d\n", state);
    g_mo_video_is_barred = state == 0x03 ? TRUE : FALSE;
}

void set_incoming_video_call_barring_state(int state)
{
    log_msg(MSGL_VGSM_INFO,"%d\n", state);
    g_mt_video_is_barred = state == 0x03 ? TRUE : FALSE;
}

void set_call_waiting_entry(call_waiting_entry_t * entry, int num)
{
    if(num == 0)
	memset(g_cw_entry, 0, sizeof(g_cw_entry));
    else
    {
	g_cw_entry[num-1].tel_class = entry->tel_class;
	g_cw_entry[num-1].ss_mode = entry->ss_mode;
	g_cw_entry[0].count = num;
	memset(&g_cw_entry[num-1].number, 0, MAX_GSM_DIALED_DIGITS_NUMBER);
	memcpy(&g_cw_entry[num-1].number, &entry->number, strlen((char*)&entry->number));
    }
    log_msg(MSGL_VGSM_INFO,"[set_call_waiting_entry]--------class : %d, ss_mode : %d, num : %d\n", g_cw_entry[num-1].tel_class, g_cw_entry[num-1].ss_mode, num);
}

call_waiting_entry_t * get_call_waiting_entry()
{
    return (call_waiting_entry_t *)&g_cw_entry;
}

call_waiting_entry_t g_cw_entry_tmp;

call_waiting_entry_t * find_call_waiting_entry(int tel_class)
{
    int i, status=4;
    call_waiting_entry_t * entry ;

    for(i = 0; i<g_cw_entry[0].count; i++)
    {
	log_msg(MSGL_VGSM_INFO,"%d. [%s] class : %d, ss_mode : %d with matching telclass : %d\n"
		, i, __FUNCTION__, g_cw_entry[i].tel_class, g_cw_entry[i].ss_mode, tel_class);

	if(g_cw_entry[i].tel_class == tel_class){
	    log_msg(MSGL_VGSM_INFO,"Found:  class : %d, ss_mode : %d\n"
		    , g_cw_entry[i].tel_class, g_cw_entry[i].ss_mode);

	    return &g_cw_entry[i];
	}
    }

    log_msg(MSGL_VGSM_INFO,"Not Found:  Fake return class : %d, ss_mode : %d\n"
	    , tel_class, status);

    entry = &g_cw_entry_tmp ;

    memset(entry, 0, sizeof(call_waiting_entry_t));
    entry->tel_class = tel_class;
    entry->ss_mode = status;
    entry->count = 1;
    strcpy(entry->number, SUBSCRIBER_NUM);
    return entry;
}

void set_call_barring_entry(call_barring_entry_t * entry, int num)
{
    if(num == 0)
    {
	memset(g_cb_entry, 0, sizeof(call_barring_entry_t)*20);
	g_cb_entry[0].count = 0;
    }
    else
    {
	g_cb_entry[num-1].tel_class = entry->tel_class;
	g_cb_entry[num-1].type = entry->type;
	g_cb_entry[num-1].ss_mode = entry->ss_mode;
	g_cb_entry[0].count = num;
    }
    //	log_msg(MSGL_VGSM_INFO,"[set_call_barring_entry]--------class : %d, type : %d, ss_mode : %d, num : %d\n", g_cb_entry[num-1].tel_class, g_cb_entry[num-1].type, g_cb_entry[num-1].ss_mode, num);
}

call_barring_entry_t * get_call_barring_entry()
{
    return (call_barring_entry_t *)&g_cb_entry;
}

//We don't have to return class and type again. notify 'status' only.

call_barring_entry_t g_cb_entry_tmp;

int send_call_barring_entry(call_barring_entry_t* entry)
{
    unsigned char * data = 0;
    int data_len =4 ;
    int ss_status, ret;

    log_msg(MSGL_VGSM_INFO,"entry->count = %d \n", entry->count);
    if(entry->ss_mode == SS_MODE_REG || entry->ss_mode == SS_MODE_ACT)
	ss_status = AT_SS_STATUS_ACTIVE;
    else
	ss_status = AT_SS_STATUS_NOT_ACTIVE;

    data_len =4;
    data = malloc(sizeof(unsigned char) * data_len);
    data[0] = entry->type;
    data[1] = 1;    // entry->count 
    data[2] = entry->tel_class;
    data[3] = ss_status;

    log_msg(MSGL_VGSM_INFO,"data[0] : %x\ndata[1] : %x\ndata[2] : %x\ndata[3] : %x\nss_mode : %x\n", data[0], data[1], data[2], data[3], entry->ss_mode);

    ret = oem_tx_ss_cb_resp(data, data_len);
    free(data);

    return 0;
}

call_barring_entry_t *  find_call_barring_entry(int tel_class, int type)
{
    int i, status = SS_MODE_DEACT, found = 0;
    call_barring_entry_t * entry;// = (call_barring_entry_t*)malloc(sizeof(call_barring_entry_t));

    log_msg(MSGL_VGSM_INFO,"1. [find_call_barring_entry]--------telclass : %d, type : %d\n", tel_class, type );
    for(i = 0; i<g_cb_entry[0].count; i++)
    {
	log_msg(MSGL_VGSM_INFO,"2. [find_call_barring_entry]--------class : %d, ss_mode : %d, type : %d\n", g_cb_entry[i].tel_class, g_cb_entry[i].ss_mode, g_cb_entry[i].type);

	if((tel_class == AT_CALL_SS_CLASS_ALL) ? (g_cb_entry[i].type == type) : (g_cb_entry[i].tel_class == tel_class) && (g_cb_entry[i].type == type))
	{
	    log_msg(MSGL_VGSM_INFO,"entry: count %d \n", g_cb_entry[i].count);

	    entry = &g_cb_entry_tmp;

	    memset(entry, 0, sizeof(call_barring_entry_t));
	    entry->tel_class = g_cb_entry[i].tel_class;
	    //entry->tel_class = class;
	    entry->type = g_cb_entry[i].type;
	    if(entry->tel_class == AT_CALL_SS_CLASS_VIDEO)		// video call
	    {
		if(g_cb_entry[i].type == SS_CB_TYPE_BOIC || g_cb_entry[i].type == SS_CB_TYPE_BOIC_NOT_HC)
		{
		    if(get_outgoing_video_call_barring_state())
			entry->ss_mode = SS_MODE_ACT;
		    else
			entry->ss_mode = g_cb_entry[i].ss_mode;
		}
		else if(g_cb_entry[i].type == SS_CB_TYPE_BIC_ROAM)
		{
		    if(get_incoming_video_call_barring_state())
			entry->ss_mode = SS_MODE_ACT;
		    else
			entry->ss_mode = g_cb_entry[i].ss_mode;
		}
		else
		    entry->ss_mode = g_cb_entry[i].ss_mode;
	    }
	    else					// voice call
	    {
		if(g_cb_entry[i].type == SS_CB_TYPE_BOIC || g_cb_entry[i].type == SS_CB_TYPE_BOIC_NOT_HC)
		{
		    if(get_outgoing_voice_call_barring_state())
			entry->ss_mode = SS_MODE_ACT;
		    else
			entry->ss_mode = g_cb_entry[i].ss_mode;
		}
		else if(g_cb_entry[i].type == SS_CB_TYPE_BIC_ROAM)
		{
		    if(get_incoming_voice_call_barring_state())
			entry->ss_mode = SS_MODE_ACT;
		    else
			entry->ss_mode = g_cb_entry[i].ss_mode;
		}
		else
		    entry->ss_mode = g_cb_entry[i].ss_mode;
	    }

	    entry->count = g_cb_entry[i].count;	// it should be 0 ??? check plz...

	    send_call_barring_entry(entry);
	    found++;
	}
    }
    if(found > 0)
	return entry;

    log_msg(MSGL_VGSM_INFO,"not found \n");

    if(tel_class == AT_CALL_SS_CLASS_VIDEO)			// video call
    {	
	if(type == SS_CB_TYPE_BAIC || type == SS_CB_TYPE_BIC_ROAM)	// incoming
	{
	    log_msg(MSGL_VGSM_INFO,"class: %d, type: %d \n", tel_class, type);
	    if(get_incoming_video_call_barring_state())
		status = SS_MODE_ACT;
	    else
		status = SS_MODE_DEACT;
	}
	else							// outgoing
	{
	    log_msg(MSGL_VGSM_INFO,"class: %d, type: %d \n", tel_class, type);
	    if(get_outgoing_video_call_barring_state())
		status = SS_MODE_ACT;
	    else
		status = SS_MODE_DEACT;
	}
    }
    else						// voice call
    {
	if(type == SS_CB_TYPE_BAIC || type == SS_CB_TYPE_BIC_ROAM)      // incoming
	{
	    log_msg(MSGL_VGSM_INFO,"class: %d, type: %d \n", tel_class, type);
	    if(get_incoming_voice_call_barring_state())
		status = SS_MODE_ACT;
	    else
		status = SS_MODE_DEACT;
	}
	else                                                    // outgoing
	{
	    log_msg(MSGL_VGSM_INFO,"class: %d, type: %d \n", tel_class, type);
	    if(get_outgoing_voice_call_barring_state())
		status = SS_MODE_ACT;
	    else
		status = SS_MODE_DEACT;
	}
    }
    entry = &g_cb_entry_tmp ;

    memset(entry, 0, sizeof(call_barring_entry_t));
    entry->type = type;
    entry->ss_mode = status;
    entry->count = 1;

    if(tel_class == AT_CALL_SS_CLASS_ALL)
    {
	entry->tel_class = AT_CALL_SS_CLASS_VOICE;
	send_call_barring_entry(entry);

	entry->tel_class = AT_CALL_SS_CLASS_VIDEO;
	send_call_barring_entry(entry);
    }
    else
    {
	entry->tel_class = tel_class;
	send_call_barring_entry(entry);
    }

    return entry;
}

void set_call_forwarding_entry(call_forwarding_entry_t * entry, int num)
{
    log_msg(MSGL_VGSM_INFO, "num: %d\n", num);
    if(num == 0)
	memset(g_cf_entry, 0, sizeof(call_forwarding_entry_t) * 20);
    else
    {
	g_cf_entry[num-1].tel_class = entry->tel_class;
	g_cf_entry[num-1].type = entry->type;
	memset(&g_cf_entry[num-1].number, 0, MAX_GSM_DIALED_DIGITS_NUMBER);
	memcpy(&g_cf_entry[num-1].number, &entry->number, strlen((char*)&entry->number));
	g_cf_entry[num-1].replyTime = entry->replyTime;
	g_cf_entry[num-1].ss_mode = entry->ss_mode;
    }
    g_cf_entry[0].count = num;
    //	log_msg(MSGL_VGSM_INFO,"[set_call_forwarding_entry]--------[%s] -> <%s>\n", entry->number, g_cf_entry[num-1].number);
}

call_forwarding_entry_t * get_call_forwarding_entry()
{
    return (call_forwarding_entry_t *)&g_cf_entry;
}

call_forwarding_entry_t g_cf_entry_tmp;

call_forwarding_entry_t * find_call_forwarding_entry(int tel_class, int type)
{
    int i, class = -128;
    call_forwarding_entry_t * entry ;

    log_msg(MSGL_VGSM_INFO,"tel_class=0x%x  type=%d\n", tel_class,type);

    for(i = 0; i<g_cf_entry[0].count; i++)
    {
	log_msg(MSGL_VGSM_INFO,"tel_class=0x%x  type=%d\n", g_cf_entry[i].tel_class,g_cf_entry[i].type);
	// 090221 090418
	if(g_cf_entry[i].tel_class == -1)
	    class = 1;	// voice
	if((class == tel_class) && (g_cf_entry[i].type == type))
	{
	    log_msg(MSGL_VGSM_INFO,"entry: count %d \n", g_cf_entry[i].count);

	    entry = &g_cf_entry_tmp;

	    memset(entry, 0, sizeof(call_forwarding_entry_t));
	    //entry->tel_class = g_cf_entry[i].tel_class;
	    entry->tel_class = class;
	    entry->type = g_cf_entry[i].type;
	    entry->ss_mode = g_cf_entry[i].ss_mode;
	    entry->count = g_cf_entry[i].count;	// it should be 0 ??? check plz...
	    strcpy(entry->number, g_cf_entry[i].number);
	    entry->replyTime = g_cf_entry[i].replyTime;
	    //entry->number[0] = '\0';
	    //entry->replyTime = 0;

	    return entry;
	    //return &g_cf_entry[i];
	}
    }
    log_msg(MSGL_VGSM_INFO,"entry is not found !!! \n");

    //entry = (call_forwarding_entry_t *)malloc(sizeof(call_forwarding_entry_t));
    entry = &g_cf_entry_tmp;

    memset(entry, 0, sizeof(call_forwarding_entry_t));
    entry->tel_class = tel_class;
    entry->type = type;
    entry->ss_mode = SS_MODE_DEACT;
    entry->count = 1;	// it should be 0 ??? check plz...
    entry->number[0] = '\0';
    entry->replyTime = 0;

    return entry;

}

///////////////////////////////////////////////////////////

char* get_callbarring_pwd(void)
{
    return g_cb_pwd;
}

void set_callbarring_pwd(char* cb_pwd)
{
    strcpy(g_cb_pwd,cb_pwd);
}

int increase_callbarring_pwd_fail_count(void)
{
    g_cb_pwd_fail_count++;
    return g_cb_pwd_fail_count;

}

void clear_callbarring_pwd_fail_count(void)
{
    g_cb_pwd_fail_count = 0;
}

