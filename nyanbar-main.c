/* nyanbar-main.c
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
#include "nyanbar.h"
#include <stdlib.h>

gint main(gint argc, gchar **argv)
{
	NyanBar *nb __attribute__((unused));
	gtk_init(&argc, &argv);
	nb = nyanbar_new();
	gtk_main();
	return EXIT_SUCCESS;
}
