//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_INJECT_CONTAINER_HPP
#define DI_INJECT_CONTAINER_HPP

#include <list>

#include <di/detail/memory_pool.hpp>
#include <di/detail/spin_lock.hpp>
#include <di/detail/lock_guard.hpp>
#include <di/configuration.hpp>

namespace di {
namespace detail {

template<typename T>
class inject_container {

	struct node {
		node() : injection(0), next(0) {}

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
		static detail::memory_pool<MAX_INJECTIONS_PER_TYPE> mem_pool;
	};

public:
	inline static void insert(T** injection) {
		detail::lock_guard<detail::spin_lock> guard(lock);
		if(0 == head) {
			tail = (head = new node());
		}
		else {
			tail = (tail->next = new node());
		}
		tail->injection = injection;
	}

	inline static T** remove(char* address, size_t range) {
		detail::lock_guard<detail::spin_lock> guard(lock);
		if(0 != head) {
			if(head->is_in_range(address,range)) {
				T** injection = head->injection;
				node* tmp = head;
				head = head->next;
				delete tmp;
				return injection;
			}
			node* prev = head;
			node* curr = head->next;
			while(curr) {
				if(curr->is_in_range(address,range)) {
					T** injection = curr->injection;
					prev->next = curr->next;
					delete curr;
					return injection;
				}
				curr = curr->next;
			}
		}
		return 0;
	}

private:
	static node* head;
	static node* tail;
	static detail::spin_lock lock;
};

template<typename T>
typename inject_container<T>::node* inject_container<T>::head = 0;

template<typename T>
typename inject_container<T>::node* inject_container<T>::tail = 0;

template<typename T>
detail::spin_lock inject_container<T>::lock;

template<typename T>
detail::memory_pool<MAX_NUM_INJECTIONS> inject_container<T>::node::mem_pool;

} //namespace detail
} //namespace di

#endif //DI_INJECT_CONTAINER_HPP
