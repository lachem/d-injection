//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_ALLOCATOR_HPP
#define DI_ALLOCATOR_HPP

#include <cassert>

namespace di {
namespace detail {

template<size_t size>
class memory_pool {
private:
	struct mem_block{void* p1; void* p2;};
	
private:
	mem_block* available_mem;
	mem_block* stack[size];
	unsigned int head;

public:
	memory_pool() {
		available_mem = new mem_block[size];
		head = size;
		for(int i=0; i<size; ++i) {
			stack[i] = available_mem + i;
		}
	}

	void* malloc() {
		return empty() ? ::malloc(sizeof(mem_block)) : stack[--head];
	}

	void free(void* block) {
		assert(0 != block);
		if(owns(block)) {
			stack[head++] = reinterpret_cast<mem_block*>(block);
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
		return (available_mem <= block) && (block < available_mem + size);
	}	
};

} //namespace detail
} //namespace di

#endif //DI_ALLOCATOR_HPP
