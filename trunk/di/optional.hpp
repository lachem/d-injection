//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_OPTIONAL_HPP
#define DI_OPTIONAL_HPP

#include <di/detail/injection.hpp>
#include <di/detail/utility.hpp>

namespace di {

template<typename T>
struct optional : public detail::injection< T,ordinary<T> > {	
	optional() : detail::injection< T,ordinary<T> >(false) {}
};

template<typename T>
struct optional< shared<T> > : public detail::injection< T,shared<T> > {
	optional() : detail::injection< T,shared<T> >(false) {}
};

template<typename T>
struct optional< unique<T> > : public detail::injection< T,unique<T> > {
	optional() : detail::injection< T,unique<T> >(false) {}
};

} //namspace di

#endif //DI_OPTIONAL_HPP
