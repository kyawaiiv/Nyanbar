/* applets/launcher-applet.h
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
#ifndef launcher_applet_h
#define launcher_applet_h

#include "nyanbar-applet.h"

typedef struct _LauncherApplet LauncherApplet;
typedef struct _LauncherAppletClass LauncherAppletClass;

#define LAUNCHER_APPLET_TYPE (launcher_applet_get_type())

#define LAUNCHER_APPLET(obj) \
	(G_TYPE_CHECK_INSTANCE_CAST((obj), \
	LAUNCHER_APPLET_TYPE))

#define IS_LAUNCHER_APPLET(obj) \
	(G_TYPE_CHECK_INSTANCE_TYPE((obj), \
	LAUNCHER_APPLET_TYPE))

#define LAUNCHER_APPLET_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), \
	LAUNCHER_APPLET_TYPE, LauncherAppletClass))

#define IS_LAUNCHER_APPLET_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), \
	LAUNCHER_APPLET_TYPE))

#define LAUNCHER_APPLET_GET_CLASS(obj) \
	(G_TYPE_INSTANCE_GET_CLASS((obj), \
	LAUNCHER_APPLET_TYPE, LauncherAppletClass))

struct _LauncherApplet {
	BarApplet parent;
	GtkWidget *da;
};

struct _LauncherAppletClass {
	BarAppletClass parent_class;
};

GType launcher_applet_get_type(void);

GtkWidget *launcher_applet_new(void);

#endif
