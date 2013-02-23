//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef METHOD_HPP
#define METHOD_HPP

#include <string>

namespace model {

struct Method {
	enum kind {constructor, destructor, function};

    std::string description;
	std::string name;
	std::string signature;
};

} // namespace Model

#endif // METHOD_HPP