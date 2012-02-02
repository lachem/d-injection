//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_BUILDER_HPP
#define DI_BUILDER_HPP

#include <boost/fusion/include/for_each.hpp>
#include <cstdlib>

namespace di {

template<typename T>
class builder {	
public:
	builder() {
		boost::fusion::for_each(injections,detail::set_null());
	}
	T* build() {
		//TODO: static_assert for type compatibility
		//FIXME: synchronize
		//boost::fusion::for_each(injections,detail::reset_list());
		//void* memory = malloc(sizeof(T));
		//can do something inbetween
		//T* subject = new (memory) T;
		T* subject = new T;
		boost::fusion::for_each(injections,detail::perform_injection<T>(subject));
		return subject;
	}
	template<typename U>
	builder<T>& use(U& object) {
		//FIXME: must be synchronized
		boost::fusion::for_each(injections,detail::set_next_same_type<U>(&object));
		return *this;
	}
	template<typename U>
	builder<T>& replace(U& object, int at=0) {
		//FIXME: must be synchronized
		boost::fusion::for_each(injections,detail::set_nth_same_type<U>(&object,at));
		return *this;
	}
private:
	typename T::type injections;
};

} //namspace di

#endif //DI_BUILDER_HPP

