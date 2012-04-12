//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_REQUIRED_HPP
#define DI_REQUIRED_HPP

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <di/detail/specialized_injection.hpp>
#include <di/detail/utility.hpp>
#include <di/detail/atomic.hpp>

#define DI_REQUIRED_TYPE_ID 1

namespace di {

template<typename T>
struct required : 
	public detail::specialized_injection<T,DI_REQUIRED_TYPE_ID>, 
	public detail::nonallocatable 
{
	friend struct detail::perform_injection;
};

template<typename T>
struct required< shared<T> > : 
	public detail::specialized_injection<T,DI_REQUIRED_TYPE_ID>, 
	public detail::nonallocatable 
{
private:
	typedef detail::specialized_injection<T,DI_REQUIRED_TYPE_ID> base;
public:
	required() : instance_count(new detail::uint32_t(1)) {}
	required(const required< shared<T> >& req) : base(req){
		instance_count = req.instance_count;
		detail::atomic_inc32(req.instance_count);
	}
	required< shared<T> >& operator=(const required< shared<T> >& req) {
		if(this != &req) {
			base::operator=(req);
			detail::atomic_inc32(req.instance_count);
			instance_count = req.instance_count;
		}
		return *this;
	}
	~required() {
		detail::atomic_dec32(instance_count);
		if(should_be_deleted()) {
			delete instance_count;
			delete detail::injection<T>::get_object();
		}
	}
private:
	bool should_be_deleted() {
		return !detail::atomic_read32(instance_count) && !empty();
	}

	detail::uint32_t* instance_count;
};

template<typename T>
struct required< unique<T> > : 
	public detail::specialized_injection<T,DI_REQUIRED_TYPE_ID>, 
	public detail::nonallocatable, public detail::noncopyable
{
	required(){};
	~required() {
		if(!empty()) {
			delete detail::injection<T>::get_object();
		}
	}
};

} //namspace di

#undef DI_REQUIRED_TYPE_ID

#endif //DI_REQUIRED_HPP
