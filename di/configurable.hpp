//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_CONFIGURABLE_HPP
#define DI_CONFIGURABLE_HPP

#include <di/detail/variadics.hpp>
#include <di/detail/utility.hpp>
#include <di/configuration.hpp>
#include <exception>
#include <sstream>
#include <string>

namespace di {

struct out_of_range : public std::exception {
	~out_of_range() throw () {}
	
	virtual const char* what() const throw() {
		return "Builder cannot handle any more injections of given type";
	}
};

struct requirement_not_satisfied : public std::exception {
	requirement_not_satisfied(void* subject_address) {
		std::stringstream sstream;
		sstream << "Builder has failed to satisfy all requirements of subject at " << subject_address;
		message = sstream.str();
	}

	~requirement_not_satisfied() throw() {}

	virtual const char* what() const throw() {
		return message.c_str();
	}

private:
	std::string message;
};

template<typename T>
class configurable {	

	BOOST_MPL_ASSERT_MSG((boost::is_base_of<detail::subject_base,T>::value),ProvidedTypeDoesNotDeriveFromSubject,);

public:
	typedef T subject;

	template<typename U>
	configurable<T>& use(U& object) {
		return this->use(ordinary<U>(&object));
	}

	/**
	 * @brief Inserts object at first unoccupied position matching given type.
	 * @param object Reference to injection type.
	 * @pre Injection's type is known.
	 * @pre Injection contents are not null.
	 * @post Injection has been saved inside the container.
	 */
	template<template <typename> class SPtr, typename U>
	configurable<T>& use(const SPtr<U>& object) {
		BOOST_MPL_ASSERT_MSG((
			boost::is_same< SPtr<U>,unique<U> >::value || 
			boost::is_same< SPtr<U>,shared<U> >::value || 
			boost::is_same< SPtr<U>,ordinary<U> >::value ||
			boost::is_same< SPtr<U>,service<U> >::value),
			ProvidedTypeIsUnsupported,);
		assert(object.object);

		do_usage(object);
		return *this;
	}

	template<typename U>
	configurable<T>& replace(U& object, size_t at=0) {
		return this->replace(di::ordinary<U>(&object),at);
	}

	/**
	 * @brief Replaces nth injection of given type.
	 * @param object Reference to injection type.
	 * @param at Position at which the replacement is to be performed in terms of given type.
	 * @pre Injection's type is known.
	 * @pre Injection contents are not null.
	 * @post Injection has been saved inside the container.
	 */
	template<template <typename> class SPtr, typename U>
	configurable<T>& replace(const SPtr<U>& object, size_t at=0) {
		BOOST_MPL_ASSERT_MSG((
			boost::is_same< SPtr<U>,unique<U> >::value || 
			boost::is_same< SPtr<U>,shared<U> >::value || 
			boost::is_same< SPtr<U>,ordinary<U> >::value ||
			boost::is_same< SPtr<U>,service<U> >::value),
			ProvidedTypeIsUnsupported,);
		
		do_replacement(object, at);
		return *this;
	}

	/**
	 * @brief Removes nth injection of given type, where n == at.
	 * @param at Position at which the removal is to be performed.
	 * @post Injection of requested type has been removed.
	 */
	template<typename U>
	configurable<T>& remove(size_t at=0) {
		do_removal<U>(at);
		return *this;
	}

private:
	template<typename U>
	void do_usage(U& object) {
		BOOST_MPL_ASSERT_MSG((
			boost::mpl::contains<typename T::raw_type, typename U::type>::type::value),WrongTypeProvided,);

		typedef detail::injection_source_container<typename U::type,
			boost::mpl::count<typename T::raw_type, typename U::type>::type::value> holder;
		holder& element = boost::fusion::at_key<holder>(injections);
		if(!element.push(object)) {
			out_of_bounds();
		}
	}
	
	template<typename U>
	void do_replacement(U& object, size_t at) {
		BOOST_MPL_ASSERT_MSG((
			boost::mpl::contains<typename T::raw_type, typename U::type>::type::value),WrongTypeProvided,);
		
		typedef detail::injection_source_container<typename U::type,
			boost::mpl::count<typename T::raw_type, typename U::type>::type::value> holder;
		holder& element = boost::fusion::at_key<holder>(injections);
		if(!element.replace(object,at)) {
			out_of_bounds();
		}
	}

	template<typename U>
	void do_removal(size_t at) {
		BOOST_MPL_ASSERT_MSG((
			boost::mpl::contains<typename T::raw_type, U>::type::value),WrongTypeProvided,);
		
		typedef detail::injection_source_container<U,
			boost::mpl::count<typename T::raw_type, U>::type::value> holder;
		holder& element = boost::fusion::at_key<holder>(injections);
		if(!element.remove(at)) {
			out_of_bounds();
		}
	}

	virtual void out_of_bounds() {
		T::diagnostics::out_of_bounds();
	}

protected:
	typename T::type injections;
};

} //namspace di

#endif //DI_CONFIGURABLE_HPP

