/*
    ParserFilter.cpp

    Parsing filter for XMLFramework
*/

#include "ParserFilter.hpp"
#include "FilterFactory.hpp"
#include "XMLDocumentParser.hpp"
#include "XMLDocumentTemplateParser.hpp"
#include "XMLParserFactory.hpp"
#include "XMLDocumentHandlerParser.hpp"
#include <memory>

// Register class in the FilterFactory
bool ParserFilter::registered = FilterFactory::Register("input", "XML parsing", ParserFilter::factory);

/*
    Constructor

    @param filename Filename for action to parse
*/
ParserFilter::ParserFilter(std::string_view filename)
    : inputFilename(filename) {
}

/*
    Apply an XML parser to the XML document

    @param doc Ignored starting document
    @return XML document from parsing file from stdin
*/
XMLDocument ParserFilter::action(XMLDocument document) {
    int size = sizeof(document); // 32
    XMLParserFactory* parserType = XMLParserFactory::instance();
    parserType->factory(size); // returns std::unique_put<XMLDocumentParser>
    
    // parse using an XMLDocument parser
    std::unique_ptr<XMLDocumentParser> parser(new XMLDocumentTemplateParser);
    return parser->parse(inputFilename);
}

/*
    Factory method

    @param xpath XPath expression applied to the document
    @return ParserFilter object
*/
std::unique_ptr<Filter> ParserFilter::factory(std::string_view xpath) {

    return std::make_unique<ParserFilter>(xpath);
}
