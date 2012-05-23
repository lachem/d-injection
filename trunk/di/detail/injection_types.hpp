//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_INJECTION_TYPES_HPP
#define DI_INJECTION_TYPES_HPP

namespace di {

template<typename T>
struct ordinary {
	typedef T type;
	explicit ordinary(T* an_object) : object(an_object) {}
	T* object;
};

template<typename T>
struct unique {
	typedef T type;
	explicit unique(T* an_object) : object(an_object) {}
	T* object;
};

template<typename T>
struct shared {
	typedef T type;
	explicit shared(T* an_object) : object(an_object) {}
	T* object;
};

} // namespace di

#endif //DI_INJECTION_TYPES_HPP
