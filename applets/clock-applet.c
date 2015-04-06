/* applets/clock-applet.c
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
#include "clock-applet.h"
#include "nyanbar-settings.h"

#include <stdlib.h>
#include <string.h>

G_DEFINE_TYPE(ClockApplet, clock_applet, BAR_APPLET_TYPE)

static void clock_applet_init(ClockApplet *self);
static void clock_applet_class_init(ClockAppletClass *klass);
static void clock_applet_dispose(GObject *object);
static gboolean clock_applet_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data);
static gboolean on_update(GtkWidget *widget);

static void clock_applet_init(ClockApplet *self)
{
	self->da = gtk_drawing_area_new();
	gtk_container_add(GTK_CONTAINER(self), self->da);
	g_signal_connect(G_OBJECT(self->da), "draw", G_CALLBACK(clock_applet_draw), self);
	g_timeout_add(1000, (GSourceFunc) on_update, (gpointer) self);
}

static void clock_applet_class_init(ClockAppletClass *klass)
{
	GObjectClass *g_object_class;

	g_object_class = G_OBJECT_CLASS(klass);
	g_object_class->dispose = &clock_applet_dispose;
}

static void clock_applet_dispose(GObject *object)
{
	G_OBJECT_CLASS(clock_applet_parent_class)->dispose(object);
}

GtkWidget *clock_applet_new(void)
{
	ClockApplet *self;

	self = g_object_new(CLOCK_APPLET_TYPE, NULL);
	return GTK_WIDGET(self);
}

static gboolean on_update(GtkWidget *widget)
{
	gtk_widget_queue_draw(widget);
	return TRUE;
}

static gboolean clock_applet_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data)
{
	static PangoLayout *plo;
	PangoFontDescription *pfd;
	cairo_font_extents_t extents;
	GdkRGBA c;
	time_t current;
	time(&current);
	gchar buf[13];
	gint w;

	plo = pango_cairo_create_layout(cr);
	pfd = pango_font_description_from_string(CLOCK_APPLET(user_data)->font);
	pango_font_description_set_weight(pfd, 450);
	pango_layout_set_font_description(plo, pfd);
	pango_font_description_free(pfd);

	pango_cairo_context_set_resolution(pango_layout_get_context(plo), DPI);
	cairo_font_extents(cr, &extents);

	strftime(buf, 13, clock_format, localtime(&current));
	pango_layout_set_text(plo, buf, -1);
	gdk_rgba_parse(&c, CLOCK_APPLET(user_data)->color);
	cairo_set_source_rgba(cr, c.red, c.green, c.blue, c.alpha);
	cairo_move_to(cr, 0, 0.5 - extents.descent + extents.height / 2);
	pango_cairo_show_layout(cr, plo);

	pango_layout_get_pixel_size(plo, &w, NULL);
	gtk_widget_set_size_request(CLOCK_APPLET(user_data)->da, w, BARH);
}
