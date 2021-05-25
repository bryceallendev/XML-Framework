/*
    OutputFilter.hpp

    Output filter for XMLFramework
*/

#ifndef INCLUDED_OUTPUTFILTER_HPP
#define INCLUDED_OUTPUTFILTER_HPP

#include "XMLDocument.hpp"
#include "Filter.hpp"
#include <string>
#include <memory>

// @pattern Template Method @role Concrete Class
class OutputFilter : public Filter {
public:
    /*
        Constructor

        @param filename Filename for output file
    */
    OutputFilter(std::string_view filename);

    /*
        Output the XML document to a file

        @param doc XML document to output to a file
        @return Input XML document
    */
    virtual XMLDocument action(XMLDocument document) override;

    /*
        Does the filter perform output
    */
    virtual bool isOutput();

    /*
        Factory method

        @param xpath XPath expression applied to the document
        @return OutputFilter object
    */
    static std::unique_ptr<Filter> factory(std::string_view xpath);

private:

    std::string outputFilename;

    // Filter registration
    static bool registered;
};

#endif
