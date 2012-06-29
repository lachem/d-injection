//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_INJECTION_SOURCE_HPP
#define DI_INJECTION_SOURCE_HPP

#include <di/detail/injection_types.hpp>

namespace di {
namespace detail {

template<typename T>
struct injection_source {
	typedef T type;

	injection_source(int an_id) : id(an_id) {}
	virtual ~injection_source() {}

	template<typename Y>
	bool holds() {
		return this->id == Y::id;
	}

	bool empty() {
		return is_empty;
	}

	virtual const T* get_object() const = 0; 

protected:
	const int id;
	bool is_empty;
};

template<class U>
struct injection_source_imp : public injection_source<typename U::type> {
	injection_source_imp(const U& inj) : 
		object(inj.object) , injection_source<typename U::type>(U::id) {
		is_empty = (inj.object == NULL);
	}

	injection_source_imp() : injection_source<typename U::type>(U::id) {
		is_empty = true;
	}

	virtual const typename U::type* get_object() const {
		return U::extract(&object);
	}

	typename U::representation object;
};

} //namspace detail
} //namspace di

#endif //DI_INJECTION_SOURCE_HPP
