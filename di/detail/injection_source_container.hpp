//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_INJECTION_SOURCE_CONTAINER_HPP
#define DI_INJECTION_SOURCE_CONTAINER_HPP

#include <di/detail/injection_source.hpp>
#include <di/detail/utility.hpp>
#include <di/injection_types.hpp>

namespace di {
namespace detail {

template<typename T, size_t capacity>
struct injection_source_container : di::detail::noncopyable {
private:
	enum internal{max_elem_size = sizeof(injection_source_imp< di::shared<T> >)};

public:
	struct const_iterator {
		const_iterator(injection_source<T>* const * element) : current(element) {}
		
		const_iterator operator+(size_t num) {
			return const_iterator(current+num);
		}
		const_iterator operator-(size_t num) {
			return const_iterator(current-num);
		}
		const_iterator& operator++() {
			++current;
			return *this;
		}
		const_iterator& operator--() {
			--current;
			return *this;
		}
		injection_source<T>* const operator*() {
			return *current;
		}
		injection_source<T>* const operator->() {
			return *current;
		}
		bool operator==(const const_iterator& it) {
			return current == it.current;
		}

		bool operator!=(const const_iterator& it) {
			return current != it.current;
		}
	private:
		injection_source<T>* const * current;
	};

	typedef injection_source<T> value_type;
	enum {size = capacity};

	injection_source_container() : current(0) {
		for(int i=0; i<capacity; ++i) {
			elements[i] = NULL;
		}
	}

	~injection_source_container() {
		for(int i=0; i<capacity; ++i) {
			if(elements[i]) {
				elements[i]->~injection_source();
			}
		}
	}

	const_iterator begin() const {
		return const_iterator(elements);
	}
	const_iterator end() const {
		return const_iterator(elements+capacity);
	}

	template<template <typename> class SPtr, typename U>
	bool push(const SPtr<U>& elem) {
		assert(elem.object);

		if(current >= capacity) {
			return false;
		}
		elements[current] = new (memory + current) detail::injection_source_imp< SPtr<U> >(elem);
		while(++current != capacity && elements[current]) {}
		return true;
	}

	template<template <typename> class SPtr, typename U>
	bool replace(const SPtr<U>& elem, size_t at) {
		assert(elem.object);

		if(at >= capacity) {
			return false;
		}
		if(elements[at]) {
			elements[at]->~injection_source();
		}
		elements[at] = new (memory + at) detail::injection_source_imp< SPtr<U> >(elem);
		return true;
	}

	bool remove(size_t at) {
		if(at >= capacity) {
			return false;
		}
		if(elements[at]) {
			elements[at]->~injection_source();
			elements[at] = NULL;
			if(current>at) {
				current = at;
			}
		}
		return true;
	}
	
private:
	injection_source<T>* elements[capacity];
	size_t current;
	
	mem_block<max_elem_size> memory[capacity];
};

} //namspace detail
} //namspace di

#endif //DI_INJECTION_SOURCE_CONTAINER_HPP
