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
#include <stdio.h>
#include <string.h>

#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>

void os_get_db_path(char *const buffer, const size_t buflen) {
	char* datadir = getenv("XDG_DATA_HOME");
	if(datadir != NULL) {
		snprintf(buffer, buflen, "%s/%s/%s/%s", datadir, APP_VENDOR, APP_NAME, DBFILE);
		return;
	}
	
	char* home = getenv("HOME");
	if(home == NULL) {
		struct passwd *user = getpwuid(getuid());
		home = user->pw_dir;
	}
	
	snprintf(buffer, buflen, "%s/.local/share/%s/%s/%s", home, APP_VENDOR, APP_NAME, DBFILE);
}

void os_sleep(const int seconds) {
	sleep(seconds);
}
