//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_ITEM_TYPES_HPP
#define DI_ITEM_TYPES_HPP

#include <memory>
#include <boost/shared_ptr.hpp>
#include <di/detail/item.hpp>

namespace di {
namespace detail {

//TODO: void* casting is dangerous, maybe there is other way to remove virtuality
template<typename T>
struct ordinary_item : public item<T> {
	ordinary_item(T** rep, bool required) : item<T>(rep,required){
		item<T>::do_assignement = assignement;
	}
private:
	static bool assignement(void* address, injection_source<T>& object) {
		*reinterpret_cast< T** >(address) = object.ordinary_object;
		return object.ordinary_object != NULL;
	}
};

template<typename T>
struct shared_item : public item<T> {
	shared_item(boost::shared_ptr<T>* rep, bool required) : item<T>(rep,required) {
		item<T>::do_assignement = assignement;
	}
private:
	static bool assignement(void* address, injection_source<T>& object) {
		*reinterpret_cast< boost::shared_ptr<T>* >(address) = object.shared_object;
		return object.shared_object.get() != NULL;
	}
};

template<typename T>
struct unique_item : public item<T> {
	unique_item(std::auto_ptr<T>* rep, bool required) : item<T>(rep,required) {
		item<T>::do_assignement = assignement;
	}
private:
	static bool assignement(void* address, injection_source<T>& object) {
		std::auto_ptr<T>* ptr = reinterpret_cast< std::auto_ptr<T>* >(address);
		ptr->reset(object.unique_object.release());
		return ptr->get() != NULL;
	}
};

} // namespace detail
} // namespace di

#endif //DI_ITEM_TYPES_HPP
