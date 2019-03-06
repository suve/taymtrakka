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
#include <time.h>

#include "formatstr.h"
#include "utils.h"

static size_t copymem(char *dest, const size_t destsize, const char *source, const size_t srcsize) {
	size_t bytes = (srcsize <= destsize) ? srcsize : destsize;
	memcpy(dest, source, bytes);
	return bytes;
}

static int countargs(const struct FormatArg *args) {
	int count = 0;
	while(args->type != ARG_NONE) ++count, ++args;
	return count;
}

enum SpecifierFormat {
	SPEC_NONE,
	SPEC_INT,
	SPEC_HEX,
	SPEC_FLOAT,
	SPEC_CHAR,
	SPEC_STRING_RAW,
	SPEC_STRING_HTML,
	SPEC_TIME,
	SPEC_DATE,
};

struct ArgumentSpecifier {
	int argno;
	enum SpecifierFormat format;
};

static enum SpecifierFormat defaultformat(const enum FormatArgType fat) {
	switch(fat) {
		case ARG_CHAR:
		case ARG_STRING:
			return SPEC_STRING_RAW;

		case ARG_INT:
			return SPEC_INT;

		case ARG_FLOAT:
		case ARG_DOUBLE:
			return SPEC_FLOAT;

		default:
			return SPEC_NONE;
	}
}

#define streq(a, b)  (strcmp( (a) , (b) ) == 0)
static struct ArgumentSpecifier parsespec(const char *open, const char *close, const struct FormatArg *args, const int argcount) {
	struct ArgumentSpecifier spec;
	
	size_t speclen = close - open - 1;
	char specstr[32];
	memcpy(specstr, open+1, speclen);
	specstr[speclen] = '\0';

	char *colon = strchr(specstr, ':');
	if(colon == NULL) {
		spec.argno = atoi(specstr);
		spec.format = (spec.argno < argcount) ? defaultformat(args[spec.argno].type) : SPEC_NONE;
		return spec;
	}

	*colon = '\0';
	spec.argno = atoi(specstr);
	if(spec.argno >= argcount) {
		spec.format = SPEC_NONE;
		return spec;
	}

	++colon;
	if(streq(colon, "int")) {
		spec.format = SPEC_INT;
	} else if(streq(colon, "hex")) {
		spec.format = SPEC_HEX;
	} else if(streq(colon, "float")) {
		spec.format = SPEC_FLOAT;
	} else if(streq(colon, "char")) {
		spec.format = SPEC_CHAR;
	} else if(streq(colon, "str")) {
		spec.format = SPEC_STRING_RAW;
	} else if(streq(colon, "html")) {
		spec.format = SPEC_STRING_HTML;
	} else if(streq(colon, "time")) {
		spec.format = SPEC_TIME;
	} else if(streq(colon, "date")) {
		spec.format = SPEC_DATE;
	} else {
		spec.format = defaultformat(args[spec.argno].type);
	}

	return spec;
}

static size_t printspec_int(char *buffer, const size_t bufsize, const struct FormatArg arg, const int hex) {
	long int value;

	switch(arg.type) {
		case ARG_CHAR:
			value = arg.v_char;
		break;

		case ARG_STRING:
			value = atol(arg.v_string);
		break;

		case ARG_INT:
			value = arg.v_int;
		break;
		
		case ARG_FLOAT:
			value = arg.v_float;
		break;

		case ARG_DOUBLE:
			value = arg.v_double;
		break;

		default:
			value = 0;
	}

	// Special case with a magic value, yay!
	// TODO: at least add "#define BUTT 2" or something
	if(hex == 2) return duration(buffer, bufsize, value);

	return snprintf(buffer, bufsize, hex ? "%lx" : "%ld", value);
}

static size_t printspec_float(char *buffer, const size_t bufsize, const struct FormatArg arg) {
	double value;

	switch(arg.type) {
		case ARG_CHAR:
			value = arg.v_char;
		break;

		case ARG_STRING:
			value = strtod(arg.v_string, NULL);
		break;

		case ARG_INT:
			value = arg.v_int;
		break;
		
		case ARG_FLOAT:
			value = arg.v_float;
		break;

		case ARG_DOUBLE:
			value = arg.v_double;
		break;

		default:
			value = 0;
	}

	return snprintf(buffer, bufsize, "%.2lf", value);
}

static size_t printspec_char(char *buffer, const size_t bufsize, const struct FormatArg arg) {
	(void)(bufsize);

	switch(arg.type) {
		case ARG_CHAR:
			*buffer = arg.v_char;
			return 1;

		case ARG_STRING:
			if(arg.v_string[0] != '\0') {
				*buffer = arg.v_string[0];
				return 1;
			} else {
				return 0;
			}

		case ARG_INT:
			*buffer = arg.v_int % 256;
			return 1;
		
		case ARG_FLOAT:
			*buffer = ((int)arg.v_float) % 256;
			return 1;

		case ARG_DOUBLE:
			*buffer = ((int)arg.v_double) % 256;
			return 1;

		default:
			return 0;
	}
}

static size_t printspec_string(char *buffer, const size_t bufsize, const struct FormatArg arg, const int html) {
	long unsigned int intval;

	switch(arg.type) {
		case ARG_CHAR:
			*buffer = arg.v_char;
			return 1;

		case ARG_STRING:
			if(html)
				return escapeHTML(buffer, bufsize, arg.v_string);
			else
				return snprintf(buffer, bufsize, "%s", arg.v_string);

		case ARG_INT:
			intval = arg.v_int;
		break;
		
		case ARG_FLOAT:
			intval = arg.v_float;
		break;

		case ARG_DOUBLE:
			intval = arg.v_double;
		break;	

		default:
			return 0;
	}

	if(intval == 0) {
		*buffer = '\0';
		return 1;
	}

	char bytes[16];
	memset(bytes, 0, sizeof(bytes));
	for(int b = sizeof(long unsigned int) - 1; b <= 0; ++b) {
		bytes[b] = intval % 256;
		intval /= 256;
	}

	int start = 0;
	while(bytes[start] == 0) ++start;

	// TODO: Support strings with zero bytes (NUL-bytes) in them
	if(html)
		return escapeHTML(buffer, bufsize, bytes);
	else
		return snprintf(buffer, bufsize, "%s", bytes);
}

static size_t printspec_date(char *buffer, const size_t bufsize, const struct FormatArg arg) {
	time_t epoch;
	struct tm tm;
	int use_tm = 0;

	struct tm *blah;
	switch(arg.type) {
		case ARG_CHAR:
			epoch = arg.v_char;
		break;

		// TODO: Make this work on Windows
		case ARG_STRING:
#if defined(unix) || defined(__unix) || defined(__unix__)
			blah = getdate(arg.v_string);
			if(blah != NULL) {
				tm = *blah;
				use_tm = 1;
			} else {
				epoch = time(NULL);
			}
#else
			epoch = 0;
#endif
		break;

		case ARG_INT:
			epoch = arg.v_int;
		break;

		case ARG_FLOAT:
			epoch = arg.v_float;
		break;

		case ARG_DOUBLE:
			epoch = arg.v_double;
		break;

		default:
			epoch = 0;
	}

	if(!use_tm) gmtime_r(&epoch, &tm);
	return strftime(buffer, bufsize, "%A %Y-%m-%d", &tm);
}

static size_t printspec(char *buffer, const size_t bufsize, const struct ArgumentSpecifier spec, const struct FormatArg *args) {
	switch(spec.format) {
		case SPEC_INT:
			return printspec_int(buffer, bufsize, args[spec.argno], 0);

		case SPEC_HEX:
			return printspec_int(buffer, bufsize, args[spec.argno], 1);

		case SPEC_FLOAT:
			return printspec_float(buffer, bufsize, args[spec.argno]);

		case SPEC_CHAR:
			return printspec_char(buffer, bufsize, args[spec.argno]);

		case SPEC_STRING_RAW:
			return printspec_string(buffer, bufsize, args[spec.argno], 0);

		case SPEC_STRING_HTML:
			return printspec_string(buffer, bufsize, args[spec.argno], 1);

		case SPEC_TIME:
			return printspec_int(buffer, bufsize, args[spec.argno], 2);

		case SPEC_DATE:
			return printspec_date(buffer, bufsize, args[spec.argno]);

		case SPEC_NONE:
		default:
			return 0;
	}
}

#define _BUFFER_  buffer+written, bufsize-written
size_t formatstr(char *buffer, const size_t bufsize, const char *format, const struct FormatArg *args) {
	int argcount = countargs(args);
	size_t written = 0;

	const char *fmt = format;
	for(;;) {
		char *open = strchr(fmt, '{');
		if(open == NULL) {
			written += copymem(_BUFFER_, fmt, strlen(fmt));
			break;
		}

		written += copymem(_BUFFER_, fmt, open - fmt);
		if(written == bufsize) break;
		
		char *close = strchr(open, '}');
		if(close == NULL) break;
		fmt = close + 1;

		struct ArgumentSpecifier spec = parsespec(open, close, args, argcount);
		written += printspec(_BUFFER_, spec, args);
		if(written == bufsize) break;
	}

	if(written == bufsize) --written;
	buffer[written] = 0;
	return written;
}
