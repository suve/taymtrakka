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
#include <time.h>

#include "db.h"
#include "httpd.h"
#include "options.h"
#include "os.h"
#include "wm.h"

#define MAX_SLEEP_TIME (3 * sleepTime)

static int terminate = 0;

static void taymtrakka_loop(unsigned int sleepTime) {
	int64_t last_windowID = -1;
	int64_t last_dpID = -1;
	time_t last_time = time(NULL);
	
	while(!terminate) {
		os_sleep(sleepTime);
		time_t now = time(NULL);
		
		long long int slept = now - last_time;
		if(slept > MAX_SLEEP_TIME) {
			printf("Unexpectedly long sleep time (%lld seconds). Ignoring this time period.\n", slept);
			last_time = now;
			continue;
		}

		char buffer[1024];
		if(wm_getActiveWindow(buffer, sizeof(buffer)) != 0) {
			puts("Failed to get current window :(");
			last_time = now;
			continue;
		}

		int64_t windowID = db_getWindowID(buffer);
		if(windowID < 0) continue;
		
		if(last_windowID != windowID) {
			int64_t dpID = db_datapoint_new(windowID, last_time, now);
			last_dpID = dpID;
		} else {
			db_datapoint_update(last_dpID, now);
		}
		
		last_windowID = windowID;
		last_time = now;
	}
}

int main(int argc, char **argv) {
	struct Options opts;
	options_parse(argc, argv, &opts);
	
	os_install_signal_handler(&terminate);
	
	wm_init();
	db_open();
	db_init();
	httpd_start(opts.portNumber);

	taymtrakka_loop(opts.updateInterval);

	httpd_stop();
	db_close();
	wm_quit();
}
