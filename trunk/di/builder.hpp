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
		bool use_result = false;
		boost::fusion::for_each(injections,detail::set_next_same_type<U>(&object,&use_result));
		handle_use_result(use_result);
		return *this;
	}
	template<typename U>
	builder<T>& replace(U& object, int at=0) {
		bool replace_result = false;
		boost::fusion::for_each(injections,detail::set_nth_same_type<U>(&object,at,&replace_result));
		handle_replace_result(replace_result);
		return *this;
	}

private:
	virtual void handle_use_result(bool success) = 0;
	virtual void handle_replace_result(bool success) = 0;

protected:
	typename T::type injections;
};

} //namspace di

#endif //DI_BUILDER_HPP

