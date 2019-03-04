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

#include <errno.h>
#include <fcntl.h>
#include <pwd.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

const char DirSeparator = '/';

size_t os_get_db_path(char *const buffer, const size_t buflen) {
	char* datadir = getenv("XDG_DATA_HOME");
	if(datadir != NULL) {
		return snprintf(buffer, buflen, "%s/%s/%s/%s", datadir, APP_VENDOR, APP_NAME, DBFILE);
	}
	
	char* home = getenv("HOME");
	if(home == NULL) {
		struct passwd *user = getpwuid(getuid());
		home = user->pw_dir;
	}
	
	return snprintf(buffer, buflen, "%s/.local/share/%s/%s/%s", home, APP_VENDOR, APP_NAME, DBFILE);
}

int os_mkdir(char *const buffer) {
	if(mkdir(buffer, 0700) == 0) return 0;
	
	// EEXIST tells us that the directory already exists.
	if(errno == EEXIST) return 0;
	
	// ENOENT tells us that one of the parent dirs does not exist.
	if(errno != ENOENT) {
		perror(APP_NAME ": Failed to create database directory");
		return -1;
	}
	
	// Find the last dirname separator so we can cut off the last part
	char *slashpos = strrchr(buffer, DirSeparator);
	if(slashpos == NULL) return -1;
	
	*slashpos = '\0';
	int make_parent = os_mkdir(buffer);
	*slashpos = DirSeparator;
	
	return make_parent;
}

static int *exitFlag = NULL;

#define UNUSED(x) (void)(x)
static void signalHandler(int signum) {
	UNUSED(signum);

	*exitFlag = 1;
}

int os_install_signal_handler(int *const arg_exitFlag) {
	exitFlag = arg_exitFlag;

	struct sigaction action;
	action.sa_handler = &signalHandler;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;

	return sigaction(SIGINT, &action, NULL);
}

void os_sleep(const int seconds) {
	sleep(seconds);
}
