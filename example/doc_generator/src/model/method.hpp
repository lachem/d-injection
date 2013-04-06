//          Copyright Adam Lach 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef METHOD_HPP
#define METHOD_HPP

#include <string>

#define NOTHING "none"

namespace model {

struct Method {
	enum kind {constructor, destructor, function};

    struct Description {
        Description() : 
			precondition(NOTHING), effects(NOTHING), postcondition(NOTHING), 
			returns(NOTHING), throws(NOTHING), complexity("no info"),
			note(NOTHING), danger(NOTHING), rationale(NOTHING) {}

        std::string brief;
        std::string detailed;
        std::string precondition;
		std::string effects;
        std::string postcondition;
        std::string returns;
		std::string throws;
		std::string complexity;
		std::string note;
		std::string danger;
		std::string rationale;
    };

    Description description;
	std::string name;
	std::string signature;
};

} // namespace Model

#endif // METHOD_HPP