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
#ifndef TAYMTRAKKA_VERSION_H
#define TAYMTRAKKA_VERSION_H

#define APP_VENDOR "suve"
#define APP_NAME "taymtrakka"

#define APP_VERSION_MAJOR  0
#define APP_VERSION_MINOR  1
#define APP_VERSION_BUGFIX 0

#define __STRINGIFY(val) #val
#define __TOSTRING(val) __STRINGIFY(val)

#if APP_VERSION_BUGFIX > 0
	#define APP_VERSION_STRING "v." __TOSTRING(APP_VERSION_MAJOR) "." __TOSTRING(APP_VERSION_MINOR) "." __TOSTRING(APP_VERSION_BUGFIX)
#else
	#define APP_VERSION_STRING "v." __TOSTRING(APP_VERSION_MAJOR) "." __TOSTRING(APP_VERSION_MINOR)
#endif

#undef __STRINGIFY
#undef __TOSTRING

#endif
