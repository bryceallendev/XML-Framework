/*
    XMLDocumentParser.hpp

    Parses XML into an XMLDocument
*/

#ifndef INCLUDED_XMLDOCUMENTPARSER_HPP
#define INCLUDED_XMLDOCUMENTPARSER_HPP

#include "XMLDocument.hpp"
#include <string_view>

// @pattern Template Method/Facade @role Abstract Class/Subsystem Class
class XMLDocumentParser {
public:
    /*
        Parse the file into an XMLDocument

        @param filename Filename of the file to parse
    */
    virtual XMLDocument parse(const std::string_view filename) = 0;

    // destructor
    virtual ~XMLDocumentParser() = default;
};

#endif
