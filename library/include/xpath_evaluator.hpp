/*
    xpath_evaluator.hpp

    Set of functions for xpath evaluation
*/

#ifndef INCLUDED_XPATH_EVALUATOR_HPP
#define INCLUDED_XPATH_EVALUATOR_HPP

#include <string>

// Return status indicating no errors
#define XPATH_STATUS_OK                   0

// Return status indicating general errors occurred
#define XPATH_STATUS_ERROR                1

// Return status indicating an invalid argument
#define XPATH_STATUS_INVALID_ARGUMENT     2

// Return status indicating that there is some problem with the input
#define XPATH_STATUS_INVALID_INPUT        3

// Return status indicating an invalid read I/O operation (such as write on read only archive)
#define XPATH_STATUS_INVALID_IO_OPERATION 4

// Return status indicating that there is some problem with the input
#define XPATH_STATUS_IO_ERROR             5

struct xpath_processor;

/**
 * Create a new xpath evaluator instance
 * @note Archive must be freed using xpath_free()
 * @return The created xpath processor
 */
xpath_processor* xpath_create(const std::string& document);

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
int xpath_set_attribute(xpath_processor* processor,
                        const std::string& prefix, const std::string& namespace_uri,
                        const std::string& attr_name, const std::string& attr_value);

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
int xpath_set_element(xpath_processor* processor,
                      const std::string& prefix, const std::string& namespace_uri,
                      const std::string& element);

/**
 * Transformation result types
 */
#define XPATH_RESULTS_NONE    0
#define XPATH_RESULTS_XML     1
#define XPATH_RESULTS_BOOLEAN 2
#define XPATH_RESULTS_NUMBER  3
#define XPATH_RESULTS_STRING  4

/**
 * Transformation result. Passed to srcml_unit_apply_transforms() to collect
 * results of transformation
 */
struct xpath_result_t {
    /** Transformation result type */
    int type;
    /** Result for type XPATH_RESULTS_XML */
    std::string xml;
    /** Result for type XPATH_RESULTS_BOOLEAN */
    int boolValue;
    /** Result for type XPATH_RESULTS_NUMBER */
    double numberValue;
    /** Result for type XPATH_RESULTS_STRING */
    std::string stringValue;
};

/**
 * Apply the XPath from the processor to document. If parameter
 * result is NULL, result replaces the unit that the transformation
 * was performed on. If parameter result is not NULL, results
 * are placed in the proper field of the result, with the result_type
 * parameter indicating which is appropriate.
 *
 * @param processor A configured XPath processor with a document
 * @param xpath The XPath to apply to the document
 * @param results Optional struct of different result types
 * @returns Returns XPATH_STATUS_OK on success and a status error codes on failure.
 */
int xpath_apply(xpath_processor* processor, const std::string& xpath, xpath_result_t* result);

/** 
 * Free an xpath_processor that was previously allocated y using xpath_create()
 * The processor must be reallocated/re-created to use again.
 * 
 * @param processor An XPath processor
 */
void xpath_free(xpath_processor* processor);

#endif
