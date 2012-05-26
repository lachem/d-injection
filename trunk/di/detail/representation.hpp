//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_REPRESENTATION_HPP
#define DI_REPRESENTATION_HPP

#include <memory>
#include <boost/shared_ptr.hpp>
#include <di/detail/utility.hpp>
#include <di/detail/injection_types.hpp>

namespace di {
namespace detail {

template<typename T>
struct representation;

template<typename T>
struct representation< ordinary<T> > {
	typedef typename ordinary<T>::representation type;

	static T* get(type* rep) {
		return *rep;
	}
	static void init(type* rep) {
		*rep = NULL;
	}
};

template<typename T>
struct representation< unique<T> > {
	typedef typename unique<T>::representation type;

	static T* get(type* rep) {
		return rep->get();
	}
	static void init(type* rep) {
		rep->reset(NULL_PTR(T));
	}
};

template<typename T>
struct representation< shared<T> > {
	typedef typename shared<T>::representation type;

	static T* get(type* rep) {
		return rep->get();
	}
	static void init(type* rep) {
		rep->reset(NULL_PTR(T));
	}
};

} // namespace detail
} // namespace di

#endif //DI_REPRESENTATION_HPP
