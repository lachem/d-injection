//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_INJECT_HPP
#define DI_INJECT_HPP

#include <boost/fusion/include/for_each.hpp>
#include "configuration.hpp"

namespace di {

template<typename T>
class inject {
public:
	inject() {
		addNode(this);
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

	T const& operator*() const {
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

	operator T const&() const {
		return *object;
	}

	//TODO synchronize
	inline static void addNode(inject<T>* injection) {
		list[tail++] = injection;
	}

	//TODO synchronize
	inline static T** removeFirstMatching(char* address, size_t range) {
		return (tail != head) ? &(list[head++]->object) : 0;
	}

	inline static void resetList() {
		head = tail = 0;
	}

private:
	static inline bool isObjectInRange(inject<T>* injection, char* address, size_t range) {
		char* injectAddress = reinterpret_cast<char*>(injection);
		return (injectAddress >= address && injectAddress < address + range);
	}

private:
	T* object;
	static unsigned int head;
	static unsigned int tail;
	static inject<T>* list[MAX_CONCURRENT_INJECTIONS];
};

template<typename T>
typename inject<T>* inject<T>::list[MAX_CONCURRENT_INJECTIONS];

template<typename T>
unsigned int inject<T>::head = 0;

template<typename T>
unsigned int inject<T>::tail = 0;

} //namspace di

#endif //DI_INJECT_HPP
