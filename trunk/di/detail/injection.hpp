//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_INJECTION_HPP
#define DI_INJECTION_HPP

#include <di/detail/inject_container.hpp>

namespace di {
namespace detail {

struct perform_injection;

template<typename T>
class injection {
public:
	injection() : object(NULL) {
		detail::inject_container<T>::insert(this);
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
	
	virtual bool satisified() = 0;

protected:
	T* object;

	friend struct perform_injection;
};

} //namspace detail
} //namspace di

#endif //DI_INJECTION_HPP
