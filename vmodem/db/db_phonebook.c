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

/*****************************************************************************/
/*
   Developed by: lucky29.park
   Last update : 2007.02.13
 */
/*****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <glib.h>
#include <glib/gstdio.h>

#include "vgsm_phone.h"
#include "vgsm_pb.h"
#include "server_common_security.h"
#include "db/db_phonebook.h"
#include "server_tx_phonebook.h"
#include "fileio.h"
#define ALP_NOTIFY_DATABASE_DIR  "./db"
#define ALP_NOTIFY_DATABASE_FILE "pb.db"
#define ALP_NOTIFY_DATABASE_MODE (S_ISGID|S_ISVTX|S_IRWXU|S_IRWXG)


// ----------------------------------------------------------------------------
// sqlite3 specific code
// ----------------------------------------------------------------------------

int g_cnt=-1;
int g_setted_storage=-1;
int ii_cnt=0;
PB *g_pb=NULL;
//090724
int isdata=0;					// 0 : There isn't data, 1 : There is data

//080117 -for synch both of libvgsm and vgsm db part.
PB_ST_type _convert_pbtype_to_dbtype(gsm_pb_storage_type_e_type storage_type);

int db_sim_pb_restore_callback(void * ref, int ncol, char ** cols, char ** name)
{
    SIM_DEBUG("ncol : %d \n", ncol);

    if(ii_cnt==0)
    {
	g_pb = malloc(sizeof(PB) * g_cnt);
	memset(g_pb,0,sizeof(PB) * g_cnt);
    }

    //g_pb[ii_cnt].index = atoi(*(cols));
    memcpy(g_pb[ii_cnt].name, *(cols), strlen(*(cols)));
    memcpy(g_pb[ii_cnt].number, *(cols+1), strlen(*(cols+1)));
    memcpy(g_pb[ii_cnt].address, *(cols+2), strlen(*(cols+2)));
    SIM_DEBUG("1.\n");

    ii_cnt++;

    return 0;
}


int db_sim_pb_get_callback(void * ref, int ncol, char ** cols, char ** name)
{
    SIM_DEBUG(" \n");

    int *storage = (int *)ref;

    PB *pb;
    pb = malloc(sizeof(PB));

    memset(pb,0,sizeof(PB));

    SIM_DEBUG("ncol : %d \n", ncol);

    pb->db_st_type = *storage;
    //pb->index= atoi(*(cols));
    memcpy(pb->name, *(cols), strlen(*(cols)));
    memcpy(pb->number, *(cols+1), strlen(*(cols+1)));
    memcpy(pb->address, *(cols+2), strlen(*(cols+2)));

    //tx, ui shoot
    server_tx_sim_pb_get_access_RX_UI(pb,0);

    free( pb);

    return 0;
}

//090723
static int db_sim_pb_get_callback_check(void * ref, int ncol, char ** cols, char ** name)
{
    int cmp=1;

    SIM_DEBUG("col : %s \n", *cols);

    isdata=1;		// 1 : There is data.

    return cmp;
}

int db_sim_pb_cnt_callback(void * ref, int ncol, char ** cols, char ** name)
{
    g_cnt = atoi(*(cols));
    return 0;
}


int db_sim_pb_setted_storage_callback(void * ref, int ncol, char ** cols, char ** name)
{

    g_setted_storage = atoi(*(cols));
    return 0;
}


// initialize the database
// create the phonebook table
int db_sim_pb_create(void)
{
    sqlite3 * db = 0;
    int err,err2,err3,err4,err5,err6,err7,err8,err9,err10,err11,err12,err13;
    char * mesg;
    char str[1000];

    err=err2=err3=err4=err5=err6=err7=err8=err9=err10=err11=err12=err13=SQLITE_OK;

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
		SIM_DEBUG("unable to remove %s: %d %s\n", directory, errno, strerror(errno));

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
	SIM_DEBUG("sqlite3_open err=%d\n",err);

	goto Done;
    }

    //create set st table
    err = sqlite3_exec(db,"CREATE TABLE setted_st (setted_storage INT)",0,0,&mesg);

    if (err != SQLITE_OK)
    {
	SIM_DEBUG("sqlite3_setted_st create err=%d\n",err);
	goto Done;
    }

    sprintf(str,"INSERT INTO setted_st VALUES (%d)",0);

    err = sqlite3_exec(db,str,0,0,&mesg);

    memset(str,0,1000);

    if (err != SQLITE_OK)
    {
	SIM_DEBUG("sqlite3_setted_st INSERT err=%d\n",err);

	goto Done;
    }


    // create the table
    err = sqlite3_exec(db,"CREATE TABLE st_fdn (name VARCHAR(30) , number VARCHAR(20),Address VARCHAR(30))",0,0,&mesg);

    if (err == SQLITE_OK)
	err2 = sqlite3_exec(db,"CREATE TABLE st_ldn (name VARCHAR(30) , number VARCHAR(20),Address VARCHAR(30))",0,0,&mesg);

    if (err2 == SQLITE_OK)
	err3 = sqlite3_exec(db,"CREATE TABLE st_own (name VARCHAR(30) , number VARCHAR(20),Address VARCHAR(30))",0,0,&mesg);

    if (err3 == SQLITE_OK)
	err4 = sqlite3_exec(db,"CREATE TABLE st_adn (name VARCHAR(30) , number VARCHAR(20),Address VARCHAR(30))",0,0,&mesg);

    if (err4 == SQLITE_OK)
	err5 = sqlite3_exec(db,"CREATE TABLE st_sdn (name VARCHAR(30) , number VARCHAR(20),Address VARCHAR(30))",0,0,&mesg);

    ///////////////////////////////////////
    if (err5 == SQLITE_OK)
	err6 = sqlite3_exec(db,"CREATE TABLE st_dc (name VARCHAR(30) , number VARCHAR(20),Address VARCHAR(30))",0,0,&mesg);

    if (err6 == SQLITE_OK)
	err7 = sqlite3_exec(db,"CREATE TABLE st_en (name VARCHAR(30) , number VARCHAR(20),Address VARCHAR(30))",0,0,&mesg);

    if (err7 == SQLITE_OK)
	err8 = sqlite3_exec(db,"CREATE TABLE st_mc (name VARCHAR(30) , number VARCHAR(20),Address VARCHAR(30))",0,0,&mesg);

    if (err8 == SQLITE_OK)
	err9 = sqlite3_exec(db,"CREATE TABLE st_me (name VARCHAR(30) , number VARCHAR(20),Address VARCHAR(30))",0,0,&mesg);

    if (err9 == SQLITE_OK)
	err10 = sqlite3_exec(db,"CREATE TABLE st_mt (name VARCHAR(30) , number VARCHAR(20),Address VARCHAR(30))",0,0,&mesg);

    if (err10 == SQLITE_OK)
	err11 = sqlite3_exec(db,"CREATE TABLE st_rc (name VARCHAR(30) , number VARCHAR(20),Address VARCHAR(30))",0,0,&mesg);
    ///////////////////////////////////////

    if (err11 == SQLITE_OK)
    {
	err12 = sqlite3_exec(db,"CREATE TABLE count_ (st_fdn INT, st_ldn INT, st_own INT, st_adn INT, st_sdn INT, st_dc INT, st_en INT,st_mc INT,st_me INT,st_mt INT,st_rc INT )",0,0,&mesg);

	sprintf(str,"INSERT INTO count_ VALUES (%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)",0,0,0,0,0,0,0,0,0,0,0);
	err13 = sqlite3_exec(db,str,0,0,&mesg);
    }


    if (	err != SQLITE_OK ||err2!= SQLITE_OK ||err3 != SQLITE_OK ||err4 != SQLITE_OK ||err5 != SQLITE_OK ||
	    err6 != SQLITE_OK ||err7 != SQLITE_OK || err8 != SQLITE_OK|| err9 != SQLITE_OK|| err10 != SQLITE_OK||
	    err11 != SQLITE_OK ||err12 != SQLITE_OK ||err13 != SQLITE_OK)
    {
	SIM_DEBUG("sqlite3_exec err\n");

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
    return err == SQLITE_OK ? PB_SUCCESS : err;
}

// add a new registration to the database
// insert a row into the database which contains the registration information
int db_sim_pb_add(PB *pb)
{
    sqlite3 * db = 0;
    int err;
    char * mesg;
    char str[1000];


    SIM_DEBUG("\n  db_sim_pb_add : %d , %s ,%s, %s",pb->db_st_type,pb->name,pb->number,pb->address);

    char dbname[256];

    char *bin_path = get_bin_path();
    sprintf(dbname,"%s/%s/%s", bin_path, ALP_NOTIFY_DATABASE_DIR, ALP_NOTIFY_DATABASE_FILE);

    if (bin_path)
	g_free(bin_path);

    SIM_DEBUG("name, strlen : %s, %d", pb->name,strlen(pb->name));
    db_sim_pb_get_check(pb);

    if(isdata == 1)
    {
	SIM_DEBUG("\nThere is same data in Phonebook\n");
	// callback
	callback_pb_action(pb);
	isdata=0;
	SIM_DEBUG("\ncalling callback_pb_action is succeeded\n");
	return 0;
    }
    isdata=0;

    SIM_DEBUG("\n");
    SIM_DEBUG("\nThere isn't same data in Phonebook\n");


    //get setted Storage
    if(pb->db_st_type == DB_REQ)
	pb->db_st_type =db_sim_pb_set_mgr(0,0);

    // open the database
    err = sqlite3_open(dbname,&db);

    if (err != SQLITE_OK)
    {
	SIM_DEBUG("sqlite3_open err=%d\n",err);

	goto Done;
    }

    //
    switch(pb->db_st_type)
    {
	case DB_FDN:
	    db_sim_pb_cnt_mgr(db,DB_FDN,1);
	    sprintf(str,"INSERT INTO st_fdn VALUES ('%s','%s','%s')",pb->name,pb->number,pb->address);
	    break;

	case DB_LDN:
	    db_sim_pb_cnt_mgr(db,DB_LDN,1);
	    sprintf(str,"INSERT INTO st_ldn VALUES ('%s','%s','%s')",pb->name,pb->number,pb->address);
	    break;

	case DB_OWN:
	    db_sim_pb_cnt_mgr(db,DB_OWN,1);
	    sprintf(str,"INSERT INTO st_own VALUES ('%s','%s','%s')",pb->name,pb->number,pb->address);
	    break;

	case DB_ADN:
	    db_sim_pb_cnt_mgr(db,DB_ADN,1);
	    sprintf(str,"INSERT INTO st_adn VALUES ('%s','%s','%s')",pb->name,pb->number,pb->address);
	    break;

	case DB_SDN:
	    db_sim_pb_cnt_mgr(db,DB_SDN,1);
	    sprintf(str,"INSERT INTO st_sdn VALUES ('%s','%s','%s')",pb->name,pb->number,pb->address);
	    break;

	case DB_DC:
	    db_sim_pb_cnt_mgr(db,DB_DC,1);
	    sprintf(str,"INSERT INTO st_dc VALUES ('%s','%s','%s')",pb->name,pb->number,pb->address);
	    break;

	case DB_EN:
	    db_sim_pb_cnt_mgr(db,DB_EN,1);
	    sprintf(str,"INSERT INTO st_en VALUES ('%s','%s','%s')",pb->name,pb->number,pb->address);
	    break;

	case DB_MC:
	    db_sim_pb_cnt_mgr(db,DB_MC,1);
	    sprintf(str,"INSERT INTO st_mc VALUES ('%s','%s','%s')",pb->name,pb->number,pb->address);
	    break;

	case DB_ME:
	    db_sim_pb_cnt_mgr(db,DB_ME,1);
	    sprintf(str,"INSERT INTO st_me VALUES ('%s','%s','%s')",pb->name,pb->number,pb->address);
	    break;

	case DB_MT:
	    db_sim_pb_cnt_mgr(db,DB_MT,1);
	    sprintf(str,"INSERT INTO st_mt VALUES ('%s','%s','%s')",pb->name,pb->number,pb->address);
	    break;

	case DB_RC:
	    db_sim_pb_cnt_mgr(db,DB_RC,1);
	    sprintf(str,"INSERT INTO st_rc VALUES ('%s','%s','%s')",pb->name,pb->number,pb->address);
	    break;

	default:
	    break;

    }

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
    else
    {
	server_tx_sim_pb_get_access_RX_UI(pb,1);
    }

Done:
    // close the database
    if (db) sqlite3_close(db);

    // return status
    return err == SQLITE_OK ? PB_SUCCESS : err;
}


#if 0
// update using selected index value.
int db_sim_pb_edit(PB *pb)
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

    //get setted Storage
    if(pb->db_st_type == DB_REQ)
	pb->db_st_type =db_sim_pb_set_mgr(0,0);

    // open the database
    err = sqlite3_open(dbname,&db);

    if (err != SQLITE_OK)
    {
	SIM_DEBUG("sqlite3_open err=%d\n",err);

	goto Done;
    }

    //
    switch(pb->db_st_type)
    {
	case DB_FDN:
	    db_sim_pb_cnt_mgr(db,DB_FDN,1);
	    sprintf(str,"UPDATE st_fdn SET name = '%s', number = '%s', address = '%s' WHERE name = '%s'",pb->name,pb->number,pb->address,pb->name);
	    //sprintf(str,"UPDATE st_fdn SET name = '%s', number = '%s' WHERE index_ = %d",pb->name,pb->number,pb->index);
	    break;

	case DB_LDN:
	    db_sim_pb_cnt_mgr(db,DB_LDN,1);
	    sprintf(str,"UPDATE st_ldn SET name = '%s', number = '%s', address = '%s' WHERE name = '%s'",pb->name,pb->number,pb->address,pb->name);
	    //sprintf(str,"UPDATE st_ldn SET name = '%s', number = '%s' WHERE index_ = %d",pb->name,pb->number,pb->index);
	    break;

	case DB_OWN:
	    db_sim_pb_cnt_mgr(db,DB_OWN,1);
	    sprintf(str,"UPDATE st_own SET name = '%s', number = '%s', address = '%s' WHERE name = '%s'",pb->name,pb->number,pb->address,pb->name);
	    //sprintf(str,"UPDATE st_own SET name = '%s', number = '%s' WHERE index_ = %d",pb->name,pb->number,pb->index);
	    break;

	case DB_ADN:
	    db_sim_pb_cnt_mgr(db,DB_ADN,1);
	    sprintf(str,"UPDATE st_adn SET name = '%s', number = '%s', address = '%s' WHERE name = '%s'",pb->name,pb->number,pb->address,pb->name);
	    //sprintf(str,"UPDATE st_adn SET name = '%s', number = '%s' WHERE index_ = %d",pb->name,pb->number,pb->index);
	    break;

	case DB_SDN:
	    db_sim_pb_cnt_mgr(db,DB_SDN,1);
	    sprintf(str,"UPDATE st_sdn SET name = '%s', number = '%s', address = '%s' WHERE name = '%s'",pb->name,pb->number,pb->address,pb->name);
	    //sprintf(str,"UPDATE st_sdn SET name = '%s', number = '%s' WHERE index_ = %d",pb->name,pb->number,pb->index);
	    break;

	case DB_DC:
	    db_sim_pb_cnt_mgr(db,DB_DC,1);
	    sprintf(str,"UPDATE st_dc SET name = '%s', number = '%s', address = '%s' WHERE name = '%s'",pb->name,pb->number,pb->address,pb->name);
	    //sprintf(str,"UPDATE st_dc SET name = '%s', number = '%s' WHERE index_ = %d",pb->name,pb->number,pb->index);
	    break;

	case DB_EN:
	    db_sim_pb_cnt_mgr(db,DB_EN,1);
	    sprintf(str,"UPDATE st_en SET name = '%s', number = '%s', address = '%s' WHERE name = '%s'",pb->name,pb->number,pb->address,pb->name);
	    //sprintf(str,"UPDATE st_en SET name = '%s', number = '%s' WHERE index_ = %d",pb->name,pb->number,pb->index);
	    break;

	case DB_MC:
	    db_sim_pb_cnt_mgr(db,DB_MC,1);
	    sprintf(str,"UPDATE st_mc SET name = '%s', number = '%s', address = '%s' WHERE name = '%s'",pb->name,pb->number,pb->address,pb->name);
	    //sprintf(str,"UPDATE st_mc SET name = '%s', number = '%s' WHERE index_ = %d",pb->name,pb->number,pb->index);
	    break;

	case DB_ME:
	    db_sim_pb_cnt_mgr(db,DB_ME,1);
	    sprintf(str,"UPDATE st_me SET name = '%s', number = '%s', address = '%s' WHERE name = '%s'",pb->name,pb->number,pb->address,pb->name);
	    //sprintf(str,"UPDATE st_me SET name = '%s', number = '%s' WHERE index_ = %d",pb->name,pb->number,pb->index);
	    break;

	case DB_MT:
	    db_sim_pb_cnt_mgr(db,DB_MT,1);
	    sprintf(str,"UPDATE st_mt SET name = '%s', number = '%s', address = '%s' WHERE name = '%s'",pb->name,pb->number,pb->address,pb->name);
	    //sprintf(str,"UPDATE st_mt SET name = '%s', number = '%s' WHERE index_ = %d",pb->name,pb->number,pb->index);
	    break;

	case DB_RC:
	    db_sim_pb_cnt_mgr(db,DB_RC,1);
	    sprintf(str,"UPDATE st_rc SET name = '%s', number = '%s', address = '%s' WHERE name = '%s'",pb->name,pb->number,pb->address,pb->name);
	    //sprintf(str,"UPDATE st_rc SET name = '%s', number = '%s' WHERE index_ = %d",pb->name,pb->number,pb->index);
	    break;

	default:
	    break;

    }

    // update a row
    SIM_DEBUG("SQL statement : %s\n", str);

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
    else
    {
	server_tx_sim_pb_get_access_RX_UI(pb,1);
    }

Done:
    // close the database
    if (db) sqlite3_close(db);

    // return status
    return err == SQLITE_OK ? PB_SUCCESS : err;
}
#endif


// remove a registration from the database
// delete a row from the database which contains the registration information
int db_sim_pb_remove(PB *pb)
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

    /////////////////////
    //090723
    SIM_DEBUG("name, strlen : %s, %d", pb->name,strlen(pb->name));
    db_sim_pb_get_check(pb);

    if(isdata != 1)
    {
	SIM_DEBUG("\nThere isn't same data in Phonebook\n");
	SIM_DEBUG("\n1. isdata : %d\n", isdata);
	// callback
	//callback_pb_action(pb);
	isdata=0;
	return 0;
    }

    SIM_DEBUG("\nThere is same data in Phonebook\n");
    SIM_DEBUG("\n2. isdata : %d\n", isdata);
    isdata=0;
    SIM_DEBUG("\n3. isdata(after init) : %d\n", isdata);
    ////////////////////

    //get setted Storage
    if(pb->db_st_type == DB_REQ)
	pb->db_st_type	=	db_sim_pb_set_mgr(0,0);

    // open the database
    err = sqlite3_open(dbname,&db);

    if (err != SQLITE_OK)
    {
	SIM_DEBUG("sqlite3_open err=%d\n",err);

	goto Done;
    }

    //
    switch(pb->db_st_type)
    {
	case DB_FDN:
	    db_sim_pb_cnt_mgr(db,DB_FDN,2);
	    sprintf(str,"DELETE FROM st_fdn WHERE name = '%s'",pb->name);
	    //sprintf(str,"DELETE FROM st_fdn WHERE index_ = %d",pb->index);
	    break;

	case DB_LDN:
	    db_sim_pb_cnt_mgr(db,DB_LDN,2);
	    sprintf(str,"DELETE FROM st_ldn WHERE name = '%s'",pb->name);
	    //sprintf(str,"DELETE FROM st_ldn WHERE index_ = %d",pb->index);
	    break;

	case DB_OWN:
	    db_sim_pb_cnt_mgr(db,DB_OWN,2);
	    sprintf(str,"DELETE FROM st_own WHERE name = '%s'",pb->name);
	    //sprintf(str,"DELETE FROM st_own WHERE index_ = %d",pb->index);
	    break;

	case DB_ADN:
	    db_sim_pb_cnt_mgr(db,DB_ADN,2);
	    sprintf(str,"DELETE FROM st_adn WHERE name = '%s'",pb->name);
	    //sprintf(str,"DELETE FROM st_adn WHERE index_ = %d",pb->index);
	    break;

	case DB_SDN:
	    db_sim_pb_cnt_mgr(db,DB_SDN,2);
	    sprintf(str,"DELETE FROM st_sdn WHERE name = '%s'",pb->name);
	    //sprintf(str,"DELETE FROM st_sdn WHERE index_ = %d",pb->index);
	    break;

	case DB_DC:
	    db_sim_pb_cnt_mgr(db,DB_DC,1);
	    sprintf(str,"DELETE FROM st_dc WHERE name = '%s'",pb->name);
	    //sprintf(str,"DELETE FROM st_dc WHERE index_ = %d",pb->index);
	    break;

	case DB_EN:
	    db_sim_pb_cnt_mgr(db,DB_EN,1);
	    sprintf(str,"DELETE FROM st_en WHERE name = '%s'",pb->name);
	    //sprintf(str,"DELETE FROM st_en WHERE index_ = %d",pb->index);
	    break;

	case DB_MC:
	    db_sim_pb_cnt_mgr(db,DB_MC,1);
	    sprintf(str,"DELETE FROM st_mc WHERE name = '%s'",pb->name);
	    //sprintf(str,"DELETE FROM st_mc WHERE index_ = %d",pb->index);
	    break;

	case DB_ME:
	    db_sim_pb_cnt_mgr(db,DB_ME,1);
	    sprintf(str,"DELETE FROM st_me WHERE name = '%s'",pb->name);
	    //sprintf(str,"DELETE FROM st_me WHERE index_ = %d",pb->index);
	    break;

	case DB_MT:
	    db_sim_pb_cnt_mgr(db,DB_MT,1);
	    sprintf(str,"DELETE FROM st_mt WHERE name = '%s'",pb->name);
	    //sprintf(str,"DELETE FROM st_mt WHERE index_ = %d",pb->index);
	    break;

	case DB_RC:
	    db_sim_pb_cnt_mgr(db,DB_RC,1);
	    sprintf(str,"DELETE FROM st_rc WHERE name = '%s'",pb->name);
	    //sprintf(str,"DELETE FROM st_rc WHERE index_ = %d",pb->index);
	    break;

	default:
	    break;

    }

    SIM_DEBUG("SQL statement : %s\n", str);

    // add a row
    err = sqlite3_exec(db,str,0,0,&mesg);

    //if(pb)
    //free(pb);

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
    else
    {
	server_tx_sim_pb_get_access_RX_UI(pb,2);
    }

Done:
    // close the database
    if (db) sqlite3_close(db);

    // return status
    return err == SQLITE_OK ? PB_SUCCESS : err;

}


int db_sim_pb_cnt_mgr(sqlite3 * db, int pb_type, int action)
{
    int err;
    char str[1000];
    char str2[1000];
    char * mesg;
    char tb_name[7];
    int flag=0;

    SIM_DEBUG("\n");

    if(db == NULL)
    {
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
	    return -1;
	}

	flag =1;  //if called db poiter is NULL
    }

    //set table name for query
    switch(pb_type)
    {
	case DB_FDN:
	    sprintf(tb_name,"st_fdn");
	    break;

	case DB_LDN:
	    sprintf(tb_name,"st_ldn");
	    break;

	case DB_OWN:
	    sprintf(tb_name,"st_own");
	    break;

	case DB_ADN:
	    sprintf(tb_name,"st_adn");
	    break;

	case DB_SDN:
	    sprintf(tb_name,"st_sdn");
	    break;

	case DB_DC:
	    sprintf(tb_name,"st_dc");
	    break;

	case DB_EN:
	    sprintf(tb_name,"st_en");
	    break;

	case DB_MC:
	    sprintf(tb_name,"st_mc");
	    break;

	case DB_ME:
	    sprintf(tb_name,"st_me");
	    break;

	case DB_MT:
	    sprintf(tb_name,"st_mt");
	    break;

	case DB_RC:
	    sprintf(tb_name,"st_rc");
	    break;

	default:
	    break;
    }


    sprintf(str2,"SELECT %s FROM count_", tb_name);
    err = sqlite3_exec(db,str2,db_sim_pb_cnt_callback,0,&mesg);
    SIM_DEBUG("SQL statement : %s\n", str2);

    if (err != SQLITE_OK)
    {
	SIM_DEBUG("sqlite3_exec SELECT  err=%d\n",err);
	return -1;
    }
    else
    {
	// actions
	if(action == GSM_PB_MODE_READ || action == GSM_PB_MODE_EDIT)
	    return g_cnt;
	else if(action == GSM_PB_MODE_ADD)
	    g_cnt++;
	else if(action == GSM_PB_MODE_DEL)
	{
	    if(g_cnt!=0)
		g_cnt--;
	}

	sprintf(str,"UPDATE count_ SET %s = %d",tb_name, g_cnt);
	SIM_DEBUG("SQL statement : %s\n", str);
	err = sqlite3_exec(db,str,0,0,&mesg);

	if (err != SQLITE_OK)
	{
	    SIM_DEBUG("sqlite3_exec err=%d\n",err);
	    return -1;
	}
    }

    // free db
    if(flag ==1)
    {
	if (db)sqlite3_close(db);
    }

    return g_cnt;
}


int db_sim_pb_set_mgr(int pb_type,int action)
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
	SIM_DEBUG("db_sim_pb_set_mgr: sqlite3_open err=%d\n",err);

	return -1;
    }

    // action  =>   0 is get ,   1 is  set
    if(action == 0)	//get
    {
	err = sqlite3_exec(db,"SELECT setted_storage  FROM setted_st", db_sim_pb_setted_storage_callback, 0, &mesg);

	if (err != SQLITE_OK)
	    SIM_DEBUG("sqlite3_exec SELECT  err=%d\n",err);

	//close db
	if (db) sqlite3_close(db);

	if(g_setted_storage == -1)
	{
	    SIM_DEBUG("ERROR.  g_setted_storage \n");
	    return -1;
	}

	return g_setted_storage;
    }
    else if(action == 1)	//set
    {
	sprintf(str,"UPDATE setted_st SET setted_storage = %d", pb_type);
	err = sqlite3_exec(db, str, 0, 0, &mesg);

	SIM_DEBUG("SQL statement : %s\n", str);

	if (err != SQLITE_OK)
	    SIM_DEBUG("sqlite3_exec err=%d\n",err);
    }

    // close the database
    if (db) sqlite3_close(db);

    return 1;
}


// restore all previously save registrations
// select all rows and process each one in the callback
int db_sim_pb_restore(PB *pb)
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

    switch(pb->db_st_type)
    {
	case DB_FDN:
	    db_sim_pb_cnt_mgr(db,DB_FDN,0);
	    sprintf(str,"SELECT ALL * FROM st_fdn ");
	    break;

	case DB_LDN:
	    db_sim_pb_cnt_mgr(db,DB_LDN,0);
	    sprintf(str,"SELECT ALL * FROM st_ldn ");
	    break;

	case DB_OWN:
	    db_sim_pb_cnt_mgr(db,DB_OWN,0);
	    // create the sql string
	    sprintf(str,"SELECT ALL * FROM st_own ");
	    break;

	case DB_ADN:
	    db_sim_pb_cnt_mgr(db,DB_ADN,0);
	    // create the sql string
	    sprintf(str,"SELECT ALL * FROM st_adn ");
	    break;

	case DB_SDN:
	    db_sim_pb_cnt_mgr(db,DB_SDN,0);
	    // create the sql string
	    sprintf(str,"SELECT ALL * FROM st_sdn ");
	    break;

	case DB_DC:
	    db_sim_pb_cnt_mgr(db,DB_DC,0);
	    // create the sql string
	    sprintf(str,"SELECT ALL * FROM st_dc ");
	    break;

	case DB_EN:
	    db_sim_pb_cnt_mgr(db,DB_EN,0);
	    // create the sql string
	    sprintf(str,"SELECT ALL * FROM st_en ");
	    break;

	case DB_MC:
	    db_sim_pb_cnt_mgr(db,DB_MC,0);
	    // create the sql string
	    sprintf(str,"SELECT ALL * FROM st_MC ");
	    break;

	case DB_ME:
	    db_sim_pb_cnt_mgr(db,DB_ME,0);
	    // create the sql string
	    sprintf(str,"SELECT ALL * FROM st_me ");
	    break;

	case DB_MT:
	    db_sim_pb_cnt_mgr(db,DB_MT,0);
	    // create the sql string
	    sprintf(str,"SELECT ALL * FROM st_mt ");
	    break;

	case DB_RC:
	    db_sim_pb_cnt_mgr(db,DB_RC,0);
	    // create the sql string
	    sprintf(str,"SELECT ALL * FROM st_rc ");
	    break;

	default :
	    SIM_DEBUG("\n*DEFAULT*\n");
	    break;
    }

    // dump all the rows

    if(g_pb)
    {
	free(g_pb);
	g_pb=NULL;
    }

    SIM_DEBUG(" SQL statement : %s\n", str);
    err = sqlite3_exec(db,str,db_sim_pb_restore_callback,0,&mesg);

    if(g_pb)
    {
	g_pb[0].db_st_type = pb->db_st_type;
	g_pb[0].cnt =g_cnt;
	ii_cnt=0;

	server_tx_sim_pb_get_all_db(g_pb,g_cnt);
    }

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
    return err == SQLITE_OK ? PB_SUCCESS : err;
}

////////////////////////////////////

int db_sim_pb_get_check(PB *pb)
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

    //get setted Storage
    if(pb->db_st_type == DB_REQ)
	pb->db_st_type =db_sim_pb_set_mgr(0,0);

    // open the database
    err = sqlite3_open(dbname,&db);

    if (err != SQLITE_OK)
    {
	SIM_DEBUG("sqlite3_open err=%d\n",err);
	goto Done;
    }

    switch(pb->db_st_type)
    {
	case DB_FDN:
	    sprintf(str,"SELECT * FROM st_fdn WHERE (name = '%s')",pb->name);
	    break;

	case DB_LDN:
	    sprintf(str,"SELECT * FROM st_ldn WHERE (name = '%s')",pb->name);
	    break;

	case DB_OWN:
	    sprintf(str,"SELECT * FROM st_own WHERE (name = '%s')",pb->name);
	    break;

	case DB_ADN:
	    sprintf(str,"SELECT * FROM st_adn WHERE (name = '%s')",pb->name);
	    break;

	case DB_SDN:
	    sprintf(str,"SELECT * FROM st_sdn WHERE (name = '%s')",pb->name);
	    break;

	case DB_DC:
	    sprintf(str,"SELECT * FROM st_dc WHERE (name = '%s')",pb->name);
	    break;

	case DB_EN:
	    sprintf(str,"SELECT * FROM st_en WHERE (name = '%s')",pb->name);
	    break;

	case DB_MC:
	    sprintf(str,"SELECT * FROM st_mc WHERE (name = '%s')",pb->name);
	    break;

	case DB_ME:
	    sprintf(str,"SELECT * FROM st_me WHERE (name = '%s')",pb->name);
	    break;

	case DB_MT:
	    sprintf(str,"SELECT * FROM st_mt WHERE (name = '%s')",pb->name);
	    break;

	case DB_RC:
	    sprintf(str,"SELECT * FROM st_rc WHERE (name = '%s')",pb->name);
	    break;

	default :
	    break;

    }

    //get row
    SIM_DEBUG(" \n 1.SQL statement : %s\n", str);
    err = sqlite3_exec(db,str,db_sim_pb_get_callback_check,&pb->db_st_type,&mesg);

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
    SIM_DEBUG(" \n 2.SQL statement : %s\n", str);

Done:
    // close the database
    if (db) sqlite3_close(db);

    // return status
    return err == SQLITE_OK ? PB_SUCCESS : err;

}
////////////////////////////////////

int db_sim_pb_get(PB *pb)
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

    //get setted Storage
    if(pb->db_st_type == DB_REQ)
	pb->db_st_type =db_sim_pb_set_mgr(0,0);

    // open the database
    err = sqlite3_open(dbname,&db);

    if (err != SQLITE_OK)
    {
	SIM_DEBUG("sqlite3_open err=%d\n",err);
	goto Done;
    }

    switch(pb->db_st_type)
    {
	case DB_FDN:
	    //db_sim_pb_cnt_mgr(db,DB_FDN,2);
	    // create the sql string
	    sprintf(str,"SELECT * FROM st_fdn WHERE (name = '%s')",pb->name);
	    //sprintf(str,"SELECT * FROM st_fdn WHERE (index_ == %d)",pb->index);
	    break;

	case DB_LDN:
	    //db_sim_pb_cnt_mgr(db,DB_LDN,2);
	    // create the sql string
	    sprintf(str,"SELECT * FROM st_ldn WHERE (name = '%s')",pb->name);
	    //sprintf(str,"SELECT * FROM st_ldn WHERE (index_ == %d)",pb->index);
	    break;

	case DB_OWN:
	    //db_sim_pb_cnt_mgr(db,DB_OWN,2);
	    // create the sql string
	    sprintf(str,"SELECT * FROM st_own WHERE (name = '%s')",pb->name);
	    //sprintf(str,"SELECT * FROM st_own WHERE (index_ == %d)",pb->index);
	    break;

	case DB_ADN:
	    //db_sim_pb_cnt_mgr(db,DB_ADN,2);
	    // create the sql string
	    sprintf(str,"SELECT * FROM st_adn WHERE (name = '%s')",pb->name);
	    //sprintf(str,"SELECT * FROM st_adn WHERE (index_ == %d)",pb->index);
	    break;

	case DB_SDN:
	    //db_sim_pb_cnt_mgr(db,DB_SDN,2);
	    // create the sql string
	    sprintf(str,"SELECT * FROM st_sdn WHERE (name = '%s')",pb->name);
	    //sprintf(str,"SELECT * FROM st_sdn WHERE (index_ == %d)",pb->index);
	    break;

	case DB_DC:
	    //db_sim_pb_cnt_mgr(db,DB_SDN,2);
	    // create the sql string
	    sprintf(str,"SELECT * FROM st_dc WHERE (name = '%s')",pb->name);
	    //sprintf(str,"SELECT * FROM st_dc WHERE (index_ == %d)",pb->index);
	    break;

	case DB_EN:
	    //db_sim_pb_cnt_mgr(db,DB_SDN,2);
	    // create the sql string
	    sprintf(str,"SELECT * FROM st_en WHERE (name = '%s')",pb->name);
	    //sprintf(str,"SELECT * FROM st_en WHERE (index_ == %d)",pb->index);
	    break;

	case DB_MC:
	    //db_sim_pb_cnt_mgr(db,DB_SDN,2);
	    // create the sql string
	    sprintf(str,"SELECT * FROM st_mc WHERE (name = '%s')",pb->name);
	    //sprintf(str,"SELECT * FROM st_mc WHERE (index_ == %d)",pb->index);
	    break;

	case DB_ME:
	    //db_sim_pb_cnt_mgr(db,DB_SDN,2);
	    // create the sql string
	    sprintf(str,"SELECT * FROM st_me WHERE (name = '%s')",pb->name);
	    //sprintf(str,"SELECT * FROM st_me WHERE (index_ == %d)",pb->index);
	    break;

	case DB_MT:
	    //db_sim_pb_cnt_mgr(db,DB_SDN,2);
	    // create the sql string
	    sprintf(str,"SELECT * FROM st_mt WHERE (name = '%s')",pb->name);
	    //sprintf(str,"SELECT * FROM st_mt WHERE (index_ == %d)",pb->index);
	    break;

	case DB_RC:
	    //db_sim_pb_cnt_mgr(db,DB_SDN,2);
	    // create the sql string
	    sprintf(str,"SELECT * FROM st_rc WHERE (name = '%s')",pb->name);
	    //sprintf(str,"SELECT * FROM st_rc WHERE (index_ == %d)",pb->index);
	    break;

	default :
	    break;

    }

    //get row
    SIM_DEBUG(" SQL statement : %s\n", str);
    err = sqlite3_exec(db,str,db_sim_pb_get_callback,&pb->db_st_type,&mesg);

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
    return err == SQLITE_OK ? PB_SUCCESS : err;

}


//080117 - converting enum type from gsm_pb_storage_type_e_type in libvgsmSim.h to PB_ST_type inDb_pb.h
    PB_ST_type
_convert_pbtype_to_dbtype(gsm_pb_storage_type_e_type storage_type)
{
    PB_ST_type	phonebook;
    switch(storage_type)
    {
	case GSM_PB_ST_FD :
	    phonebook	=	 DB_FDN;
	    break;
	case GSM_PB_ST_LD :
	    phonebook	=	 DB_LDN;
	    break;
	case GSM_PB_ST_ON :
	    phonebook	=	 DB_OWN;
	    break;
	case GSM_PB_ST_SIM :
	    phonebook	=	 DB_ADN;
	    break;
	case GSM_PB_ST_SDN :
	    phonebook	=	 DB_SDN;
	    break;
	case GSM_PB_ST_DC :
	    phonebook	=	 DB_DC;
	    break;
	case GSM_PB_ST_EN :
	    phonebook	=	 DB_EN;
	    break;
	case GSM_PB_ST_MC :
	    phonebook	=	 DB_MC;
	    break;
	case GSM_PB_ST_ME :
	    phonebook	=	 DB_ME;
	    break;
	case GSM_PB_ST_MT :
	    phonebook	=	 DB_MT;
	    break;
	case GSM_PB_ST_RC :
	    phonebook	=	 DB_RC;
	    break;
	default :
	    phonebook	=	DB_REQ;
	    SIM_DEBUG("ERROR - not handled storage type=[%x] in PhoneBook DB\n", storage_type);
	    break;
    }

    return phonebook;
}

// ----------------------------------------------------------------------------
// end of sqlite3 specific code
// ----------------------------------------------------------------------------

