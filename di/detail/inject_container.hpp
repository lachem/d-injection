//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_INJECT_CONTAINER_HPP
#define DI_INJECT_CONTAINER_HPP

#include <di/detail/memory_pool.hpp>
#include <di/detail/spinlock.hpp>
#include <di/detail/lock_guard.hpp>
#include <di/configuration.hpp>

namespace di {
namespace detail {

template<typename T>
class inject_container {

	struct node {
		node() : injection(0), next(0) {}
		explicit node(node* a_next) : injection(0), next(a_next) {}

		inline bool is_in_range(char* address, size_t range) {
			char* inject_address = reinterpret_cast<char*>(injection);
			return (address <= inject_address && inject_address < address + range);
		}

		void* operator new(size_t size) {
			return mem_pool.malloc();
		}

		void operator delete(void* block) {
			mem_pool.free(block);
		}
	
		T** injection;
		node* next;

	private:
		static detail::memory_pool<DI_MAX_INJECTIONS_PER_TYPE> mem_pool;
	};

public:
	inline static void insert(T** injection) {
		#ifndef DI_NO_MULTITHREADING
		detail::lock_guard<detail::spinlock> guard(lock);
		#endif

		node* insert_node = new node();
		if(empty()) {
			tail = head_sentinel.next = insert_node;
		}
		else {
			tail = tail->next = insert_node;
		}
		tail->injection = injection;
	}

	inline static T** remove(char* address, size_t range) {
		#ifndef DI_NO_MULTITHREADING
		detail::lock_guard<detail::spinlock> guard(lock);
		#endif

		node* prev = &head_sentinel;
		node* curr = head_sentinel.next;
		while(curr != tail) {
			if(curr->is_in_range(address,range)) {
				T** injection = curr->injection;
				prev->next = curr->next;
				delete curr;
				return injection;
			}
			prev = curr;
			curr = curr->next;
		}
		if(curr->is_in_range(address,range)) {
			T** injection = curr->injection;
			tail = prev;
			prev->next = 0;
			delete curr;
			return injection;
		}
		return 0;
	}

private:
	inline static bool empty() {
		return head_sentinel.next == 0;
	}

private:
	static node head_sentinel;
	static node* tail;
	static detail::spinlock lock;
};

template<typename T>
typename inject_container<T>::node inject_container<T>::head_sentinel;

template<typename T>
typename inject_container<T>::node* inject_container<T>::tail = 0;

template<typename T>
detail::spinlock inject_container<T>::lock;

template<typename T>
detail::memory_pool<DI_MAX_INJECTIONS_PER_TYPE> inject_container<T>::node::mem_pool;

} //namespace detail
} //namespace di

#endif //DI_INJECT_CONTAINER_HPP
