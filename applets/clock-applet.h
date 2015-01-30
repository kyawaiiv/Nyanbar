/* applets/clock-applet.h
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
#ifndef clock_applet_h
#define clock_applet_h

#include "nyanbar-applet.h"

typedef struct _ClockApplet ClockApplet;
typedef struct _ClockAppletClass ClockAppletClass;

#define CLOCK_APPLET_TYPE (clock_applet_get_type())

#define CLOCK_APPLET(obj) \
	(G_TYPE_CHECK_INSTANCE_CAST((obj), \
	CLOCK_APPLET_TYPE))

#define IS_CLOCK_APPLET(obj) \
	(G_TYPE_CHECK_INSTANCE_TYPE((obj), \
	CLOCK_APPLET_TYPE))

#define CLOCK_APPLET_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), \
	CLOCK_APPLET_TYPE, ClockAppletClass))

#define IS_CLOCK_APPLET_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), \
	CLOCK_APPLET_TYPE))

#define CLOCK_APPLET_GET_CLASS(obj) \
	(G_TYPE_INSTANCE_GET_CLASS((obj), \
	CLOCK_APPLET_TYPE, ClockAppletClass))

struct _ClockApplet {
	BarApplet parent;
	GtkWidget *da;
};

struct _ClockAppletClass {
	BarAppletClass parent_class;
};

GType clock_applet_get_type(void);

GtkWidget *clock_applet_new(void);

#endif
