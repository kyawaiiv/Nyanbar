/* nyanbar-applet.h
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
#ifndef nyanbar_applet_h
#define nyanbar_applet_h

#include <glib-object.h>
#include <gtk/gtk.h>

typedef struct _BarApplet BarApplet;
typedef struct _BarAppletClass BarAppletClass;

#define BAR_APPLET_TYPE (bar_applet_get_type())

#define BAR_APPLET(obj) \
	(G_TYPE_CHECK_INSTANCE_CAST((obj), \
	BAR_APPLET_TYPE, BarApplet))

#define IS_BAR_APPPLET(obj) \
	(G_TYPE_CHECK_INSTANCE_TYPE((obj), \
	BAR_APPLET_TYPE))

#define BAR_APPLET_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_cAST((klass), \
	BAR_APPLET_TYPE, BarAppletClass))

#define IS_BAR_APPLET_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), \
	BAR_APPLET_TYPE))

#define BAR_APPLET_GET_CLASS(obj) \
	(G_TYPE_INSTANCE_GET_CLASS((obj), \
	BAR_APPLET_TYPE, BarAppletClass))

struct _BarApplet {
	GtkBin parent;
};

struct _BarAppletClass {
	GtkBinClass parent_class;
};

GType bar_applet_get_type(void);

GtkWidget *bar_applet_new(void);

#endif
