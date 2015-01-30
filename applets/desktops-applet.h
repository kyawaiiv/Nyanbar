/* applets/desktops-applet.h
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
#ifndef desktops_applet_h
#define desktops_applet_h

#include "nyanbar-applet.h"

typedef struct _DesktopsApplet DesktopsApplet;
typedef struct _DesktopsAppletClass DesktopsAppletClass;

#define DESKTOPS_APPLET_TYPE (desktops_applet_get_type())

#define DESKTOPS_APPLET(obj) \
	(G_TYPE_CHECK_INSTANCE_CAST((obj), \
	DESKTOPS_APPLET_TYPE, DesktopsApplet))

#define IS_DESKTOPS_APPLET(obj) \
	(G_TYPE_CHECK_INSTANCE_TYPE((obj), \
	DESKTOPS_APPLET_TYPE))

#define DESKTOPS_APPLET_CLASS(klass) \
	((G_TYPE_CHECK_CLASS_CAST((klass), \
	DESKTOPS_APPLET_TYPE, DesktopsAppletClass))

#define IS_DESKTOPS_APPLET_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), \
	DESKTOPS_APPLET_TYPE))

#define DESKTOPS_APPLET_GET_CLASS(obj) \
	(G_TYPE_INSTANCE_GET_CLASS((obj), \
	DESKTOPS_APPLET_TYPE, DesktopsAppletClass))

struct _DesktopsApplet {
	BarApplet parent;
	GtkWidget *da;
};

struct _DesktopsAppletClass {
	BarAppletClass(parent_class);
};

GType desktops_applet_get_type(void);

GtkWidget *desktops_applet_new(void);

#endif
