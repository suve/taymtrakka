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

import rcssmin
import sys

def main():
	input_str = sys.stdin.read()
	
	output_str = rcssmin.cssmin(input_str)
	output_str = output_str.replace("\\", "\\\\")
	output_str = output_str.replace("\"", "\\\"")
	
	sys.stdout.write('"' + output_str + '"')


if __name__ == "__main__":
	main()
