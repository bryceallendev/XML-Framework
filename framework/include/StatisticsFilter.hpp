/*
    StatisticsFilter.hpp

    Statistics filter for XMLFramework
*/

#ifndef INCLUDED_STATISTICSFILTER_HPP
#define INCLUDED_STATISTICSFILTER_HPP

#include "XMLDocument.hpp"
#include "Filter.hpp"
#include <string>
#include <memory>

// @pattern Template Method @role Concrete Class
class StatisticsFilter : public Filter {
public:
    /*
        Constructor
    */
    StatisticsFilter();

    /*
        Output the XML document to a file

        @param doc XML document to output to a file
        @return Input XML document
    */
    virtual XMLDocument action(XMLDocument document) override;

    // Outputs stats of XMLDocument
    static std::unique_ptr<Filter> displayStats();
    
private:

    // Filter registration
    static bool registered;
};

#endif