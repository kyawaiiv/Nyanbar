/* applets/menu-applet.h
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
#ifndef menu_applet_h
#define menu_applet_h

#include "nyanbar-applet.h"
#include "applets/menubar-applet.h"

typedef struct _MenuApplet MenuApplet;
typedef struct _MenuAppletClass MenuAppletClass;

#define MENU_APPLET_TYPE (menu_applet_get_type())

#define MENU_APPLET(obj) \
	(G_TYPE_CHECK_INSTANCE_CAST((obj), \
	MENU_APPLET_TYPE, MenuApplet))

#define IS_MENU_APPLET(obj) \
	(G_TYPE_CHECK_INSTANCE_TYPE((obj), \
	MENU_APPLET_TYPE))

#define MENU_APPLET_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), \
	MENU_APPLET_TYPE, MenuAppletClass))

#define IS_MENU_APPLET_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), \
	MENU_APPLET_TYPE))

#define MENU_APPLET_GET_CLASS(obj) \
	(G_TYPE_INSTANCE_GET_CLASS((obj), \
	MENU_APPLET_TYPE, MenuAppletClass))

struct _MenuApplet {
	BarApplet parent;
	GtkWidget *da;
	MenubarApplet *menubar;
	gboolean active;
	gchar *fgcolor;
	gchar *bgcolor;
	gchar *hlcolor;
};

struct _MenuAppletClass {
	BarAppletClass parent_class;
};

GType menu_applet_get_type(void);

GtkWidget *menu_applet_new(void);

#endif
