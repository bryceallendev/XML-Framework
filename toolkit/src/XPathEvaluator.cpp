/*
    XPathEvaluator.cpp

    Evaluates an XPath expression
*/

#include "XPathEvaluator.hpp"
#include <string>
#include "xpath_evaluator.hpp"

/*
    Constructor

    @param xpath XPath expression
*/    
XPathEvaluator::XPathEvaluator(std::string_view xpath)
    : xpath(xpath) {}

/*
    Evaluate the XPath on the document

    @param doc XML document to apply XPath to
    @return Resulting XML document of applying XPath
*/
XMLDocument XPathEvaluator::evaluate(XMLDocument document) {

    // create the xpath evaluator on the current document
    auto evaluator = xpath_create(std::string(document.get()));

    // apply xpath to this document
    xpath_result_t result;
    auto status = xpath_apply(evaluator, xpath, &result);
    if (status != XPATH_STATUS_OK)
        return XMLDocument();

    // check the result
    if (result.type != XPATH_RESULTS_XML)
        return XMLDocument();

    // create the document with the moved xml
    return XMLDocument(std::move(result.xml));
}
