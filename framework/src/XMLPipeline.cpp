/*
    XMLPipeline.cpp

    Implementation for XML pipeline
*/

#include "XMLPipeline.hpp"
#include "OutputFilter.hpp"
#include "FilterFactory.hpp"
#include "MacroFilter.hpp"

/*
    Add a filter by name

    @param name The name of the filter
    @param parameter Parameter to the filter
    @return Does the filter exist
*/
bool XMLPipeline::addFilter(std::string_view name, std::string_view parameter) {

    // create the proper filter
    std::unique_ptr<Filter> filter = FilterFactory::Create(name, parameter);

    // if no filter created, then the name is most likely invalid
    if (!filter)
        throw false;

    // append xml filter
    //filters.push_back(std::move(filter));
    //MacroFilter filterSeries;
    
    filterSeries.addFilter(std::move(filter));

    return true;
}

/*
    Run the pipeline of filters
*/
void XMLPipeline::run() const {

    // apply all filters in sequential order
    XMLDocument document;
    
    MacroFilter& ref = const_cast <MacroFilter&>(filterSeries);
    
    document = ref.action(std::move(document));
}
