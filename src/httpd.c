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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <microhttpd.h>

#include "db.h"
#include "formatstr.h"
#include "httpd.h"
#include "utils.h"

static struct MHD_Daemon *daemon = NULL;

const char *const response500 = "<!DOCTYPE html><html>"
	"<head><title>taymtrakka</title></head>"
	"<body><h1>Internal server error</h1>"
	"The application ran into an internal error while trying to generate a response to your request."
	"</body></html>"
;

#define UNUSED(x) (void)(x)
#define BUFFER_SIZE (64 * 1024)

struct buffer_info {
	char *buffer;
	size_t length;
};

static void buildWindowRow(const char *windowname, int64_t totalSeconds, void* userdata) {
	struct buffer_info *info = userdata;
	#define _INFOBUF_    info->buffer + info->length, BUFFER_SIZE - info->length

	struct FormatArg args[] = {
		{.type = ARG_STRING, .v_string = windowname},
		{.type = ARG_INT, .v_int = totalSeconds},
		{.type = ARG_NONE},
	};
	info->length += formatstr(_INFOBUF_, "<tr><td>{0:html}</td><td>{1:time}</td></tr>\n", args);
}

static void buildResponse(char **response_buffer, int *freebuf, size_t *response_length, int *response_code) {
	char *buffer = malloc(64 * 1024);
	if(buffer != NULL) {
		struct buffer_info info = (struct buffer_info){
			.buffer = buffer,
			.length = 0
		};
		
		time_t now_epoch = time(NULL);
		struct tm now;
		gmtime_r(&now_epoch, &now);
		
		struct FormatArg headerArgs[] = {
			{.type = ARG_STRING, .v_string = "taymtrakka"},
			{.type = ARG_INT, .v_int = now_epoch},
			{.type = ARG_STRING, .v_string = "Window name"},
			{.type = ARG_STRING, .v_string = "Total time"},
			{.type = ARG_NONE}
		};
		info.length += formatstr(
			buffer + info.length,
			BUFFER_SIZE - info.length,
			"<!DOCTYPE html><html><head>\n"
			"<title>{0}</title>\n"
			"<link type=\"text/css\" rel=\"stylesheet\" href=\"/files/style.css\">\n"
			"</head><body><h1>Statistics for {1:date}</h1>\n"
			"<table><thead><tr><th>{2}</th><th>{3}</th></tr></thead><tbody>\n",
			headerArgs
		);
		
		now.tm_isdst = -1;
		now.tm_hour = now.tm_min = now.tm_sec = 0;
		time_t today_start = mktime(&now);
		
		now.tm_isdst = -1;
		now.tm_hour = 24;
		time_t today_end = mktime(&now);
		
		db_windowstats(today_start, today_end, &buildWindowRow, &info);
		
		info.length += snprintf(
			buffer + info.length,
			BUFFER_SIZE - info.length,
			"</tbody></table></html>"
		);
		
		*response_buffer = buffer;
		*freebuf = 1;
		*response_length = info.length;
		*response_code = 200;
	} else {
		*response_buffer = (char*)response500; // Explicitly discard const
		*freebuf = 0;
		*response_length = strlen(response500);
		*response_code = 500;
	}
}

static void serveStaticFile(const char *url, char **response_buffer, size_t *response_length, int *response_code) {
	// Look into the Makefile to find out how the list of url-data pairs is constructed.
	// Beware, for it is not pretty.
	char *staticdata;
	#include "files/index.c"

	*response_buffer = staticdata;
	*response_length = (staticdata != NULL) ? strlen(staticdata) : 0;
	*response_code = (staticdata != NULL) ? 200 : 404;
}

#define STATICFILES_PREFIX "/files/"
#define STATICFILES_PREFIX_LENGTH strlen(STATICFILES_PREFIX)
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
	UNUSED(method);
	UNUSED(version);
	UNUSED(upload_data);
	UNUSED(upload_data_size);
	
	char *response_buffer;
	int freebuf = 0;
	size_t response_length;
	int response_code;
	
	if(strncmp(url, STATICFILES_PREFIX, STATICFILES_PREFIX_LENGTH) == 0)
		serveStaticFile(url+STATICFILES_PREFIX_LENGTH, &response_buffer, &response_length, &response_code);
	else
		buildResponse(&response_buffer, &freebuf, &response_length, &response_code);
	
	struct MHD_Response *response = MHD_create_response_from_buffer(response_length, response_buffer, MHD_RESPMEM_MUST_COPY);
	// MHD_add_response_header(response, "Content-Type", "text/html;charset=UTF-8");
	
	int result = MHD_queue_response(connection, response_code, response);
	MHD_destroy_response(response);
	
	if(freebuf) free(response_buffer);
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
