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

#include <windows.h>

int wm_init(void) {
	return 0;
}

int wm_quit(void) {
	return 0;
}

int wm_getActiveWindow(char *const buffer, const size_t bufsize) {
	HWND foreground = GetForegroundWindow();
	if(foreground == NULL) return 1;

    	GetWindowText(foreground, buffer, bufsize);
	return 0;
}
