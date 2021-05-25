/*
    FilterFactory.hpp

    Declaration of Filter factory for XMLFramework
*/

#ifndef INCLUDED_FILTERFACTORY_HPP
#define INCLUDED_FILTERFACTORY_HPP

#include "Filter.hpp"
#include <memory>
#include <string>
#include <string_view>
#include <vector>
#include <map>
#include <utility>

// @pattern Factory Method @role Creator
class FilterFactory {
public:
    
    using CreateType = std::unique_ptr<Filter>(*)(std::string_view);

    /*
        Registers a filter

        @param name Unique name of the filter
        @return funcCreate Create method of the filter
    */
    static bool Register(std::string_view name, std::string_view description, CreateType funcCreate);

    /*
        If a filter is registered

        @param name Name of the filter
        @return If the name is registered as a filter
    */
    static bool isRegistered(std::string_view name);

    /*
        Collection of filter names

        @return Collection of filter names
    */
    static std::vector<std::pair<std::string, std::string>> getFilterNames();

    /*
        Creates a filter based on the name

        @param name Unique name of the filter
        @return parameter Parameter for constructor
    */
    static std::unique_ptr<Filter> Create(std::string_view name, std::string_view parameter);

private:

    // store self-registered filters
    static std::map<std::string, std::pair<std::string, CreateType>, std::less<>> methods;
};

#endif
