//          Copyright Adam Lach 2013
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef REFERENCE_HPP
#define REFERENCE_HPP

#include "asciidoc_output/document_element.hpp"
#include "asciidoc_output/grouper.hpp"

namespace asciidoc_output {

class Reference : public DocumentElement {
public:
    Reference(const model::Model* aModel, asciidoc_output::Grouper* aGrouper) : model(aModel), grouper(aGrouper) {}

private:
    void print(std::ostream& stream) const {
        stream << "Reference::" << std::endl;
        for(auto&& group : getGroups()) {
			stream << "\t" << group.name << ":::" << std::endl;
			for(auto&& cls : group.classes) {
				stream << "\t\t" << "link:" << "class/" << cls.name + ".html"
					   << "[Class " << cls.name << "]::::" << std::endl;
			}
		}	
    }

    std::vector<Group> getGroups() const {
		if(grouper) {
			grouper->add(model->classes);
			return std::move(grouper->getGroups());
		}
		else {
			std::vector<Group> result;
			Group group;
			group.classes = model->classes;
			result.push_back(group);
			return std::move(result);
		}
	}

    const model::Model* model; 
    asciidoc_output::Grouper* grouper;
};

} // namespace asciidoc_output

#endif // REFERENCE_HPP