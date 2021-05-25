/*
    ParserFilter.hpp

    Parser filter for XMLFramework
*/

#ifndef INCLUDED_PARSERFILTER_HPP
#define INCLUDED_PARSERFILTER_HPP

#include "XMLDocument.hpp"
#include "Filter.hpp"
#include <string>
#include <memory>

// @pattern Template Method @role Concrete Class
class ParserFilter : public Filter {
public:
    /*
        Constructor

        @param filename Filename for action to parse
    */
    ParserFilter(std::string_view filename);

    /*
        Replaces the current XML document with the result of parsing 
        the filename

        @param doc Ignore starting document
        @return XML document from parsing input file
    */
    virtual XMLDocument action(XMLDocument document) final override;

    /*
        Factory method

        @param xpath XPath expression applied to the document
        @return OutputFilter object
    */
    static std::unique_ptr<Filter> factory(std::string_view xpath);

private:

    std::string inputFilename;

    // Filter registration
    static bool registered;
};

#endif
