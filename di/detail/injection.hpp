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
struct injection {
protected:
	explicit injection(T* an_object) : object(an_object) {
		detail::inject_container<T>::insert(this);
	}

protected:
	T* object;

	friend struct perform_injection;
};

} //namspace detail
} //namspace di

#endif //DI_INJECTION_HPP
