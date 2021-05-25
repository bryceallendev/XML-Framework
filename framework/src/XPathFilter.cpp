/*
    XPathFilter.cpp

    XPath filter for XMLFramework
*/

#include "XPathFilter.hpp"
#include "XPathEvaluator.hpp"
#include "FilterFactory.hpp"
#include <iostream>

// Register class in the FilterFactory
bool XPathFilter::registered = FilterFactory::Register("xpath", "XPath query", XPathFilter::factory);

/*
    Constructor

    @param xpath XPath expression
*/
XPathFilter::XPathFilter(std::string_view xpath)
    : xpath(xpath) {
}

/*
    Applies the XPath to the input XML document producing
    a new XML document with the query results

    @param doc Input XML document
    @return Result of XPath applied to the input XML document
*/
XMLDocument XPathFilter::action(XMLDocument document) {

    XPathEvaluator evaluator(xpath);

    // declare needed namespaces
    

    return evaluator.evaluate(std::move(document));
}

/*
    Factory method

    @param xpath XPath expression applied to the document
    @return XPathFilter object
*/
std::unique_ptr<Filter> XPathFilter::factory(std::string_view xpath) {

    return std::make_unique<XPathFilter>(xpath);
}
