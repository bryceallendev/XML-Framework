/*
    XMLParserDocumentHandler.hpp

    Declaration file for the XMLParserHandler for XMLDocument
*/

#ifndef INCLUDED_XMLPARSERDOCUMENTHANDLER_HPP
#define INCLUDED_XMLPARSERDOCUMENTHANDLER_HPP

#include "XMLParserHandler.hpp"

// @pattern Template Method @role Concrete Class
class XMLParserDocumentHandler : public XMLParserHandler {
public:

    // constructor
    XMLParserDocumentHandler();

    // destructor
    virtual ~XMLParserDocumentHandler() = default;

};

#endif
