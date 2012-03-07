//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_HELPERS_HPP
#define DI_HELPERS_HPP

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include <di/detail/inject_container.hpp>

namespace di {
namespace detail {

struct set_null {
	template<typename V>
	void operator()(V& v) const {
		v = 0;
	}
};

template<typename T>
struct contains {
	contains(T* an_object, bool* a_result) : object(an_object), result(a_result) {}
	template<typename V>
	void operator()(V& v, typename boost::enable_if<boost::is_same<V,T* > >::type* dummy = 0) const {
		if(v == object) {
			*result = true;
		}
	}
	template<typename V>
	void operator()(V& v,typename boost::disable_if<boost::is_same<V,T* > >::type* dummy = 0) const {
		//empty
	}
private:
	mutable T* object;
	mutable bool* result;
};

template<typename T>
struct set_next_same_type {

	set_next_same_type(T* an_object) : object(an_object), set(false){};

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

	bool has_succeeded() {
		return set;
	}

private:
	mutable T* object;
	mutable bool set;
};

template<typename T>
struct set_nth_same_type {

	set_nth_same_type(T* an_object,int at) : object(an_object), counter(at){};

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

	bool has_succeeded() {
		return counter == -1;
	}

private:
	mutable T* object;
	mutable int counter;
};

struct perform_injection {
	perform_injection(char* an_address, size_t a_size) : address(an_address), size(a_size) {}

	template<typename V>
	void operator()(V& v) const {
		typedef inject_container<typename boost::remove_pointer<V>::type> container;
		V* injection = container::remove(address,size);
		if(0 != injection) {
			*injection = v;
		}
	}

private:
	char* const address;
	size_t const size;
};

} // namespace detail
} // namspace di

#endif //DI_HELPERS_HPP
