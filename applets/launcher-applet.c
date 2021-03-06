/* applets/launcher-applet.c
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
#include "launcher-applet.h"
#include "nyanbar-settings.h"

#include <stdlib.h>
#include <string.h>

pid_t parent_pid;
gboolean running = FALSE, active = FALSE;
gint timediff;

G_DEFINE_TYPE(LauncherApplet, launcher_applet, BAR_APPLET_TYPE)

static void launcher_applet_init(LauncherApplet *self);
static void launcher_applet_class_init(LauncherAppletClass *klass);
static void launcher_applet_dispose(GObject *object);
static gboolean launcher_applet_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data);
static gboolean button_pressed(GtkWidget *widget, GdkEventButton *event, gpointer user_data);
static gboolean button_released(GtkWidget *widget, GdkEventButton *event, gpointer user_data);
void signal_handler(int sig);

static void launcher_applet_init(LauncherApplet *self)
{
	self->da = gtk_drawing_area_new();
	gtk_widget_set_size_request(self->da, 60, BARH);
	gtk_container_add(GTK_CONTAINER(self), self->da);
	g_signal_connect(G_OBJECT(self->da), "draw", G_CALLBACK(launcher_applet_draw), self);
	g_signal_connect(G_OBJECT(self->da), "button_press_event", G_CALLBACK(button_pressed), NULL);
	g_signal_connect(G_OBJECT(self->da), "button_release_event", G_CALLBACK(button_released), NULL);

	gtk_widget_set_events(self->da, gtk_widget_get_events(self->da)
	    | GDK_BUTTON_PRESS_MASK
	    | GDK_BUTTON_RELEASE_MASK);
}

static void launcher_applet_class_init(LauncherAppletClass *klass)
{
	GObjectClass *g_object_class;

	g_object_class = G_OBJECT_CLASS(klass);
	g_object_class->dispose = &launcher_applet_dispose;
}

static void launcher_applet_dispose(GObject *object)
{
	G_OBJECT_CLASS(launcher_applet_parent_class)->dispose(object);
}

GtkWidget *launcher_applet_new(void)
{
	LauncherApplet *self;

	self = g_object_new(LAUNCHER_APPLET_TYPE, NULL);
	return GTK_WIDGET(self);
}

void signal_handler(int sig)
{
	// kill child process but not parent
	pid_t self = getpid();
	if(parent_pid != self)
		_exit(0);
}


static gboolean button_pressed(GtkWidget *widget, GdkEventButton *event, gpointer user_data)
{
	// event->type == GDK_BUTTON_PRESS
	active = TRUE;
	gtk_widget_queue_draw(widget);

	if(!running) {
		parent_pid = getpid();
		pid_t pid;
		if(pid = fork() == 0)
			execlp(launcher_app, NULL);
		running = TRUE;
		timediff = event->time;
	} else { // process is already running -- kill it with fire
		signal(SIGQUIT, signal_handler);
		kill(-parent_pid, SIGQUIT);
		running = FALSE;
	}
	return TRUE;
}

static gboolean button_released(GtkWidget *widget, GdkEventButton *event, gpointer user_data)
{
	// event->type == GDK_BUTTON_RELEASE
	active = FALSE;
	gtk_widget_queue_draw(widget);

	timediff = event->time - timediff; // get click length

	if(running && timediff > 1000) { // if click was longer than 1s -- kill child process now
		signal(SIGQUIT, signal_handler);
		kill(-parent_pid, SIGQUIT);
		running = FALSE;
	}
	return TRUE;
}

static gboolean launcher_applet_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data)
{
	cairo_surface_t *icon;
	GdkRGBA c;

	cairo_set_operator(cr, CAIRO_OPERATOR_ATOP);

	/* fill background color */
	gdk_rgba_parse(&c, active ? LAUNCHER_APPLET(user_data)->highlight_bg
		       : LAUNCHER_APPLET(user_data)->norm_bg);
	cairo_set_source_rgba(cr, c.red, c.green, c.blue, 0.80);
	cairo_paint(cr);

	/* set icon color and paint */
	icon = cairo_image_surface_create_from_png("/usr/share/icons/NyanBar/Launcher.png");
	gdk_rgba_parse(&c, active ? LAUNCHER_APPLET(user_data)->highlight_fg
		       : LAUNCHER_APPLET(user_data)->norm_fg);
	cairo_set_source_rgba(cr, c.red, c.green, c.blue, c.alpha);
	cairo_mask_surface(cr, icon, 0, 0);
}
