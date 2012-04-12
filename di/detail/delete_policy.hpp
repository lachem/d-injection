//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_DELETE_POLICY_HPP
#define DI_DELETE_POLICY_HPP

namespace di {

template<typename T>
struct unique{
	explicit unique(T* an_object) : object(an_object) {}
	T* object;
};

template<typename T>
struct shared{
	explicit shared(T* an_object) : object(an_object) {}
	T* object;
};

} // namespace di

#endif //DI_DELETE_POLICY_HPP
