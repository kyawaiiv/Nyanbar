/* nyanbar-settings.h
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
#ifndef nyan_bar_settings_h
#define nyan_bar_settings_h

#define BARW	    2560	    /* width of bar in pixels */
#define BARH	    44		    /* height of bar in pixels */

static gboolean topbar = TRUE;
static gint icon_size = 32;
static gint padding = 34;
static gint icon_padding = 38;
static gint launcher_padding = 32;

static gchar *desktops[6] = {"Home", "Term", "Code", "Web", "Chat", "Misc"};
static gchar *clock_format = "%a %I:%M %p";

static gchar *launcher_app = "/home/trevor/nyanpanel/nyanpanel";

/* colors */
static const char *bgcolor	    = "#fafafa";

static const char *desktop_norm	    = "#bdbdbd";
static const char *desktop_occ	    = "#484848";
static const char *desktop_urg	    = "#136ae2";

static const char *clock_primary    = "#32363a";

static const char *separator_color  = "#484848";

/* fonts */
static int DPI		    = 227;			    /* dots-per-inch for fonts */
static char *font	    = "Helvetica Neue 9.5";	    /* primary font */
static char *clock_font	    = "Helvetica Neue 9";	    /* clock applet font */

#endif
