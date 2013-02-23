//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_SUBJECT_HPP
#define DI_SUBJECT_HPP

#include <di/detail/variadics.hpp>
#include <di/detail/injection_source_container.hpp>
#include <di/detail/utility.hpp>
#include <di/diagnostics.hpp>

// Visual macro expansion CL /EP /C YourCodeFile.cpp 

namespace di {
namespace detail {

template<typename Seq>
struct make_injection_source {
	template<typename T>
	struct apply {
		typedef injection_source_container<T,boost::mpl::count<Seq,T>::type::value> type;
	};
};

template<typename Seq>
struct vector_of_injection_sources {
	typedef typename boost::mpl::transform<Seq, detail::make_injection_source<Seq> >::type type;
};

template<typename Seq>
struct set_of_unique_types {
	typedef typename boost::mpl::copy<
		Seq, boost::mpl::inserter< boost::mpl::set< >, boost::mpl::insert<boost::mpl::_1,boost::mpl::_2 > >
	>::type type;
};

} // namespace detail

/**
 * Subject is a base class for all classes that are inteded do have objects injected to it.
 * All injection types are to be listed in subject's variadic template parameter list. To support
 * multiple instances of the same types a number of operations on the list of types are performed.
 * In result a list of for example <T,T,Y,Z,Y> is transformed into <<T,2>,<T,2>,<Z>>.
 */
template <BOOST_PP_ENUM_BINARY_PARAMS(DI_MAX_NUM_INJECTIONS, typename T, =detail::void_ BOOST_PP_INTERCEPT)> \
class subject : public detail::subject_base {	
	typedef boost::mpl::vector< BOOST_PP_ENUM_BINARY_PARAMS(DI_MAX_NUM_INJECTIONS, T,BOOST_PP_INTERCEPT) > raw_mpl_vector;
	typedef typename detail::vector_without_voids<raw_mpl_vector>::type trimmed_vector;
	typedef typename detail::vector_of_injection_sources<trimmed_vector>::type source_vector;
	typedef typename detail::set_of_unique_types<source_vector>::type unique_set;

protected:
	typedef subject<BOOST_PP_ENUM_BINARY_PARAMS(DI_MAX_NUM_INJECTIONS, T,BOOST_PP_INTERCEPT)> subject_type;

public:
	typedef trimmed_vector raw_type;
	typedef typename boost::fusion::result_of::as_set<unique_set>::type type;
	typedef di::using_assertions<subject_type> diagnostics;
	
	/**
	 * @brief Classes with injections derive publicly from subject, therefore virtual destructor is required. 
	 */
	virtual ~subject() {}

	/**
	 * @brief There are cases when a user wants could invoke actions on injected objects right after the injection
	 *        procedure has finished. Overriding this method provides such possiblity. Each builder calls this
	 *        method after successfuly performing injections.
	 */
	virtual void constructed() {}
};

} // namespace di

#endif //DI_SUBJECT_HPP
