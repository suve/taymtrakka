#
# Makefile for Taym Trakka
# Copyright (C) 2019 Artur "suve" Iwicki
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License, version 3,
# as published by the Free Software Foundation.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along with
# this program (LICENCE.txt). If not, see <http://www.gnu.org/licenses/>.
#
CFLAGS += -std=c99 -iquote ./src/ -Wall -Wextra
CFLAGS += -D_POSIX_C_SOURCE=199309L
CFLAGS += -D_XOPEN_SOURCE=500
CFLAGS += -Wfloat-equal -Wparentheses -Wmisleading-indentation -Wmissing-field-initializers
CFLAGS += -Werror=incompatible-pointer-types -Werror=discarded-qualifiers -Werror=int-conversion -Werror=div-by-zero -Werror=sequence-point -Werror=uninitialized -Werror=duplicated-cond -Werror=return-type -Werror=implicit-function-declaration

LDLIBS += -lm -lmicrohttpd -lsqlite3 -lX11


.PHONY = clean


SOURCES := $(shell ls src/*.c)
OBJECTS := $(SOURCES:src/%.c=build/%.o)

OS_SOURCES := $(shell ls src/wm/*.c)
WM_SOURCES := $(shell ls src/wm/*.c)

QUERIES := $(shell ls sql/*.sql)
SQL_C_FILES := $(QUERIES:sql/%.sql=src/sql/%.c)

STATIC_FILES := $(shell ls files/*)
STATIC_C_FILES := $(STATIC_FILES:files/%=src/files/%.c) 


build/taymtrakka: $(OBJECTS)
	mkdir -p build
	$(CC) $(CFLAGS) -o "$@" $^ $(LDLIBS)

build/db.o: src/db.c src/files/index.c $(SQL_C_FILES)
	mkdir -p build
	$(CC) $(CFLAGS) -c -o "$@" "$<"

build/httpd.o: src/httpd.c $(STATIC_C_FILES)
	mkdir -p build
	$(CC) $(CFLAGS) -c -o "$@" "$<"

build/os.o: src/os.c $(OS_SOURCES)
	mkdir -p build
	$(CC) $(CFLAGS) -c -o "$@" "$<"

build/wm.o: src/wm.c $(WM_SOURCES)
	mkdir -p build
	$(CC) $(CFLAGS) -c -o "$@" "$<"

build/%.o: src/%.c
	mkdir -p build
	$(CC) $(CFLAGS) -c -o "$@" "$<"

src/sql/%.c: sql/%.sql
	mkdir -p src/sql
	tools/squish-sql.py < "$<" > "$@"

src/files/index.c: $(STATIC_FILES)
	mkdir -p src/files
	echo -n '' > "$@"
	for FILE in $(notdir $(STATIC_FILES)); do echo -e -n "if(strcmp(url, \"$$FILE\") == 0) staticdata = \n#include \"files/$$FILE.c\"\n;else " >> "$@"; done
	echo "staticdata = NULL;" >> "$@"

src/files/%.css.c: files/%.css
	mkdir -p src/files
	{ echo -n '"'; < "$<" tr -s '\n\r\t' '  ' | sed -e 's|\/\*.*\*\/||g' -e 's|\\|\\\\|g' -e 's|"|\\"|g' -e 's/;[ ]*$$//'; echo -n '"'; } > "$@"

clean:
	rm -rf src/sql/ src/files/
	rm -rf build/
