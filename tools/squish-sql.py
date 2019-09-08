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

import re
import sys

def main():
	input_str = sys.stdin.read()
	input_lines = input_str.split("\n")
	
	output_lines = []
	for line in input_lines:
		line = line.strip()
		
		if line == "":
			continue
		if line.startswith("--"):
			continue
		
		line = line.replace("\\", "\\\\")
		line = line.replace("\"", "\\\"")
		
		output_lines.append(line)
	
	output_str = " ".join(output_lines)
	
	output_str = re.sub(r";[ ]*", r";", output_str)
	output_str = output_str.rstrip("; ")
	
	output_str = re.sub(r",[ ]*", r",", output_str)
	output_str = re.sub(r"[ ]*([()])[ ]*", r"\1", output_str)
	
	sys.stdout.write('"' + output_str + '"')


if __name__ == "__main__":
	main()
