//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef IOCFACTORY_HPP
#define IOCFACTORY_HPP

#include <boost/fusion/include/for_each.hpp>

template<typename T>
class IocFactory {	
public:
	IocFactory() {
		boost::fusion::for_each(injections,detail::set_null());
		boost::fusion::for_each(T::injections,detail::set_null());
	}
	T* create() {
		//TODO: static_assert for type compatibility
		//FIXME:  must be synchronized
		boost::fusion::for_each(injections,detail::set_unoccupied());
		T::injections = injections;
		return new T;
	}
	template<typename U>
	IocFactory<T>& use(U& object) {
		//FIXME: must be synchronized
		boost::fusion::for_each(injections,detail::set_next_same_type<U>(&object));
		return *this;
	}
	template<typename U>
	IocFactory<T>& replace(U& object, int at=0) {
		//FIXME: must be synchronized
		boost::fusion::for_each(injections,detail::set_nth_same_type<U>(&object,at));
		return *this;
	}
private:
	typename T::type injections;
};

#endif //IOCFACTORY_HPP

