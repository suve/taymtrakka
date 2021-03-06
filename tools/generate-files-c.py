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

import argparse
import mimetypes
import os
import sys

def load_template(filename):
	tpl = open(filename, "r").read().split("\n")
	
	while True:
		popped = tpl.pop(0).rstrip()
		if popped == ' */': break
	
	tpl.insert(0,
		'/**\n'
		' * THIS FILE IS AUTO-GENERATED BY THE\n'
		' *   "' + sys.argv[0] + '"\n'
		' * HELPER SCRIPT FROM THE\n'
		' *   "' + filename + '"\n'
		' * TEMPLATE FILE.\n'
		' * \n'
		' * DO NOT EDIT THIS FILE - ANY HAND-MADE CHANGES WILL MOST LIKELY BE LOST.\n'
		' * REFER TO THE GENERATOR OR THE TEMPLATE FILE INSTEAD.\n'
		' */') 
	
	return "\n".join(tpl) 

def escape_str(string):
	string = string.replace("\\", "\\\\")
	string = string.replace("\"", "\\\"")
	return string

def genfunc_getContents(filelist):
	mimetypes.init()
	
	func = ""
	for f_raw in filelist:
		f_raw = os.path.basename(f_raw)
		f_safe = escape_str(f_raw)
		
		mime_type, _ = mimetypes.guess_type(f_raw)
		mime = (mime_type if mime_type is not None else "application/octet-stream") + ";charset=UTF-8" 
		
		func += ( 
			'\tif(strcmp(name, "' + f_safe + '") == 0) {\n'
			'\t\tcontent = ""\n'
			'\t\t\t#include "files/' + f_safe + '.c"\n'
			'\t\t;\n'
			'\t\tmimetype = "' + escape_str(mime) + '";\n'
			'\t}\n'
		)
	return func

def parse_args():
	parser = argparse.ArgumentParser(add_help=False)
	parser.add_argument('filelist', nargs='*')
	parser.add_argument('--template', type=str, nargs=1)
	args = parser.parse_args()
	args = vars(args)
	
	return args

def main():
	args = parse_args()
	tpl = load_template(args['template'][0])
	
	tpl = tpl.replace('«files_getContent()»', genfunc_getContents(args['filelist']))
	
	sys.stdout.write(tpl)


if __name__ == "__main__":
	main()
