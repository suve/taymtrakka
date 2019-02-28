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
#include <stdlib.h>
#include <string.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

static int x11_getActiveWindow(Display *display, Window *window) {
	Atom property = XInternAtom(display, "_NET_ACTIVE_WINDOW", False), type;
	int form;
	unsigned long remain, len;
	unsigned char *list;

	int status = XGetWindowProperty(
		display,
		XDefaultRootWindow(display),
		property,
		0,1024,
		False,
		33,
		&type,
		&form,
		&len,
		&remain,
		&list
	);
	if(status != Success) {
		return 1;
	}
	if(len == 0) {
		return 2;
	}

	*window = ((Window*)list)[0];
	return 0;
}

static int x11_getWindowName(Display *display, Window window, char *const buffer, const size_t bufsize) {
	Atom property = XInternAtom(display, "WM_NAME", False), type;
	int form;
	unsigned long remain, len;
	unsigned char *list;

	int status = XGetWindowProperty(
		display,
		window,
		property,
		0, 1024,
		False,
		AnyPropertyType,
		&type,
		&form,
		&len,
		&remain,
		&list
	);
	if(status != Success) {
		return 1;
	}

	size_t namelen = strlen((char*)list)+1;
	if(namelen <= bufsize) {
		memcpy(buffer, list, namelen);
	} else {
		memcpy(buffer, list, bufsize);
		buffer[bufsize-1] = '\0';
	}
	return 0;
}

static struct wm_x11_state_t {
	Display *display;
} wm_state;

int wm_init(void) {
	wm_state.display = XOpenDisplay(NULL);
	return wm_state.display == NULL;
}

int wm_quit(void) {
	XCloseDisplay(wm_state.display);
	return 0;
}

int wm_getActiveWindow(char *const buffer, const size_t bufsize) {
	Window window;
	if(x11_getActiveWindow(wm_state.display, &window) != 0) return 1;

	return x11_getWindowName(wm_state.display, window, buffer, bufsize);
}
