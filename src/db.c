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
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include <sqlite3.h>

#include "db.h"
#include "os.h"

static struct {
	sqlite3 *handle;
	sqlite3_stmt *insert_window, *select_window;
	sqlite3_stmt *insert_datapoint, *update_datapoint;
	sqlite3_stmt *select_windowstats;
} db;

int db_init(void) {
	sqlite3_stmt *query;

	const char *sql =
		#include "sql/db-init.c"
	;

	do {
		int err = sqlite3_prepare_v2(db.handle, sql, -1, &query, &sql);
		if(err != SQLITE_OK) {
			const char *errmsg = sqlite3_errstr(err);
			fprintf(stderr, "Failed to parse query: %s\n", errmsg);

			sqlite3_finalize(query);
			return -1;
		}

		err = sqlite3_step(query);
		if(err != SQLITE_DONE) {
			const char *errmsg = sqlite3_errstr(err);
			fprintf(stderr, "Failed to execute query: %s\n", errmsg);

			sqlite3_finalize(query);
			return -1;
		}

		sqlite3_finalize(query);
	} while(strlen(sql) > 0);

	return 0;
}

static int insert_window(const char *const windowName, sqlite3_int64 *rowID) {
	int err;
	if(db.insert_window == NULL) {
		err = sqlite3_prepare_v2(db.handle,
			#include "sql/insert-window.c"
			, -1,
			&db.insert_window,
			NULL
		);
		if(err != SQLITE_OK) {
			const char *errmsg = sqlite3_errstr(err);
			fprintf(stderr, "Failed to parse query: %s\n", errmsg);

			sqlite3_finalize(db.insert_window);
			db.insert_window = NULL;
			return -1;
		}
	}
	sqlite3_reset(db.insert_window);

	err = sqlite3_bind_text(db.insert_window, 1, windowName, -1, SQLITE_STATIC);
	if(err != SQLITE_OK) {
		const char *errmsg = sqlite3_errstr(err);
		fprintf(stderr, "Failed to bind query parameter: %s\n", errmsg);
		return -1;
	}

	err = sqlite3_step(db.insert_window);
	if(err != SQLITE_DONE) {
		const char *errmsg = sqlite3_errstr(err);
		fprintf(stderr, "Failed to execute query: %s\n", errmsg);
		return -1;
	}

	*rowID = sqlite3_last_insert_rowid(db.handle);
	return 0;
}

static int select_window(const char *const windowName, sqlite3_int64 *rowID) {
	int err;
	if(db.select_window == NULL) {
		int err = sqlite3_prepare_v2(db.handle,
			#include "sql/select-window.c"
			, -1,
			&db.select_window,
			NULL
		);
		if(err != SQLITE_OK) {
			const char *errmsg = sqlite3_errstr(err);
			fprintf(stderr, "Failed to parse query: %s\n", errmsg);

			sqlite3_finalize(db.select_window);
			db.select_window = NULL;
			return -1;
		}
	}
	sqlite3_reset(db.select_window);

	err = sqlite3_bind_text(db.select_window, 1, windowName, -1, SQLITE_STATIC);
	if(err != SQLITE_OK) {
		const char *errmsg = sqlite3_errstr(err);
		fprintf(stderr, "Failed to bind query parameter: %s\n", errmsg);
		return -1;
	}

	err = sqlite3_step(db.select_window);
	if(err == SQLITE_ROW) {
		*rowID = sqlite3_column_int64(db.select_window, 0);
		return 0;
	} else if(err == SQLITE_DONE) {
		return insert_window(windowName, rowID);
	} else {
		const char *errmsg = sqlite3_errstr(err);
		fprintf(stderr, "Failed to execute query: %s\n", errmsg);
		return -1;
	}
}

int64_t db_getWindowID(const char *const windowName) {
	sqlite3_int64 rowID;
	int err = select_window(windowName, &rowID);

	return (err == 0) ? rowID : -1;
}

int64_t db_datapoint_new(const int64_t windowID, const time_t startTime, const time_t endTime) {
	int err;
	if(db.insert_datapoint == NULL) {
		int err = sqlite3_prepare_v2(db.handle,
			#include "sql/insert-datapoint.c"
			, -1,
			&db.insert_datapoint,
			NULL
		);
		if(err != SQLITE_OK) {
			const char *errmsg = sqlite3_errstr(err);
			fprintf(stderr, "Failed to parse query: %s\n", errmsg);

			sqlite3_finalize(db.insert_datapoint);
			db.insert_datapoint = NULL;
			return -1;
		}
	}
	sqlite3_reset(db.insert_datapoint);

	err = sqlite3_bind_int64(db.insert_datapoint, 1, windowID);
	if(err == SQLITE_OK) err = sqlite3_bind_int64(db.insert_datapoint, 2, startTime);
	if(err == SQLITE_OK) err = sqlite3_bind_int64(db.insert_datapoint, 3, endTime);
	if(err != SQLITE_OK) {
		const char *errmsg = sqlite3_errstr(err);
		fprintf(stderr, "Failed to bind query parameter: %s\n", errmsg);
		return -1;
	}

	err = sqlite3_step(db.insert_datapoint);
	if(err != SQLITE_DONE) {
		const char *errmsg = sqlite3_errstr(err);
		fprintf(stderr, "Failed to execute query: %s\n", errmsg);
		return -1;
	}

	return sqlite3_last_insert_rowid(db.handle);
} 

int64_t db_datapoint_update(const int64_t dpID, const time_t endTime) {
	int err;
	if(db.update_datapoint == NULL) {
		int err = sqlite3_prepare_v2(db.handle,
			#include "sql/update-datapoint.c"
			, -1,
			&db.update_datapoint,
			NULL
		);
		if(err != SQLITE_OK) {
			const char *errmsg = sqlite3_errstr(err);
			fprintf(stderr, "Failed to parse query: %s\n", errmsg);

			sqlite3_finalize(db.update_datapoint);
			db.update_datapoint = NULL;
			return -1;
		}
	}
	sqlite3_reset(db.update_datapoint);

	err = sqlite3_bind_int64(db.update_datapoint, 1, dpID);
	if(err == SQLITE_OK) err = sqlite3_bind_int64(db.update_datapoint, 3, endTime);
	if(err != SQLITE_OK) {
		const char *errmsg = sqlite3_errstr(err);
		fprintf(stderr, "Failed to bind query parameter: %s\n", errmsg);
		return -1;
	}

	err = sqlite3_step(db.update_datapoint);
	if(err != SQLITE_DONE) {
		const char *errmsg = sqlite3_errstr(err);
		fprintf(stderr, "Failed to execute query: %s\n", errmsg);
		return -1;
	}

	return dpID;
}

int db_windowstats(const time_t startTime, const time_t endTime, WindowstatRowHandler handler, void* userdata) {
	int err;
	if(db.select_windowstats == NULL) {
		int err = sqlite3_prepare_v2(db.handle,
			#include "sql/select-windowstats.c"
			, -1,
			&db.select_windowstats,
			NULL
		);
		if(err != SQLITE_OK) {
			const char *errmsg = sqlite3_errstr(err);
			fprintf(stderr, "Failed to parse query: %s\n", errmsg);

			sqlite3_finalize(db.select_windowstats);
			db.select_windowstats = NULL;
			return -1;
		}
	}
	sqlite3_reset(db.select_windowstats);

	err = sqlite3_bind_int64(db.select_windowstats, 1, startTime);
	if(err == SQLITE_OK) err = sqlite3_bind_int64(db.select_windowstats, 2, endTime);
	if(err != SQLITE_OK) {
		const char *errmsg = sqlite3_errstr(err);
		fprintf(stderr, "Failed to bind query parameter: %s\n", errmsg);
		return -1;
	}

	int rowcount = 0;
	while(1) {
		err = sqlite3_step(db.select_windowstats);
		if(err == SQLITE_DONE) break;
		
		if(err != SQLITE_ROW) {
			const char *errmsg = sqlite3_errstr(err);
			fprintf(stderr, "Failed to execute query: %s\n", errmsg);
			return -1;
		}
		
		const char *windowName = sqlite3_column_text(db.select_windowstats, 0);
		int64_t seconds = sqlite3_column_int64(db.select_windowstats, 1);
		handler(windowName, seconds, userdata);
		
		++rowcount;
	}
	return rowcount;
}

int db_open(void) {
	// Check if db is already open
	if(db.handle != NULL) return 0;

	char buffer[1024];
	os_get_db_path(buffer, sizeof(buffer));

	// TODO: Check the function's exit code for more detailed error info.
	int err = sqlite3_open_v2(buffer, &db.handle, SQLITE_OPEN_READWRITE, NULL);
	if(err == SQLITE_OK) return 0;

	sqlite3_close(db.handle);
	db.handle = NULL;

	char *slash = strrchr(buffer, '/');
	*slash = '\0';
	if(os_mkdir(buffer) != 0) return -1;

	*slash = '/';
	err = sqlite3_open_v2(buffer, &db.handle, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
	if(err != SQLITE_OK) {
		const char *errmsg = sqlite3_errstr(err);
		fprintf(stderr, "Failed to open database: %s\n", errmsg);

		return -1;
	}

	return 0;
}

int db_close(void) {
	sqlite3_finalize(db.insert_window);
	sqlite3_finalize(db.select_window);
	sqlite3_finalize(db.insert_datapoint);
	sqlite3_finalize(db.update_datapoint);
	sqlite3_finalize(db.select_windowstats);

	sqlite3_close(db.handle);
	db.handle = NULL;
	return 0;
}
