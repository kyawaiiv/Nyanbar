/* applets/menu_separator-applet.h
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
#ifndef menu_separator_applet_h
#define menu_separator_applet_h

#include "nyanbar-applet.h"

typedef struct _MenuSeparatorApplet MenuSeparatorApplet;
typedef struct _MenuSeparatorAppletClass MenuSeparatorAppletClass;

#define MENU_SEPARATOR_APPLET_TYPE (menu_separator_applet_get_type())

#define MENU_SEPARATOR_APPLET(obj) \
	(G_TYPE_CHECK_INSTANCE_CAST((obj), \
	MENU_SEPARATOR_APPLET_TYPE, MenuSeparatorApplet))

#define IS_MENU_SEPARATOR_APPLET(obj) \
	(G_TYPE_CHECK_INSTANCE_TYPE((obj), \
	MENU_SEPARATOR_APPLET_TYPE))

#define MENU_SEPARATOR_APPLET_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), \
	MENU_SEPARATOR_APPLET_TYPE, MenuSeparatorAppletClass))

#define IS_MENU_SEPARATOR_APPLET_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), \
	MENU_SEPARATOR_APPLET_TYPE))

#define MENU_SEPARATOR_APPLET_GET_CLASS(obj) \
	(G_TYPE_INSTANCE_GET_CLASS((obj), \
	MENU_SEPARATOR_APPLET_TYPE, MenuSeparatorAppletClass))

struct _MenuSeparatorApplet {
	BarApplet parent;
	GtkWidget *da;

	gchar *color;
};

struct _MenuSeparatorAppletClass {
	BarAppletClass parent_class;
};

GType menu_separator_applet_get_type(void);

GtkWidget *menu_separator_applet_new(void);

#endif
