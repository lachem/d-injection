//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef INJECT_HPP
#define INJECT_HPP

#include <boost/fusion/include/for_each.hpp>
#include <helpers.hpp>

template<typename T, typename U>
class Inject {
public:
	Inject() {
		boost::fusion::for_each(U::injections,get_next_same_type<T>(&object));
	}

	T* operator->() {
		return object;	
	}

	T& operator*() {
		return *object;
	}

	T const* operator->() const {
		return object;
	}

	const T& operator*() const {
		return *object;
	}

	operator T*() {
		return object;
	}

	operator T const*() const {
		return object;
	}

	operator T&() {
		return *object;
	}

	operator const T&() const {
		return *object;
	}

private:
	T* object;
};

#endif //INJECT_HPP
