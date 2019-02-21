/**
 * This file is part of Taym Trakka.
 * Copyright (C) 2019 Artur "suve" Iwicki
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, version 3,
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program (LICENCE.txt). If not, see <http://www.gnu.org/licenses/>.
 */
#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>

#define APP_NAME    "taymtrakka"
#define APP_VERSION "v.0.1"

int main(void) {
	Display *disp = XOpenDisplay(NULL);
	if(disp == NULL) {
		fprintf(stderr, APP_NAME ": Failed to open X11 display\n");
		exit(EXIT_FAILURE);
	}

	XCloseDisplay(disp);
	return 0;
}
