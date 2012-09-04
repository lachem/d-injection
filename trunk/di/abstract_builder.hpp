//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_ABSTRACT_BUILDER_HPP
#define DI_ABSTRACT_BUILDER_HPP

#include <di/configurable.hpp>

namespace di {
	
/**
 * @brief abstract_builder provides convinient abstraction for building objects of abstract or concrete types.
 * The main advantage of using abstract_builder is the possibility to mock build methods. If the user does not 
 * need mocking or can test building using other means (checking injections directly) then using more convinient 
 * generic_builder is advised.
 */
template<typename T>
class abstract_builder : public di::configurable<T> {	
public:
	typedef T subject;

	/**
	 * @brief Creates the object of type C, performs injections and calls constructed() on subject.
	 * @pre Injections required by the object under construction were provided to the builder.
	 * @post All provided injections have been injected, subject<T...>::constucted() has been called.
	 * @return New instance of subject type.
	 */
	virtual T* build() = 0;

	/**
	 * @brief Performs injections and calls constructed() on subject.
	 * @pre Injections required by the instance object were provided to the builder.
	 * @post All provided injections have been injected, subject<T...>::constucted() has been called.
	 * @param Instance of builder's corresponding subject.
	 */
	virtual void build(T&) = 0;

	template<typename U>
	abstract_builder<T>& use(U& object) {
		(void) di::configurable<T>::use(object);
		return *this;
	}

	template<template <typename> class SPtr, typename U>
	abstract_builder<T>& use(const SPtr<U>& object) {
		(void) di::configurable<T>::use(object);
		return *this;
	}

	template<typename U>
	abstract_builder<T>& replace(U& object, size_t at=0) {
		(void) di::configurable<T>::replace(object,at);
		return *this;
	}

	template<template <typename> class SPtr, typename U>
	abstract_builder<T>& replace(const SPtr<U>& object, size_t at=0) {
		(void) di::configurable<T>::replace(object,at);
		return *this;
	}

	template<typename U>
	abstract_builder<T>& remove(size_t at=0) {
		(void) di::configurable<T>::template remove<U>(at);
		return *this;
	}
};

} //namspace di

#endif //DI_ABSTRACT_BUILDER_HPP

