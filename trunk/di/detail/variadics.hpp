//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_VARIADICS_HPP
#define DI_VARIADICS_HPP

#include <di/configuration.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>

#if FUSION_MAX_VECTOR_SIZE < DI_MAX_NUM_INJECTIONS
	#define FUSION_MAX_VECTOR_SIZE DI_MAX_NUM_INJECTIONS
#endif

#if FUSION_MAX_SET_SIZE < DI_MAX_NUM_INJECTIONS
	#define FUSION_MAX_VECTOR_SIZE DI_MAX_NUM_INJECTIONS
#endif

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

#if BOOST_MPL_LIMIT_METAFUNCTION_ARITY < DI_MAX_NUM_INJECTIONS
	#ifndef BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
		#define BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
	#endif
	#define BOOST_MPL_LIMIT_METAFUNCTION_ARITY DI_MAX_NUM_INJECTIONS
#endif

#include <boost/fusion/include/as_vector.hpp>
#include <boost/fusion/include/as_set.hpp>
#include <boost/fusion/adapted/mpl.hpp>
#include <boost/fusion/include/mpl.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <boost/fusion/include/at_key.hpp>

#include <boost/mpl/vector.hpp>
#include <boost/mpl/remove.hpp>
#include <boost/mpl/inserter.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/count.hpp>
#include <boost/mpl/copy.hpp>
#include <boost/mpl/set.hpp>
#include <boost/mpl/inherit.hpp>
#include <boost/mpl/joint_view.hpp>
#include <boost/mpl/contains.hpp>
#include <boost/mpl/size.hpp>

#undef FUSION_MAX_VECTOR_SIZE
#undef BOOST_MPL_LIMIT_VECTOR_SIZE
#undef BOOST_MPL_LIMIT_SET_SIZE
#undef BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
//#undef BOOST_MPL_LIMIT_METAFUNCTION_ARITY

namespace di {
namespace detail {
	
struct void_ {};

} // namespace detail
} // namespace di

#endif //DI_VARIADICS_HPP
