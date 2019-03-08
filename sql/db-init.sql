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
PRAGMA foreign_keys = ON;

CREATE TABLE IF NOT EXISTS "windows" (
	"wnd_id" INTEGER PRIMARY KEY ASC ON CONFLICT FAIL AUTOINCREMENT,
	"wnd_name" TEXT
);

CREATE UNIQUE INDEX IF NOT EXISTS "windows_name_idx" ON "windows" ("wnd_name");

CREATE TABLE IF NOT EXISTS "datapoints" (
	"dp_id" INTEGER PRIMARY KEY ASC ON CONFLICT FAIL AUTOINCREMENT,
	"dp_wnd_id" INTEGER REFERENCES "windows" ("wnd_id")
	"dp_start" INTEGER NOT NULL,
	"dp_end" INTEGER NOT NULL
);

CREATE INDEX IF NOT EXISTS "datapoints_start_idx" ON "datapoints" ("dp_start");
CREATE INDEX IF NOT EXISTS "datapoints_end_idx" ON "datapoints" ("dp_end");

CREATE TABLE IF NOT EXISTS "labels" (
	"lbl_id" INTEGER PRIMARY KEY ASC ON CONFLICT FAIL AUTOINCREMENT,
	"lbl_name" TEXT,
	"lbl_regex" TEXT
);

