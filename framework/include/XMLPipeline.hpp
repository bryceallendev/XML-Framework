/*
    XMLPipeline.hpp

    Include for XML pipeline
*/

#ifndef INCLUDED_XMLPIPELINE_HPP
#define INCLUDED_XMLPIPELINE_HPP

#include "MacroFilter.hpp"
#include <vector>

// @pattern Strategy @role Strategy
class XMLPipeline {
public:

    /*
        Add a filter by name

        @param name Name of the filter
        @param parameter Parameter to the filter
        @return Does the filter exist
    */
    bool addFilter(const std::string_view name, const std::string_view parameter);

    /*
        Run the pipeline of filters
    */
    void run() const;

private:

    // pipeline filters
    MacroFilter filterSeries;
};

#endif
