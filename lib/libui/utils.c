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

/**
	@file 	utils.c
	@brief	miscellaneous functions used in ISE
*/

#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <gtk/gtk.h>
#include <time.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>

#include "utils.h"

static GHashTable *windows_hash = NULL; /* hash table to get widow and widget of Simulator */
//GHashTable *attribute_hash = NULL;

gchar *current_es_save_filename;		/**<event manager save file */
gchar *current_testcase_save_filename;	/**<testcase save file*/
GPid simulator_pid = -1;		/**<pid of simulator window*/


/**
	@brief 	hash intialization
*/
GHashTable *window_hash_init (void)
{
	windows_hash = g_hash_table_new_full (g_direct_hash, g_direct_equal, NULL, NULL);
	return windows_hash;
}


/**
	@brief 	add window widget to hash
*/
void add_window (GtkWidget *win, gint window_id)
{
    g_hash_table_insert (windows_hash, GINT_TO_POINTER (window_id), win);
}


/**
	@brief 	search window widget by using window id as a key
	@return	matched window widget
*/
GtkWidget *get_window (gint window_id)
{
    GtkWidget *win = (GtkWidget *) g_hash_table_lookup (windows_hash, GINT_TO_POINTER (window_id));
    return win;
}


/**
	@brief 	destroy hash
*/
void window_hash_destroy (void)
{
    g_hash_table_destroy (windows_hash);
}


/**
	@brief 	add widget to hash 
	@param 	window_id: ID of widget
	@param	widget_name: widget name
	@param	widget: widget to add in hash
*/
void add_widget (gint window_id, gchar * widget_name, GtkWidget * widget)
{
    if (!windows_hash) {
		log_msg(MSGL_WARN,"Parent window not exist!\n");
		return;
    }
	
    GtkWidget *parent = get_window (window_id);
    if (!parent) {
		log_msg(MSGL_WARN,"Parent window not exist!\n");
		return;
    }
	
    g_object_set_data_full (G_OBJECT (parent), widget_name, gtk_widget_ref (widget), (GDestroyNotify) gtk_widget_unref);
}


/**
	@brief 	get widget from hash
	@param 	window_id: ID of widget
	@param	widget_name: widget name to search in hash
	@return	widget: widget to matched widget_name
*/
GtkWidget *get_widget (gint window_id, gchar *widget_name)
{
    GtkWidget *parent;
	
    parent = get_window (window_id);
    if (!parent) {
		log_msg(MSGL_WARN, "Parent window not exist!\n");
		return NULL;
    }

    GtkWidget *w = (GtkWidget *) g_object_get_data (G_OBJECT (parent), widget_name);
    if (!w) {
		log_msg(MSGL_INFO, "Widget(%s) not found!\n", widget_name);
		return NULL;
    }
	
    return w;
}


/*
void
raise_window (GtkWidget * win)
{
    if (!gtk_window_has_toplevel_focus (GTK_WINDOW (win)))
	gtk_window_present (GTK_WINDOW (win));
}

void
remove_window (gint window_id)
{
    g_hash_table_remove (windows_hash, GINT_TO_POINTER (window_id));
}
*/


/*
void
print_window (void)
{
    g_hash_table_foreach (windows_hash, print_entry, NULL);
}
*/


/*
void
process_events (void)
{
    while (g_main_iteration (FALSE));
}
*/

/**
	@brief 	create icon widget
	@param 	img_dir: directory of icon file exist
	@param	filename: icon file name
	@return	newly created image widget
*/

GtkWidget *create_pixmap (const gchar* img_dir, const gchar * filename)
{
    gchar* image_file;
    image_file = g_strdup_printf("%s/%s", img_dir, filename);
    GtkWidget *pixmap;

    if (!image_file) {
		g_warning ("Couldn't find pixmap file: %s", image_file);
		return gtk_image_new ();
    }
	
    pixmap = gtk_image_new_from_file (image_file);
    g_free(image_file);
    return pixmap;
}

/*
//======================= Attribute Hash Table Fuction ================================
GHashTable *
attr_tlb_init (void)
{
    return (GHashTable *) g_hash_table_new_full (g_direct_hash,
						 g_direct_equal, NULL, NULL);
}

int
attr_tlb_set_ent (GHashTable * hash, gint key, gpointer value, int size)
{
    AttrTlbEnt *new_entry;

    new_entry = g_hash_table_lookup (hash, GINT_TO_POINTER (key));
    if (new_entry != NULL) {
	g_hash_table_remove (hash, GINT_TO_POINTER (key));
	new_entry = NULL;
    }

    new_entry = (AttrTlbEnt *) g_malloc (sizeof (AttrTlbEnt));
    if (new_entry == NULL)
	return -1;

    new_entry->size = size;
    new_entry->value = (gpointer) g_malloc (size);
    if (new_entry->value == NULL)
	return -1;
    // g_print("%d: key_str: %s\n", ++i, (gchar*)key);
    g_memcpy (new_entry->value, value, size);
    g_hash_table_insert (hash, GINT_TO_POINTER (key), (gpointer) new_entry);

    return 0;
}

int
attr_tlb_del_ent (GHashTable * hash, gint key)
{
    AttrTlbEnt *old_entry;
    old_entry = g_hash_table_lookup (hash, GINT_TO_POINTER (key));
    if (old_entry == NULL)
	return -1;

    g_free (old_entry->value);
    old_entry->size = 0;
    old_entry->value = NULL;

    g_hash_table_remove (hash, GINT_TO_POINTER (key));
    g_free (old_entry);

    return 0;
}

gpointer
attr_tlb_find_ent (GHashTable * hash, gint key)
{
    AttrTlbEnt *ret_entry;

    ret_entry =
	(AttrTlbEnt *) g_hash_table_lookup (hash, GINT_TO_POINTER (key));
    if (ret_entry == NULL)
	return NULL;

    return ret_entry->value;
}

GHashTable *
attr_tlb_open (const gchar * fileName)
{
    FILE *fp;
    GHashTable *hash;
    gint valueSize;
    gint prevValueSize = 0;
    gint key;
    gpointer value;

    if ((fp = fopen (fileName, "r")) == NULL) {
	//g_print ("Can't %s file open\n", fileName);
	//g_log(ISE_DOMAIN, G_LOG_LEVEL_DEBUG, "[%s:%s:%5d]Can't %s file open",__FILE__, __func__,  __LINE__, fileName);
	return NULL;
    }

    hash = attr_tlb_init ();

    while (!feof (fp)) {
	fread (&key, sizeof (int), 1, fp);

	fread (&valueSize, sizeof (int), 1, fp);

	// ff();
	// g_print("key =%s. keyLength=%d. valuesize = %d.\n", (gchar*)key, keyLength, valueSize);
	// ff();

	if (prevValueSize == 0) {
	    value = (gpointer) g_malloc (valueSize);
	    prevValueSize = valueSize;
	} else if (valueSize > prevValueSize) {
	    value = (gpointer) g_realloc (value, valueSize);
	    prevValueSize = valueSize;
	}
	fread (value, valueSize, 1, fp);

	// g_print ("%d: key =%s. keyLength=%d. valuesize = %d.\n", ++i, (gchar*)key, keyLength, valueSize);
	attr_tlb_set_ent (hash, key, value, valueSize);
    }

    g_free (value);
    fclose (fp);

    return hash;
}

static void save_entry (gpointer key, gpointer value, gpointer data)
{
    AttrTlbEnt *entry = (AttrTlbEnt *) value;
    FILE *fp = (FILE *) data;
    gint key_int = (gint) key;

    fwrite (&key_int, sizeof (gint), 1, fp);
    fwrite (&entry->size, sizeof (gint), 1, fp);
    fwrite (entry->value, entry->size, 1, fp);
}

int
attr_tlb_save (GHashTable * hash, const gchar * fileName)
{
    FILE *fp;

    if ((fp = fopen (fileName, "w")) == NULL) {
	//g_print ("Can't %s file open\n", fileName);
	g_log(ISE_DOMAIN, G_LOG_LEVEL_DEBUG, "[%s:%s:%5d]Can't %s file open",__FILE__, __func__,  __LINE__, fileName);
	return -1;
    };

    g_hash_table_foreach (hash, save_entry, fp);
    fclose (fp);

    return 0;
}

static void
del_entry (gpointer key, gpointer value, gpointer data)
{
    AttrTlbEnt *old_entry;

    old_entry = (AttrTlbEnt *) value;
    g_free (old_entry->value);
    g_free (old_entry);
}

void
attr_tlb_close (GHashTable * hash)
{
    g_hash_table_foreach (hash, del_entry, NULL);
    g_hash_table_destroy (hash);
}

void
print_entry (gpointer key, gpointer value, gpointer data)
{
    //g_print ("key=0x%x.\n", (gint) key);
    g_log(ISE_DOMAIN, G_LOG_LEVEL_INFO, "[%s:%s:%5d]key=0x%x.",__FILE__, __func__,  __LINE__, (gint)key);
}


//======================= Etc Util Function ======================= 

*/

/**
	@brief 	convert string  to lower case string
	@param 	string string to covert
*/
#ifndef _WIN32
void
strlwr (char *string)
{
    //while (0 != (*string++ = (char) tolower (*string)));
    while (1)
	{

		*string = (char) tolower (*string);

		if (*string == 0)  {
			return;
		}
#if 0
		*string++;
#else
		string++;
#endif
	}
}
#endif

