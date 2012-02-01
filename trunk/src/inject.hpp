//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_INJECT_HPP
#define DI_INJECT_HPP

#include <boost/fusion/include/for_each.hpp>

namespace di {

template<typename T>
class inject {

	struct node {
		node() : injection(0), next(0) {}
		inject<T>* injection;
		node* next;
	};

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
	static void addNode(inject<T>* injection) {
		if(0 == head) {
			head = new node();
			head->injection = injection;
		}
		else {
			node* currentNode = head;
			while(0 != currentNode->next) {
				currentNode = currentNode->next;
			}
			node* newNode = new node();
			newNode->injection = injection;
			currentNode->next = newNode;
		}
	}

	//TODO synchronize
	static T** removeFirstMatching(char* address, size_t range) {
		if(0 != head) {
			if(isObjectInRange(head->injection,address,range)) {
				T** injection = &(head->injection->object);
				node* tmp = head;
				head = head->next;
				delete tmp;
				return injection;
			}
			node* current = head;
			while(0 != current->next) {
				node* next = current->next;
				if(isObjectInRange(next->injection,address,range)) {
					T** injection = &(next->injection->object);
					node* tmp = next;
					head = next->next;
					delete next;
					return injection;
				}
				current = next;
			}
		}
		
		return 0;
	}

private:
	static inline bool isObjectInRange(inject<T>* injection, char* address, size_t range) {
		char* injectAddress = reinterpret_cast<char*>(injection);
		return (injectAddress >= address && injectAddress < address + range);
	}

private:
	T* object;
	static node* head;
};

template<typename T>
typename inject<T>::node* inject<T>::head = 0;

} //namspace di

#endif //DI_INJECT_HPP
