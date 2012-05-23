//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_BUILDER_HPP
#define DI_BUILDER_HPP

#include <boost/fusion/include/for_each.hpp>
#include <boost/fusion/include/value_at.hpp>
#include <boost/mpl/contains.hpp>
#include <boost/mpl/size.hpp>
#include <di/detail/helpers.hpp>
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
class builder {	
public:
	builder() {
		//boost::fusion::for_each(injections,detail::set_null());
	}

	virtual T* build() const = 0;
	virtual void delegate(T&) const = 0;
	
	template<typename U>
	builder<T>& use(U& object) {
		return this->use(ordinary<U>(&object));
	}

	template<template <typename> class SPtr, typename U>
	builder<T>& use(const SPtr<U>& object) {
		BOOST_STATIC_ASSERT((
			boost::is_same< SPtr<U>,unique<U> >::value || 
			boost::is_same< SPtr<U>,shared<U> >::value || 
			boost::is_same< SPtr<U>,ordinary<U> >::value));
		
		do_usage<const SPtr<U> >(object);
		return *this;
	}

	template<typename U>
	builder<T>& replace(U& object, int at=0) {
		return this->replace(di::ordinary<U>(&object),at);
	}

	template<template <typename> class SPtr, typename U>
	builder<T>& replace(const SPtr<U>& object, int at=0) {
		BOOST_STATIC_ASSERT((
			boost::is_same< SPtr<U>,unique<U> >::value || 
			boost::is_same< SPtr<U>,shared<U> >::value || 
			boost::is_same< SPtr<U>,ordinary<U> >::value));

		do_replacement<const SPtr<U> >(object,at);
		return *this;
	}

private:
	template<typename U>
	void do_usage(U& object) {
		BOOST_STATIC_ASSERT((boost::mpl::contains<typename T::type,detail::injection_source<typename U::type> >::type::value));
		bool use_succeeded = false;
		boost::fusion::for_each(injections,
			detail::set_next_same_type<U>(&object,&use_succeeded));
		if(!use_succeeded) {
			out_of_bounds();
		}
	}
	
	template<typename U>
	void do_replacement(U& object, int at) {
		BOOST_STATIC_ASSERT((boost::mpl::contains<typename T::type,detail::injection_source<typename U::type> >::type::value));
		bool replace_succeeded = false;
		boost::fusion::for_each(injections,
			detail::set_nth_same_type<U>(&object,at,&replace_succeeded));
		if(!replace_succeeded) {
			out_of_bounds();
		}
	}

	virtual void out_of_bounds() = 0;

protected:
	typename T::type injections;
};

} //namspace di

#endif //DI_BUILDER_HPP

