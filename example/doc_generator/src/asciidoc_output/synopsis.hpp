//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SYNOPSIS_HPP
#define SYNOPSIS_HPP

#include "asciidoc_output/document_element.hpp"
#include "model/class.hpp"

namespace asciidoc_output {

class Synopsis : public DocumentElement {
public:
    Synopsis(const model::Class* aClass) : cls(aClass) {}

private:
    void print(std::ostream& stream) const {
        static std::string codeDelimeter = "---------------------------------------------------------------------\n";

        std::string indentation;
        std::string synopsis =  "Synopsis";
        stream << synopsis + "\n" + std::string(synopsis.length(),'~') + "\n\n";
        stream << codeDelimeter;
        if(!cls->space.empty()) {
            stream << indentation + "namespace " + cls->space + "\n";
            stream << indentation + "{\n";
            increase(indentation);
        }
        stream << indentation + cls->signature + "\n";
        stream << indentation + "{\n";

        increase(indentation);
        for(auto&& method : cls->methods) {
            stream << indentation + method.signature + ";\n";
        }
        decrease(indentation);

        stream << indentation + "};\n";
        if(!cls->space.empty()) {
            decrease(indentation);
            stream << indentation + "}\n";
        }
        stream << codeDelimeter;
    }

    void increase(std::string& indentation) const {
        indentation += "  ";
    }

    void decrease(std::string& indentation) const {
        indentation.pop_back();
        indentation.pop_back();
    }

    const model::Class* cls;
};

} // namespace asciidoc_output

#endif // SYNOPSIS_HPP