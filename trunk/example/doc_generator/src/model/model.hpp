//          Copyright Adam Lach 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef MODEL_HPP
#define MODEL_HPP

#include <vector>
#include "model/class.hpp"

namespace model {

struct Model {
	std::vector<Class> classes;
};

} // namespace model

#endif // MODEL_HPP