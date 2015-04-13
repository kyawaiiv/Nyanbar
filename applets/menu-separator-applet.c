/* applets/menu_separator-applet.c
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
#include "menu-separator-applet.h"
#include "nyanbar-settings.h"

#include <stdlib.h>
#include <string.h>

G_DEFINE_TYPE(MenuSeparatorApplet, menu_separator_applet, BAR_APPLET_TYPE)

static void menu_separator_applet_init(MenuSeparatorApplet *self);
static void menu_separator_applet_class_init(MenuSeparatorAppletClass *klass);
static void menu_separator_applet_dispose(GObject *object);
static gboolean menu_separator_applet_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data);
static gboolean on_update(GtkWidget *widget);

static void menu_separator_applet_init(MenuSeparatorApplet *self)
{
	self->da = gtk_drawing_area_new();
	gtk_widget_set_size_request(self->da, menu_w, menu_separator_w);
	gtk_container_add(GTK_CONTAINER(self), self->da);
	g_signal_connect(G_OBJECT(self->da), "draw", G_CALLBACK(menu_separator_applet_draw), self);
}

static void menu_separator_applet_class_init(MenuSeparatorAppletClass *klass)
{
	GObjectClass *g_object_class;

	g_object_class = G_OBJECT_CLASS(klass);
	g_object_class->dispose = &menu_separator_applet_dispose;
}

static void menu_separator_applet_dispose(GObject *object)
{
	G_OBJECT_CLASS(menu_separator_applet_parent_class)->dispose(object);
}

GtkWidget *menu_separator_applet_new(void)
{
	MenuSeparatorApplet *self;

	self = g_object_new(MENU_SEPARATOR_APPLET_TYPE, NULL);
	return GTK_WIDGET(self);
}

static gboolean on_update(GtkWidget *widget)
{
	gtk_widget_queue_draw(widget);
	return TRUE;
}

static gboolean menu_separator_applet_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data)
{
	GdkRGBA c;
	gdouble w;

	cairo_set_line_width(cr, menu_separator_w);

	cairo_move_to(cr, 0, 0);
	gdk_rgba_parse(&c, MENU_SEPARATOR_APPLET(user_data)->color);
	cairo_set_source_rgba(cr, c.red, c.green, c.blue, c.alpha);
	cairo_line_to(cr, menu_w, 0);
	cairo_stroke(cr);
}
