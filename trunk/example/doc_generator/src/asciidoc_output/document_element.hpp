//          Copyright Adam Lach 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DOCUMENT_ELEMENT_HPP
#define DOCUMENT_ELEMENT_HPP

#define CODE_DELIMETER "---------------------------------------------------------------------"

#include <ostream>

namespace asciidoc_output {

class DocumentElement {
    friend std::ostream& operator<<(std::ostream&, const DocumentElement&); 

    virtual void print(std::ostream&) const = 0;
};

std::ostream& operator<<(std::ostream& stream, const DocumentElement& element) {
    element.print(stream);
    return stream;
}

} // namespace asciidoc_output

#endif // DOCUMENT_ELEMENT_HPP