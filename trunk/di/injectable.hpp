//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_INJECTABLE_HPP
#define DI_INJECTABLE_HPP

#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>
#include <boost/preprocessor/iteration/local.hpp>
#include <boost/preprocessor/cat.hpp>

#include <boost/fusion/include/vector.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/eval_if.hpp>

#include <di/configuration.hpp>

// Visual macro expansion CL /EP /C YourCodeFile.cpp 

namespace di {

//GENERATE THE LIST OF INJECTABLES I.E. INJECTABLE1, INJECTABLE2 etc.

#define BOOST_PP_LOCAL_MACRO(N) \
template < BOOST_PP_ENUM_BINARY_PARAMS(N, typename T, BOOST_PP_INTERCEPT)> \
struct injectable##N{ \
	typedef boost::fusion::vector<BOOST_PP_ENUM_BINARY_PARAMS(N, T,*BOOST_PP_INTERCEPT) > type; \
};

#define BOOST_PP_LOCAL_LIMITS (1, DI_MAX_NUM_INJECTIONS)
#include BOOST_PP_LOCAL_ITERATE()

//GENERATE THE MAIN INJECTABLE WITH DEFAULT TEMPLATE PARAMETERS

struct ERROR_AT_LEAST_ONE_INJECTION_TYPE_IS_REQUIRED{};

#define GENERATE_INJECTABLE(N) \
template <BOOST_PP_ENUM_BINARY_PARAMS(N, typename T, =void BOOST_PP_INTERCEPT)> \
struct injectable{ \
	typedef BOOST_PP_REPEAT(DI_MAX_NUM_INJECTIONS, EVAL_IF_IS_SAME,) \
		BOOST_PP_CAT(injectable,N)<BOOST_PP_ENUM_PARAMS(DI_MAX_NUM_INJECTIONS, T)> \
		BOOST_PP_REPEAT(DI_MAX_NUM_INJECTIONS, TEMPLATE_END,)\
		::type type; \
};

#define EVAL_IF_IS_SAME(z, n, data) \
	typename boost::mpl::eval_if_c<boost::is_same<T##n,void>::value,\
		BOOST_PP_IF(n, DECLARE_INJECTABLE, DECLARE_ERROR)(n),

#define DECLARE_INJECTABLE(n) \
	injectable##n<BOOST_PP_ENUM_PARAMS(n, T)>

#define DECLARE_ERROR(n) \
	ERROR_AT_LEAST_ONE_INJECTION_TYPE_IS_REQUIRED

#define TEMPLATE_END(z, n, data) >

GENERATE_INJECTABLE(DI_MAX_NUM_INJECTIONS)

} //namspace di

#endif //DI_INJECTABLE_HPP
