//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_SPECIALIZED_INJECTION_HPP
#define DI_SPECIALIZED_INJECTION_HPP

#include <di/detail/injection.hpp>

namespace di {
namespace detail {

template<typename T, int type_id>
struct specialized_injection : public injection<T> {
private:
	typedef injection<T> base;
public:
	specialized_injection() : base(type_id_as_ptr()) {}

	specialized_injection(const specialized_injection<T,type_id>& s_inj) : injection<T>(s_inj) {}

	T* operator->() {
		assert(!empty());
		return base::get_object();	
	}

	T& operator*() {
		assert(!empty());
		return base::get_object();
	}

	operator T*() {
		assert(!empty());
		return base::get_object();
	}

	T const* operator->() const {
		assert(!empty());
		return base::get_object();
	}

	T const& operator*() const {
		assert(!empty());
		return base::get_object();
	}

	operator T const*() const {
		assert(!empty());
		return base::get_object();
	}

	T* get() {
		assert(!empty());
		return base::get_object();	
	}

	T const* get() const {
		assert(!empty());
		return base::get_object();	
	}

	bool empty() {
		return base::get_object() == NULL || base::get_object() == type_id_as_ptr();
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
