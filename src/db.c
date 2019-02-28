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
#include <sqlite3.h>

#include "db.h"
#include "os.h"

static sqlite3* datab = NULL;


int db_init(void) {
	// Check if db is already open
	if(datab != NULL) return 0;
	
	char buffer[1024];
	os_get_db_path(buffer, sizeof(buffer));
	
	// TODO: Check the function's exit code for more detailed error info.
	sqlite3_open_v2(buffer, &datab, SQLITE_OPEN_READONLY, NULL);
	if(datab != NULL) return 0;
	
	char *slash = strrchr(buffer, '/');
	*slash = '\0';
	if(os_mkdir(buffer) != 0) return -1;
	
	*slash = '/';
	sqlite3_open_v2(buffer, &datab, SQLITE_OPEN_READONLY | SQLITE_OPEN_CREATE, NULL);
	return (datab != NULL) ? 0 : -1;
}

int db_quit(void) {
	sqlite3_close(datab);
	datab = NULL;
	return 0;
}
