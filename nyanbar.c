/* nyanbar.c
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
#include "nyanbar.h"
#include "nyanbar-settings.h"
#include "applets/desktops-applet.h"
#include "applets/clock-applet.h"
#include "applets/launcher-applet.h"
#include "applets/menu-applet.h"
#include "applets/menuitem-applet.h"
#include "applets/menubar-applet.h"
#include "applets/menu-separator-applet.h"
#include "applets/MenuAction.h"

#include <string.h>
#include <stdlib.h>

#include <pwd.h>
#include <stdio.h>
#include <sys/types.h>

G_DEFINE_TYPE(NyanBar, nyanbar, GTK_TYPE_WINDOW)

static void nyanbar_init(NyanBar *self);
static void nyanbar_class_init(NyanBarClass *klass);
static void nyanbar_dispose(GObject *object);
static gboolean nyanbar_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data);

static void nyanbar_class_init(NyanBarClass *klass)
{
	GObjectClass *g_object_class;

	g_object_class = G_OBJECT_CLASS(klass);
	g_object_class->dispose = &nyanbar_dispose;
}

static void nyanbar_dispose(GObject *object)
{
	G_OBJECT_CLASS(nyanbar_parent_class)->dispose(object);
}

NyanBar *nyanbar_new(void)
{
	NyanBar *self;
	self = g_object_new(NYANBAR_TYPE, NULL);
	return self;
}

static void on_realized(GtkWidget *widget, gpointer user_data)
{
	NyanBar *self;
	GdkScreen *screen;
	gint height, width;

	self = NYANBAR(user_data);
	screen = gtk_widget_get_screen(widget);
	width = gdk_screen_get_width(screen);
	height = gdk_screen_get_height(screen);

	gtk_window_move(GTK_WINDOW(self), (width - BARW) / 2, topbar ? 0 : height - BARH);
	gtk_widget_queue_draw(GTK_WIDGET(self));
}

static gboolean on_update(GtkWidget *widget)
{
	gtk_widget_queue_draw(widget);
	return TRUE;
}

static void nyanbar_init(NyanBar *self)
{
	GdkScreen *screen;
	GdkVisual *visual;

	GtkWidget *layout;

	GtkWidget *desktops;
	GtkWidget *clock;
	GtkWidget *launcher;
	GtkWidget *menu;

	GKeyFile *settings;
	gchar *color, *font;
	GError *error = NULL;

	/* init window properties */
	gtk_widget_set_app_paintable(GTK_WIDGET(self), TRUE);
	screen = gdk_screen_get_default();
	visual = gdk_screen_get_rgba_visual(screen);
	gtk_widget_set_visual(GTK_WIDGET(self), visual);

	gtk_window_set_default_size(GTK_WINDOW(self), BARW, BARH);
	gtk_window_set_type_hint(GTK_WINDOW(self), GDK_WINDOW_TYPE_HINT_DOCK);

	/* signals */
	g_signal_connect(self, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(self, "draw", G_CALLBACK(nyanbar_draw), self);
	g_signal_connect(self, "realize", G_CALLBACK(on_realized), self);

	/* init layout */
	layout = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_widget_set_valign(layout, GTK_ALIGN_START);
	gtk_container_add(GTK_CONTAINER(self), layout);

	/* add menu applet */
	menu = menu_applet_new();
	gtk_box_pack_start(GTK_BOX(layout), menu, FALSE, FALSE, 20);

	/* add desktops applet */
	desktops = desktops_applet_new();
	self->desktops = desktops;
	gtk_box_pack_start(GTK_BOX(layout), desktops, FALSE, FALSE, 0);

	/* add launcher applet to end*/
	launcher = launcher_applet_new();
	self->launcher = launcher;
	gtk_box_pack_end(GTK_BOX(layout), launcher, FALSE, FALSE, launcher_padding);

	/* add clock applet to center | end */
	clock = clock_applet_new();
	self->clock = clock;
	if(center_clock)
		gtk_box_set_center_widget(GTK_BOX(layout), clock);
	else
		gtk_box_pack_end(GTK_BOX(layout), clock, FALSE, FALSE, 0);

	/* load settings */
	struct passwd *passwdEnt = getpwuid(getuid());
	char *path = passwdEnt->pw_dir;
	settings = g_key_file_new();
	strcat(path, "/.nyanbar/themes/");
	strcat(path, theme);

	g_key_file_load_from_file(settings, path, G_KEY_FILE_NONE, &error);
	self->font = g_key_file_get_string(settings, "NyanBar", "font", &error);

	/* main / fallback colors */
	self->bgcolor = g_key_file_get_string(settings, "NyanBar", "background-color", &error);
	self->fgcolor = g_key_file_get_string(settings, "NyanBar", "foreground-color", &error);
	self->highlight = g_key_file_get_string(settings, "NyanBar", "highlight-color", &error);

	/* fonts */
	font = g_key_file_get_string(settings, "Clock Applet", "font", &error);
	CLOCK_APPLET(clock)->font = font != NULL ? font : self->font;

	font = g_key_file_get_string(settings, "Desktops Applet", "font", &error);
	DESKTOPS_APPLET(desktops)->font = font != NULL ? font : self->font;

	/* desktops colors */
	color = g_key_file_get_string(settings, "Desktops Applet", "foreground-normal", &error);
	DESKTOPS_APPLET(desktops)->norm_fg = color != NULL ? color : self->fgcolor;
	color = g_key_file_get_string(settings, "Desktops Applet", "foreground-occupied", &error);
	DESKTOPS_APPLET(desktops)->occ_fg = color != NULL ? color : self->fgcolor;
	color = g_key_file_get_string(settings, "Desktops Applet", "foreground-urgent", &error);
	DESKTOPS_APPLET(desktops)->urg_fg = color != NULL ? color : self->fgcolor;
	color = g_key_file_get_string(settings, "Desktops Applet", "separator-color", &error);
	DESKTOPS_APPLET(desktops)->separator_color = color != NULL ? color : self->fgcolor;

	/* clock colors*/
	color = g_key_file_get_string(settings, "Clock Applet", "color", &error);
	CLOCK_APPLET(clock)->color = color != NULL ? color : self->fgcolor;

	/* launcher colors */
	color = g_key_file_get_string(settings, "Launcher Applet", "normal-fg", &error);
	LAUNCHER_APPLET(launcher)->norm_fg = color != NULL ? color : self->fgcolor;
	color = g_key_file_get_string(settings, "Launcher Applet", "normal-bg", &error);
	LAUNCHER_APPLET(launcher)->norm_bg = color != NULL ? color : self->bgcolor;
	color = g_key_file_get_string(settings, "Launcher Applet", "highlight-fg", &error);
	LAUNCHER_APPLET(launcher)->highlight_fg = color != NULL ? color : self->fgcolor;
	color = g_key_file_get_string(settings, "Launcher Applet", "highlight-bg", &error);
	LAUNCHER_APPLET(launcher)->highlight_bg = color != NULL ? color : self->highlight;

	/* create menu */
	MenuitemApplet *menu_about;
	menu_about = menuitem_applet_new("About NyanBar", nyanbar_about);
	menubar_applet_add(MENU_APPLET(menu)->menubar, GTK_WIDGET(menu_about));
	
	GtkWidget *separator1;
	separator1 = menu_separator_applet_new();
	menubar_applet_add(MENU_APPLET(menu)->menubar, separator1);

	MenuitemApplet *menu_kill;
	menu_kill = menuitem_applet_new("Force Quit...", nyanbar_kill);
	menubar_applet_add(MENU_APPLET(menu)->menubar, GTK_WIDGET(menu_kill));

	GtkWidget *separator2;
	separator2 = menu_separator_applet_new();
	menubar_applet_add(MENU_APPLET(menu)->menubar, separator2);

	MenuitemApplet *menu_sleep;
	menu_sleep = menuitem_applet_new("Sleep", nyanbar_sleep);
	menubar_applet_add(MENU_APPLET(menu)->menubar, GTK_WIDGET(menu_sleep));

	MenuitemApplet *menu_restart;
	menu_restart = menuitem_applet_new("Restart...", nyanbar_restart);
	menubar_applet_add(MENU_APPLET(menu)->menubar, GTK_WIDGET(menu_restart));

	MenuitemApplet *menu_shutdown;
	menu_shutdown = menuitem_applet_new("Shut Down...", nyanbar_shutdown);
	menubar_applet_add(MENU_APPLET(menu)->menubar, GTK_WIDGET(menu_shutdown));

	GtkWidget *separator3;
	separator3 = menu_separator_applet_new();
	menubar_applet_add(MENU_APPLET(menu)->menubar, separator3);
	
	MenuitemApplet *menu_logout;
	menu_logout = menuitem_applet_new("Log Out...", nyanbar_logout);
	menubar_applet_add(MENU_APPLET(menu)->menubar, GTK_WIDGET(menu_logout));

	/* TODO: settings should be set on object init
		 so that all instances inherit settings? */ 	
	int i;
	for(i = 0; i < MENU_APPLET(menu)->menubar->size; i++) {
		/* Menu items should not draw own (normal) bg color!
		color = g_key_file_get_string(settings, "Menu Applet", "background-color", &error);
		MENU_APPLET(menu)->menubar->menus[i]->bgcolor = color != NULL ? color : self->bgcolor;
		*/
		color = g_key_file_get_string(settings, "Menu Applet", "foreground-color", &error);
		MENU_APPLET(menu)->menubar->menus[i]->fgcolor = color != NULL ? color : self->fgcolor;

		color = g_key_file_get_string(settings, "Menu Applet", "highlight", &error);
		MENU_APPLET(menu)->menubar->menus[i]->highlight = color != NULL ? color : self->highlight;

		font = g_key_file_get_string(settings, "Menu Applet", "font", &error);
		MENU_APPLET(menu)->menubar->menus[i]->font = font;
	}

	/* menu window bg color */
	color = g_key_file_get_string(settings, "Menu Applet", "background-color", &error);
	MENU_APPLET(menu)->menubar->bgcolor = color != NULL ? color : self->bgcolor;

	/* menu separator color */
	color = g_key_file_get_string(settings, "Menu Applet", "separator-color", &error);
	MENU_SEPARATOR_APPLET(separator1)->color = color != NULL ? color : self->fgcolor;
	color = g_key_file_get_string(settings, "Menu Applet", "separator-color", &error);
	MENU_SEPARATOR_APPLET(separator2)->color = color != NULL ? color : self->fgcolor;
	color = g_key_file_get_string(settings, "Menu Applet", "separator-color", &error);
	MENU_SEPARATOR_APPLET(separator3)->color = color != NULL ? color : self->fgcolor;

	/* menu applet icon colors */
	color = g_key_file_get_string(settings, "Menu Applet", "icon-foreground", &error);
	MENU_APPLET(menu)->fgcolor = color != NULL ? color : self->bgcolor;

	color = g_key_file_get_string(settings, "Menu Applet", "icon-normal-bg", &error);
	MENU_APPLET(menu)->bgcolor = color != NULL ? color : self->bgcolor;

	color = g_key_file_get_string(settings, "Menu Applet", "icon-highlight-bg", &error);
	MENU_APPLET(menu)->hlcolor = color != NULL ? color : self->bgcolor;

	gtk_widget_show_all(GTK_WIDGET(self));
}

static gboolean nyanbar_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data)
{
	GdkRGBA c;

	pango_cairo_create_layout(cr);
	gdk_rgba_parse(&c, NYANBAR(user_data)->bgcolor);
	cairo_set_source_rgba(cr, c.red, c.green, c.blue, 0.80);
	cairo_paint(cr);

	gtk_container_propagate_draw(GTK_CONTAINER(widget), gtk_bin_get_child(GTK_BIN(widget)), cr);
	return FALSE;
}	
