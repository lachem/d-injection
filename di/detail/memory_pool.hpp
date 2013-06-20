//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_ALLOCATOR_HPP
#define DI_ALLOCATOR_HPP

#include <di/configuration.hpp>
#include <cassert>

namespace di {
namespace detail {

template<size_t size, size_t capacity = DI_MAX_INJECTIONS_PER_TYPE>
class memory_pool {
private:
	mem_block<size>* available_mem;
	mem_block<size>* stack[capacity];
	unsigned int head;

public:
	memory_pool() {
		available_mem = new mem_block<size>[capacity];
		head = capacity;
		for(int i=0; i<capacity; ++i) {
			stack[i] = available_mem + i;
		}
	}

	~memory_pool() {
		delete [] available_mem;
	}

	void* malloc() {
		return empty() ? ::malloc(sizeof(mem_block<size>)) : stack[--head];
	}

	void free(void* block) {
		assert(0 != block);
		if(owns(block)) {
			stack[head++] = reinterpret_cast<mem_block<size>*>(block);
		}
		else {
			::free(block);
		}
	}

private:

	bool empty() {
		return 0 == head;
	}

	bool owns(void* block) {
		return (available_mem <= block) && (block < available_mem + capacity);
	}	
};

} //namespace detail
} //namespace di

#endif //DI_ALLOCATOR_HPP
