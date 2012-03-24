//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_SUBJECT_HPP
#define DI_SUBJECT_HPP

#include <di/configuration.hpp>

#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>

#if FUSION_MAX_VECTOR_SIZE < DI_MAX_NUM_INJECTIONS
	#define FUSION_MAX_VECTOR_SIZE DI_MAX_NUM_INJECTIONS
#endif

#include <boost/fusion/include/as_vector.hpp>
#include <boost/fusion/adapted/mpl.hpp>
#include <boost/fusion/include/mpl.hpp>

#undef FUSION_MAX_VECTOR_SIZE

#if BOOST_MPL_LIMIT_VECTOR_SIZE < DI_MAX_NUM_INJECTIONS
	#ifndef BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
		#define BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
	#endif
	#define BOOST_MPL_LIMIT_VECTOR_SIZE DI_MAX_NUM_INJECTIONS
#endif

#if BOOST_MPL_LIMIT_SET_SIZE < DI_MAX_NUM_INJECTIONS
	#ifndef BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
		#define BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
	#endif
	#define BOOST_MPL_LIMIT_SET_SIZE DI_MAX_NUM_INJECTIONS
#endif

#include <boost/mpl/vector.hpp>
#include <boost/mpl/remove.hpp>
#include <boost/mpl/inserter.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/copy.hpp>
#include <boost/mpl/set.hpp>

#undef BOOST_MPL_LIMIT_VECTOR_SIZE
#undef BOOST_MPL_LIMIT_SET_SIZE

// Visual macro expansion CL /EP /C YourCodeFile.cpp 

namespace di {
namespace detail {

struct void_ {};

} // namespace detail

template <BOOST_PP_ENUM_BINARY_PARAMS(DI_MAX_NUM_INJECTIONS, typename T, =detail::void_ BOOST_PP_INTERCEPT)> \
class subject {
	typedef boost::mpl::vector< BOOST_PP_ENUM_BINARY_PARAMS(DI_MAX_NUM_INJECTIONS, T,*BOOST_PP_INTERCEPT) > raw_mpl_vector;
	typedef typename boost::mpl::remove<raw_mpl_vector, detail::void_*>::type mpl_vector;

public:
	typedef typename boost::fusion::result_of::as_vector<mpl_vector>::type type;
	virtual void constructed(){};
};

} // namespace di

#endif //DI_SUBJECT_HPP
