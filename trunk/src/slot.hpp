//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SLOT_HPP_
#define SLOT_HPP_

template<typename T>
struct Slot {
	typedef T type;

	Slot() : object(0), occupied(false) {}

	void reset() {
		object = 0;
		occupied = false;
	}

	type object;
	bool occupied;
};

#endif // SLOT_HPP_
