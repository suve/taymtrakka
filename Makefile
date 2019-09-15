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

OS_SOURCES := $(shell ls src/os/*.c)
WM_SOURCES := $(shell ls src/wm/*.c)

QUERIES := $(shell ls sql/*.sql)
SQL_C_FILES := $(QUERIES:sql/%.sql=src/sql/%.c)

STATIC_FILES := $(shell ls files/*)
STATIC_C_FILES := $(STATIC_FILES:files/%=src/files/%.c) 


build/taymtrakka: $(OBJECTS)
	mkdir -p "$(dir $@)"
	$(CC) $(CFLAGS) -o "$@" $^ $(LDLIBS)

build/db.o: src/db.c $(SQL_C_FILES)
build/httpd.o: src/httpd.c src/files.c $(STATIC_C_FILES)
build/os.o: src/os.c $(OS_SOURCES)
build/wm.o: src/wm.c $(WM_SOURCES)
build/%.o: src/%.c
	mkdir -p "$(dir $@)"
	$(CC) $(CFLAGS) -c -o "$@" "$<"

src/sql/%.c: sql/%.sql tools/squish-sql.py
	mkdir -p "$(dir $@)"
	tools/squish-sql.py < "$<" > "$@"

src/files.c: tools/generate-files-c.py src/files.c.template $(STATIC_FILES)
	mkdir -p "$(dir $@)"
	tools/generate-files-c.py $(STATIC_FILES) --template "src/files.c.template" > "$@"

src/files/%.css.c: files/%.css tools/squish-css.py
	mkdir -p "$(dir $@)"
	tools/squish-css.py < "$<" > "$@"

clean:
	rm -rf src/sql/ src/files/ build/
