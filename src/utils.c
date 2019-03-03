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
#include <string.h>

size_t escapeHTML(char *buffer, size_t bufsize, const char *source) {
	size_t originalSize = bufsize;
	--bufsize; // Subtract 1 so we always have space for the NUL-terminator
	
	for(;;) {
		switch(*source) {
			case 0:
				goto nulterm;
			
			case '"':
				if(bufsize < 6) goto nulterm;
				bufsize -= 6;
				
				*buffer++ = '&';
				*buffer++ = 'q';
				*buffer++ = 'u';
				*buffer++ = 'o';
				*buffer++ = 't';
				*buffer++ = ';';
			break;
			
			case '&':
				if(bufsize < 5) goto nulterm;
				bufsize -= 5;
				
				*buffer++ = '&';
				*buffer++ = 'a';
				*buffer++ = 'm';
				*buffer++ = 'p';
				*buffer++ = ';';
			break;
			
			case '<':
				if(bufsize < 4) goto nulterm;
				bufsize -= 4;
				
				*buffer++ = '&';
				*buffer++ = 'l';
				*buffer++ = 't';
				*buffer++ = ';';
			break;
			
			case '>':
				if(bufsize < 4) goto nulterm;
				bufsize -= 4;
				
				*buffer++ = '&';
				*buffer++ = 'g';
				*buffer++ = 't';
				*buffer++ = ';';
			break;
			
			default:
				*buffer++ = *source;
				if(--bufsize == 0) goto nulterm;
		}
		++source;
	}
	
	nulterm: {
		*buffer = '\0';
		return originalSize - (bufsize + 1);
	}
}
