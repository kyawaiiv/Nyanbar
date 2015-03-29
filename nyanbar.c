/* nyanbar.c
 * Copyright (C) 2014 Trevor Kulhanek <trevor@nocodenolife.com>
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

	GtkWidget *menu_icon;
	GtkWidget *desktops;
	GtkWidget *clock;
	GtkWidget *launcher;

	GdkPixbuf *pb;

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

	/* display arch linux icon */
	pb = gdk_pixbuf_new_from_file_at_scale("/usr/share/icons/NyanBar/ArchLinux.svg", icon_size, icon_size, TRUE, NULL);
	menu_icon = gtk_image_new_from_pixbuf(pb);
	gtk_box_pack_start(GTK_BOX(layout), menu_icon, FALSE, FALSE, icon_padding);

	/* add desktops widget */
	desktops = desktops_applet_new();
	self->desktops = desktops;
	gtk_box_pack_start(GTK_BOX(layout), desktops, FALSE, FALSE, 0);

	/* add clock widget to center */
	clock = clock_applet_new();
	self->clock = clock;
	gtk_box_set_center_widget(GTK_BOX(layout), clock);

	/* add launcher applet to end*/
	launcher = launcher_applet_new();
	self->launcher = launcher;
	gtk_box_pack_end(GTK_BOX(layout), launcher, FALSE, FALSE, launcher_padding);

	gtk_widget_show_all(GTK_WIDGET(self));
}

static gboolean nyanbar_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data)
{
	GdkRGBA c;

	pango_cairo_create_layout(cr);
	gdk_rgba_parse(&c, bgcolor);
	cairo_set_source_rgba(cr, c.red, c.green, c.blue, 0.80);
	cairo_paint(cr);

	gtk_container_propagate_draw(GTK_CONTAINER(widget), gtk_bin_get_child(GTK_BIN(widget)), cr);
	return FALSE;
}	
