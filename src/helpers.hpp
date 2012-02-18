//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_HELPERS_HPP
#define DI_HELPERS_HPP

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include "inject_container.hpp"

namespace di {
namespace detail {

struct set_null {
	template<typename V>
	void operator()(V& v) const {
		v = 0;
	}
};

template<typename T>
struct set_next_same_type {

	set_next_same_type(T* obj) : object(obj), set(false){};

	template<typename V>
	void operator()(V& v,typename boost::enable_if<boost::is_same<V,T* > >::type* dummy = 0) const {
		if(!v && !set) {
			v = object;
			set = true;
		}
	}
	template<typename V>
	void operator()(V& v,typename boost::disable_if<boost::is_same<V,T* > >::type* dummy = 0) const {
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
	void operator()(V& v,typename boost::enable_if<boost::is_same<V,T* > >::type* dummy = 0) const {
		if(0 == counter--) {
			v = object;
		}
	}
	template<typename V>
	void operator()(V& v,typename boost::disable_if<boost::is_same<V,T* > >::type* dummy = 0) const {
		//empty
	}

private:
	mutable T* object;
	mutable int counter;
};

template<typename T>
struct perform_injection {
	perform_injection(T* sub) : subject(sub) {}

	template<typename V>
	void operator()(V& v) const {
		typedef inject_container<typename boost::remove_pointer<V>::type> container;
		V* injection = container::remove(reinterpret_cast<char*>(subject),sizeof(T));
		if(0 != injection) {
			*injection = v;
		}
	}

private:
	mutable T* subject;
};

} // namespace detail
} // namspace di

#endif //DI_HELPERS_HPP
