/*
    XPathFilter.hpp

    XPath filter for XMLFramework
*/

#ifndef INCLUDED_XPATHFILTER_HPP
#define INCLUDED_XPATHFILTER_HPP

#include "XMLDocument.hpp"
#include "Filter.hpp"
#include <string>
#include <memory>

// @pattern Template Method @role Concrete Class
class XPathFilter final : public Filter {
public:

    /*
        Constructor

        @param xpath XPath expression
    */
    XPathFilter(std::string_view xpath);

    /*
        Applies the XPath to the input XML document producing
        a new XML document with the query results

        @param doc Input XML document
        @return Result of XPath applied to the input XML document
    */
    virtual XMLDocument action(XMLDocument document) override;

    /*
        Factory method

        @param xpath XPath expression applied to the document
        @return XPathFilter object
    */
    static std::unique_ptr<Filter> factory(std::string_view xpath);

private:

    // xpath expression
    std::string xpath;

    // Filter registration
    static bool registered;
};

#endif
