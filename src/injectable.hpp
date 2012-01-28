//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef INJECTABLE_HPP_
#define INJECTABLE_HPP_

#include <boost/fusion/include/vector.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/type_traits/is_same.hpp>
#include "slot.hpp"
#include "generator.hpp"

#define BOOST_PP_LOCAL_LIMITS (1, MAX_NUM_INJECTIONS)
#include BOOST_PP_LOCAL_ITERATE()

GENERATE_INJECTABLE(MAX_NUM_INJECTIONS)

/*template<typename T1>
class Injectable1 {
public:
	typedef boost::fusion::vector< Slot<T1> > type;
};

template<typename T1,typename T2>
class Injectable2 {
public:
	typedef boost::fusion::vector< Slot<T1>,Slot<T2> > type;
};

template<typename T1,typename T2, typename T3>
class Injectable3 {
public:
	typedef boost::fusion::vector< Slot<T1>,Slot<T2>,Slot<T3> > type;
};


template<typename T1, typename T2=void, typename T3=void>
class Injectable {
public:
	typedef typename boost::mpl::eval_if_c<boost::is_same<T2,void>::value,
				Injectable1<T1>,
				 	typename boost::mpl::eval_if_c<boost::is_same<T3,void>::value,
						Injectable2<T1,T2>,
						Injectable3<T1,T2,T3>
					>
				>::type type;
	static type injections;
};

template<typename T1, typename T2, typename T3>
typename Injectable<T1,T2,T3>::type Injectable<T1,T2,T3>::injections;*/

#endif
