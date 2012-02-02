//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_INJECTABLE_HPP_
#define DI_INJECTABLE_HPP_

#include <boost/fusion/include/vector.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/type_traits/is_same.hpp>
#include "generator.hpp"
#include "configuration.hpp"

namespace di {

#define BOOST_PP_LOCAL_LIMITS (1, MAX_NUM_INJECTIONS)
#include BOOST_PP_LOCAL_ITERATE()

GENERATE_INJECTABLE(MAX_NUM_INJECTIONS)

} //namspace di

#endif
