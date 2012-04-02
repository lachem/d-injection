//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_OPTIONAL_HPP
#define DI_OPTIONAL_HPP

#include <di/detail/specialized_injection.hpp>

#define DI_OTPIONAL_TYPE_ID 2

namespace di {

template<typename T,typename P = di::none>
struct optional : public detail::specialized_injection<T,P,DI_OTPIONAL_TYPE_ID>{	
	friend struct detail::perform_injection;
};

} //namspace di

#undef DI_OTPIONAL_TYPE_ID

#endif //DI_OPTIONAL_HPP
