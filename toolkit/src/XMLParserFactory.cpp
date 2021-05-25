/*
    XMLParserFactory.cpp

    Creates XMLDocumentParser's
*/

#include "XMLParserFactory.hpp"
#include "XMLDocumentTemplateParser.hpp"
#include "XMLDocumentHandlerParser.hpp"

/*
   Factory method to create XMLDocumentParser
*/
std::unique_ptr<XMLDocumentParser> XMLParserFactory::factory(int size) {

    if (size < 1000) {
        return std::unique_ptr<XMLDocumentHandlerParser>();
    }
    else {
        return std::unique_ptr<XMLDocumentTemplateParser>();
    }
}

// constructor
XMLParserFactory::XMLParserFactory() {
    
}

// Create pointer instance of XMLParserFactory
XMLParserFactory* XMLParserFactory::pinstance { nullptr };

// Create single instance of XMLParserFactory
XMLParserFactory* XMLParserFactory::instance() {
    if (pinstance == nullptr) {
        pinstance = new XMLParserFactory;
    }
    
    return pinstance;
}