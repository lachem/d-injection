//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_HELPERS_HPP
#define DI_HELPERS_HPP

#include <boost/function.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include <di/detail/inject_container.hpp>
#include <di/detail/item.hpp>

namespace di {
namespace detail {

struct set_null {
	template<typename V>
	void operator()(V& v) const {
		v = NULL;
	}
};

template<typename T>
struct set_next_same_type {

	set_next_same_type(T* an_object, bool* result) : 
		object(an_object), set(result) {
		assert(*set == false);
	};

	template<typename V>
	void operator()(V& v,typename boost::enable_if<boost::is_same<V,T* > >::type* dummy = 0) const {
		if(!v && !*set) {
			v = object;
			*set = true;
		}
	}
	template<typename V>
	void operator()(V& v,typename boost::disable_if<boost::is_same<V,T* > >::type* dummy = 0) const {
		//empty
	}

private:
	mutable T* object;
	mutable bool* set;
};

template<typename T>
struct set_nth_same_type {

	set_nth_same_type(T* an_object,int at, bool* a_result) : 
		object(an_object), counter(at), result(a_result) {
		assert(*a_result == false);
	};

	template<typename V>
	void operator()(V& v,typename boost::enable_if<boost::is_same<V,T* > >::type* dummy = 0) const {
		if(0 == counter--) {
			v = object;
			*result = true;
		}
	}
	template<typename V>
	void operator()(V& v,typename boost::disable_if<boost::is_same<V,T* > >::type* dummy = 0) const {
		//empty
	}

private:
	mutable T* object;
	mutable int counter;
	mutable bool* result;
};

template<typename T>
struct perform_injection {
	perform_injection(T* subject, void (*an_unsatisfied_req_handler)(T*)) : 
		subject(subject), unsatisfied_req_handler(an_unsatisfied_req_handler)  {}

	template<typename V>
	void operator()(V& v) const {
		typedef typename boost::remove_pointer<V>::type injected_type;
		typedef inject_container< item<injected_type> > container;
		
		item<injected_type> inj_item = container::remove(reinterpret_cast<char*>(subject),sizeof(T));
		if(!inj_item.assign(v)) {
			unsatisfied_req_handler(subject);
		}
	}

private:
	T* subject;
	void (*unsatisfied_req_handler)(T*);
};

} // namespace detail
} // namspace di

#endif //DI_HELPERS_HPP
