//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_OPTIONAL_HPP
#define DI_OPTIONAL_HPP

#include <di/detail/inject_container.hpp>
#include <di/detail/injection.hpp>

namespace di {

template<typename T>
struct optional : public detail::injection<T> {
	optional() : detail::injection<T>(default_ptr()) {}

	T* operator->() {
		assert(!empty());
		return object;	
	}

	T& operator*() {
		assert(!empty());
		return *object;
	}

	operator T*() {
		assert(!empty());
		return object;
	}

	T const* operator->() const {
		assert(!empty());
		return object;
	}

	T const& operator*() const {
		assert(!empty());
		return *object;
	}

	operator T const*() const {
		assert(!empty());
		return object;
	}

	bool empty() {
		return  detail::injection<T>::object == NULL || 
				detail::injection<T>::object == default_ptr();
	}
	
private:
	enum {default_ptr_value = 2};

	static T* default_ptr() {
		return reinterpret_cast<T*>(default_ptr_value);
	}

	//TODO: that's ugly...
	static bool is_same(T* object) {
		return default_ptr() == object;
	}

	friend struct detail::perform_injection;
};

} //namspace di

#endif //DI_OPTIONAL_HPP
