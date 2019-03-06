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
#ifndef TAYMTRAKKA_FORMATSTR_H
#define TAYMTRAKKA_FORMATSTR_H

#include <stdint.h>
#include <string.h>

enum FormatArgType {
	ARG_CHAR,
	ARG_STRING,
	ARG_INT,
	ARG_FLOAT,
	ARG_DOUBLE,
	ARG_NONE,
};

struct FormatArg {
	enum FormatArgType type;
	union {
		const char    v_char;
		const char*   v_string;
		const int64_t v_int;
		const float   v_float;
		const double  v_double;
	};
};

extern size_t formatstr(char *buffer, size_t bufsize, const char *format, const struct FormatArg *args);

#endif
