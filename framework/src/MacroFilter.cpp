/*
    MacroFilter.cpp

    Macro filter that contains many filters
*/

#include "XMLDocument.hpp"
#include "XPathFilter.hpp"
#include "MacroFilter.hpp"
#include "OutputFilter.hpp"
#include <vector>
#include <memory>


/*
    Constructor
*/
MacroFilter::MacroFilter() {
    
}

/*
    Replaces the current XML document with the result of parsing 
    the filename

    @param doc Ignore starting document
    @return XML document from parsing input file
*/
XMLDocument MacroFilter::action(XMLDocument document) {
    // a NOP with no filters
    if (filters.empty())
        std::cout << "No filters found.";
    
    XMLDocument doc;

    // apply all filters in sequential order
    for (const auto& filter : filters) {
        document = filter->action(std::move(document));
    }
    
    // perform a default OutputFilter if the last filter in the pipeline is not an output
    if (!filters.back()->isOutput()) {
        OutputFilter("-").action(std::move(document));
    }
    
    return document;
}

// Add Filter into MacroFilter
void MacroFilter::addFilter(std::unique_ptr<Filter> filter) {
    
    // append xml filter
    filters.push_back(std::move(filter));
}
