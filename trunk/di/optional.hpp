//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_OPTIONAL_HPP
#define DI_OPTIONAL_HPP

#include <di/detail/inject_container.hpp>
#include <di/detail/injection.hpp>

namespace di {

template<typename T>
class optional : public detail::injection<T> {
public:
	operator bool() {
		return object != NULL;
	}
	virtual bool satisified() {
		return true;
	}
};

} //namspace di

#endif //DI_OPTIONAL_HPP
