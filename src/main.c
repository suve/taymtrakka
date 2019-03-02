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
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>

#include "db.h"
#include "wm.h"

#define APP_NAME    "taymtrakka"
#define APP_VERSION "v.0.1"


static int terminate = 0;

#define UNUSED(x) (void)(x)
void signalHandler(int signum) {
	UNUSED(signum);

	terminate = 1;
}

int main(void) {
	wm_init();
	
	db_open();
	db_init();

	struct sigaction action;
	action.sa_handler = &signalHandler;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;
	sigaction(SIGINT, &action, NULL);
	
	int64_t last_windowID = -1;
	int64_t last_dpID = -1;
	time_t last_time = time(NULL);
	
	while(!terminate) {
		sleep(5);
		
		char buffer[1024];
		if(wm_getActiveWindow(buffer, sizeof(buffer)) != 0) {
			puts("Failed to get current window :(");
			continue;
		}

		int64_t windowID = db_getWindowID(buffer);
		if(windowID < 0) continue;
		
		time_t now = time(NULL);
		if(last_windowID != windowID) {
			int64_t dpID = db_datapoint_new(windowID, last_time, now);
			last_dpID = dpID;
		} else {
			db_datapoint_update(last_dpID, now);
		}
		
		last_windowID = windowID;
		last_time = now;
	}

	db_close();
	wm_quit();
}
