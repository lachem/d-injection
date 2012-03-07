//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_INJECT_HPP
#define DI_INJECT_HPP

#include <di/detail/inject_container.hpp>

namespace di {

template<typename T>
class inject {
public:
	inject() {
		detail::inject_container<T>::insert(&object);
	}

	T* operator->() {
		return object;	
	}

	T& operator*() {
		assert(object);
		return *object;
	}

	operator T*() {
		return object;
	}

	T const* operator->() const {
		assert(object);
		return object;
	}

	T const& operator*() const {
		assert(object);
		return *object;
	}

	operator T const*() const {
		return object;
	}
	
private:
	T* object;
};

} //namspace di

#endif //DI_INJECT_HPP
