//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_SPECIALIZED_INJECTION_HPP
#define DI_SPECIALIZED_INJECTION_HPP

#include <di/detail/injection.hpp>

namespace di {
namespace detail {

template<typename T, typename P, int type_id>
struct specialized_injection : public injection<typename representation<T,P>::type> {
	specialized_injection() : injection<typename representation<T,P>::type>(type_id_as_ptr()) {}

	T* operator->() {
		assert(!empty());
		return getObject();	
	}

	T& operator*() {
		assert(!empty());
		return getObject();
	}

	operator T*() {
		assert(!empty());
		return getObject();
	}

	T const* operator->() const {
		assert(!empty());
		return getObject();
	}

	T const& operator*() const {
		assert(!empty());
		return getObject();
	}

	operator T const*() const {
		assert(!empty());
		return getObject();
	}

	T* get() {
		assert(!empty());
		return getObject();	
	}

	T const* get() const {
		assert(!empty());
		return getObject();	
	}

	bool empty() {
		return  getObject() == NULL || getObject() == type_id_as_ptr();
	}

protected:
	static bool is_same(T* id_as_ptr) {
		return type_id_as_ptr() == id_as_ptr;
	}

private:
	static T* type_id_as_ptr() {
		return reinterpret_cast<T*>(type_id);
	}	
};


} //namspace detail
} //namspace di

#endif //DI_SPECIALIZED_INJECTION_HPP
