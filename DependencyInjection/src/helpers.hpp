//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef HELPERS_HPP
#define HELPERS_HPP

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>

namespace detail {

struct set_null {
	template<typename V>
	void operator()(V& v) const {
		v.object = 0;
	}
};

struct set_unoccupied {
	template<typename V>
	void operator()(V& v) const {
		v.occupied = false;
	}
};

template<typename T>
struct get_same_unocuppied_type {

	get_same_unocuppied_type(T** obj) : object(obj), set(false){};

	template<typename V>
	void operator()(V& v,typename boost::enable_if<boost::is_same<typename V::type,T*> >::type* dummy = 0) const {
		if(!v.occupied && !set) {
			*object = v.object;
			v.occupied = set = true;
		}
	}
	template<typename V>
	void operator()(V& v,typename boost::disable_if<boost::is_same<typename V::type,T*> >::type* dummy = 0) const {
		//empty
	}
private:
	mutable T** object;
	mutable bool set;
};

template<typename T>
struct set_next_same_type {

	set_next_same_type(T* obj) : object(obj), set(false){};

	template<typename V>
	void operator()(V& v,typename boost::enable_if<boost::is_same<typename V::type,T*> >::type* dummy = 0) const {
		if(!v.object && !set) {
			v.object = object;
			set = true;
		}
	}
	template<typename V>
	void operator()(V& v,typename boost::disable_if<boost::is_same<typename V::type,T*> >::type* dummy = 0) const {
		//empty
	}

private:
	mutable T* object;
	mutable bool set;
};

template<typename T>
struct set_nth_same_type {

	set_nth_same_type(T* obj,int at) : object(obj), counter(at){};

	template<typename V>
	void operator()(V& v,typename boost::enable_if<boost::is_same<typename V::type,T*> >::type* dummy = 0) const {
		if(0 == counter--) {
			v.object = object;
		}
	}
	template<typename V>
	void operator()(V& v,typename boost::disable_if<boost::is_same<typename V::type,T*> >::type* dummy = 0) const {
		//empty
	}

private:
	mutable T* object;
	mutable int counter;
};

} // namespace detail

#endif //HELPERS_HPP

