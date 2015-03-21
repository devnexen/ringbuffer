/*************************************************************************/
/* test.cpp - test files for minimal                                     */
/* Copyright (C) 2014-2015                                               */
/* Johannes Lorenz (jlsf2013 @ sourceforge)                              */
/*                                                                       */
/* This program is free software; you can redistribute it and/or modify  */
/* it under the terms of the GNU General Public License as published by  */
/* the Free Software Foundation; either version 3 of the License, or (at */
/* your option) any later version.                                       */
/* This program is distributed in the hope that it will be useful, but   */
/* WITHOUT ANY WARRANTY; without even the implied warranty of            */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU      */
/* General Public License for more details.                              */
/*                                                                       */
/* You should have received a copy of the GNU General Public License     */
/* along with this program; if not, write to the Free Software           */
/* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110, USA  */
/*************************************************************************/

#include <iostream>
#include <cassert>
#include "../lib/ringbuffer.h"

int main()
{
	try {
		// test the ringbuffer
		ringbuffer_t rb(4);
		ringbuffer_reader_t rd(rb);
		ringbuffer_reader_t rd2(rb);
		assert(!rd.read_space());

		std::size_t n = rb.write("abcd", 5);
		assert(n==3);
		assert(!rb.write_space());
		// simulate impossible write
		assert(!rb.write("xyz", 4));
		{
			assert(rd.read_space() == 3);
			auto s = rd.read_sequence(3);
			assert(s[0] == 97 && s[1] == 98 && s[2] == 99);
		}
		assert(!rd.read_space());
		assert(!rb.write_space()); // reader 2 is still missing
		{
			rd2.read_sequence(3);
		}

		assert(rb.write("ab", 2) == 2);
		assert(!rb.write_space());
		{
			assert(rd.read_space() == 2);
			auto s = rd.read_sequence(1);
			assert(s[0] == 97);
		}
		{
			assert(rd.read_space() == 1);
			auto s2 = rd.read_sequence(1);
			assert(s2[0] == 98);
		}
		assert(!rb.write_space());
		{
			rd2.read_sequence(2);
		}
		assert(rb.write_space() == 2);
		assert(!rd.read_space());
		rb.write("x", 1);
		assert(rb.write_space() == 1);
		{
			assert(rd2.read_space() == 1);
			auto s = rd2.read_sequence(1);
			assert(s[0] == 120);
		}
		assert(rb.write_space() == 1);
		{
			rd.read_sequence(2);
		}
		assert(rb.write_space() == 3);

	} catch (const char* s)
	{
		std::cerr << s << std::endl;
		return 1;
	}

	std::cerr << "SUCCESS!" << std::endl;

	return 0;
}

