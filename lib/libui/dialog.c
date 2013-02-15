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
	@file 	dialog.c
	@brief	miscellaneous functions
*/

#include "dialog.h"


/**
 * @brief   show message dialog
 * @param   dialog title 
 * @param   dialog message 
 * @date    Nov 21. 2008
 * */
gchar current_filename[255];

void show_message(const char *szTitle, const char *szMessage)
{
	GtkWidget *label;
	GtkWidget *button;
	GtkWidget *dialog_window;
	PangoFontDescription *pPangoFont = NULL;

	/* create Dialog */
	
	dialog_window = gtk_dialog_new();

	/* Widget Icon set */

	GdkPixbuf* tmppixbuf = NULL;
	tmppixbuf=gtk_widget_render_icon((GtkWidget *) (dialog_window), GTK_STOCK_PROPERTIES, GTK_ICON_SIZE_MENU, NULL);
	gtk_window_set_icon(GTK_WINDOW (dialog_window), tmppixbuf);

	/* set Title and Border */
	
	gtk_window_set_title(GTK_WINDOW(dialog_window), szTitle);
	gtk_container_set_border_width(GTK_CONTAINER(dialog_window), 0);

	/* create OK Button and Set Reponse */
	
	button = gtk_dialog_add_button(GTK_DIALOG(dialog_window), GTK_STOCK_OK, GTK_RESPONSE_OK);

	/* set OK Button to Default Button */
	
	GTK_WIDGET_SET_FLAGS(button, GTK_CAN_DEFAULT);
	gtk_widget_grab_default(button);

	/* create Label */
	
	label = gtk_label_new(szMessage);

	/* set Mono Space Font */
	
	pPangoFont = pango_font_description_from_string("Monospace 11");
	gtk_widget_modify_font(label, pPangoFont);

	/* set Padding arround Label */ 
	
	gtk_misc_set_padding(GTK_MISC(label), 10, 10);

	/* pack Label to Dialog */
	
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog_window)->vbox), label, TRUE, TRUE, 0);

	gtk_widget_show_all(dialog_window);

	gtk_dialog_run(GTK_DIALOG(dialog_window));

	gtk_widget_destroy(dialog_window);
}


void show_message_for_qemu(const char *szTitle, const char *szMessage)
{
	GtkWidget *label;
	GtkWidget *button;
	GtkWidget *dialog_window;
	PangoFontDescription *pPangoFont = NULL;

	/* create Dialog */
	
	dialog_window = gtk_dialog_new();

	/* Widget Icon set */

	GdkPixbuf* tmppixbuf = NULL;
	tmppixbuf=gtk_widget_render_icon((GtkWidget *) (dialog_window), GTK_STOCK_PROPERTIES, GTK_ICON_SIZE_MENU, NULL);
	gtk_window_set_icon(GTK_WINDOW (dialog_window), tmppixbuf);

	/* set Title and Border */
	
	gtk_window_set_title(GTK_WINDOW(dialog_window), szTitle);
	gtk_container_set_border_width(GTK_CONTAINER(dialog_window), 0);

	/* create OK Button and Set Reponse */
	
	button = gtk_dialog_add_button(GTK_DIALOG(dialog_window), GTK_STOCK_OK, GTK_RESPONSE_OK);

	/* set OK Button to Default Button */
	
	GTK_WIDGET_SET_FLAGS(button, GTK_CAN_DEFAULT);
	gtk_widget_grab_default(button);

	/* create Label */
	
	label = gtk_label_new(szMessage);

	/* set Mono Space Font */
	
	pPangoFont = pango_font_description_from_string("Monospace 11");
	gtk_widget_modify_font(label, pPangoFont);

	/* set Padding arround Label */ 
	
	gtk_misc_set_padding(GTK_MISC(label), 10, 10);

	/* pack Label to Dialog */
	
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog_window)->vbox), label, TRUE, TRUE, 0);

	gtk_widget_show_all(dialog_window);

    g_signal_connect_swapped (dialog_window, "response", G_CALLBACK (gtk_widget_destroy),
			      (gpointer) dialog_window);

	g_signal_connect_swapped (dialog_window, "delete-event", G_CALLBACK (gtk_widget_destroy),
			      (gpointer) dialog_window);

//	gtk_dialog_run(GTK_DIALOG(dialog_window));

//	gtk_widget_destroy(dialog_window);
}


/**
 * @brief   remove show message window
 * @param   widget: wiget to remove
 * @return  data: pointer to data
 * @date    Nov 21. 2008
 * */
void clear_show_message(GtkWidget *widget, gpointer data)
{
	gtk_grab_remove(widget);
}


/**
 * @brief   dialog for file open
 * @param   title: title of dialog
 * @param	parent: parent window
 * @param	ext: extension of file to filter
 * @param   call_back: callback function called when OK clicked
 * @date    Nov 21. 2008
 * */
void file_open_dialog (gchar *title, GtkWindow *parent, gchar *ext, void (*call_back) (gchar *))
{
    GtkWidget *file_chooser = gtk_file_chooser_dialog_new (title,
							   parent,
							   GTK_FILE_CHOOSER_ACTION_OPEN,
							   GTK_STOCK_CANCEL,
							   GTK_RESPONSE_CANCEL,
							   GTK_STOCK_OPEN,
							   GTK_RESPONSE_ACCEPT,
							   NULL);

	/* path setting (20090310) */
	gchar *ptr = get_bin_path();

	if ((ptr != NULL) && (strlen(ptr) > 1))
		if ((current_filename == NULL) || (strlen(current_filename) < 1))
			snprintf(current_filename, sizeof(current_filename), "%s", ptr);

	g_free(ptr);
    gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (file_chooser), current_filename);

    if (ext) {
		char *t;
		GtkFileFilter *filter;
		filter = gtk_file_filter_new ();

		t = g_malloc (strlen (ext) + 1);
		strcpy (t, ext);

		gtk_file_filter_add_pattern (filter, t);
		gtk_file_chooser_set_filter (GTK_FILE_CHOOSER (file_chooser), filter);
    }

    if (gtk_dialog_run (GTK_DIALOG (file_chooser)) == GTK_RESPONSE_ACCEPT) {
		gchar *filename, *t;
		filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (file_chooser));

		t = (gchar *) g_malloc (strlen (filename) + 1);
		strcpy (t, filename);
		call_back (t);
		g_free (filename);
    }

    gtk_widget_destroy (file_chooser);


}


/**
 * @brief   dialog for file save
 * @param   title: title of dialog
 * @param	parent: parent window
 * @param	default_filename: default filename for save
 * @param   ext: extension of file to filter
 * @param   call_back: callback function called when OK clicked
 * @date    Nov 21. 2008
 * */
void file_save_dialog (gchar *title, GtkWindow *parent, gchar *default_filename, gchar *ext, void (*call_back) (gchar *))
{
    GtkWidget *file_chooser = gtk_file_chooser_dialog_new (title,
							   parent,
							   GTK_FILE_CHOOSER_ACTION_SAVE,
							   GTK_STOCK_CANCEL,
							   GTK_RESPONSE_CANCEL,
							   GTK_STOCK_SAVE,
							   GTK_RESPONSE_ACCEPT,
							   NULL);

    if (ext) {
		GtkFileFilter *filter;
		filter = gtk_file_filter_new ();

		gtk_file_filter_add_pattern (filter, ext);
		gtk_file_chooser_set_filter (GTK_FILE_CHOOSER (file_chooser), filter);
    }

    if (default_filename)
		gtk_file_chooser_set_current_name (GTK_FILE_CHOOSER (file_chooser), default_filename);

	/* path setting (20090310) */
	gchar *ptr = get_bin_path();

	if ((ptr != NULL) && (strlen(ptr) > 1))
		if ((current_filename == NULL) || (strlen(current_filename) < 1))
			snprintf(current_filename, sizeof(current_filename), "%s", ptr);

	g_free(ptr);
    gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (file_chooser), current_filename);

    if (gtk_dialog_run (GTK_DIALOG (file_chooser)) == GTK_RESPONSE_ACCEPT) {
		gchar *filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (file_chooser));
		strncpy(current_filename, filename, sizeof(current_filename));
		call_back (filename);
		g_free (filename);
    }
	
    gtk_widget_destroy (file_chooser);
}


/**
 * @brief   dialog for get name
 * @param   dlg_title: title of dialog
 * @param	dlg_label: label of dialog
 * @param	call_back: callback function called when OK clicked
 * @return	dialog window widget
 * @date    Nov 21. 2008
 * */
GtkWidget *input_name_dialog (gchar *dlg_title, gchar *dlg_label, void (*call_back) (GtkWidget *, gpointer))
{
    GtkWidget *dlg;

    dlg = gtk_dialog_new ();

    gtk_window_set_title (GTK_WINDOW (dlg), (dlg_title));
    gtk_window_set_modal (GTK_WINDOW (dlg), TRUE);
    gtk_window_set_type_hint (GTK_WINDOW (dlg), GDK_WINDOW_TYPE_HINT_DIALOG);

    GtkWidget *vbox, *hbox;
    vbox = GTK_DIALOG (dlg)->vbox;


    GtkWidget *label;

    label = gtk_label_new (_(dlg_label));
    gtk_box_pack_start (GTK_BOX (vbox), label, FALSE, FALSE, 0);


    hbox = gtk_hbox_new (FALSE, 0);
    gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);

    label = gtk_label_new (_("Name : "));
    gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
    GtkWidget *entry = gtk_entry_new ();
    gtk_box_pack_start (GTK_BOX (hbox), entry, TRUE, TRUE, 0);

    GtkWidget *dlg_action_area = GTK_DIALOG (dlg)->action_area;
    gtk_button_box_set_layout (GTK_BUTTON_BOX (dlg_action_area), GTK_BUTTONBOX_END);

    GtkWidget *cancel_btn = gtk_button_new_from_stock ("gtk-cancel");
    gtk_dialog_add_action_widget (GTK_DIALOG (dlg), cancel_btn, GTK_RESPONSE_CANCEL);
    GTK_WIDGET_SET_FLAGS (cancel_btn, GTK_CAN_DEFAULT);

    GtkWidget *ok_btn = gtk_button_new_from_stock ("gtk-ok");
    gtk_dialog_add_action_widget (GTK_DIALOG (dlg), ok_btn, GTK_RESPONSE_OK);
    gtk_widget_show_all (dlg);

    g_signal_connect ((gpointer) ok_btn, "clicked", G_CALLBACK (call_back), (gpointer) entry);
    g_signal_connect_swapped ((gpointer) ok_btn, "clicked", G_CALLBACK (gtk_widget_destroy), (gpointer) dlg);
    g_signal_connect_swapped ((gpointer) cancel_btn, "clicked", G_CALLBACK (gtk_widget_destroy), (gpointer) dlg);

    return dlg;
}


/**
	@brief	dialog for rename dialog
	@param	default_text: default text
	@param 	dlg_title: diglog title
	@param 	dlg_label: dialog label
	@param 	call_back: callback function called when OK clicked
	@return	dialog window widget
*/
GtkWidget *rename_dialog (gchar *default_text, gchar *dlg_title, gchar *dlg_label, void (*call_back) (GtkWidget *, gpointer))
{
    GtkWidget *dlg;

    dlg = gtk_dialog_new ();

    gtk_window_set_title (GTK_WINDOW (dlg), _(dlg_title));
    gtk_window_set_modal (GTK_WINDOW (dlg), TRUE);
    gtk_window_set_type_hint (GTK_WINDOW (dlg), GDK_WINDOW_TYPE_HINT_DIALOG);

    GtkWidget *vbox, *hbox;
    vbox = GTK_DIALOG (dlg)->vbox;


    GtkWidget *label;

    label = gtk_label_new (_(dlg_label));
    gtk_box_pack_start (GTK_BOX (vbox), label, FALSE, FALSE, 0);


    hbox = gtk_hbox_new (FALSE, 0);
    gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);

    label = gtk_label_new (_("Name : "));
    gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
    GtkWidget *entry = gtk_entry_new ();
    gtk_entry_set_text (GTK_ENTRY (entry), default_text);
    gtk_box_pack_start (GTK_BOX (hbox), entry, TRUE, TRUE, 0);

    GtkWidget *dlg_action_area = GTK_DIALOG (dlg)->action_area;
    gtk_button_box_set_layout (GTK_BUTTON_BOX (dlg_action_area),
			       GTK_BUTTONBOX_END);

    GtkWidget *cancel_btn = gtk_button_new_from_stock ("gtk-cancel");
    gtk_dialog_add_action_widget (GTK_DIALOG (dlg), cancel_btn,
				  GTK_RESPONSE_CANCEL);
    GTK_WIDGET_SET_FLAGS (cancel_btn, GTK_CAN_DEFAULT);

    GtkWidget *ok_btn = gtk_button_new_from_stock ("gtk-ok");
    gtk_dialog_add_action_widget (GTK_DIALOG (dlg), ok_btn, GTK_RESPONSE_OK);
    gtk_widget_show_all (dlg);

#if 0
	g_signal_connect ((gpointer) entry, "key-press-event", G_CALLBACK (enter_key_pressed), (gpointer) dlg);
#endif
    g_signal_connect ((gpointer) ok_btn, "clicked", G_CALLBACK (call_back), (gpointer) entry);
    g_signal_connect_swapped ((gpointer) ok_btn, "clicked", G_CALLBACK (gtk_widget_destroy), (gpointer) dlg);
    g_signal_connect_swapped ((gpointer) cancel_btn, "clicked", G_CALLBACK (gtk_widget_destroy), (gpointer) dlg);

    return dlg;
}

/**
	@brief	add combo box for plmn status message
	@param	combo: combo box to append text
	
*/

static void
add_plmn_status_msg (GtkWidget * combo)
{
    gtk_combo_box_append_text (GTK_COMBO_BOX (combo), _("PLMN_UNKNOWN"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (combo), _("PLMN_AVAIL"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (combo), _("PLMN_CURRENT"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (combo), _("PLMN_FORBIDDEN"));
}


/**
	@brief	add combo box for technology message
	@param	combo: combo box to append text
	
*/

static void
add_access_technology_msg (GtkWidget * combo)
{
    gtk_combo_box_append_text (GTK_COMBO_BOX (combo), _("GSM"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (combo), _("GSM Compact"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (combo), _("GPRS"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (combo), _("UTRAN"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (combo), _("EGPRS"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (combo), _("NOT SPECIFIED"));
}


/**
	@brief	dialog for combo box
	@param	dlg_title: title of dialog
	@param 	select_column: combo box to display
	@param 	call_back: callback function called when OK clicked
	@return	dialog window widget
*/

GtkWidget *combo_dialog (gchar * dlg_title, gint select_column, void (*call_back) (GtkWidget *, gpointer))
{
    GtkWidget *dlg;
    GtkWidget *label;

    dlg = gtk_dialog_new ();

    gtk_window_set_title (GTK_WINDOW (dlg), _(dlg_title));
    gtk_window_set_modal (GTK_WINDOW (dlg), TRUE);
    gtk_window_set_type_hint (GTK_WINDOW (dlg), GDK_WINDOW_TYPE_HINT_DIALOG);

    GtkWidget *vbox, *hbox;
    vbox = GTK_DIALOG (dlg)->vbox;

    hbox = gtk_hbox_new (FALSE, 0);
    gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 20);

    label = gtk_label_new (_("Select : "));
    // gtk_box_pack_start (GTK_BOX(hbox), label, FALSE, FALSE, 0);
    gtk_box_pack_start_defaults (GTK_BOX (hbox), label);

    GtkWidget *combo = gtk_combo_box_new_text ();
    // gtk_box_pack_start (GTK_BOX(hbox), combo, FALSE, FALSE, 0);
    gtk_box_pack_start_defaults (GTK_BOX (hbox), combo);

    if (select_column == 1) {
	// FIXME
	add_plmn_status_msg (combo);
    }

    else {
	// FIXME
	add_access_technology_msg (combo);
	// add_rssi_level_msg(combo); 
    }

    GtkWidget *dlg_action_area = GTK_DIALOG (dlg)->action_area;
    gtk_button_box_set_layout (GTK_BUTTON_BOX (dlg_action_area),
			       GTK_BUTTONBOX_END);

    GtkWidget *cancel_btn = gtk_button_new_from_stock ("gtk-cancel");
    gtk_dialog_add_action_widget (GTK_DIALOG (dlg), cancel_btn,
				  GTK_RESPONSE_CANCEL);
    GTK_WIDGET_SET_FLAGS (cancel_btn, GTK_CAN_DEFAULT);

    GtkWidget *ok_btn = gtk_button_new_from_stock ("gtk-ok");
    gtk_dialog_add_action_widget (GTK_DIALOG (dlg), ok_btn, GTK_RESPONSE_OK);
    gtk_widget_show_all (dlg);

    g_signal_connect ((gpointer) ok_btn, "clicked", G_CALLBACK (call_back),
		      (gpointer) combo);

    g_signal_connect_swapped ((gpointer) ok_btn, "clicked",
			      G_CALLBACK (gtk_widget_destroy),
			      (gpointer) dlg);

    g_signal_connect_swapped ((gpointer) cancel_btn, "clicked",
			      G_CALLBACK (gtk_widget_destroy),
			      (gpointer) dlg);

    return dlg;
}


/**
	@brief	callback when enter key pressed in rename_dialgo and set_up_menu_dialog
	@param	widget	the Widget event happend
	@param 	event	event type
	@param 	user_data	dialog widget
	@return	true if event is enter key or false
*/

gboolean
enter_key_pressed (GtkWidget * widget, GdkEventKey * event,
		   gpointer user_data)
{
    if (event->keyval == GDK_Return) {
	GtkWidget *entry = GTK_WIDGET (widget);
	GtkWidget *clist = get_widget (EVENT_MANAGER_ID, "ID_CLIST_ES_DESC");
	const gchar *text;
	text = gtk_entry_get_text (GTK_ENTRY (entry));

	gtk_clist_set_text (GTK_CLIST (clist), GTK_CLIST (clist)->focus_row,
			    5, text);
	gtk_widget_destroy (GTK_WIDGET (GTK_WIDGET (user_data)));
	return TRUE;
    } else
	return FALSE;
}

/**
	@brief	dialog for message
	@param	parent: widget of parent window
	@param	type: GTK message type
	@param	buttons: type of button
	@param	message: mesage for dialog

*/

void
message_dialog (GtkWindow * parent, GtkMessageType type,
		GtkButtonsType buttons, gchar * message)
{
    GtkWidget *dialog =
	gtk_message_dialog_new (parent, GTK_DIALOG_DESTROY_WITH_PARENT,
				type,
				buttons, message);
    gtk_dialog_run (GTK_DIALOG (dialog));
    gtk_widget_destroy (dialog);
}

