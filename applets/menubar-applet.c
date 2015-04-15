/* applets/menu-applet.c
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
#include "menu-applet.h"
#include "nyanbar-settings.h"
#include "applets/menuitem-applet.h"

G_DEFINE_TYPE(MenubarApplet, menubar_applet, GTK_TYPE_WINDOW)

static void menubar_applet_init(MenubarApplet *self);
static void menubar_applet_class_init(MenubarAppletClass *klass);
static void menubar_applet_dispose(GObject *object);
static gboolean menubar_applet_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data);
static gboolean on_update(GtkWidget *widget);
static gboolean on_click(GtkWidget *widget, GdkEventButton *event, gpointer user_data);
static void on_realized(GtkWidget *widget, gpointer user_data);

static void menubar_applet_init(MenubarApplet *self)
{
	self->size = 0;
	self->active = FALSE;

	gtk_widget_set_app_paintable(GTK_WIDGET(self), TRUE);
	GdkScreen *screen;
	GdkVisual *visual;
	screen = gdk_screen_get_default();
	visual = gdk_screen_get_rgba_visual(screen);
	gtk_widget_set_visual(GTK_WIDGET(self), visual);

	gtk_window_set_default_size(GTK_WINDOW(self), menu_w, 100);
	gtk_window_set_type_hint(GTK_WINDOW(self), GDK_WINDOW_TYPE_HINT_DOCK);

	g_signal_connect(self, "draw", G_CALLBACK(menubar_applet_draw), self);
	g_signal_connect(self, "realize", G_CALLBACK(on_realized), self);
	g_signal_connect(self, "button_release_event", G_CALLBACK(on_click), self);

	gtk_widget_set_events(GTK_WIDGET(self), gtk_widget_get_events(GTK_WIDGET(self))
		| GDK_BUTTON_RELEASE_MASK);

	self->layout = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_widget_set_valign(self->layout, GTK_ALIGN_START);
	gtk_container_add(GTK_CONTAINER(self), self->layout);
}

static void menubar_applet_class_init(MenubarAppletClass *klass)
{
	GObjectClass *g_object_class;

	g_object_class = G_OBJECT_CLASS(klass);
	g_object_class->dispose = &menubar_applet_dispose;
}

static void menubar_applet_dispose(GObject *object)
{
	G_OBJECT_CLASS(menubar_applet_parent_class)->dispose(object);
}

MenubarApplet *menubar_applet_new(void)
{
	MenubarApplet *self;

	self = g_object_new(MENUBAR_APPLET_TYPE, NULL);
	self->active = FALSE;
	return self;
}

void menubar_applet_set_owner(MenubarApplet *menu, GtkWidget *owner)
{
	menu->owner = owner;
}

static void on_realized(GtkWidget *widget, gpointer user_data)
{
	MenubarApplet *self;
	self = MENUBAR_APPLET(user_data);
	gtk_window_move(GTK_WINDOW(self), 20, BARH);
	gtk_widget_queue_draw(GTK_WIDGET(self));
}

static gboolean on_update(GtkWidget *widget)
{
	gtk_widget_queue_draw(widget);
	return TRUE;
}

static gboolean on_click(GtkWidget *widget, GdkEventButton *event, gpointer user_data)
{
	menubar_applet_hide(MENUBAR_APPLET(user_data));
}

void menubar_applet_add(MenubarApplet *menu, GtkWidget *menuitem)
{
	if(IS_MENUITEM_APPLET(menuitem)) {
		MENUITEM_APPLET(menuitem)->owner = GTK_WIDGET(menu);
		menu->menus[menu->size] = MENUITEM_APPLET(menuitem);
		menu->size++;
	}
	gtk_box_pack_start(GTK_BOX(menu->layout), menuitem, FALSE, FALSE, 5);
}

void menubar_applet_show(MenubarApplet *menu)
{
	menu->active = menu->active == TRUE ? FALSE : TRUE;
	gtk_widget_queue_draw(menu->owner);
	gtk_widget_show_all(GTK_WIDGET(menu));
}

void menubar_applet_hide(MenubarApplet *menu)
{
	gtk_widget_queue_draw(menu->owner);
	menu->active = menu->active == TRUE ? FALSE : TRUE;

	gtk_widget_hide(GTK_WIDGET(menu));
}

static gboolean menubar_applet_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data)
{
	GdkRGBA c;
	pango_cairo_create_layout(cr);
	gdk_rgba_parse(&c, MENUBAR_APPLET(user_data)->bgcolor);
	cairo_set_source_rgba(cr, c.red, c.green, c.blue, 0.90);
	cairo_paint(cr);

	gtk_container_propagate_draw(GTK_CONTAINER(widget), gtk_bin_get_child(GTK_BIN(widget)), cr);
	return FALSE;
}
