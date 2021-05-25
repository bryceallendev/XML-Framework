/*
    XMLParser.hpp

    Parser for XMLFramework
*/

#ifndef INCLUDED_XMLPARSER_HPP
#define INCLUDED_XMLPARSER_HPP

#include "XMLParserHandler.hpp"
#include <string>

// @pattern Facade @role Parser
class XMLParser {
public:

    // constructor
    XMLParser();
    
    // constructor with handler
    XMLParser(XMLParserHandler& handler);

    // destructor
    virtual ~XMLParser() = default;
    
    // parsing function
    void parse();
    
     // Iterator matches end of buffer
    bool isEnd();

    // XML parsing is inside a tag
    bool isInside();

    // XML parsing is inside a start tag
    bool isInsideStartTag();

    // XML parsing is inside an endtag
    bool isInsideEndTag();

    // XML parsing determines distance
    bool isDistance();

    // XML parsing is at the start of declaration
    bool isDeclarationStart();

    // XML parses the end tag
    bool isEndTag();

    // XML parses the start tag
    bool isStartTag();

    // XML parses the attribute
    bool isAttribute();

    // XML parses CDATA
    bool isCDATA();

    // XML parses a comment
    bool isComment();
    
    // XML parses characters outside of XML
    bool isExternalCharacter();

    // XML parsing detects "&"
    bool isEntityReference();

    // XML parsing does not detect "<"
    bool isCharacter();

    //Parse XML
    void parse(long& total, int& expr_count, int& function_count, int& decl_count, int& class_count, 
               int& file_count, int& comment_count, int& return_count, int& loc, int& literal_count, 
               int& line_comment_count, int& textsize);

    // Adjust Buffer
    void adjustBuffer(long& totalBytes);
    
    // Parse comments
    void parseComments(long& totalBytes);

    // Parse attributes
    void parseAttributes(int& line_comment_count, int& literal_count);

    // Parse namespace
    void parseNamespace();

    // Parse end tag
    void parseEndTag(long& totalBytes);

    // Parse declaration
    void parseDeclaration(long& totalBytes);

    // Parse inside tag
    void parseInsideTag();

    // Parse start tag
    void parseStartTag(long& totalBytes, int& expr_count, int& function_count, int& decl_count, 
        int& class_count, int& file_count, int& comment_count, int& literal_count, int& return_count);

    // Parse external characters
    void parseExternalCharacters();

    // Count parsed characters
    void countCharacters(int& loc, int& textsize);

    // Parse CDATA
    void parseCDATA(long& totalBytes, int& loc, int& textsize);

    // Parse entity references
    void parseEntityReferences(long& totalBytes, int& textsize);
    
    // Skip an input character
    void skipOneCharacter();

    // Skip two input characters
    void skipTwoCharacters();
    
private:
    XMLParserHandler& handler;
    bool intag = false;
    int depth = 0;
    std::string buffer;
    std::string::const_iterator pc = buffer.cend();
    std::string url;
};

#endif
