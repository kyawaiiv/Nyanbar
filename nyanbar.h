/* nyanbar.h
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
#ifndef nyan_bar_h
#define nyan_bar_h

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <pango/pango.h>
#include <cairo/cairo.h>
#include "nyanbar-applet.h"

typedef struct _NyanBar NyanBar;
typedef struct _NyanBarClass NyanBarClass;

#define NYANBAR_TYPE (nyanbar_get_type())

#define NYANBAR(obj) \
	(G_TYPE_CHECK_INSTANCE_CAST((obj), \
	NYANBAR_TYPE, NyanBar))

#define IS_NYANBAR(obj) \
	(G_TYPE_CHECK_INSTANCE_TYPE((obj), \
	NYANBAR_TYPE))

#define NYANBAR_CLASS(klass), \
	(G_TYPE_CHECK_CLASS_CAST((klass), \
	NYANBAR_TYPE, NyanBarClass))

#define IS_NYANBAR_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), NYANBAR_TYPE))

#define NYANBAR_GET_CLASS(obj) \
	(G_TYPE_INSTANCE_GET_CLASS((obj), \
	NYANBAR_TYPE, NyanBarClass))

struct _NyanBar {
	GtkWindow parent;
	GtkWidget *desktops;
	GtkWidget *clock;
	GtkWidget *launcher;

	gchar *font;
	gchar *bgcolor;
	gchar *fgcolor;
	gchar *highlight;
};

struct _NyanBarClass {
	GtkWindowClass parent_class;
};

GType nyanbar_get_type(void);

NyanBar *nyanbar_new(void);

#endif
