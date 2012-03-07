//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_BUILDER_HPP
#define DI_BUILDER_HPP

#include <boost/fusion/include/for_each.hpp>
#include <boost/fusion/include/value_at.hpp>
#include <di/detail/helpers.hpp>
#include <di/configuration.hpp>

namespace di {

template<typename T>
class builder {	
public:
	builder() {
		boost::fusion::for_each(injections,detail::set_null());
	}

	virtual T* build() = 0;
	virtual void delegate(T&) = 0;

	template<typename U>
	builder<T>& use(U& object) {
		boost::fusion::for_each(injections,detail::set_next_same_type<U>(&object));
		assert(use_succeeded(object));
		return *this;
	}
	template<typename U>
	builder<T>& replace(U& object, int at=0) {
		boost::fusion::for_each(injections,detail::set_nth_same_type<U>(&object,at));
		assert(replace_succeeded(object));
		return *this;
	}

protected:
	template<typename U>
	bool replace_succeeded(U& object) {
		return use_succeeded(object);
	}
	template<typename U>
	bool use_succeeded(U& object) {
		bool result = false;
		boost::fusion::for_each(injections,detail::contains<U>(&object,&result));
		return result;
	}

	typename T::type injections;
};

} //namspace di

#endif //DI_BUILDER_HPP

