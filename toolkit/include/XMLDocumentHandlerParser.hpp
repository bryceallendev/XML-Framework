/*
    XMLDocumentHandlerParser.hpp

    Parses XML into an XMLDocument
*/

#ifndef INCLUDED_XMLDOCUMENTHANDLERPARSER_HPP
#define INCLUDED_XMLDOCUMENTHANDLERPARSER_HPP

#include "XMLDocumentParser.hpp"
#include "XMLParserDocumentHandler.hpp"

// @pattern Template Method/Facade @role Concrete Class/Subsystem Class
class XMLDocumentHandlerParser : public XMLDocumentParser {
public:
    /*
        Parse the file into an XMLDocument

        @param filename Filename of the file to parse
    */
    virtual XMLDocument parse(std::string_view filename);

private:
    XMLParserDocumentHandler& handler;
};

#endif
