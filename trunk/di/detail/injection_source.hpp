//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_INJECTION_SOURCE_HPP
#define DI_INJECTION_SOURCE_HPP

#include <di/injection_types.hpp>

namespace di {
namespace detail {

template<typename T>
struct injection_source {
	typedef T type;

	explicit injection_source(short an_id) : id(an_id), is_empty(true) {}
	virtual ~injection_source() {}

	template<typename Y>
	bool holds() const {
		return this->id == Y::id;
	}
	bool empty() const {
		return is_empty;
	}
	short get_injection_type_id() const {
		return id;
	}
	virtual const T* get_object() const = 0; 

protected:
	const short id;
	bool is_empty;
};

template<class U>
struct injection_source_imp : public injection_source<typename U::type> {
	typedef injection_source<typename U::type> base;

	explicit injection_source_imp(const U& inj) : object(inj.get_object()), base(U::id) {
		base::is_empty = (inj.get_object() == NULL);
	}

	injection_source_imp() : base(U::id) {
		base::is_empty = true;
	}

	virtual const typename U::type* get_object() const {
		return U::extract(&object);
	}

	typename U::representation object;
};

} //namspace detail
} //namspace di

#endif //DI_INJECTION_SOURCE_HPP
