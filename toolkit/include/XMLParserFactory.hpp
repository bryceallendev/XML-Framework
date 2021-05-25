/*
    XMLParserFactory.hpp

    Creates XMLDocumentParser's
*/

#ifndef INCLUDED_XMLPARSERFACTORY_HPP
#define INCLUDED_XMLPARSERFACTORY_HPP

#include <memory>
#include "XMLDocumentParser.hpp"

// @pattern Factory Method/Singleton @role Creator
class XMLParserFactory {
public:
    // Create single instance of XMLParserFactory
    static XMLParserFactory* instance();

    /*
       Factory method to create XMLDocumentParser
    */
    static std::unique_ptr<XMLDocumentParser> factory(int size);
    
protected:
    XMLParserFactory();
private:
    // Create pointer instance of XMLParserFactory
    static XMLParserFactory* pinstance;


};

#endif
