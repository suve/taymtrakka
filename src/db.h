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
#ifndef TAYMTRAKKA_DB_H
#define TAYMTRAKKA_DB_H

#include <stdint.h>
#include <time.h>

int db_open(void);
int db_init(void);
int db_close(void);

int64_t db_getWindowID(const char *const windowTitle);
int64_t db_datapoint_new(const int64_t windowID, const time_t startTime, const time_t endTime);
int64_t db_datapoint_update(const int64_t dpID, const time_t endTime);

typedef void (*WindowstatRowHandler)(const char *windowname, int64_t totalSeconds, void* userdata);
int db_windowstats(const time_t startTime, const time_t endTime, WindowstatRowHandler handler, void *userdata);

#endif
