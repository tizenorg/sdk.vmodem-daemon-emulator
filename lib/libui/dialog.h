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

#ifndef __DIALOG_H__
#define __DIALOG_H__

#include <gtk/gtk.h>
#include <string.h>
#include <stdio.h>
#include <gdk/gdkkeysyms.h>

#include "ui_imageid.h"
#include "utils.h"

void show_message(const char *szTitle, const char *szMessage);
void show_message_for_qemu(const char *szTitle, const char *szMessage);
void clear_show_message(GtkWidget *widget, gpointer data);
void file_open_dialog (gchar *title, GtkWindow *parent, gchar *ext, void (*call_back) (gchar *));
void file_save_dialog (gchar *title, GtkWindow *parent, gchar *default_filename, gchar *ext, void (*call_back) (gchar *));
GtkWidget *input_name_dialog (gchar *dlg_title, gchar *dlg_label, void (*call_back) (GtkWidget *, gpointer));
GtkWidget *rename_dialog (gchar *default_text, gchar *dlg_title, gchar *dlg_label, void (*call_back) (GtkWidget *, gpointer));
GtkWidget *combo_dialog(gchar* dlg_title, gint select_column, void (*call_back)(GtkWidget *, gpointer));
void message_dialog(GtkWindow *parent, GtkMessageType type, GtkButtonsType buttons, gchar *message);
gboolean enter_key_pressed (GtkWidget * widget, GdkEventKey * event, gpointer user_data);

#define _(String)			String

#endif
