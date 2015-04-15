/* applets/MenuAction.c
 * Copyright (C) 2015 Trevor Kulhanek <trevor@nocodenolife.com>
 *
 * This file is part of NyanBar.
 *
 * NyanBar is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * NyanBar is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with NyanBar.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "MenuAction.h"

void nyanbar_about()
{
	GtkWidget *about;

	about = gtk_about_dialog_new();
	gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(about), "Nyanbar");
	gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(about), "© Trevor Kulhanek 2015");
	gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(about), "https://github.com/kyawaiiv/NyanBar");
	gtk_about_dialog_set_logo(GTK_ABOUT_DIALOG(about), NULL);

	gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(about),
		"Nyanbar is a cute and functional bar for use with bspwm.");

	gtk_about_dialog_set_license_type(GTK_ABOUT_DIALOG(about), GTK_LICENSE_GPL_3_0);

	gtk_dialog_run(GTK_DIALOG(about));
	gtk_widget_destroy(about);
}

void nyanbar_kill()
{
	system("xkill");
}

void nyanbar_shutdown()
{
	GtkWidget *dialog, *label, *ca;

	dialog = gtk_dialog_new();
	ca = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

	label = gtk_label_new("Are you sure you want to shut down your computer now?");
	gtk_container_add(GTK_CONTAINER(ca), label);
	
	gtk_dialog_add_button(GTK_DIALOG(dialog), "Cancel", GTK_RESPONSE_NO);
	gtk_dialog_add_button(GTK_DIALOG(dialog), "Shut Down", GTK_RESPONSE_YES);

	gtk_widget_show(label);
	gint result = gtk_dialog_run(GTK_DIALOG(dialog));

	switch(result) {
		case GTK_RESPONSE_YES:
			system("systemctl poweroff");
			break;
		case GTK_RESPONSE_NO:
			break;
	}
	gtk_widget_destroy(dialog);
}

void nyanbar_restart()
{
	GtkWidget *dialog, *label, *ca;

	dialog = gtk_dialog_new();
	ca = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

	label = gtk_label_new("Are you sure you want to restart your computer now?");
	gtk_container_add(GTK_CONTAINER(ca), label);
	
	gtk_dialog_add_button(GTK_DIALOG(dialog), "Cancel", GTK_RESPONSE_NO);
	gtk_dialog_add_button(GTK_DIALOG(dialog), "Restart", GTK_RESPONSE_YES);

	gtk_widget_show(label);
	gint result = gtk_dialog_run(GTK_DIALOG(dialog));

	switch(result) {
		case GTK_RESPONSE_YES:
			system("systemctl reboot");
			break;
		case GTK_RESPONSE_NO:
			break;
	}
	gtk_widget_destroy(dialog);
}

void nyanbar_sleep()
{
	system("systemctl suspend");
}


void nyanbar_logout()
{
	GtkWidget *dialog, *label, *ca;

	dialog = gtk_dialog_new();
	ca = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

	label = gtk_label_new("Are you sure you want to quit all applications and log out now?");
	gtk_container_add(GTK_CONTAINER(ca), label);
	
	gtk_dialog_add_button(GTK_DIALOG(dialog), "Cancel", GTK_RESPONSE_NO);
	gtk_dialog_add_button(GTK_DIALOG(dialog), "Logout", GTK_RESPONSE_YES);

	gtk_widget_show(label);
	gint result = gtk_dialog_run(GTK_DIALOG(dialog));

	switch(result) {
		case GTK_RESPONSE_YES:
			system("pkill X");
			break;
		case GTK_RESPONSE_NO:
			break;
	}
	gtk_widget_destroy(dialog);
}
