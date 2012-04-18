//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_DELETE_POLICY_HPP
#define DI_DELETE_POLICY_HPP

#include <boost/shared_ptr.hpp>
#include <memory>
#include <di/detail/item.hpp>
#include <di/detail/utility.hpp>

namespace di {

template<typename T>
struct ordinary{};

template<typename T>
struct unique {
	explicit unique(T* an_object) : object(an_object) {}
	T* object;
};

template<typename T>
struct shared {
	explicit shared(T* an_object) : object(an_object) {}
	T* object;
};

namespace detail {

template<typename R, typename T>
struct ordinary_item : public item<T> {
	ordinary_item(R* rep, bool required) : item<T>(rep,required){}
private:
	virtual void do_assignement(T* object) {
		*reinterpret_cast< R* >(address) = object;
	}
};

template<typename R, typename T>
struct smart_ptr_item : public item<T> {
	smart_ptr_item(R* rep, bool required) : item<T>(rep,required) {}
private:
	virtual void do_assignement(T* object) {
		reinterpret_cast< R* >(address)->reset(object);
	}
};

template<typename T>
struct representation;

template<typename T>
struct representation< ordinary<T> > {
	typedef T* type;
	typedef ordinary_item<type,T> item;

	static T* get(type* rep) {
		return *rep;
	}
	static void init(type* rep) {
		*rep = NULL;
	}
};

template<typename T>
struct representation< unique<T> > {
	typedef std::auto_ptr<T> type;
	typedef smart_ptr_item<type,T> item;

	static T* get(type* rep) {
		return rep->get();
	}
	static void init(type* rep) {
		rep->reset(NULL_PTR(T));
	}
};

template<typename T>
struct representation< shared<T> > {
	typedef boost::shared_ptr<T> type;
	typedef smart_ptr_item<type,T> item;

	static T* get(type* rep) {
		return rep->get();
	}
	static void init(type* rep) {
		rep->reset(NULL_PTR(T));
	}
};

} // namespace detail
} // namespace di

#endif //DI_DELETE_POLICY_HPP
