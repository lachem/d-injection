//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_INJECTION_TYPES_HPP
#define DI_INJECTION_TYPES_HPP

#include <memory>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/shared_ptr.hpp>
#include <di/detail/utility.hpp>

namespace di {
namespace detail {

namespace injection_id {

enum injection_id {invalid,ordinary,unique,shared,service,any};

} // injection_id
} // namespace detail

template<typename T>
struct smart_ptr {
	typedef boost::shared_ptr<T> shared_ptr;

#if !defined(BOOST_NO_CXX11_SMART_PTR)
	typedef std::unique_ptr<T> single_ptr;
#elif defined(BOOST_NO_CXX11_SMART_PTR) && !defined(BOOST_NO_AUTO_PTR)
	typedef std::auto_ptr<T> single_ptr;
#else
	typedef boost::shared_ptr<T> single_ptr;
#endif

	static void assign(T*& dest, T*& src) {
		dest = src;
	}

	static void assign(shared_ptr& dest, shared_ptr& src) {
		dest = src;
	}

	static void assign(single_ptr& dest, single_ptr& src) {
#if !defined(BOOST_NO_CXX11_SMART_PTR)
		dest = std::move(src);
#elif defined(BOOST_NO_CXX11_SMART_PTR) && !defined(BOOST_NO_AUTO_PTR)
		dest = src;
#endif
	}
};

template<typename T>
struct ordinary {
	enum{id=detail::injection_id::ordinary};

	typedef T type;
	typedef T* representation;
	
	explicit ordinary(T* an_object) : object(an_object) {}
    
    T* get_object() const {
        return object;
    }

	static T* extract(representation* rep) {
		return *rep;
	}
	static const T* extract(const representation* rep) {
		return *rep;
	}
	static void init(representation* rep) {
		*rep = NULL;
	}

private:
	T* object;
};

template<typename T>
struct unique {
	enum{id=detail::injection_id::unique};

	typedef T type;
	typedef typename smart_ptr<T>::single_ptr representation;

	explicit unique(T* an_object) : object(an_object) {}
	explicit unique(representation an_object) : object(an_object) {}

    T* get_object() const {
        return object;
    }

	static T* extract(representation* rep) {
		return rep->get();
	}
	static const T* extract(const representation* rep) {
		return rep->get();
	}
	static void init(representation* rep) {
		rep->reset(NULL_PTR(T));
	}

private:
	T* object;
};

template<typename T>
struct shared {
	enum{id=detail::injection_id::shared};

	typedef T type;
	typedef typename smart_ptr<T>::shared_ptr representation;

	explicit shared(T* an_object) : object(an_object) {}
	explicit shared(const representation& an_object) : object(an_object) {}

    operator representation() {
		return object;
	}
    representation get_object() const {
        return object;
    }

	static T* extract(representation* rep) {
		return rep->get();
	}
	static const T* extract(const representation* rep) {
		return rep->get();
	}
	static void init(representation* rep) {
		rep->reset(NULL_PTR(T));
	}

private:
	representation object;
};

// di::service casts away constnes for its private storage to enable dependency matching between same
// const and non-cost services in di::application. This happens when one module provides a read/write 
// service and the second module needs it as read only.
template<typename T>
struct service {
	enum{id=detail::injection_id::service};

	typedef T type;
	typedef typename boost::remove_const<T>::type non_const_type;
	typedef typename smart_ptr<T>::shared_ptr representation;
	typedef typename smart_ptr<non_const_type>::shared_ptr storage;

	service() : object(NULL_PTR(non_const_type)) {}
	explicit service(T* an_object) : object(const_cast<non_const_type*>(an_object)) {}
	explicit service(const representation& an_object) : object(boost::const_pointer_cast<non_const_type>(an_object)) {}

	operator representation() {
		return representation(object);
	}
	representation get_object() const {
        	return representation(object);
	}

	static T* extract(representation* rep) {
		return rep->get();
	}
	static const T* extract(const representation* rep) {
		return rep->get();
	}
	static void init(representation* rep) {
		rep->reset(NULL_PTR(T));
	}

private:
	storage object;
};

template<typename T>
di::shared<T> as_ordinary(T* object) {
	return di::ordinary<T>(object);
}

template<typename T>
di::shared<T> as_unique(T* object) {
	return di::unique<T>(object);
}

template<typename T>
di::shared<T> as_shared(T* object) {
	return di::shared<T>(object);
}

template<typename T>
di::shared<T> as_service(T* object) {
	return di::service<T>(object);
}

} // namespace di

#endif //DI_INJECTION_TYPES_HPP
