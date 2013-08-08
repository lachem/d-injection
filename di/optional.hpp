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
struct optional : 
	public detail::injection< T,ordinary<T> >, 
	public detail::nonallocatable  
{	
	typedef detail::injection< T,ordinary<T> > base;

	optional() : base(false) {}
	optional(const optional<T>& source) : base(source) {
		base::do_copy(source,false);
	}
	optional<T>& operator=(const optional<T>& source) {
		base::do_assignement(source,false);
		return *this;
	}
	operator T*() {
		assert(!base::empty());
		return base::get_object();
	}
	operator T const*() const {
		assert(!base::empty());
		return base::get_object();
	}
};

template<typename T>
struct optional< shared<T> > : 
	public detail::injection< T,shared<T> >, 
	public detail::nonallocatable 
{
	typedef detail::injection< T,shared<T> > base;

	optional() : base(false) {}
	optional(const optional< shared<T> >& source) : base(source) {
		base::do_copy(source,false);
	}
	optional< shared<T> >& operator=(const optional< shared<T> >& source) {
		base::do_assignement(source,false);
		return *this;
	}
	operator typename shared<T>::representation () {
		return base::rep_object;
	}
	operator const typename shared<T>::representation () const {
		return base::rep_object;
	}
};

template<typename T>
struct optional< unique<T> > :
	public detail::injection< T,unique<T> >, 
	public detail::nonallocatable, public detail::noncopyable 
{
	typedef detail::injection< T,unique<T> > base;

	optional() : base(false) {}
};


template<typename T>
struct optional< service<T> > : 
	public detail::injection< T,service<T> >, 
	public detail::nonallocatable 
{
	typedef detail::injection< T,service<T> > base;

	optional() : base(false) {}
	optional(const optional< service<T> >& source) : base(source) {
		base::do_copy(source,false);
	}
	optional< service<T> >& operator=(const optional< service<T> >& source) {
		base::do_assignement(source,false);
		return *this;
	}
	operator typename service<T>::representation () {
		return base::rep_object;
	}
	operator const typename service<T>::representation () const {
		return base::rep_object;
	}
};

} //namspace di

#endif //DI_OPTIONAL_HPP
