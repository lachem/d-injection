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

	template<typename T>
	bool holds() {
		return this->id == T::id;
	}

private:
	const int id;
};

template<class U>
struct injection_source_imp : public injection_source<typename U::type> {
	injection_source_imp(const U& inj) : 
		object(inj.object) , injection_source<typename U::type>(U::id) {}

	typename U::representation object;
};

} //namspace detail
} //namspace di

#endif //DI_INJECTION_SOURCE_HPP
