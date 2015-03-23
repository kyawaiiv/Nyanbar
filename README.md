NyanBar
===
NyanBar is a simple and elegant bar designed for use with <a href="https://github.com/baskerville/bspwm">bspwm</a>. The goal of this project is for a semi-lightweight and elegant bar written entirely in C. This is achieved through the use of GTK3 combined with the Cairo library, which provides modern, eye-appealing graphics and exceptional font rendering using the Pango backend.

**Note:** This is an alpha release. Most of the code isn't very clean (yet!) and I am still working on various design aspects for future builds. Most of the code dealing with the visual presentation (colors, fonts, padding, etc...) is currently hardcoded for my system, so it may not necessarily display correctly on yours.

Dependencies:
---
* gtk3 `<Extra/gtk3>`
* cairo `<Extra/cairo>`
* pango `<Extra/pango>`

Installation:
---
As of now, I am only including a Makefile until the project is more complete. I will include an Arch Linux PKGBUILD in future builds, but for now you can simply run:

	$ make
	# make install

In order to retrieve the desktops state, you must create a FIFO for bspwm to write to. This can be achieved by adding the following lines to your ~/.xinitrc:

	export BAR_FIFO="/tmp/bar-fifo"
	[ -e "$BAR_FIFO" ] && rm "$BAR_FIFO"
	mkfifo "$BAR_FIFO"
	exec bspwm

You can then direct bspwm to write to the newly created FIFO by adding the following line to you bspwm config file:

	bspc control --subscribe > $BAR_FIFO &

You must also have bspwm allocate space for the bar to be displayed by adding another line to your bspwm config file:

	bspc config top_padding xx

_where "xx" equals the bar height - border width in pixels._

If you prefer the bar to be at the bottom of the screen, change the 'topbar' variable to FALSE in nyanbar-settings.h and use `bottom_padding` instead of `top_padding` in your bspwm config file.

**Important:** The Arch Linux icon needs to be manually copied to _/usr/share/icons/NyanBar/_. The path is hardcoded right now but will be able to be changed eventually.

**Fonts:**

* Helvetica Neue

Todo:
---
* Global menu bar?
* Minor re-write and cleanup of desktops applet.
* ~~Major re-write of clock applet.~~
* Minor cleanup of launcher applet.
* Restructure rendering to increase responsiveness.
* Basic mouse support -> clickable 'desktops' selection.
* Native systray?
* Dropdown menu from icon. 
* Add support for other WMs into desktops applet.

Bugs:
---
* Desktops status is not shown correctly if updated more than once per second.
* ~~Desktops status does not always update automatically (without external event).~~

License:
---
This project is licensed under version 3 of the GNU General Public License. A copy of the license has been provided, see `COPYING`.

Copyright © 2014 Trevor Kulhanek <trevor@nocodenolife.com>
