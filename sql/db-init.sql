--
-- This file is part of Taym Trakka.
-- Copyright (C) 2019 Artur "suve" Iwicki
--
-- This program is free software: you can redistribute it and/or modify
-- it under the terms of the GNU General Public License, version 3,
-- as published by the Free Software Foundation.
--
-- This program is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
-- GNU General Public License for more details.
--
-- You should have received a copy of the GNU General Public License along with
-- this program (LICENCE.txt). If not, see <http://www.gnu.org/licenses/>.
--
PRAGMA page_size = 4096;

CREATE TABLE IF NOT EXISTS "windows" (
	"wnd_id" INTEGER PRIMARY KEY ASC ON CONFLICT FAIL AUTOINCREMENT,
	"wnd_name" TEXT UNIQUE
);

CREATE TABLE IF NOT EXISTS "datapoints" (
	"dp_id" INTEGER PRIMARY KEY ASC ON CONFLICT FAIL AUTOINCREMENT,
	"dp_wnd_id" INTEGER,
	"dp_start" INTEGER,
	"dp_end" INTEGER
);

CREATE TABLE IF NOT EXISTS "labels" (
	"lbl_id" INTEGER PRIMARY KEY ASC ON CONFLICT FAIL AUTOINCREMENT,
	"lbl_name" TEXT,
	"lbl_regex" TEXT
);

