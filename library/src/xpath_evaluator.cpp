/*
    xpath_evaluator.cpp

    Set of functions for xpath evaluation
*/

#include "xpath_evaluator.hpp"
#include <string>

// xpath processor opaque definition
struct xpath_processor {
    std::string docstr;
};

/**
 * Create a new xpath evaluator instance
 * @note Archive must be freed using xpath_free()
 * @return The created xpath processor
 */
xpath_processor* xpath_create(const std::string& document) {

    return new xpath_processor{document};
}

/**
 * Append the XPath expression to the list of transformations/queries.
 * Instead of outputting the results in a separate unit tag, output the complete
 * processor marking the XPath results with a user provided attribute
 *
 * @param processor A configured XPath processor with a document
 * @param prefix Attribute prefix
 * @param namespace_uri Attribute namespace
 * @param attr_name Attribute name
 * @param attr_value Attribute value
 * @return XPATH_STATUS_OK on success
 * @return Status error code on failure
 */
int xpath_set_attribute(xpath_processor* /* processor */,
                        const std::string& /* prefix */, const std::string& /* namespace_uri */,
                        const std::string& /* attr_name */, const std::string& /* attr_value */) {

    return XPATH_STATUS_OK;
}

/** 
 * Append the XPath expression to the list of transformations/queries.
 * Instead of outputting the results in a separate unit tag, output the complete
 * processor marking the XPath results with a user provided element.
 *
 * @param processor A configured XPath processor with a document
 * @param prefix Element prefix
 * @param namespace_uri Element namespace
 * @param element Element name
 * @return XPATH_STATUS_OK on success
 * @return Status error code on failure
 */
int xpath_set_element(xpath_processor* /* processor */,
                      const std::string& /* prefix */, const std::string& /* namespace_uri */,
                      const std::string& /* element */) {

    return XPATH_STATUS_OK;
}

/**
 * Apply the XPath from the processor to document. Results
 * placed in the proper field of the result, with the result_type
 * parameter indicating which is appropriate.
 *
 * @param processor A configured XPath processor with a document
 * @param xpath The XPath to apply to the document
 * @param results Optional struct of different result types
 * @returns Returns XPATH_STATUS_OK on success and a status error codes on failure.
 */
int xpath_apply(xpath_processor* processor, const std::string& /* xpath */, xpath_result_t* result) {

    // required arguments
    if (processor == nullptr || result == nullptr)
        return XPATH_STATUS_INVALID_ARGUMENT;

    // result type of XML only
    result->type = XPATH_RESULTS_XML;
    result->xml = processor->docstr;

    return XPATH_STATUS_OK;
}

/** 
 * Free an xpath_processor that was previously allocated y using xpath_create()
 * The processor must be reallocated/re-created to use again.
 * 
 * @param processor An XPath processor
 */
void xpath_free(xpath_processor* processor) {

    if (processor == nullptr)
        return;
    
    delete processor;
}
