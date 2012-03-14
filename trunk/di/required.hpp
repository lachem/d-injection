//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_REQUIRED_HPP
#define DI_REQUIRED_HPP

#include <di/detail/inject_container.hpp>
#include <di/detail/injection.hpp>

namespace di {

template<typename T>
struct required : public detail::injection<T> {
	required() : detail::injection<T>(default_ptr()) {
	}

private:
	enum {default_ptr_value = 1};

	//Works only when injection is uninitialized
	static bool is_same(detail::injection<T>& injection) {
		return default_ptr() == injection;
	}

	static T* default_ptr() {
		return reinterpret_cast<T*>(default_ptr_value);
	}

	friend struct detail::perform_injection;
};

} //namspace di

#endif //DI_REQUIRED_HPP
