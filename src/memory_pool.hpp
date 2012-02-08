//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_ALLOCATOR_HPP
#define DI_ALLOCATOR_HPP

#include <cassert>

namespace di {
namespace detail {

template<size_t size = INJECTIONS_MEMPOOL_SIZE>
class memory_pool {
	struct mem_block{void* p1; void* p2;};
	
	static mem_block available_mem[size];
	static mem_block* stack[size];
	static unsigned int head;

public:
	memory_pool() : head(size) {
		for(int i=0; i<size; ++i) {
			stack[i] = available_mem[i];
		}
	}

	static void* malloc() {
		return empty() ? ::malloc(size) : stack[--head] ;
	}

	static void free(void* block) {
		if(owns(block)) {
			stack[head++] = reinterpret_cast<mem_block*>(block);
		}
		else {
			::free(block);
		}
	}

	static bool empty() {
		return head == 0;
	}

	static bool owns(void* block) {
		return (available_mem >= block) && (block < available_mem + size);
	}	
};

template<size_t size>
typename memory_pool<size>::mem_block memory_pool<size>::available_mem[size];

template<size_t size>
typename memory_pool<size>::mem_block* memory_pool<size>::stack[size];

template<size_t size>
unsigned int memory_pool<size>::head;

} //namespace detail
} //namespace di

#endif //DI_ALLOCATOR_HPP
