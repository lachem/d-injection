//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_SERVICE_LIST_HPP
#define DI_SERVICE_LIST_HPP

#include <di/detail/variadics.hpp>

namespace di {
namespace detail {

struct wrap_in_ptr {
	template<typename T>
	struct apply {
		typedef T* type;
	};
};

template<typename Seq>
struct add_pointers {
	typedef typename boost::mpl::transform<Seq, detail::wrap_in_ptr>::type type;
};

} // namespace detail

template <BOOST_PP_ENUM_BINARY_PARAMS(DI_MAX_NUM_INJECTIONS, typename M, =detail::void_ BOOST_PP_INTERCEPT)>
struct service_list {
private:
	typedef boost::mpl::vector< BOOST_PP_ENUM_BINARY_PARAMS(DI_MAX_NUM_INJECTIONS, M,* BOOST_PP_INTERCEPT) > raw_types;
	typedef typename boost::mpl::remove<raw_types, detail::void_*>::type trimmed_types;
	typedef typename detail::add_pointers<trimmed_types>::type trimmed_types_with_pointers;
public:
	typedef typename boost::fusion::result_of::as_set<trimmed_types>::type type;
	typedef typename boost::fusion::result_of::as_set<trimmed_types_with_pointers>::type ref_type;
};

} //namspace di

#endif //DI_SERVICE_LIST_HPP
