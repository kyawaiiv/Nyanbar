/* applets/desktops-applet.c
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
#include "desktops-applet.h"
#include "nyanbar-settings.h"

#include <fcntl.h> /* O_RDWR, O_NONBLOCK */
#include <string.h>

static gchar desktop_status[] = {'O','f','f','f','f','f','f','f','f','f'};
static gchar *tiling_state = "Ltiled";

G_DEFINE_TYPE(DesktopsApplet, desktops_applet, BAR_APPLET_TYPE)

static void desktops_applet_init(DesktopsApplet *self);
static void desktops_applet_class_init(DesktopsAppletClass *klass);
static void desktops_applet_dispose(GObject *object);
static gboolean desktops_applet_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data);
static gboolean get_desktops_status(GtkWidget *widget);

static void desktops_applet_init(DesktopsApplet *self)
{
	self->da = gtk_drawing_area_new();
	gtk_widget_set_size_request(self->da, BARW / 3, BARH);

	gtk_container_add(GTK_CONTAINER(self), self->da);
	g_signal_connect(G_OBJECT(self->da), "draw", G_CALLBACK(desktops_applet_draw), NULL);
	g_timeout_add(50, (GSourceFunc) get_desktops_status, NULL);
}

static void desktops_applet_class_init(DesktopsAppletClass *klass)
{
	GObjectClass *g_object_class;

	g_object_class = G_OBJECT_CLASS(klass);
	g_object_class->dispose = &desktops_applet_dispose;
}

static void desktops_applet_dispose(GObject *object)
{
	G_OBJECT_CLASS(desktops_applet_parent_class)->dispose(object);
}

GtkWidget *desktops_applet_new(void)
{
	DesktopsApplet *self;

	self = g_object_new(DESKTOPS_APPLET_TYPE, NULL);
	return GTK_WIDGET(self);
}

static gboolean desktops_applet_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data)
{
	static PangoLayout *plo;
	PangoFontDescription *pfd;
	cairo_font_extents_t extents;
	GdkRGBA c;
	gint x = 0;
	gint w;
	gint i;

	plo = pango_cairo_create_layout(cr);
	pfd = pango_font_description_from_string(font);

	pango_cairo_context_set_resolution(pango_layout_get_context(plo), DPI);
	cairo_font_extents(cr, &extents);
	gtk_widget_queue_draw(widget);

	for(i = 0; i < sizeof(desktops) / sizeof(*desktops); i++) {
		gdk_rgba_parse(&c, desktop_status[i] == 'f' ? desktop_norm : desktop_status[i] == 'u' ? desktop_urg : desktop_occ);
		cairo_set_source_rgba(cr, c.red, c.green, c.blue, c.alpha);

		pango_font_description_set_weight(pfd, desktop_status[i] == 'O' || desktop_status[i] == 'F' ? PANGO_WEIGHT_ULTRABOLD : 450);
		pango_layout_set_font_description(plo, pfd);

		pango_layout_set_text(plo, desktops[i], -1);
		cairo_move_to(cr, x, 0.5 - extents.descent + extents.height / 2);
		pango_cairo_show_layout(cr, plo);
		pango_layout_get_pixel_size(plo, &w, NULL);
		x += w + padding;
	}
	pango_font_description_set_weight(pfd, 450);
	pango_layout_set_font_description(plo, pfd);
	pango_font_description_free(pfd);

	gdk_rgba_parse(&c, separator_color);
	cairo_set_source_rgba(cr, c.red, c.green, c.blue, c.alpha);
	cairo_set_line_width(cr, 1);
	
	cairo_move_to(cr, x, BARH * 0.15);
	cairo_line_to(cr, x, BARH - BARH * 0.15);
	cairo_stroke(cr);
	x += padding;

	cairo_move_to(cr, x, 0.5 - extents.descent + extents.height / 2);
	pango_layout_set_text(plo, tiling_state, -1);
	pango_cairo_show_layout(cr, plo);

	return FALSE;
}

static gboolean get_desktops_status(GtkWidget *widget)
{
	while(g_main_context_pending(NULL))
		g_main_context_iteration(NULL, FALSE);
	fd_set set;
	struct timeval timeout;
	gint rv;
	FILE *infile;
	gint fd;
	gchar *buf = NULL;
	size_t len = 0;

	fd = open("/tmp/panel-fifo", O_RDWR | O_NONBLOCK); // TODO: change later
	FD_ZERO(&set);
	FD_SET(fd, &set);

	timeout.tv_sec = 0;
	timeout.tv_usec = 75000;

	rv = select(fd + 1, &set, NULL, NULL, &timeout);
	if(rv > 0) {
		infile = fdopen(fd, "r");
		getline(&buf, &len, infile);
		fclose(infile);
	}
	close(fd);

	if(buf != NULL) {
		gint i = 0;
		gchar *pch;
		gchar *test;
		gchar *tmp[10];

		test = strtok_r(buf, ":", &pch);
		while(test != NULL && i < 10) {
			test = strtok_r(NULL, ":", &pch);
			tmp[i++] = &test[0];
			gchar c;
			c = *(tmp[i - 1]);
			desktop_status[i-1] = c;
		}
		/* last line is tiling state */
		tiling_state = strtok_r(NULL, ":", &pch);
		
		gtk_widget_queue_draw(widget);
	}
	return TRUE;
}
