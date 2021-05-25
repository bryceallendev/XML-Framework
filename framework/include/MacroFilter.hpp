/*
    MacroFilter.hpp

    Macro filter that contains many filters
*/

#ifndef INCLUDED_MACROFILTER_HPP
#define INCLUDED_MACROFILTER_HPP

#include "XMLDocument.hpp"
#include "Filter.hpp"
#include <vector>
#include <memory>

// @pattern Template Method/Command) @role Concrete Class/Macro
class MacroFilter : public Filter {
public:
    /*
        Constructor
    */
    MacroFilter();

    /*
        Replaces the current XML document with the result of parsing 
        the filename

        @param doc Ignore starting document
        @return XML document from parsing input file
    */
    virtual XMLDocument action(XMLDocument document) override;
    
    // Add Filter into MacroFilter
    void addFilter(std::unique_ptr<Filter> filter);
    

private:

    std::vector<std::unique_ptr<Filter>> filters;
};

#endif
