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
#ifndef menubar_applet_h
#define menubar_applet_h

//#include "nyanbar-applet.h"
#include "applets/menuitem-applet.h"

typedef struct _MenubarApplet MenubarApplet;
typedef struct _MenubarAppletClass MenubarAppletClass;

#define MENUBAR_APPLET_TYPE (menubar_applet_get_type())

#define MENUBAR_APPLET(obj) \
	(G_TYPE_CHECK_INSTANCE_CAST((obj), \
	MENUBAR_APPLET_TYPE, MenubarApplet))

#define IS_MENUBAR_APPLET(obj) \
	(G_TYPE_CHECK_INSTANCE_TYPE((obj), \
	MENUBAR_APPLET_TYPE))

#define MENUBAR_APPLET_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), \
	MENUBAR_APPLET_TYPE, MenubarAppletClass))

#define IS_MENUBAR_APPLET_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), \
	MENUBAR_APPLET_TYPE))

#define MENUBAR_APPLET_GET_CLASS(obj) \
	(G_TYPE_INSTANCE_GET_CLASS((obj), \
	MENUBAR_APPLET_TYPE, MenubarAppletClass))

struct _MenubarApplet {
	GtkWindow parent;
	GtkWidget *layout;
	GtkWidget *owner;
	MenuitemApplet *menus[10];
	int size;
	gboolean active;
	gchar *bgcolor;
};

struct _MenubarAppletClass {
	GtkWindowClass parent_class;
};

GType menubar_applet_get_type(void);

MenubarApplet *menubar_applet_new(void);

void menubar_applet_add(MenubarApplet *menu, GtkWidget *menuitem);

void menubar_applet_show(MenubarApplet *menu);

void menubar_applet_hide(MenubarApplet *menu);

void menubar_applet_set_owner(MenubarApplet *menu, GtkWidget *owner);

#endif
