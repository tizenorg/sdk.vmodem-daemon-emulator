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
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <glib.h>
#include <glib/gstdio.h>

#include <vgsm_network.h>
#include "state.h"
#include <sqlite3.h>
#include "db_gprs.h"
#include "logmsg.h"
#include "fileio.h"

#ifndef ALP_NOTIFY_DATABASE_DIR
#define ALP_NOTIFY_DATABASE_DIR  "./db"
#endif // P_NOTIFY_DATABASE_DIR

#ifndef ALP_NOTIFY_DATABASE_MODE
#define ALP_NOTIFY_DATABASE_MODE (S_ISGID|S_ISVTX|S_IRWXU|S_IRWXG)
#endif // ALP_NOTIFY_DATABASE_MODE

#define ALP_NOTIFY_GPRS_DATABASE_FILE "gprs.db"


GprsDbEntry g_gprs_tmp_ipconf = {"", "", {0xFF, 0, {0,}, {0,}, {0,}, {0,}, {0,}}};
GprsConfList g_gprs_tmp_conflist = {0, 0};



// initialize the database
// create the gprs table
static int vgsm_gprs_sqlite_init(void)
{
    sqlite3 * db = 0;
    int err;
    char * mesg;

    log_msg(MSGL_VGSM_INFO,"vgsm_gprs_sqlite_init\n");


    char directory[256];
    char dbname[256];


    //char *dbname = ALP_NOTIFY_DATABASE_DIR "/" ALP_NOTIFY_DATABASE_FILE;
    //char *directory = ALP_NOTIFY_DATABASE_DIR;
    char *bin_path = get_bin_path();
    sprintf(directory,"%s/%s", bin_path, ALP_NOTIFY_DATABASE_DIR);
    sprintf(dbname,"%s/%s/%s", bin_path, ALP_NOTIFY_DATABASE_DIR, ALP_NOTIFY_GPRS_DATABASE_FILE);	

    if (bin_path)
	g_free(bin_path);

    /* make sure database directory exists */
    if (!g_file_test (directory, G_FILE_TEST_IS_DIR)) {
	log_msg(MSGL_VGSM_INFO,"vgsm_gprs_sqlite_init: "
		"directory %s does not exit or is not directory\n",
		directory);

	/* check if exists and if so, attempt to remove non-directory */
	if (g_file_test (directory, G_FILE_TEST_EXISTS)) {
	    if (g_remove (directory) < 0) {

		log_msg(MSGL_VGSM_ERR,"vgsm_gprs_sqlite_init: unable to remove %s: %d %s\n",
			directory, errno, strerror(errno));

		err = errno;
		goto Done;
	    }
	}

	/* create directory */
	if (g_mkdir_with_parents (directory, ALP_NOTIFY_DATABASE_MODE) < 0) {
	    /* unable to create directory */
	    log_msg(MSGL_VGSM_INFO,"vgsm_gprs_sqlite_init: unable to create %s: %d %s\n",
		    directory, errno, strerror(errno));

	    err = errno;
	    goto Done;
	}
    }

    // open the database
    err = sqlite3_open(dbname,&db);
    if (err != SQLITE_OK) {
	log_msg(MSGL_VGSM_ERR,"vgsm_gprs_sqlite_init: sqlite3_open err=%d\n",err);

	goto Done;
    }

    // create the table
    err = sqlite3_exec(db,
	    "CREATE TABLE gprs (apn TEXT, interface TEXT, cid INT, field INT, ip INT, pdns INT, sdns INT, gateway INT, subnet INT)",
	    0, 0, &mesg);

    if (err != SQLITE_OK) {
	log_msg(MSGL_VGSM_ERR,"sqlite3_exec err=%d\n",err);

	if (mesg) {
	    log_msg(MSGL_VGSM_INFO,"sqlite3_exec: %s\n",mesg);
	    sqlite3_exec(db,"TRUNCATE TABLE gprs",0,0,&mesg);
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


// initialize the database
int vgsm_gprs_database_init(void)
{
    log_msg(MSGL_VGSM_INFO,"vgsm_gprs_database_init\n");

    // initialize the database
    return vgsm_gprs_sqlite_init();
}


// add a new registration to the database
// insert a row into the database which contains the registration information
static int vgsm_gprs_sqlite_add(char *apn, char *intf, char cid, short fieldflag, unsigned int ip, unsigned int pdns,
	unsigned int sdns, unsigned int gateway, unsigned int subnet)
{
    sqlite3 * db = 0;
    int err;
    char * mesg;
    char str[1000];

    log_msg(MSGL_VGSM_INFO,"vgsm_gprs_sqlite_add\n");

    char dbname[256];

    char *bin_path = get_bin_path();
    sprintf(dbname,"%s/%s/%s", bin_path, ALP_NOTIFY_DATABASE_DIR, ALP_NOTIFY_GPRS_DATABASE_FILE);

    if (bin_path)
	g_free(bin_path);

    // open the database
    err = sqlite3_open(dbname,&db);

    if (err != SQLITE_OK) {
	log_msg(MSGL_VGSM_ERR,"vgsm_gprs_sqlite_add: sqlite3_open err=%d\n",err);

	goto Done;
    }

    // create the sql string
    sprintf(str,"INSERT INTO gprs VALUES ('%s', '%s', %d, %d, %d, %d, %d, %d, %d)",
	    apn, intf, cid, fieldflag, ip, pdns, sdns, gateway, subnet);

    log_msg(MSGL_VGSM_ERR," SQL statement : %s\n", str);

    // add a row
    err = sqlite3_exec(db, str, 0, 0, &mesg);
    if (err != SQLITE_OK) {

	log_msg(MSGL_VGSM_ERR,"vgsm_gprs_sqlite_add: sqlite3_exec err=%d\n", err);

	if (mesg) {

	    log_msg(MSGL_VGSM_INFO,"vgsm_gprs_sqlite_add: sqlite3_exec: %s\n", mesg);

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


// add a registration
int vgsm_gprs_database_add(GprsDbEntry *entry)
{
    log_msg(MSGL_VGSM_INFO,"vgsm_gprs_database_add\n");

    struct in_addr tmp_addr;
    unsigned int ip, pdns, sdns, gateway, subnet;

    memcpy(&ip, (entry->ipconf.ip_addr), 4);
    memcpy(&pdns, (entry->ipconf.primary_dns), 4);
    memcpy(&sdns, (entry->ipconf.secondary_dns), 4);
    memcpy(&gateway, &(entry->ipconf.default_gateway), 4);
    memcpy(&subnet, &(entry->ipconf.subnet_mask), 4);

    tmp_addr.s_addr = ip;
    log_msg(MSGL_VGSM_INFO,"ip(%u:%s) \n" , ip, inet_ntoa(tmp_addr));
    tmp_addr.s_addr = pdns;
    log_msg(MSGL_VGSM_INFO,"pdns(%u:%s) \n" , pdns, inet_ntoa(tmp_addr));
    tmp_addr.s_addr = sdns;
    log_msg(MSGL_VGSM_INFO,"sdns(%u:%s) \n" , sdns, inet_ntoa(tmp_addr));
    tmp_addr.s_addr = gateway;
    log_msg(MSGL_VGSM_INFO,"gateway(%u:%s) \n" , gateway, inet_ntoa(tmp_addr));
    tmp_addr.s_addr = subnet;
    log_msg(MSGL_VGSM_INFO,"subnet(%u:%s) \n" , subnet, inet_ntoa(tmp_addr));

    // add the registration to the database
    return vgsm_gprs_sqlite_add(entry->apn, entry->intf, entry->ipconf.cid, entry->ipconf.fieldflag,
	    ip, pdns, sdns, gateway, subnet);
}

static int vgsm_gprs_sqlite_remove(char *apn)
{
    sqlite3 * db = 0;
    int err;
    char * mesg;
    char str[1000];

    log_msg(MSGL_VGSM_INFO,"vgsm_gprs_sqlite_remove\n");

    char dbname[256];

    char *bin_path = get_bin_path();
    sprintf(dbname,"%s/%s/%s", bin_path, ALP_NOTIFY_DATABASE_DIR, ALP_NOTIFY_GPRS_DATABASE_FILE);

    if (bin_path)
	g_free(bin_path);

    // open the database
    err = sqlite3_open(dbname, &db);
    if (err != SQLITE_OK) {
	log_msg(MSGL_VGSM_ERR,"vgsm_gprs_sqlite_remove: sqlite3_open err=%d\n", err);

	goto Done;
    }

    log_msg(MSGL_VGSM_INFO,"vgsm_gprs_sqlite_remove: APN: %s\n", apn);

    // create the sql string
    sprintf(str, "DELETE FROM gprs WHERE (apn == '%s')", apn);

    log_msg(MSGL_VGSM_INFO,"vgsm_gprs_sqlite_remove SQL statememt: %s\n", str);

    // remove the row
    err = sqlite3_exec(db, str, 0, 0, &mesg);
    if (err != SQLITE_OK) {
	log_msg(MSGL_VGSM_ERR,"vgsm_gprs_sqlite_remove: sqlite3_exec err=%d\n",err);

	if (mesg) {

	    log_msg(MSGL_VGSM_INFO,"vgsm_gprs_sqlite_remove: sqlite3_exec: %s\n",mesg);

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


// remove a registration
int vgsm_gprs_database_remove(GprsDbEntry *entry)
{
    log_msg(MSGL_VGSM_INFO,"vgsm_gprs_database_remove\n");

    // remove the registration from the database
    return vgsm_gprs_sqlite_remove(entry->apn);
}


// callback to handle each row from "SELECT ALL * FROM gprs"
// where each row is a persistent registration
static int vgsm_gprs_sqlite_restore_callback(void * ref, int ncol, char ** cols, char ** name)
{
    int temp, num;
    struct in_addr tmp_addr;
    GprsDbEntry *pentry;

    log_msg(MSGL_VGSM_INFO,"vgsm_gprs_database_restore callback apn=%s intf=%s cid=%u,\nfield=%u, ip=%u pdns=%u\nsdns=%u gateway=%u subnetmask=%u\n",
	    *cols, *(cols+1), atoi(*(cols+2)), atoi(*(cols+3)),
	    atoi(*(cols+4)), atoi(*(cols+5)), atoi(*(cols+6)), atoi(*(cols+7)), atoi(*(cols+8)));

    g_gprs_tmp_conflist.num++;
    num = g_gprs_tmp_conflist.num;

    g_gprs_tmp_conflist.pentry = realloc(g_gprs_tmp_conflist.pentry, num * sizeof(GprsDbEntry));

    if (g_gprs_tmp_conflist.pentry == NULL) {
	log_msg(MSGL_VGSM_ERR,"vgsm_gprs_sqlite_restore callback error\n");
	return -1;
    }

    pentry = &(g_gprs_tmp_conflist.pentry[num - 1]);

    strcpy(pentry->apn, *cols);
    strcpy(pentry->intf, *(cols + 1));
    pentry->ipconf.cid = atoi(*(cols + 2));
    pentry->ipconf.fieldflag = atoi(*(cols + 3));

    temp = atoi(*(cols + 4));
    memcpy(pentry->ipconf.ip_addr, &temp, 4);
    tmp_addr.s_addr = temp;
    log_msg(MSGL_VGSM_INFO,"ip(%u:%s) \n" , temp, inet_ntoa(tmp_addr));

    temp = atoi(*(cols + 5));
    memcpy(pentry->ipconf.primary_dns, &temp, 4);
    tmp_addr.s_addr = temp;
    log_msg(MSGL_VGSM_INFO,"pdns(%u:%s) \n" , temp, inet_ntoa(tmp_addr));

    temp = atoi(*(cols + 6));
    memcpy(pentry->ipconf.secondary_dns, &temp, 4);
    tmp_addr.s_addr = temp;
    log_msg(MSGL_VGSM_INFO,"sdns(%u:%s) \n" , temp, inet_ntoa(tmp_addr));

    temp = atoi(*(cols + 7));
    memcpy(pentry->ipconf.default_gateway, &temp, 4);
    tmp_addr.s_addr = temp;
    log_msg(MSGL_VGSM_INFO,"gateway(%u:%s) \n" , temp, inet_ntoa(tmp_addr));

    temp = atoi(*(cols + 8));
    memcpy(pentry->ipconf.subnet_mask, &temp, 4);
    tmp_addr.s_addr = temp;
    log_msg(MSGL_VGSM_INFO,"subnet(%u:%s) \n" , temp, inet_ntoa(tmp_addr));

    // should always return zero from the callback
    return 0;

}


// restore all previously save registrations
int vgsm_gprs_database_restore(void)
{
    sqlite3 * db = 0;
    int err;
    char * mesg;
    int num, i;

    log_msg(MSGL_VGSM_INFO,"vgsm_gprs_database_restore\n");

    char dbname[256];

    char *bin_path = get_bin_path();
    sprintf(dbname,"%s/%s/%s", bin_path, ALP_NOTIFY_DATABASE_DIR, ALP_NOTIFY_GPRS_DATABASE_FILE);

    if (bin_path)
	g_free(bin_path);

    if (g_gprs_tmp_conflist.pentry) {
	free(g_gprs_tmp_conflist.pentry);
	g_gprs_tmp_conflist.pentry = NULL;
	g_gprs_tmp_conflist.num = 0;
    }

    // open the database
    err = sqlite3_open(dbname, &db);
    if (err != SQLITE_OK) {

	log_msg(MSGL_VGSM_ERR,"vgsm_gprs_database_restore: sqlite3_open err=%d\n",err);

	goto Done;
    }

    // dump all the rows
    err = sqlite3_exec(db, "SELECT ALL * FROM gprs", vgsm_gprs_sqlite_restore_callback, 0, &mesg);
    if (err != SQLITE_OK) {
	log_msg(MSGL_VGSM_ERR,"vgsm_gprs_database_restore: sqlite3_exec err=%d\n", err);

	if (mesg) {
	    log_msg(MSGL_VGSM_INFO,"vgsm_gprs_database_restore: sqlite3_exec: %s\n", mesg);

	    sqlite3_free(mesg);
	}

	goto Done;
    }

    if (g_gprs_conflist.pentry) {
	free(g_gprs_conflist.pentry);
	g_gprs_conflist.pentry = NULL;
	g_gprs_conflist.num = 0;
    }

    num = g_gprs_tmp_conflist.num;
    g_gprs_conflist.num = num;

    if ((g_gprs_conflist.pentry = (GprsDbEntry *)malloc(num * sizeof(GprsDbEntry))) == 0)
	return -1;

    for (i = 0; i < num; i++) {
	memcpy(&(g_gprs_conflist.pentry[i]), &(g_gprs_tmp_conflist.pentry[i]), sizeof(GprsDbEntry));
    }

Done:

    if (g_gprs_tmp_conflist.pentry) {
	free(g_gprs_tmp_conflist.pentry);
	g_gprs_tmp_conflist.pentry = 0;
	g_gprs_tmp_conflist.num = 0;
    }

    // close the database
    if (db)
	sqlite3_close(db);

    // return status
    return err == SQLITE_OK ? TRUE : err;
}


// end of file

