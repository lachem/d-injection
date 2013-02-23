//          Copyright Adam Lach 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef FILE_HPP
#define FILE_HPP

#include <vector>
#include <boost/optional.hpp>
#include "filesystem/file.hpp"

namespace filesystem {

struct File {
	std::string name;
	std::string path;
	boost::optional<std::string> content;
};

} // namespace filesystem

#endif // FILE_HPP