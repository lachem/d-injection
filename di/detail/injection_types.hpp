//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_INJECTION_TYPES_HPP
#define DI_INJECTION_TYPES_HPP

#include <memory>
#include <boost/shared_ptr.hpp>
#include <di/detail/utility.hpp>

namespace di {
namespace detail {
	enum injection_id {ordinary_id,unique_id,shared_id};
} // namespace detail

template<typename T>
struct ordinary {
	enum{id=detail::ordinary_id};

	typedef T type;
	typedef T* representation;
	
	explicit ordinary(T* an_object) : object(an_object) {}

	static T* extract(representation* rep) {
		return *rep;
	}
	static const T* extract(const representation* rep) {
		return *rep;
	}
	static void init(representation* rep) {
		*rep = NULL;
	}

	T* object;
};

template<typename T>
struct unique {
	enum{id=detail::unique_id};

	typedef T type;
	typedef std::auto_ptr<T> representation;

	explicit unique(T* an_object) : object(an_object) {}

	static T* extract(representation* rep) {
		return rep->get();
	}
	static const T* extract(const representation* rep) {
		return rep->get();
	}
	static void init(representation* rep) {
		rep->reset(NULL_PTR(T));
	}

	T* object;
};

template<typename T>
struct shared {
	enum{id=detail::shared_id};

	typedef T type;
	typedef boost::shared_ptr<T> representation;

	explicit shared(T* an_object) : object(an_object) {}

	static T* extract(representation* rep) {
		return rep->get();
	}
	static const T* extract(const representation* rep) {
		return rep->get();
	}
	static void init(representation* rep) {
		rep->reset(NULL_PTR(T));
	}

	T* object;
};

} // namespace di

#endif //DI_INJECTION_TYPES_HPP
