//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef INJECT_HPP
#define INJECT_HPP

#include <boost/fusion/include/for_each.hpp>

template<typename T>
class Inject {

	struct Node {
		Node() : inject(0), next(0), occupied(false) {}
		Inject<T>* inject;
		Node* next;
		bool occupied;
	};

public:
	Inject() {
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

	const T& operator*() const {
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

	operator const T&() const {
		return *object;
	}

	//TODO synchronize
	static void addNode(Inject<T>* inject) {
		if(0 == head) {
			head = new Node();
			head->inject = inject;
		}
		else {
			Node* currentNode = head;
			while(0 != currentNode->next) {
				currentNode = currentNode->next;
			}
			Node* newNode = new Node();
			newNode->inject = inject;
			currentNode->next = newNode;
		}
	}

	//TODO synchronize
	static T** getFirstUnoccupied(char* address, size_t range) {
		Node* currentNode = head;
		while(0 != currentNode) {
			if(!currentNode->occupied && isObjectInRange(currentNode->inject,address,range)) {
				currentNode->occupied = true;
				return &(currentNode->inject->object);
			}
			currentNode = currentNode->next;
		}
		return 0;
	}

	//TODO perhaps it is sufficient to only remove all occupied nodes?
	//TODO synchronize
	static void removeNodes(char* address, size_t range) {
		if(0 != head) {
			Inject<T>* inject = head->inject;
			if(isObjectInRange(inject,address,range)) {
				Node* tmp = head->next;
				head = head->next;
				delete tmp;
			}

			Node* currentNode = head;
			while(0 != currentNode && 0 != currentNode->next) {
				inject = currentNode->next->inject;
				if(isObjectInRange(inject,address,range)) {
					Node* tmp = currentNode->next;
					currentNode->next = currentNode->next->next;
					delete tmp;
				}

				currentNode = currentNode->next;
			}
		}
	}

private:
	static bool isObjectInRange(Inject<T>* inject, char* address, size_t range) {
		char* injectAddress = reinterpret_cast<char*>(inject);
		return (injectAddress >= address && injectAddress < address + range);
	}

private:
	T* object;
	static Node* head;
};

template<typename T>
typename Inject<T>::Node* Inject<T>::head = 0;

#endif //INJECT_HPP
