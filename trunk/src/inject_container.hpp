//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_INJECT_CONTAINER_HPP
#define DI_INJECT_CONTAINER_HPP

#include "spin_lock.hpp"
#include "lock_guard.hpp"
#include "configuration.hpp"

namespace di {
namespace detail {

template<typename T>
class inject_container {
	struct holder {
		holder() : injection(0) {}
		explicit holder(T** a_injection) : injection(a_injection) {}

		inline bool is_holding() {
			return 0 != injection;
		}

		inline bool is_in_range(char* address, size_t range) {
			char* inject_address = reinterpret_cast<char*>(injection);
			return (inject_address >= address && inject_address < address + range);
		}

		T** injection;
	};

public:
	inline static void insert(T** injection) {
		detail::lock_guard<detail::spin_lock> guard(lock);
		
		for(size_t i=0; i < size; ++i) {
			if(0 == injections[i].injection) {
				injections[i].injection = injection;
				return;
			}
		}

		enlarge_and_add(injection);
	}

	inline static T** remove(char* address, size_t range) {
		detail::lock_guard<detail::spin_lock> guard(lock);
		
		for(size_t i=0; i < size; ++i) {
			if(injections[i].is_holding() && injections[i].is_in_range(address, range) ) {
				T** object = injections[i].injection;
				injections[i].injection = 0;
				return object;
			}
		}

		return 0;
	}

private:
	inline static void enlarge_and_add(T** injection) {
		size_t tmp_size = static_cast<size_t>(size*INJECTIONS_CONTAINER_RESIZE_FACTOR) + 1;
		holder* tmp_injections = new holder[tmp_size];
		memcpy(tmp_injections, injections, size * sizeof(holder));
		delete [] injections;
		injections = tmp_injections;
		injections[size].injection = injection;
		size = tmp_size;
	}

private:
	static holder* injections;
	static size_t size;

	static detail::spin_lock lock;
};

template<typename T>
typename inject_container<T>::holder* inject_container<T>::injections = 
	new typename inject_container<T>::holder[MAX_INJECTIONS_PER_TYPE];

template<typename T>
size_t inject_container<T>::size = MAX_INJECTIONS_PER_TYPE;

template<typename T>
detail::spin_lock inject_container<T>::lock;

} //namespace detail
} //namespace di

#endif //DI_INJECT_CONTAINER_HPP
