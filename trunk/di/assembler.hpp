//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_ASSEMBLER_HPP
#define DI_ASSEMBLER_HPP

#include <boost/mpl/joint_view.hpp>
#include <di/detail/variadics.hpp>
#include <di/module.hpp>

namespace di {
namespace detail {

struct wrap_with_ptr {
	template<typename T>
	struct apply {
		typedef T* type;
	};
};

template<typename Seq>
struct add_ptrs {
	typedef typename boost::mpl::transform<Seq, detail::wrap_with_ptr>::type type;
};

} // namespace detail

template <BOOST_PP_ENUM_BINARY_PARAMS(DI_MAX_NUM_INJECTIONS, typename T, =detail::void_ BOOST_PP_INTERCEPT)> 
struct services {
private:
	typedef boost::mpl::vector< BOOST_PP_ENUM_BINARY_PARAMS(DI_MAX_NUM_INJECTIONS, T,* BOOST_PP_INTERCEPT) > raw_types;
	typedef typename boost::mpl::remove<raw_types, detail::void_*>::type trimmed_types;
	typedef typename detail::add_ptrs<trimmed_types>::type trimmed_ptr_types;
public:
	typedef typename boost::fusion::result_of::as_set<trimmed_types>::type type;
	typedef typename boost::fusion::result_of::as_set<trimmed_ptr_types>::type ref_type;
};

template<typename T0, typename T1>
struct assembler : public di::module<T0>, public di::module<T1> {
	assembler() : 
		di::module<T0>(provided_by_modules), 
		di::module<T1>(provided_by_modules) {}

private:
	typename boost::fusion::result_of::as_set<
		typename boost::mpl::joint_view<typename T0::provided::type, typename T1::provided::type>::type
	>::type provided_by_modules;
};

} //namspace di

#endif //DI_ASSEMBLER_HPP
