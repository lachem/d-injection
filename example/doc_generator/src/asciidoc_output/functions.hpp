//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include "asciidoc_output/document_element.hpp"
#include "model/class.hpp"

namespace asciidoc_output {

class Functions : public DocumentElement {
public:
    Functions(const model::Class* aClass) : cls(aClass) {}

private:
    void print(std::ostream& stream) const {
        stream << "Functions" << std::endl;
        stream << "~~~~~~~~~" << std::endl;
        stream << std::endl;
        for(auto&& function : cls->methods) {
            stream << CODE_DELIMETER << std::endl;
            stream << function.signature << std::endl;
            stream << CODE_DELIMETER << std::endl;
            stream << std::endl;
            stream << "*Requires:* none +" << std::endl;
            stream << "*Effects:* none +" << std::endl;
            stream << "*Postconditions:* none +" << std::endl;
            stream << "*Returns:* none +" << std::endl;
            stream << "*Throws:* no +" << std::endl;
            stream << "*Complexity:* no info +" << std::endl;
            stream << "*Note:* none +" << std::endl;
            stream << "*Danger:* none +" << std::endl;
            stream << "*Rationale:* none +" << std::endl;
            stream << std::endl;
        }
    }

    const model::Class* cls;
};

} // namespace asciidoc_output

#endif // FUNCTIONS_HPP