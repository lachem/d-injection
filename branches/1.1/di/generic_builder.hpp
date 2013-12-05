//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_GENERIC_BUILDER_HPP
#define DI_GENERIC_BUILDER_HPP

#include <di/detail/perform_injection.hpp>
#include <di/configurable.hpp>

namespace di {

/**
 * Generic Builder is a convient facility to build objects with injections. It does not require the subject to be
 * of any specific type, but to define which injections are to be handled by generic_builder. Therefore using 
 * generic_builder< subject<T1,T2> > is perfectly fine. The drawback of generic_builder is that it does not support
 * abstraction i.e. build and build_part methods cannot be mocked nor can build or build_part be used to perform injections
 * through interfaces.
 */
template<typename S>
class generic_builder : public di::configurable<S> {
public:
	/**
	 * @brief Allows building of any object that needs injections specified by generic_builder template type parameter
	 * @pre C is a type that contains injections specified by template S
	 * @pre generic_builder has been configured with injections of instance that were not yet satisfied
	 * @post All injections which where provided to generic_builder are injectied into instance
	 * @post constructed has been called on instance
	 * @param instance Instance of type C that needs injections specified by template S
     * @throw requirement_not_satisfied Depending on diagnostics method chosen.
	 */
	template<typename C>
	void build(C& instance) {
		bool succeeded = build_inject(&instance);
		if(!succeeded) {
			S::diagnostics::delegate_unsatisfied_requirement(&instance);
		}
		instance.constructed();
	}

	/**
	 * @brief Allows building part of an object. What follows is that neither requirements are checked nor 
	 * constructed is called.
	 * @pre C is a type that contains injections specified by template S
	 * @pre generic_builder has been configured with injections of instance that were not yet satisfied
	 * @post All injections which where provided to generic_builder are injectied into instance
	 * @param instance Instance of type C that needs injections specified by template S
	 */
	template<typename C>
	void build_part(C& instance) {
		build_part_inject(&instance);
	}

    /**
     * @see di::configurable::use
     */
	template<typename U>
	generic_builder<S>& use(U& object) {
		(void) di::configurable<S>::use(object);
		return *this;
	}

    /**
     * @see di::configurable::use
     */
	template<template <typename> class SPtr, typename U>
	generic_builder<S>& use(const SPtr<U>& object) {
		(void) di::configurable<S>::use(object);
		return *this;
	}

    /**
     * @see di::configurable::replace
     */
	template<typename U>
	generic_builder<S>& replace(U& object, size_t at=0) {
		(void) di::configurable<S>::replace(object,at);
		return *this;
	}

    /**
     * @see di::configurable::replace
     */
	template<template <typename> class SPtr, typename U>
	generic_builder<S>& replace(const SPtr<U>& object, size_t at=0) {
		(void) di::configurable<S>::replace(object,at);
		return *this;
	}

    /**
     * @see di::configurable::remove
     */
	template<typename U>
	generic_builder<S>& remove(size_t at=0) {
		(void) di::configurable<S>::template remove<U>(at);
		return *this;
	}

private:
	template<typename C>
	bool build_inject(C* instance) {
		bool succeeded = true;
		boost::fusion::for_each(di::configurable<S>::injections,
			di::detail::perform_injection<C>(instance,&succeeded));
		
		return succeeded;
	}

	template<typename C>
	void build_part_inject(C* instance) {
		boost::fusion::for_each(di::configurable<S>::injections,
			di::detail::perform_injection<C>(instance));
	}
};

} //namspace di

#endif //DI_GENERIC_BUILDER_HPP

