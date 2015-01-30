/* nyanbar-applet.c
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
#include "nyanbar-applet.h"

G_DEFINE_TYPE(BarApplet, bar_applet, GTK_TYPE_BIN)

static void bar_applet_init(BarApplet *self);
static void bar_applet_class_init(BarAppletClass *klass);
static void bar_applet_dispose(GObject *object);

static void bar_applet_init(BarApplet *self)
{
}

static void bar_applet_class_init(BarAppletClass *klass)
{
	GObjectClass *g_object_class;

	g_object_class = G_OBJECT_CLASS(klass);
	g_object_class->dispose = &bar_applet_dispose;
}

static void bar_applet_dispose(GObject *object)
{
	G_OBJECT_CLASS(bar_applet_parent_class)->dispose(object);
}

GtkWidget *bar_applet_new(void)
{
	BarApplet *self;

	self = g_object_new(BAR_APPLET_TYPE, NULL);
	return GTK_WIDGET(self);
}
