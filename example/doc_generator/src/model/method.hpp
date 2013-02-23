//          Copyright Adam Lach 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef METHOD_HPP
#define METHOD_HPP

#include <string>

namespace model {

struct Method {
	enum kind {constructor, destructor, function};

    struct Description {
        Description() : precondition("none"), postcondition("none"), throws("none"), returns("none") {}

        std::string brief;
        std::string detailed;
        std::string precondition;
        std::string postcondition;
        std::string throws;
        std::string returns;
    };

    Description description;
	std::string name;
	std::string signature;
};

} // namespace Model

#endif // METHOD_HPP