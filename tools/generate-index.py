#!/usr/bin/python3
# -*- coding: utf-8 -*-
#
# This file is part of Taym Trakka.
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

import os
import sys

def escape_str(string):
	string = string.replace("\\", "\\\\")
	string = string.replace("\"", "\\\"")
	return string

def main():
	args = sys.argv
	args.pop(0)
	
	for raw_arg in args:
		raw_arg = os.path.basename(raw_arg)
		safe_arg = escape_str(raw_arg)
		sys.stdout.write(
			'if(strcmp(url, "' + safe_arg + '") == 0) staticdata = \n'
			'\t#include "files/' + safe_arg + '.c"\n'
			'; else '
		)
	
	sys.stdout.write('staticdata = NULL;');


if __name__ == "__main__":
	main()
