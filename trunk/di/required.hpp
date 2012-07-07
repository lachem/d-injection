//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_REQUIRED_HPP
#define DI_REQUIRED_HPP

#include <di/detail/injection.hpp>
#include <di/detail/utility.hpp>

namespace di {

template<typename T>
struct required : 
	public detail::injection< T,ordinary<T> >, 
	public detail::nonallocatable 
{
	typedef detail::injection< T,ordinary<T> > base;

	required() : base(true) {}
	required(const required<T>& inj) : base(inj) {
		if(base::empty()) {			
			base::insert_self_into_container(true);
		}
	}
};

template<typename T>
struct required< shared<T> > : 
	public detail::injection< T,shared<T> >, 
	public detail::nonallocatable 
{
	typedef detail::injection< T,shared<T> > base;

	required() : base(true) {}
	required(const required< shared<T> >& inj) : base(inj) {
		if(base::empty()) {
			base::insert_self_into_container(true);
		}
	}
};

template<typename T>
struct required< unique<T> > : 
	public detail::injection< T,unique<T> >, 
	public detail::nonallocatable, public detail::noncopyable 
{
	typedef detail::injection< T,unique<T> > base;

	required() : base(true) {}
};

} //namspace di

#endif //DI_REQUIRED_HPP
