//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_INJECTION_DESTINATION_CONTAINER_HPP
#define DI_INJECTION_DESTINATION_CONTAINER_HPP

#include <di/detail/memory_pool.hpp>
#include <di/detail/spinlock.hpp>
#include <di/detail/lock_guard.hpp>
#include <di/detail/utility.hpp>

namespace di {
namespace detail {

template<typename T>
class injection_destination_container {

	struct node {
		node() : item(), next(NULL) {}
		explicit node(const injection_destination<T>& an_item) : item(an_item), next(NULL) {}

		inline bool is_in_range(char* address, size_t range) {
			return item.is_in_range(address,range);
		}

		template<size_t arity> 
		static memory_pool<arity>& allocator() {
			static memory_pool<arity> mem_pool;
			return mem_pool;
		}

		void* operator new(size_t) {
			return allocator<sizeof(node)>().malloc();
		}

		void operator delete(void* block) {
			allocator<sizeof(node)>().free(block);
		}
	
		injection_destination<T> item;
		node* next;
	};

public:
	inline static size_t size() {
		#ifndef DI_NO_MULTITHREADING
		detail::lock_guard<detail::spinlock> guard(lock);
		#endif

		size_t num_nodes = 0;
		node* curr = head_sentinel.next;
		if (empty()) {
			return num_nodes;
		}
		while(curr != tail) {
			curr = curr->next;
			++num_nodes;
		}
		return num_nodes+1;
	}

	inline static void insert(const injection_destination<T>& item) {
		#ifndef DI_NO_MULTITHREADING
		detail::lock_guard<detail::spinlock> guard(lock);
		#endif

		node* insert_node = new node(item);
		if(empty()) {
			tail = head_sentinel.next = insert_node;
		}
		else {
			tail = tail->next = insert_node;
		}
	}

	inline static injection_destination<T> remove(char* address, size_t range) {
		assert(address);
		assert(range);

		#ifndef DI_NO_MULTITHREADING
		detail::lock_guard<detail::spinlock> guard(lock);
		#endif
 
		node* prev = &head_sentinel;
		node* curr = head_sentinel.next;
		if (empty()) {
			return head_sentinel.item;
		}
		while(curr != tail) {
			if(curr->is_in_range(address,range)) {
				injection_destination<T> item = curr->item;
				prev->next = curr->next;
				delete curr;
				return item;
			}
			prev = curr;
			curr = curr->next;
		}
		if(curr->is_in_range(address,range)) {
			injection_destination<T> item = curr->item;
			tail = prev;
			prev->next = NULL;
			delete curr;
			return item;
		}
		return head_sentinel.item;
	}

private:
	inline static bool empty() {
		return head_sentinel.next == NULL;
	}

private:
	static node head_sentinel;
	static node* tail;
	static detail::spinlock lock;
};

template<typename T>
typename injection_destination_container<T>::node 
	injection_destination_container<T>::head_sentinel;

template<typename T>
typename injection_destination_container<T>::node* 
	injection_destination_container<T>::tail = NULL;

template<typename T>
detail::spinlock injection_destination_container<T>::lock;

} //namespace detail
} //namespace di

#endif //DI_INJECTION_DESTINATION_CONTAINER_HPP
