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

#ifndef __UTILS_H__
#define __UTILS_H__
#include <gtk/gtk.h>

#include "../libcommon/fileio.h"
#include "../libcommon/logmsg.h"

extern GHashTable *attribute_hash;
extern gint menu_id_array[];
extern gint window_id_array[];

extern gchar *current_es_save_filename;
extern gchar *current_testcase_save_filename;
extern GPid simulator_pid;

GHashTable *window_hash_init (void);
void add_window (GtkWidget * win, gint id);
void raise_window (GtkWidget * win);
void remove_window (gint id);
GtkWidget *get_window (gint id);
void print_window (void);
void window_hash_destroy (void);
void add_widget (gint window_id, gchar * widget_name, GtkWidget * widget);
GtkWidget *get_widget (gint window_id, gchar * widget_name);

void process_events (void);
GtkWidget *create_pixmap (const gchar *, const gchar*);
/*
typedef struct _AttrTlbEnt {
    gint size;
    gpointer value;
} AttrTlbEnt;

typedef struct _iseToolkit {
    gint x;
    gint y;
    gint width;
    gint height;

    gboolean is_active;

    GtkWidget *(*create) (GtkWidget * window);
    GtkWidget *(*destroy) (GtkWidget * window);
} ISEToolkit;


GHashTable *attr_tlb_init (void);

int attr_tlb_set_ent (GHashTable * hash, gint key, gpointer value, int size);
int attr_tlb_del_ent (GHashTable * hash, gint key);
gpointer attr_tlb_find_ent (GHashTable * hash, gint key);
GHashTable *attr_tlb_open (const gchar * fileName);
int attr_tlb_save (GHashTable * hash, const gchar * fileName);
void attr_tlb_close (GHashTable * hash);
void print_entry (gpointer key, gpointer value, gpointer data);
*/
// void dynamic_strcpy(gchar** dest, const gchar* src);

#ifndef _WIN32
void strlwr (char *string);
#endif

#endif
