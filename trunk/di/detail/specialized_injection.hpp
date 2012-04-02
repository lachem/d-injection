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
private:
	typedef injection<typename representation<T,P>::type> base;
public:
	specialized_injection() : base(type_id_as_ptr()) {}

	T* operator->() {
		assert(!empty());
		return base::getObject();	
	}

	T& operator*() {
		assert(!empty());
		return base::getObject();
	}

	operator T*() {
		assert(!empty());
		return base::getObject();
	}

	T const* operator->() const {
		assert(!empty());
		return base::getObject();
	}

	T const& operator*() const {
		assert(!empty());
		return base::getObject();
	}

	operator T const*() const {
		assert(!empty());
		return base::getObject();
	}

	T* get() {
		assert(!empty());
		return base::getObject();	
	}

	T const* get() const {
		assert(!empty());
		return base::getObject();	
	}

	bool empty() {
		return base::getObject() == NULL || base::getObject() == type_id_as_ptr();
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
