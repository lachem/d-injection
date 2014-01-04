//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_INJECTION_HPP
#define DI_INJECTION_HPP

#include <boost/bind.hpp>
#include <di/detail/injection_destination_container.hpp>

namespace di {
namespace detail {

template<typename T, typename P>
struct injection {
	T* operator->() {
		assert(!empty());
		return get_object();	
	}

	T& operator*() {
		assert(!empty());
		return *get_object();
	}

	T const* operator->() const {
		assert(!empty());
		return get_object();
	}

	T const& operator*() const {
		assert(!empty());
		return *get_object();
	}

	T* get() {
		assert(!empty());
		return get_object();	
	}

	T const* get() const {
		assert(!empty());
		return get_object();	
	}

	bool empty() const {
		return get_object() == NULL;
	}

	operator safe_bool::type() const {
		return empty() ? safe_bool::false_ : &safe_bool::true_;
	}

protected:
	explicit injection(bool is_required) {
		P::init(&rep_object);
		insert_self_into_container(is_required);
	}
	injection(const injection<T,P>& source) : rep_object(source.rep_object) {}
#ifdef DI_HAS_UNIQUE_PTR
	injection(injection<T,P>&& source) : rep_object(std::move(source.rep_object)) {}
#endif
	~injection() {
		if(empty()) {
			remove_self_from_container();
		}
	}

	injection<T,P>& operator=(const injection<T,P>& source) {
		rep_object = source.rep_object;
		return *this;
	}
#ifdef DI_HAS_UNIQUE_PTR
	injection<T,P>& operator=(injection<T,P>&& source) {
		rep_object = std::move(source.rep_object);
		return *this;
	}
#endif

	void do_copy(const injection<T,P>& source, bool is_required) {
		//need to insert self into the container if injection has not yet been executed 
		if(source.empty()) {			
			insert_self_into_container(is_required);
		}
	}

#ifdef DI_HAS_UNIQUE_PTR
	void do_move_assignement(injection<T,P>&& source, bool is_required) {
		if(!source.empty() && this->empty()) {
			remove_self_from_container();
		}
		else
		if(source.empty() && !this->empty()) {
			insert_self_into_container(is_required);
		}
		this->operator=(std::move(source));
	}
#endif
	void do_assignement(const injection<T,P>& source, bool is_required) {
		if(!source.empty() && this->empty()) {
			remove_self_from_container();
		}
		else
		if(source.empty() && !this->empty()) {
			insert_self_into_container(is_required);
		}
		this->operator=(source);
	}

	void do_move(const injection<T,P>& source, bool is_required) {
		//need to insert self into the container if injection has not yet been executed 
		if(this->empty()) {			
			insert_self_into_container(is_required);
		}
	}

	void insert_self_into_container(bool is_required) {
		injection_destination_container<T>::insert(
			injection_destination_imp<P>(&rep_object,is_required));
	}

	void remove_self_from_container() {
		injection_destination_container<T>::remove(
			injection_destination_key(&rep_object,P::id));
	}

	T* get_object() {
		return P::extract(&rep_object);
	}

	T const* get_object() const {
		return P::extract(&rep_object);
	}

	typename P::representation rep_object;
};

} //namspace detail
} //namspace di

#endif //DI_INJECTION_HPP
