/* applets/menu-applet.c
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
#include "menu-applet.h"
#include "nyanbar-settings.h"
#include "applets/menubar-applet.h"

#include <stdlib.h>
#include <string.h>

G_DEFINE_TYPE(MenuApplet, menu_applet, BAR_APPLET_TYPE)

static void menu_applet_init(MenuApplet *self);
static void menu_applet_class_init(MenuAppletClass *klass);
static void menu_applet_dispose(GObject *object);
static gboolean menu_applet_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data);
static gboolean on_update(GtkWidget *widget);
static gboolean button_clicked(GtkWidget *widget, GdkEventButton *event, gpointer user_data);

static void menu_applet_init(MenuApplet *self)
{
	self->da = gtk_drawing_area_new();
	gtk_widget_set_size_request(self->da, 70, BARH);
	gtk_container_add(GTK_CONTAINER(self), self->da);
	g_signal_connect(G_OBJECT(self->da), "draw", G_CALLBACK(menu_applet_draw), self);
	/* TODO: change to click */
	g_signal_connect(G_OBJECT(self->da), "button_press_event", G_CALLBACK(button_clicked), (gpointer) self);
	g_timeout_add(1000, (GSourceFunc) on_update, (gpointer) self);

	gtk_widget_set_events(self->da, gtk_widget_get_events(self->da)
	    | GDK_BUTTON_PRESS_MASK);

	self->menubar = menubar_applet_new();
	menubar_applet_set_owner(self->menubar, GTK_WIDGET(self));
}

static void menu_applet_class_init(MenuAppletClass *klass)
{
	GObjectClass *g_object_class;

	g_object_class = G_OBJECT_CLASS(klass);
	g_object_class->dispose = &menu_applet_dispose;
}

static void menu_applet_dispose(GObject *object)
{
	G_OBJECT_CLASS(menu_applet_parent_class)->dispose(object);
}

GtkWidget *menu_applet_new(void)
{
	MenuApplet *self;

	self = g_object_new(MENU_APPLET_TYPE, NULL);
	return GTK_WIDGET(self);
}

static gboolean on_update(GtkWidget *widget)
{
	gtk_widget_queue_draw(widget);
	return TRUE;
}

static gboolean button_clicked(GtkWidget *widget, GdkEventButton *event, gpointer user_data)
{
	if(!MENU_APPLET(user_data)->menubar->active)
		menubar_applet_show(MENU_APPLET(user_data)->menubar);
	else
		menubar_applet_hide(MENU_APPLET(user_data)->menubar);

	return TRUE;
}


static gboolean menu_applet_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data)
{
	cairo_surface_t *icon;
	GdkRGBA c;
	int w, h;

	cairo_set_operator(cr, CAIRO_OPERATOR_ATOP);

	/* fill background color */
	gdk_rgba_parse(&c, MENU_APPLET(user_data)->menubar->active
		       ? MENU_APPLET(user_data)->hlcolor
		       : MENU_APPLET(user_data)->bgcolor);
	cairo_set_source_rgba(cr, c.red, c.green, c.blue, 0.80);
	cairo_paint(cr);

	/* set icon color and paint */
	icon = cairo_image_surface_create_from_png("/usr/share/icons/NyanBar/ArchLinux.png");
	w = cairo_image_surface_get_width(icon);
	h = cairo_image_surface_get_height(icon);
	gdk_rgba_parse(&c, MENU_APPLET(user_data)->fgcolor);
	cairo_set_source_rgba(cr, c.red, c.green, c.blue, c.alpha);
	cairo_mask_surface(cr, icon, (70 - w) / 2, (BARH - h) / 2);
	
	return TRUE;
}
