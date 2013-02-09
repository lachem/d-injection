//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DI_UTILITY_HPP
#define DI_UTILITY_HPP

#ifndef NULL
#define NULL 0
#endif

#ifndef NULL_PTR
#define NULL_PTR(Class) static_cast<Class*>(0)
#endif

namespace di {
namespace detail {

class noncopyable {
	noncopyable(const noncopyable&);
	noncopyable& operator=(const noncopyable&);
public:
	noncopyable() {}
};

class nonallocatable {
	void* operator new(size_t);
	void operator delete(void*);
};

template<size_t size>
struct mem_block{
	char bytes[size];
};

struct subject_base {};

} // namespace detail
} // namespace di

#endif //DI_UTILITY_HPP
