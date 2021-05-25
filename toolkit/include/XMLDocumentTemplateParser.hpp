/*
    XMLDocumentTemplateParser.hpp

    Parses XML into an XMLDocument
*/

#ifndef INCLUDED_XMLDOCUMENTTEMPLATEPARSER_HPP
#define INCLUDED_XMLDOCUMENTTEMPLATEPARSER_HPP

#include "XMLDocumentParser.hpp"

// @pattern Template Method/Facade @role Concrete Class/Subsystem Class
class XMLDocumentTemplateParser : public XMLDocumentParser {
public:
    /*
        Parse the file into an XMLDocument

        @param filename Filename of the file to parse
    */
    virtual XMLDocument parse(std::string_view filename);
};

#endif
