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

template<typename T>
struct injectable {
	inline static T& extract(T& object) {
		return object;
	}
};

template<typename T>
struct injectable < unique<T> > {
	inline static T& extract(const unique<T>& object) {
		return *object.object;
	}
};

template<typename T>
struct injectable < shared<T> > {
	inline static T& extract(const shared<T>& object) {
		return *object.object;
	}
};

//TODO: void* casting is dangerous, maybe there is other way to remove virtuality
template<typename T>
struct ordinary_item : public item<T> {
	ordinary_item(T** rep, bool required) : item<T>(rep,required){
		item<T>::do_assignement = assignement;
	}
private:
	static void assignement(void* address, T* object) {
		*reinterpret_cast< T** >(address) = object;
	}
};

template<typename T>
struct shared_item : public item<T> {
	shared_item(boost::shared_ptr<T>* rep, bool required) : item<T>(rep,required) {
		item<T>::do_assignement = assignement;
	}
private:
	static void assignement(void* address, T* object) {
		reinterpret_cast< boost::shared_ptr<T>* >(address)->reset(object);
	}
};

template<typename T>
struct unique_item : public item<T> {
	unique_item(std::auto_ptr<T>* rep, bool required) : item<T>(rep,required) {
		item<T>::do_assignement = assignement;
	}
private:
	static void assignement(void* address, T* object) {
		reinterpret_cast< std::auto_ptr<T>* >(address)->reset(object);
	}
};

template<typename T>
struct representation;

template<typename T>
struct representation< ordinary<T> > {
	typedef T* type;
	typedef ordinary_item<T> item;

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
	typedef unique_item<T> item;

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
	typedef shared_item<T> item;

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
