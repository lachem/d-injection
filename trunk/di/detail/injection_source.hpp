//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_INJECTION_SOURCE_HPP
#define DI_INJECTION_SOURCE_HPP

#include <memory>
#include <boost/shared_ptr.hpp>
#include <di/detail/injection_types.hpp>
#include <di/detail/utility.hpp>

namespace di {
namespace detail {

template<typename T>
struct injection_source {
	typedef T type;

	injection_source() : ordinary_object(NULL),shared_object(NULL_PTR(T)),unique_object(NULL_PTR(T)),is_empty(true) {}

	injection_source<T>& operator=(const di::ordinary<T>& inj) {
		ordinary_object = inj.object;
		unique_object = std::auto_ptr<T>(NULL_PTR(T));
		shared_object = boost::shared_ptr<T>(NULL_PTR(T));
		is_empty = false;
		return *this;
	}

	injection_source<T>& operator=(const di::unique<T>& inj) {
		ordinary_object = NULL;
		unique_object = std::auto_ptr<T>(inj.object);
		shared_object = boost::shared_ptr<T>(NULL_PTR(T));
		is_empty = false;
		return *this;
	}

	injection_source<T>& operator=(const di::shared<T>& inj) {
		ordinary_object = NULL;
		unique_object = std::auto_ptr<T>(NULL_PTR(T));
		shared_object = boost::shared_ptr<T>(inj.object);
		is_empty = false;
		return *this;
	}

	injection_source<T>& operator=(boost::shared_ptr<T>& inj) {
		ordinary_object = NULL;
		unique_object = std::auto_ptr<T>(NULL_PTR(T));
		shared_object = inj;
		is_empty = false;
		return *this;
	}

	bool empty() {
		return is_empty;
	}

	T* ordinary_object;
	std::auto_ptr<T> unique_object;
	boost::shared_ptr<T> shared_object;

private:
	bool is_empty;
};

} //namspace detail
} //namspace di

#endif //DI_INJECTION_SOURCE_HPP
