//          Copyright Adam Lach 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef INTRODUCTION_HPP
#define INTRODUCTION_HPP

#include "asciidoc_output/document_element.hpp"
#include "model/class.hpp"

namespace asciidoc_output {

class Introduction : public DocumentElement {
public:
    Introduction(const model::Class* aClass) : cls(aClass) {}

private:
    void print(std::ostream& stream) const {
        std::string heading = "Class " + cls->name;

        stream << heading << std::endl;
        stream << std::string(heading.length(),'-') << std::endl;
        stream << std::endl;
        stream << cls->description;
    }

    const model::Class* cls;
};

} // namespace asciidoc_output

#endif // INTRODUCTION_HPP