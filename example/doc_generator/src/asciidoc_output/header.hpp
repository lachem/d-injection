//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef HEADER_HPP
#define HEADER_HPP

#include "asciidoc_output/document_element.hpp"
#include "model/class.hpp"

namespace asciidoc_output {

class Header : public DocumentElement {
public:
    Header(const model::Class* aClass) : cls(aClass) {}

private:
    void print(std::ostream& stream) const {
        std::string heading = "Dependency Injection - Header link:../../di/" + cls->filename + "[<"+cls->filename+">]";
        stream << heading << std::endl;
        stream << std::string(heading.length(),'=') << std::endl;
    }

    const model::Class* cls;
};

} // namespace asciidoc_output

#endif // HEADER_HPP