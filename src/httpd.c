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

#include <microhttpd.h>

#include "db.h"
#include "httpd.h"

static struct MHD_Daemon *daemon = NULL;

#define UNUSED(x) (void)(x)
static int requestHandler(
	void *userdata, 
	struct MHD_Connection *connection, 
	const char *url, 
	const char *method, 
	const char *version, 
	const char *upload_data,
	size_t *upload_data_size,
	void **conn_userdata
) {
	UNUSED(userdata);
	UNUSED(conn_userdata);
	UNUSED(url);
	UNUSED(method);
	UNUSED(version);
	UNUSED(upload_data);
	UNUSED(upload_data_size);
	
	char buffer[] = "<!DOCTYPE html><html><head><title>taymtrakka</title></head><body>This is an HTML response to a HTTP request.</body></html>";
	
	struct MHD_Response *response = MHD_create_response_from_buffer(sizeof(buffer), buffer, MHD_RESPMEM_MUST_COPY);
	int result = MHD_queue_response(connection, 200, response);
	MHD_destroy_response(response);
	
	return result;
}

int httpd_start(void) {
	if(daemon != NULL) return 0;
	
	daemon = MHD_start_daemon(
		MHD_USE_SELECT_INTERNALLY, 
		36912, 
		NULL, NULL, 
		&requestHandler, NULL, 
		MHD_OPTION_CONNECTION_TIMEOUT, 10,
		MHD_OPTION_END
	);
	if(daemon == NULL) {
		fprintf(stderr, "Failed to start the HTTP daemon\n");
		return -1;
	}
	
	return 0;
}

int httpd_stop(void) {
	MHD_stop_daemon(daemon);
	daemon = NULL;
	return 0;
}
