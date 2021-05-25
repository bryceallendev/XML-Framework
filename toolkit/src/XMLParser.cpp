/*
    XMLParser.cpp

    Parser for XMLFramework
*/

#include "XMLParser.hpp"
#include "XMLParserHandler.hpp"
#include "XMLParser.hpp"
#include "refillBuffer.hpp"

#include <iostream>
#include <cstring>
#include <sstream>
#include <algorithm>

const int BUFFER_SIZE = 16 * 16 * 4096;
const int XMLNS_SIZE = strlen("xmlns");

// constructor with handler
XMLParser::XMLParser(XMLParserHandler& handler)
    : handler(handler) {
    
    const int BUFFER_SIZE = 16 * 16 * 4096;
    std::string oldBuffer(BUFFER_SIZE, ' ');
    buffer = oldBuffer;
    
    pc = buffer.cend();
}

// Iterator matches end of buffer
bool XMLParser::isEnd() {
    return pc == buffer.cend();
}

// XML parsing is inside a tag
bool XMLParser::isInside () {
    
    return intag && *pc != '>' && *pc != '/' && std::distance(pc, buffer.cend()) > (int) XMLNS_SIZE && std::string(pc, std::next(pc, XMLNS_SIZE)) == "xmlns"
        && (*std::next(pc, XMLNS_SIZE) == ':' || *std::next(pc, XMLNS_SIZE) == '=');
}

// XML parsing is inside a start tag
bool XMLParser::isInsideStartTag() {
    
    return intag && *pc == '>';
}

// XML parsing is inside an endtag
bool XMLParser::isInsideEndTag() {
    
    return intag && *pc == '/' && *std::next(pc) == '>';
}

// XML parsing determines distance
bool XMLParser::isDistance() {
    
    return std::distance(pc, buffer.cend()) < 5;
}

// XML parsing is at the start of declaration
bool XMLParser::isDeclarationStart() {
    
    return *pc == '<' && *std::next(pc) == '?';
}

// XML parses the end tag
bool XMLParser::isEndTag() {
    
    return *pc == '<' && *std::next(pc) == '/';
}

// XML parses the start tag
bool XMLParser::isStartTag() {
    
   return *pc == '<' && *std::next(pc) != '/' && *std::next(pc) != '?';
}

// XML parses the attribute
bool XMLParser::isAttribute() {
    
    return intag && *pc != '>' && *pc != '/';
}

// XML parses CDATA
bool XMLParser::isCDATA() {
    
    return *pc == '<' && *std::next(pc) == '!' && *std::next(pc, 2) == '[';
}

// XML parses a comment
bool XMLParser::isComment() {
    
    return *pc == '<' && *std::next(pc) == '!' && *std::next(pc, 2) == '-' && *std::next(pc, 3) == '-';
}

// XML parses characters outside of XML
bool XMLParser::isExternalCharacter() {
    
    return *pc != '<' && depth == 0;
}

// XML parses a reference
bool XMLParser::isEntityReference() {
    
    return *pc == '&';
}

// XML parses a character
bool XMLParser::isCharacter() {
    
    return *pc != '<';
}

//Parse XML
void XMLParser::parse(long& total, int& expr_count, int& function_count, int& decl_count, int& class_count, 
                      int& file_count, int& comment_count, int& return_count, int& loc, int& literal_count, 
                      int& line_comment_count, int& textsize) {
    while (true) {
        if (isDistance()) {
            // refill buffer and adjust iterator
            adjustBuffer(total);
            if (isEnd())
                break;
        } else if (isDeclarationStart()) {
            // parse XML declaration
            parseDeclaration(total);
        } else if (isEndTag()) {
            // parse end tag
            parseEndTag(total);
        } else if (isStartTag()) {
            // parse start tag
            parseStartTag(total, expr_count, function_count, decl_count, class_count, 
                file_count, comment_count, literal_count, return_count);
        } else if (isInside()) {
           // parse namespace
            parseNamespace();
            if (isInsideStartTag()) {
                skipOneCharacter();
            }
            if (isInsideEndTag()) {
                skipTwoCharacters();
            }
        } else if (isAttribute()) {
            // parse attribute
            parseAttributes(line_comment_count, literal_count);
        } else if (isCDATA()) {
            // parse CDATA
            parseCDATA(total, loc, textsize);
        } else if (isComment()) {
            // parse XML comment
            parseComments(total);
        } else if (isExternalCharacter()) {
            // parse characters before or after XML
            parseExternalCharacters();
        } else if (isEntityReference()) {
            // parse entity references
            parseEntityReferences(total, textsize);
        } else if (isCharacter()) {
            countCharacters(loc, textsize);
        }
    }
}

// Adjust Buffer
void XMLParser::adjustBuffer(long& totalBytes) {
    pc = refillBuffer(pc, buffer, totalBytes);
    
    //return pc;
}

// Parse comments
void XMLParser::parseComments(long& totalBytes) {
    const std::string endcomment = "-->";
    auto endpc = std::search(pc, buffer.cend(), endcomment.begin(), endcomment.end());
    if (endpc == buffer.cend()) {
        pc = refillBuffer(pc, buffer, totalBytes);
        endpc = std::search(pc, buffer.cend(), endcomment.begin(), endcomment.end());
        if (endpc == buffer.cend()) {
            std::cerr << "parser error : Unterminated XML comment\n";
            exit(1);
        }
    }
    pc = std::next(endpc, strlen("-->"));
    pc = std::find_if_not(pc, buffer.cend(), [] (char c) { return isspace(c); });
    
    //return pc;
}

// Parse attributes
void XMLParser::parseAttributes(int& line_comment_count, int& literal_count) {
    const auto endpc = std::find(pc, buffer.cend(), '>');
    auto pnameend = std::find(pc, std::next(endpc), '=');
    if (pnameend == std::next(endpc))
    {
        exit(1);
    }
    const std::string qname(pc, pnameend);
    const auto colonpos = qname.find(':');
    std::string prefixbase;
    if (colonpos != std::string::npos)
        prefixbase = qname.substr(0, colonpos);
    const std::string prefix = std::move(prefixbase);
    std::string local_namebase;
    if (colonpos != std::string::npos)
        local_namebase = qname.substr(colonpos + 1);
    else
        local_namebase = qname;
    std::string local_name = std::move(local_namebase);
    pc = std::next(pnameend);
    pc = std::find_if_not(pc, std::next(endpc), [] (char c) { return isspace(c); });
    if (pc == buffer.cend()) {
        std::cerr << "parser error : attribute " << qname << " incomplete attribute\n";
        exit(1);
    }
    char delim = *pc;
    if (delim != '"' && delim != '\'') {
        std::cerr << "parser error : attribute " << qname << " missing delimiter\n";
        exit(1);
    }
    std::advance(pc, 1);
    auto pvalueend = std::find(pc, std::next(endpc), delim);
    if (pvalueend == std::next(endpc)) {
        std::cerr << "parser error : attribute " << qname << " missing delimiter\n";
        exit(1);
    }
    const std::string value(pc, pvalueend);
    if (local_name == "url")
    {
        url = value;
    }
    else if (local_name == "type")
    {
        if(value == "line")
        {
            ++line_comment_count;
        }
        if(value == "string")
        {
            ++literal_count;
        }
        
    }
    pc = std::next(pvalueend);
    pc = std::find_if_not(pc, std::next(endpc), [] (char c) { return isspace(c); });
    if (intag && *pc == '>') {
        std::advance(pc, 1);
        intag = false;
    }
    if (intag && *pc == '/' && *std::next(pc) == '>') {
        std::advance(pc, 2);
        intag = false;
    }
    
    //return pc;
}

// Parse namespace
void XMLParser::parseNamespace() {
   std::advance(pc, XMLNS_SIZE);
    const auto endpc = std::find(pc, buffer.cend(), '>');
    auto pnameend = std::find(pc, std::next(endpc), '=');
    if (pnameend == std::next(endpc)) {
        std::cerr << "parser error : incomplete namespace\n";
        exit(1);
    }
    //pc = pnameend;
    std::string prefix;
    if (*pc == ':') {
        std::advance(pc, 1);
        prefix.assign(pc, pnameend);
    }
    pc = std::next(pnameend);
    pc = std::find_if_not(pc, std::next(endpc), [] (char c) { return isspace(c); });
    if (pc == std::next(endpc)) {
        std::cerr << "parser error : incomplete namespace\n";
        exit(1);
    }
    const char delim = *pc;
    if (delim != '"' && delim != '\'') {
        std::cerr << "parser error : incomplete namespace\n";
        exit(1);
    }
    std::advance(pc, 1);
    auto pvalueend = std::find(pc, std::next(endpc), delim);
    if (pvalueend == std::next(endpc)) {
        std::cerr << "parser error : incomplete namespace\n";
        exit(1);
    }
    const std::string uri(pc, pvalueend);
    pc = std::next(pvalueend);
    pc = std::find_if_not(pc, std::next(endpc), [] (char c) { return isspace(c); });

    //return pc;
}

// Parse end tag
void XMLParser::parseEndTag(long& totalBytes) {
    --depth;
    auto endpc = std::find(pc, buffer.cend(), '>');
    if (endpc == buffer.cend()) {
        pc = refillBuffer(pc, buffer, totalBytes);
        endpc = std::find(pc, buffer.cend(), '>');
        if (endpc == buffer.cend()) {
            std::cerr << "parser error: Incomplete element end tag\n";
            exit(1);
        }
    }
    std::advance(pc, 2);
    auto pnameend = std::find_if(pc, std::next(endpc), [] (char c) { return isspace(c) || c == '>' || c == '/'; });
    if (pnameend == std::next(endpc)) {
          std::cerr << "parser error: Incomplete element end tag name\n";
          exit(1);
    }
    const std::string qname(pc, pnameend);
    const auto colonpos = qname.find(':');
    std::string prefixbase;
    if (colonpos != std::string::npos)
        prefixbase = qname.substr(0, colonpos);
    const std::string prefix = std::move(prefixbase);
    std::string local_namebase;
    if (colonpos != std::string::npos)
        local_namebase = qname.substr(colonpos + 1);
    else
        local_namebase = qname;
    const std::string local_name = std::move(local_namebase);
    pc = std::next(endpc);
    
    //return pc;
}

// Parse declaration
void XMLParser::parseDeclaration(long& totalBytes) {
    auto endpc = std::find(pc, buffer.cend(), '>');
    if (endpc == buffer.cend()) {
        pc = refillBuffer(pc, buffer, totalBytes);
        endpc = std::find(pc, buffer.cend(), '>');
        if (endpc == buffer.cend()) {
            std::cerr << "parser error: Incomplete XML declaration\n";
            exit(1);
        }
    }
    std::advance(pc, strlen("<?xml"));
    pc = std::find_if_not(pc, endpc, [] (char c) { return isspace(c); });
    
    // parse required version
    if (pc == endpc) {
        std::cerr << "parser error: Missing space after before version in XML declaration\n";
        exit(1);
    }
    auto pnameend = std::find(pc, endpc, '=');
    const std::string attr(pc, pnameend);
    pc = pnameend;
    std::advance(pc, 1);
    char delim = *pc;
    if (delim != '"' && delim != '\'') {
        std::cerr << "parser error: Invalid start delimiter for version in XML declaration\n";
        exit(1);
    }
    std::advance(pc, 1);
    auto pvalueend = std::find(pc, endpc, delim);
    if (pvalueend == endpc) {
        std::cerr << "parser error: Invalid end delimiter for version in XML declaration\n";
        exit(1);
    }
    if (attr != "version") {
        std::cerr << "parser error: Missing required first attribute version in XML declaration\n";
        exit(1);
    }
    const std::string version(pc, pvalueend);
    pc = std::next(pvalueend);
    pc = std::find_if_not(pc, endpc, [] (char c) { return isspace(c); });
    // parse encoding
    if (pc == endpc) {
        std::cerr << "parser error: Missing required encoding in XML declaration\n";
        exit(1);
    }
    pnameend = std::find(pc, endpc, '=');
    if (pnameend == endpc) {
        std::cerr << "parser error: Incomple encoding in XML declaration\n";
        exit(1);
    }
    const std::string attr2(pc, pnameend);
    pc = pnameend;
    std::advance(pc, 1);
    char delim2 = *pc;
    if (delim2 != '"' && delim2 != '\'') {
        std::cerr << "parser error: Invalid end delimiter for encoding in XML declaration\n";
        exit(1);
    }
    std::advance(pc, 1);
    pvalueend = std::find(pc, endpc, delim2);
    if (pvalueend == endpc) {
        std::cerr << "parser error: Incomple encoding in XML declaration\n";
        exit(1);
    }
    if (attr2 != "encoding") {
         std::cerr << "parser error: Missing required encoding in XML declaration\n";
         exit(1);
    }
    const std::string encoding(pc, pvalueend);
    pc = std::next(pvalueend);
    pc = std::find_if_not(pc, endpc, [] (char c) { return isspace(c); });
    // parse standalone
    if (pc == endpc) {
        std::cerr << "parser error: Missing required third attribute standalone in XML declaration\n";
        exit(1);
    }
    pnameend = std::find(pc, endpc, '=');
    const std::string attr3(pc, pnameend);
    pc = pnameend;
    std::advance(pc, 1);
    char delim3 = *pc;
    if (delim3 != '"' && delim3 != '\'') {
        std::cerr << "parser error : Missing attribute standalone delimiter in XML declaration\n";
        exit(1);
    }
    std::advance(pc, 1);
    pvalueend = std::find(pc, endpc, delim3);
    if (pvalueend == endpc) {
        std::cerr << "parser error : Missing attribute standalone in XML declaration\n";
        exit(1);
    }
    if (attr3 != "standalone") {
        std::cerr << "parser error : Missing attribute standalone in XML declaration\n";
        exit(1);
    }
    const std::string standalone(pc, pvalueend);
    pc = std::next(pvalueend);
    pc = std::find_if_not(pc, endpc, [] (char c) { return isspace(c); });
    std::advance(pc, strlen("?>"));
    pc = std::find_if_not(pc, buffer.cend(), [] (char c) { return isspace(c); });
    
    //return pc;
}

// Parse start tag
void XMLParser::parseStartTag(long& totalBytes, int& expr_count, int& function_count, int& decl_count, int& class_count, int& file_count, int& comment_count, int& literal_count, int& return_count) {
    auto endpc = std::find(pc, buffer.cend(), '>');
    if (endpc == buffer.cend()) {
        pc = refillBuffer(pc, buffer, totalBytes);
        endpc = std::find(pc, buffer.cend(), '>');
        if (endpc == buffer.cend()) {
            std::cerr << "parser error: Incomplete element start tag\n";
            exit(1);
        }
    }
    std::advance(pc, 1);
    auto pnameend = std::find_if(pc, std::next(endpc), [] (char c) { return isspace(c) || c == '>' || c == '/'; });
    if (pnameend == std::next(endpc)) {
        std::cerr << "parser error : Unterminated start tag '" << std::string(pc, pnameend) << "'\n";
        exit(1);
    }
    const std::string qname(pc, pnameend);
    const auto colonpos = qname.find(':');
    std::string prefixbase;
    if (colonpos != std::string::npos)
        prefixbase = qname.substr(0, colonpos);
    const std::string prefix = std::move(prefixbase);
    std::string local_namebase;
    if (colonpos != std::string::npos)
        local_namebase = qname.substr(colonpos + 1);
    else
        local_namebase = qname;
    const std::string local_name = std::move(local_namebase);
    pc = pnameend;
    if (local_name == "expr")
        ++expr_count;
    else if (local_name == "function")
        ++function_count;
    else if (local_name == "decl")
        ++decl_count;
    else if (local_name == "class")
        ++class_count;
    else if (local_name == "unit" && depth > 0)
        ++file_count;
    else if (local_name == "comment")
    {
        ++comment_count;
    }
    else if (local_name == "literal")
    {
        //++literal_count;
    }
    else if (local_name == "return")
        ++return_count;
    pc = std::find_if_not(pc, std::next(endpc), [] (char c) { return isspace(c); });
    ++depth;
    intag = true;
    if (intag && *pc == '>') {
        std::advance(pc, 1);
        intag = false;
    }
    //call the callback for the start tag (needed)
    
    if (intag && *pc == '/' && *std::next(pc) == '>') {
        // call the callback for the end tag (needed)
        
        std::advance(pc, 2);
        intag = false;
        --depth;
    }
}

// Parse external characters
void XMLParser::parseExternalCharacters() {
    pc = std::find_if_not(pc, buffer.cend(), [] (char c) { return isspace(c); });
    if (pc == buffer.cend() || !isspace(*pc)) {
        std::cerr << "parser error : Start tag expected, '<' not found\n";
        exit(1);
    }
    
    //return pc;
}

// Count parsed characters
void XMLParser::countCharacters(int& loc, int& textsize) {
    const auto endpc = std::find_if(pc, buffer.cend(), [] (char c) { return c == '<' || c == '&'; });
    const std::string characters(pc, endpc);
    loc += (int) std::count(characters.cbegin(), characters.cend(), '\n');
    textsize += (int) characters.size();
    
    pc = endpc;
}

// Parse CDATA
void XMLParser::parseCDATA(long& totalBytes, int& loc, int& textsize) {
    const std::string endcdata = "]]>";
    std::advance(pc, strlen("<![CDATA["));
    auto endpc = std::search(pc, buffer.cend(), endcdata.begin(), endcdata.end());
    if (endpc == buffer.cend()) {
        pc = refillBuffer(pc, buffer, totalBytes);
        endpc = std::search(pc, buffer.cend(), endcdata.begin(), endcdata.end());
        if (endpc == buffer.cend())
            exit(1);
    }
    const std::string characters(pc, endpc);
    textsize += (int) characters.size();
    loc += (int) std::count(characters.begin(), characters.end(), '\n');
    //literal_count += ((int) std::count(characters.begin(), characters.end(), '"') / 2);
    //literal_count += ((int) std::count(characters.begin(), characters.end(), '\'') / 2);
    //line_comment_count += (int) std::count(characters.cbegin(), characters.cend(), '/');
    
    pc = std::next(endpc, strlen("]]>"));
}

// Parse entity references
void XMLParser::parseEntityReferences(long& totalBytes, int& textsize) {
    std::string characters;
    if (std::distance(pc, buffer.cend()) < 3) {
        pc = refillBuffer(pc, buffer, totalBytes);
        if (std::distance(pc, buffer.cend()) < 3) {
            std::cerr << "parser error : Incomplete entity reference, '" << std::string(pc, buffer.cend()) << "'\n";
            exit(1);
        }
    }
    if (*std::next(pc) == 'l' && *std::next(pc, 2) == 't' && *std::next(pc, 3) == ';') {
        characters += '<';
        std::advance(pc, strlen("&lt;"));
    } else if (*std::next(pc) == 'g' && *std::next(pc, 2) == 't' && *std::next(pc, 3) == ';') {
        characters += '>';
        std::advance(pc, strlen("&gt;"));
    } else if (*std::next(pc) == 'a' && *std::next(pc, 2) == 'm' && *std::next(pc, 3) == 'p') {
        if (std::distance(pc, buffer.cend()) < 4) {
            pc = refillBuffer(pc, buffer, totalBytes);
            if (std::distance(pc, buffer.cend()) < 4) {
                std::cerr << "parser error : Incomplete entity reference, '" << std::string(pc, buffer.cend()) << "'\n";
                exit(1);
            }
        }
        if (*std::next(pc, 4) != ';') {
            const std::string partialEntity(pc, std::next(pc, 4));
            std::cerr << "parser error : Incomplete entity reference, '" << partialEntity << "'\n";
            exit(1);
        }
        characters += '&';
        std::advance(pc, strlen("&amp;"));
    } else {
        characters += '&';
        std::advance(pc, 1);
    }
    textsize += (int) characters.size();
}

// Skip an input character
void XMLParser::skipOneCharacter() {
    std::advance(pc, 1);
    intag = false;
}

// Skip two input characters
void XMLParser::skipTwoCharacters() {
    std::advance(pc, 2);
    intag = false;
}
