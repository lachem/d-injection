//          Copyright Adam Lach 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef GROUPER_HPP
#define GROUPER_HPP

#include <cctype>
#include <vector>
#include <unordered_map>
#include <boost/shared_ptr.hpp>
#include <boost/algorithm/string.hpp> 
#include "model/class.hpp"

namespace asciidoc_output {

struct Group {
    Group(const std::string& aName = "Unknown") : name(aName) {}

    std::string name;
    std::vector<model::Class> classes;
};

class Grouper {
public:
    void add(const model::Class& cls) {
        std::string groupName = getGroupName(cls);       

        groupMap[groupName].classes.push_back(cls);
        groupMap[groupName].name = std::move(groupName);
    }

    void add(const std::vector<model::Class>& classes) {
        for(auto&& cls : classes) {
            add(cls);
        }
    }

    std::vector<Group> getGroups() {
        std::vector<Group> groups;
        groups.reserve(groupMap.size());
        for(auto&& it : groupMap) {
            groups.push_back(it.second);  
        } 
        return std::move(groups);
    }

private:
    std::string getGroupName(const model::Class& cls) {
        if(belongsToBuilderGroup(cls)) {
            return "Building";
        }
        else
        if(belongsToApplicationGroup(cls)) {
            return "Application";
        }
        else {
            std::string filename = cls.filename;
            boost::replace_all(filename, "_", " ");
            filename[0] = toupper(filename[0]);
            return std::move(filename.substr(0,filename.find(".")));
        }
    }

    bool belongsToBuilderGroup(const model::Class& cls) {
        return cls.name.find("builder") != std::string::npos ||
               cls.name.find("configurable") != std::string::npos;
    }

    bool belongsToApplicationGroup(const model::Class& cls) {
        return cls.name.find("service_list") != std::string::npos ||
               cls.name.find("application") != std::string::npos ||
               cls.name.find("module") != std::string::npos;
    }

    std::unordered_map<std::string, Group> groupMap;
};

} // namespace asciidoc_output

#endif // GROUPER_HPP