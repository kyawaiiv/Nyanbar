/* applets/menuitem-applet.c
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
#include "menuitem-applet.h"
#include "nyanbar-settings.h"
#include "menubar-applet.h"

#include <stdlib.h>
#include <string.h>

G_DEFINE_TYPE(MenuitemApplet, menuitem_applet, BAR_APPLET_TYPE)

static void menuitem_applet_init(MenuitemApplet *self);
static void menuitem_applet_class_init(MenuitemAppletClass *klass);
static void menuitem_applet_dispose(GObject *object);
static gboolean menuitem_applet_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data);
static gboolean on_update(GtkWidget *widget);
static gboolean on_enter(GtkWidget *widget, GdkEvent *event, gpointer user_data);
static gboolean on_leave(GtkWidget *widget, GdkEvent *event, gpointer user_data);
static gboolean on_click(GtkWidget *widget, GdkEvent *event, gpointer user_data);

static void menuitem_applet_init(MenuitemApplet *self)
{
	self->da = gtk_drawing_area_new();
	gtk_container_add(GTK_CONTAINER(self), self->da);
	g_signal_connect(G_OBJECT(self->da), "draw", G_CALLBACK(menuitem_applet_draw), self);
	g_signal_connect(G_OBJECT(self->da), "enter-notify-event", G_CALLBACK(on_enter), self);
	g_signal_connect(G_OBJECT(self->da), "leave-notify-event", G_CALLBACK(on_leave), self);
	g_signal_connect(G_OBJECT(self->da), "button_press_event", G_CALLBACK(on_click), self);

	gtk_widget_set_events(self->da, gtk_widget_get_events(self->da)
		| GDK_ENTER_NOTIFY_MASK
		| GDK_LEAVE_NOTIFY_MASK
		| GDK_BUTTON_PRESS_MASK);

	self->selected = FALSE;
}

static void menuitem_applet_class_init(MenuitemAppletClass *klass)
{
	GObjectClass *g_object_class;

	g_object_class = G_OBJECT_CLASS(klass);
	g_object_class->dispose = &menuitem_applet_dispose;
}

static void menuitem_applet_dispose(GObject *object)
{
	G_OBJECT_CLASS(menuitem_applet_parent_class)->dispose(object);
}


MenuitemApplet *menuitem_applet_new(gchar *label, gpointer command)
{
	MenuitemApplet *self;
	self = g_object_new(MENUITEM_APPLET_TYPE, NULL);
	self->label = label;
	self->command = command;

	return self;
}

static gboolean on_enter(GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
	MENUITEM_APPLET(user_data)->selected = TRUE;
	gtk_widget_queue_draw(widget);
	return TRUE;
}

static gboolean on_leave(GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
	MENUITEM_APPLET(user_data)->selected = FALSE;
	gtk_widget_queue_draw(widget);
	return TRUE;
}

static gboolean on_click(GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
	menubar_applet_hide(MENUBAR_APPLET(MENUITEM_APPLET(user_data)->owner));
	MENUITEM_APPLET(user_data)->selected = FALSE;
	MENUITEM_APPLET(user_data)->command();
}

static gboolean menuitem_applet_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data)
{
	static PangoLayout *plo;
	PangoFontDescription *pfd;
	cairo_font_extents_t extents;
	GdkRGBA c;
	gint h;

	plo = pango_cairo_create_layout(cr);
	pfd = pango_font_description_from_string(MENUITEM_APPLET(user_data)->font);
	pango_font_description_set_weight(pfd, 450);
	pango_layout_set_font_description(plo, pfd);
	pango_font_description_free(pfd);

	pango_cairo_context_set_resolution(pango_layout_get_context(plo), DPI);
	cairo_font_extents(cr, &extents);

	/* do not paint bg if same as window */
	if(MENUITEM_APPLET(user_data)->selected == TRUE) {
		gdk_rgba_parse(&c, MENUITEM_APPLET(user_data)->highlight);
		cairo_set_source_rgba(cr, c.red, c.green, c.blue, 0.90);
		cairo_paint(cr);
	}
	cairo_move_to(cr, menu_padding, 0);
	pango_layout_set_text(plo, MENUITEM_APPLET(user_data)->label, -1);
	gdk_rgba_parse(&c, MENUITEM_APPLET(user_data)->fgcolor);
	cairo_set_source_rgba(cr, c.red, c.green, c.blue, c.alpha);
	pango_cairo_show_layout(cr, plo);

	pango_layout_get_pixel_size(plo, NULL, &h);
	gtk_widget_set_size_request(MENUITEM_APPLET(user_data)->da, menu_w, h);
}
