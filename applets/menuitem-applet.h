/* applets/menuitem-applet.h
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
#ifndef menuitem_applet_h
#define menuitem_applet_h

#include "nyanbar-applet.h"

typedef struct _MenuitemApplet MenuitemApplet;
typedef struct _MenuitemAppletClass MenuitemAppletClass;

#define MENUITEM_APPLET_TYPE (menuitem_applet_get_type())

#define MENUITEM_APPLET(obj) \
	(G_TYPE_CHECK_INSTANCE_CAST((obj), \
	MENUITEM_APPLET_TYPE, MenuitemApplet))

#define IS_MENUITEM_APPLET(obj) \
	(G_TYPE_CHECK_INSTANCE_TYPE((obj), \
	MENUITEM_APPLET_TYPE))

#define MENUITEM_APPLET_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), \
	MENUITEM_APPLET_TYPE, MenuitemAppletClass))

#define IS_MENUITEM_APPLET_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), \
	MENUITEM_APPLET_TYPE))

#define MENUITEM_APPLET_GET_CLASS(obj) \
	(G_TYPE_INSTANCE_GET_CLASS((obj), \
	MENUITEM_APPLET_TYPE, MenuitemAppletClass))

struct _MenuitemApplet {
	BarApplet parent;
	GtkWidget *owner;
	GtkWidget *da;
	gchar *label;
	void (*command) ();
	gchar *font;
	gchar *bgcolor;
	gchar *fgcolor;
	gchar *highlight;
	gboolean selected;
};

struct _MenuitemAppletClass {
	BarAppletClass parent_class;
};

GType menuitem_applet_get_type(void);

MenuitemApplet *menuitem_applet_new(gchar *label, gpointer command);

#endif
