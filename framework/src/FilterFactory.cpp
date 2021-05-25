/*
    FilterFactory.cpp

    Implementation of Filter factory for XMLFramework
*/

#include "FilterFactory.hpp"

// store self-registered filters
std::map<std::string, std::pair<std::string, FilterFactory::CreateType>, std::less<>> FilterFactory::methods;

/*
    Registers a filter

    @param name Unique name of the filter
    @return funcCreate Create method of the filter
*/
bool FilterFactory::Register(std::string_view name, std::string_view description, CreateType funcCreate) {

    if (auto it = methods.find(name); it == methods.end()) {
        methods[std::string(name)] = std::make_pair(description, funcCreate);
        return true;
    }

    return false;
}

/*
    If a filter is registered

    @param name Name of the filter
    @return If the name is registered as a filter
*/
bool FilterFactory::isRegistered(std::string_view name) {

    return methods.find(name) != methods.end();
}

/*
    Collection of filter names

    @return Collection of filter names
*/
std::vector<std::pair<std::string, std::string>> FilterFactory::getFilterNames() {

    std::vector<std::pair<std::string, std::string>> names;
    for (const auto& [key, val] : methods) {
        names.push_back(std::make_pair(key, val.first));
    }

    return names;
}

/*
    Creates a filter based on the name

    @param name Unique name of the filter
    @return parameter Parameter for constructor
*/
std::unique_ptr<Filter> FilterFactory::Create(std::string_view name, std::string_view parameter) {

    if (auto it = methods.find(name); it != methods.end()) 
        return it->second.second(parameter);

    return nullptr;
}
