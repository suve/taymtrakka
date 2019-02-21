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

#include <signal.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#define APP_NAME    "taymtrakka"
#define APP_VERSION "v.0.1"

Window getActiveWindow(Display *display) {
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
		return 0;
	}
	if(len == 0) {
		return 0;
	}

	return ((Window*)list)[0];
}

char *getWindowName(Display *display, Window window) {
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
		return NULL;
	}

	return (char*)list;
}


static int terminate = 0;

#define UNUSED(x) (void)(x)
void signalHandler(int signum) {
	UNUSED(signum);

	terminate = 1;
}

int main(void) {
	Display *display = XOpenDisplay(NULL);
	if(display == NULL) {
		fprintf(stderr, APP_NAME ": Failed to open X11 display\n");
		exit(EXIT_FAILURE);
	}

	struct sigaction action;
	action.sa_handler = &signalHandler;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;
	sigaction(SIGINT, &action, NULL);

	while(!terminate) {
		Window focused = getActiveWindow(display);
		printf("Currently focused window: \"%s\"\n", getWindowName(display, focused));
		sleep(5);
	}

	XCloseDisplay(display);
	return 0;
}
