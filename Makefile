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
CFLAGS += -Wfloat-equal -Wparentheses -Wmisleading-indentation -Wmissing-field-initializers
CFLAGS += -Werror=incompatible-pointer-types -Werror=discarded-qualifiers -Werror=int-conversion -Werror=div-by-zero -Werror=sequence-point -Werror=uninitialized -Werror=duplicated-cond -Werror=return-type -Werror=implicit-function-declaration

LDLIBS += -lm -lX11


.PHONY = clean


SOURCES := $(shell ls src/*.c)
OBJECTS := $(SOURCES:src/%.c=build/%.o)


build/taym-trakka: $(OBJECTS)
	mkdir -p build
	$(CC) $(CFLAGS) -o "$@" $^ $(LDLIBS)

build/%.o: src/%.c
	mkdir -p build
	$(CC) $(CFLAGS) -c -o "$@" "$<"

clean:
	rm -rf build/
