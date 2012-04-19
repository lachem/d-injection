//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_INJECTION_HPP
#define DI_INJECTION_HPP

#include <boost/bind.hpp>
#include <di/detail/inject_container.hpp>
#include <di/detail/delete_policy.hpp>

namespace di {
namespace detail {

template<typename T, typename P>
struct injection {
	T* operator->() {
		assert(!empty());
		return get_object();	
	}

	T& operator*() {
		assert(!empty());
		return *get_object();
	}

	operator T*() {
		assert(!empty());
		return get_object();
	}

	T const* operator->() const {
		assert(!empty());
		return get_object();
	}

	T const& operator*() const {
		assert(!empty());
		return *get_object();
	}

	operator T const*() const {
		assert(!empty());
		return get_object();
	}

	T* get() {
		assert(!empty());
		return get_object();	
	}

	T const* get() const {
		assert(!empty());
		return get_object();	
	}

	bool empty() {
		return get_object() == NULL;
	}

protected:
	explicit injection(bool is_required) {
		representation<P>::init(&rep_object);
		inject_container< item<T> >::insert(
			typename representation<P>::item(&rep_object,is_required));
	}

	injection(const injection<T,P>& inj) {
		rep_object = inj.rep_object;
	}

	T* get_object() {
		return representation<P>::get(&rep_object);
	}

	T const* get_object() const {
		return representation<P>::get(&rep_object);
	}

private:
	typename representation<P>::type rep_object;
};

} //namspace detail
} //namspace di

#endif //DI_INJECTION_HPP
