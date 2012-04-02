//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_INJECTION_HPP
#define DI_INJECTION_HPP

#include <memory>
#include <boost/shared_ptr.hpp>
#include <di/detail/inject_container.hpp>
#include <di/detail/delete_policy.hpp>

namespace di {
namespace detail {

template<typename T, typename P>
struct representation;

template<typename T>
struct representation<T,di::none> {
	typedef T type;
};

template<typename T>
struct representation<T,di::shared> {
	typedef boost::shared_ptr<T> type;
};

template<typename T>
struct representation<T,di::unique> {
	typedef std::auto_ptr<T> type;
};

struct perform_injection;

template<typename T>
struct injection {
protected:
	explicit injection(T* an_object) : object(an_object) {
		inject_container<T>::insert(this);
	}

	T* getObject() {
		return object;	
	}

	T const* getObject() const {
		return object;	
	}

private:
	T* object;

	friend struct perform_injection;
};

template<typename T>
struct injection< boost::shared_ptr<T> > {
protected:
	explicit injection(T* an_object) : object(an_object) {
		inject_container< boost::shared_ptr<T> >::insert(this);
	}
	
	T* getObject() {
		return object.get();	
	}

	T const* getObject() const {
		return object.get();	
	}

private:
	boost::shared_ptr<T> object;

	friend struct perform_injection;
};

template<typename T>
struct injection< std::auto_ptr<T> > {
private:
	injection(const injection< std::auto_ptr<T> >&);
	void operator=(const injection< std::auto_ptr<T> >&);
protected:
	explicit injection(T* an_object) : object(an_object) {
		inject_container< std::auto_ptr<T> >::insert(this);
	}

	T* getObject() {
		return object.get();	
	}

	T const* getObject() const {
		return object.get();	
	}

private:
	std::auto_ptr<T> object;

	friend struct perform_injection;
};

} //namspace detail
} //namspace di

#endif //DI_INJECTION_HPP
