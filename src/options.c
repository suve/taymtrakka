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
#include <getopt.h>
#include <stdlib.h>

#include "options.h"


static void setDefaults(struct Options *opts) {
	*opts = (struct Options){
		.portNumber = 36912,
		.updateInterval = 5,
	};
}


#define ARG_NONE    no_argument
#define ARG_MAYBE   optional_argument
#define ARG_REQUIRE required_argument

enum OptionName {
	OPT_INTERVAL = 1,
	OPT_PORT,
};

static struct option OptionList[] = {
	{ "interval", ARG_REQUIRE, NULL, OPT_PORT },
	{ "port", ARG_REQUIRE, NULL, OPT_PORT },
	{ 0, 0, 0, 0 },
};

void options_parse(int argc, char **argv, struct Options *opts) {
	setDefaults(opts);
	
	while(1) {
		int optionIndex = 0;
		int optionCurrent = 0;

		optionCurrent = getopt_long(argc, argv, "", OptionList, &optionIndex);
		if(optionCurrent == -1) break;

		switch (optionCurrent) {
			case OPT_INTERVAL:
				opts->updateInterval = atoi(optarg); // TODO: error checking
			break;
			
			case OPT_PORT:
				opts->portNumber = atoi(optarg); // TODO: error checking
			break;
		}
	}
}
