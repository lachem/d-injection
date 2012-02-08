//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_INJECT_HPP
#define DI_INJECT_HPP

#include <boost/fusion/include/for_each.hpp>
#include <cassert>
#include "configuration.hpp"
#include "memory_pool.hpp"

namespace di {

template<typename T>
class inject {

	struct node {
		node() : injection(0), next(0) {}
		
		inject<T>* injection;
		node* next;

		void* operator new(size_t size) {
			assert(size == 8);
			return detail::memory_pool<>::malloc();
		}
		void operator delete(void* block) {
			detail::memory_pool<>::free(block);
		}
	};

public:
	inject() {
		add_node(this);
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
	static void add_node(inject<T>* injection) {
		if(0 == head) {
			head = new node();
			head->injection = injection;
		}
		else {
			node* current_node = head;
			while(0 != current_node->next) {
				current_node = current_node->next;
			}
			node* new_node = new node();
			new_node->injection = injection;
			current_node->next = new_node;
		}
	}

	//TODO synchronize
	static T** remove_first_matching(char* address, size_t range) {
		if(0 != head) {
			if(is_object_in_range(head->injection,address,range)) {
				T** injection = &(head->injection->object);
				node* tmp = head;
				head = head->next;
				delete tmp;
				return injection;
			}
			node* current = head;
			while(0 != current->next) {
				node* next = current->next;
				if(is_object_in_range(next->injection,address,range)) {
					T** injection = &(next->injection->object);
					node* tmp = next;
					current->next = next->next;
					delete next;
					return injection;
				}
				current = next;
			}
		}
		
		return 0;
	}

private:
	static inline bool is_object_in_range(inject<T>* injection, char* address, size_t range) {
		char* inject_address = reinterpret_cast<char*>(injection);
		return (inject_address >= address && inject_address < address + range);
	}

private:
	T* object;
	static node* head;
};

template<typename T>
typename inject<T>::node* inject<T>::head = 0;

} //namspace di

#endif //DI_INJECT_HPP
