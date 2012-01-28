//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// Visual macro expansion CL /EP /C YourCodeFile.cpp 

#ifndef GENERATOR_HPP_
#define GENERATOR_HPP_

#include <boost/preprocessor.hpp>

#ifndef MAX_NUM_INJECTIONS
#define MAX_NUM_INJECTIONS 10
#endif

//GENERATE THE LIST OF INJECTABLES I.E. INJECABLE1, INJECTABLE2 etc.

#define BOOST_PP_LOCAL_MACRO(N) \
template < BOOST_PP_ENUM_BINARY_PARAMS(N, typename T, BOOST_PP_INTERCEPT)> \
struct Injectable##N{ \
	typedef boost::fusion::vector<BOOST_PP_ENUM_BINARY_PARAMS(N, Slot<T,>BOOST_PP_INTERCEPT) > type; \
};

//GENERATE THE MAIN INJECTABLE WITH DEFAULT TEMPLATE PARAMETERS

struct ERROR_AT_LEAST_ONE_INJECTION_TYPE_IS_REQUIRED{};

#define GENERATE_INJECTABLE(N) \
template <BOOST_PP_ENUM_BINARY_PARAMS(N, typename T, =void BOOST_PP_INTERCEPT)> \
struct Injectable{ \
	typedef BOOST_PP_REPEAT(MAX_NUM_INJECTIONS, EVAL_IF_IS_SAME,) \
		Injectable<BOOST_PP_ENUM_PARAMS(MAX_NUM_INJECTIONS, T)> \
		BOOST_PP_REPEAT(MAX_NUM_INJECTIONS, TEMPLATE_END,)\
		::type type; \
	static type injections; \
}; \
template<BOOST_PP_ENUM_PARAMS(MAX_NUM_INJECTIONS, typename T)> \
typename Injectable<BOOST_PP_ENUM_PARAMS(MAX_NUM_INJECTIONS, T)>::type \
Injectable<BOOST_PP_ENUM_PARAMS(MAX_NUM_INJECTIONS, T)>::injections;

#define EVAL_IF_IS_SAME(z, n, data) \
	typename boost::mpl::eval_if_c<boost::is_same<T##n,void>::value,\
		BOOST_PP_IF(n, DECLARE_INJECTABLE, DECLARE_ERROR)(n),

#define DECLARE_INJECTABLE(n) \
	Injectable##n<BOOST_PP_ENUM_PARAMS(n, T)>

#define DECLARE_ERROR(n) \
	ERROR_AT_LEAST_ONE_INJECTION_TYPE_IS_REQUIRED

#define TEMPLATE_END(z, n, data) >


#endif // GENERATOR_HPP_
