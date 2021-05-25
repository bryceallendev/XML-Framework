/*
    template_xml_parser.hpp

    An XML parser with template handlers
*/

#ifndef INCLUDED_TEMPLATE_XML_PARSER_HPP
#define INCLUDED_TEMPLATE_XML_PARSER_HPP

#include <istream>
#include <iterator>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>
#include <ctype.h>

namespace TemplateXMLParser {

    class XMLDocumentError : public std::runtime_error {
    public:
        XMLDocumentError(const char* what_arg)
            : std::runtime_error(what_arg) {}
    };

    const int BUFFER_SIZE = 16 * 4096;

    /*
        Refill the buffer preserving the unused data.
        Characters [pc, buffer.end()) are shifted left and new data
        is added to the rest of the buffer.

        @param pc Iterator to current position in buffer
        @param buffer Container for characters
        @param totalBytes Updated total bytes read
        @return Iterator to beginning of refilled buffer
    */
    std::vector<char>::const_iterator refillBuffer(std::istream& in, std::vector<char>::const_iterator pc, std::vector<char>& buffer, long& totalBytes);

    // XML parser that reads from stdin
    template <typename DeclarationHandler,
              typename StartTagHandler,
              typename EndTagHandler,
              typename CharacterHandler,
              typename AttributeHandler,
              typename NamespaceHandler,
              typename CommentHandler,
              typename CdataHandler>
    void template_xml_parser(
            std::istream& in,
            DeclarationHandler handleDeclaration,
            StartTagHandler handleStartTag,
            EndTagHandler handleEndTag,
            CharacterHandler handleCharacters,
            AttributeHandler handleAttribute,
            NamespaceHandler handleNamespace,
            CommentHandler handleComment,
            CdataHandler handleCDATA) {

        int depth = 0;
        long total = 0;
        bool intag = false;
        std::vector<char> buffer(BUFFER_SIZE);
        auto pc = buffer.cend();
        while (true) {

            if (std::distance(pc, buffer.cend()) < 5) {

                // refill buffer and adjust iterator to ensure 5 elements
                pc = refillBuffer(in, pc, buffer, total);
                if (pc == buffer.cend())
                    break;

            } else if (*pc == '<' && *std::next(pc) == '?') {

                // parse XML declaration
                std::vector<char>::const_iterator endpc = std::find(pc, buffer.cend(), '>');
                if (endpc == buffer.cend()) {
                    pc = refillBuffer(in, pc, buffer, total);
                    endpc = std::find(pc, buffer.cend(), '>');
                    if (endpc == buffer.cend())
                       throw XMLDocumentError("parser error: Incomplete XML declaration");
                }
                std::advance(pc, strlen("<?xml"));
                pc = std::find_if_not(pc, endpc, [] (char c) { return isspace(c); });
                // parse required version
                if (pc == endpc)
                    throw XMLDocumentError("parser error: Incomplete version in XML declaration");
                std::vector<char>::const_iterator pnameend = std::find(pc, endpc, '=');
                const std::string_view attr(&*pc, pnameend - pc);
                pc = pnameend;
                std::advance(pc, 1);
                char delim = *pc;
                if (delim != '"' && delim != '\'')
                    throw XMLDocumentError("parser error: Invalid start delimiter for version in XML declaration");
                std::advance(pc, 1);
                std::vector<char>::const_iterator pvalueend = std::find(pc, endpc, delim);
                if (pvalueend == endpc)
                    throw XMLDocumentError("parser error: Invalid end delimiter for version in XML declaration");
                if (attr != "version")
                    throw XMLDocumentError("parser error: Missing required version in XML declaration");
                const std::string_view version(&*pc, pvalueend - pc);
                pc = std::next(pvalueend);
                pc = std::find_if_not(pc, endpc, [] (char c) { return isspace(c); });
                // parse encoding
                if (pc == buffer.cend())
                    throw XMLDocumentError("parser error: Missing required encoding in XML declaration");
                pnameend = std::find(pc, endpc, '=');
                const std::string_view attr2(&*pc, pnameend - pc);
                pc = pnameend;
                std::advance(pc, 1);
                char delim2 = *pc;
                if (delim2 != '"' && delim2 != '\'')
                    throw XMLDocumentError("parser error: Missing required encoding in XML declaration");
                std::advance(pc, 1);
                pvalueend = std::find(pc, endpc, delim2);
                if (pvalueend == endpc)
                    throw XMLDocumentError("parser error: Missing required encoding in XML declaration");
                if (attr2 != "encoding")
                    throw XMLDocumentError("parser error: Missing required encoding in XML declaration");

                const std::string_view encoding(&*pc, pvalueend - pc);
                pc = std::next(pvalueend);
                pc = std::find_if_not(pc, endpc, [] (char c) { return isspace(c); });
                // parse standalone
                if (pc == endpc)
                    throw XMLDocumentError("parser error: Missing required standalone in XML declaration");
                pnameend = std::find(pc, endpc, '=');
                const std::string_view attr3(&*pc, pnameend - pc);
                pc = pnameend;
                std::advance(pc, 1);
                char delim3 = *pc;
                if (delim3 != '"' && delim3 != '\'')
                    throw XMLDocumentError("parser error: Missing required standalone in XML declaration");
                std::advance(pc, 1);
                pvalueend = std::find(pc, endpc, delim3);
                if (pvalueend == endpc)
                    throw XMLDocumentError("parser error: Missing required standalone in XML declaration");
                if (attr3 != "standalone")
                    throw XMLDocumentError("parser error: Missing required standalone in XML declaration");
                const std::string_view standalone(&*pc, pvalueend - pc);
                pc = std::next(pvalueend);
                pc = std::find_if_not(pc, endpc, [] (char c) { return isspace(c); });
                std::advance(pc, strlen("?>"));

                // declaration callback
                handleDeclaration(version, encoding, standalone);

                pc = std::find_if_not(pc, buffer.cend(), [] (char c) { return isspace(c); });

            } else if (*pc == '<' && *std::next(pc) == '/') {
                // parse end tag
                --depth;
                if (std::distance(pc, buffer.cend()) < 2)
                    pc = refillBuffer(in, pc, buffer, total);
                std::advance(pc, 2);
                std::vector<char>::const_iterator pnameend = std::find_if(pc, buffer.cend(), [] (char c) { return isspace(c) || c == '>' || c == '/'; });
                if (pnameend == buffer.cend()) {
                    pc = refillBuffer(in, pc, buffer, total);
                    pnameend = std::find_if(pc, buffer.cend(), [] (char c) { return isspace(c) || c == '>' || c == '/'; });
                    if (pnameend == buffer.cend())
                        throw XMLDocumentError("parser error: incomplete end tag");
                }
                const std::string_view qname(&*pc, pnameend - pc);
                const auto colonpos = qname.find(':');
                std::string_view prefix;
                if (colonpos != std::string_view::npos)
                    prefix = qname.substr(0, colonpos);
                std::string_view local_name;
                if (colonpos != std::string_view::npos)
                    local_name = qname.substr(colonpos + 1);

                // end tag callback
                handleEndTag(qname, prefix, local_name);

                pc = pnameend;
                std::vector<char>::const_iterator endpc = std::find(pc, buffer.cend(), '>');
                if (endpc == buffer.cend()) {
                    pc = refillBuffer(in, pc, buffer, total);
                    endpc = std::find(pc, buffer.cend(), '>');
                    if (endpc == buffer.cend())
                        throw XMLDocumentError("parser error: incomplete end tag");
                }
                pc = std::next(endpc);

            } else if (*pc == '<' && *std::next(pc) != '/' && *std::next(pc) != '?') {
                // parse start tag
                std::advance(pc, 1);
                std::vector<char>::const_iterator pnameend = std::find_if(pc, buffer.cend(), [] (char c) { return isspace(c) || c == '>' || c == '/'; });
                if (pnameend == buffer.cend()) {
                    pc = refillBuffer(in, pc, buffer, total);
                    pnameend = std::find_if(pc, buffer.cend(), [] (char c) { return isspace(c) || c == '>' || c == '/'; });
                    if (pnameend == buffer.cend()) {
                        std::string errorMsg = "parser error : Unterminated start tag '";
                        errorMsg += std::string(pc, pnameend);
                        errorMsg += "'\n";
                        throw XMLDocumentError(errorMsg.c_str());
                    }
                }
                const std::string_view qname(&*pc, pnameend - pc);
                const auto colonpos = qname.find(':');
                std::string_view prefix;
                if (colonpos != std::string_view::npos)
                    prefix = qname.substr(0, colonpos);
                std::string_view local_name;
                if (colonpos != std::string_view::npos)
                    local_name = qname.substr(colonpos + 1);

                // start tag callback
                handleStartTag(qname, prefix, local_name);

                pc = pnameend;
                pc = std::find_if_not(pc, buffer.cend(), [] (char c) { return isspace(c); });
                ++depth;
                intag = true;
                if (intag && *pc == '>') {
                    std::advance(pc, 1);
                    intag = false;
                }
                if (intag && *pc == '/' && *std::next(pc) == '>') {
                    std::advance(pc, 2);
                    intag = false;
                    --depth;
                }
                if (intag) {
                    std::vector<char>::const_iterator endpc = std::find(pc, buffer.cend(), '>');
                    if (endpc == buffer.cend()) {
                        pc = refillBuffer(in, pc, buffer, total);
                        endpc = std::find(pc, buffer.cend(), '>');
                    }
                    if (endpc == buffer.cend())
                        throw XMLDocumentError("parser error: incomplete start tag");
                }
            } else if (intag && *pc != '>' && *pc != '/' && (int) std::distance(pc, buffer.cend()) > strlen("xmlns") && std::string(pc, std::next(pc, strlen("xmlns"))) == "xmlns"
                && (*std::next(pc, strlen("xmlns")) == ':' || *std::next(pc, strlen("xmlns")) == '=')) {
                // parse namespace
                std::advance(pc, strlen("xmlns"));
                std::vector<char>::const_iterator pnameend = std::find(pc, buffer.cend(), '=');
                if (pnameend == buffer.cend())
                    throw XMLDocumentError("parser error: incomplete namespace");
                pc = pnameend;
                std::string_view prefix;
                if (*pc == ':') {
                    std::advance(pc, 1);
                    prefix = std::string_view(&*pc, pnameend - pc);
                }
                pc = std::next(pnameend);
                pc = std::find_if_not(pc, buffer.cend(), [] (char c) { return isspace(c); });
                if (pc == buffer.cend()) {
                    pc = refillBuffer(in, pc, buffer, total);
                    pc = std::find_if_not(pc, buffer.cend(), [] (char c) { return isspace(c); });
                    if (pc == buffer.cend())
                        throw XMLDocumentError("parser error: incomplete namespace");
                }
                char delim = *pc;
                if (delim != '"' && delim != '\'')
                    throw XMLDocumentError("parser error: incomplete namespace");
                std::advance(pc, 1);
                std::vector<char>::const_iterator pvalueend = std::find(pc, buffer.cend(), delim);
                if (pvalueend == buffer.cend()) {
                    pc = refillBuffer(in, pc, buffer, total);
                    pvalueend = std::find(pc, buffer.cend(), delim);
                    if (pvalueend == buffer.cend())
                        throw XMLDocumentError("parser error: incomplete namespace");
                }
                const std::string_view uri(&*pc, pvalueend - pc);

                // only needed 2 parameters (prefix, uri) but specified (prefix, name, uri)
                // so pass prefix in place of name
                handleNamespace(prefix, prefix, uri);

                pc = std::next(pvalueend);
                pc = std::find_if_not(pc, buffer.cend(), [] (char c) { return isspace(c); });
                if (intag && *pc == '>') {
                    std::advance(pc, 1);
                    intag = false;
                }
                if (intag && *pc == '/' && *std::next(pc) == '>') {
                    std::advance(pc, 2);
                    intag = false;
                }
            } else if (intag && *pc != '>' && *pc != '/') {
                // parse attribute
                std::vector<char>::const_iterator pnameend = std::find(pc, buffer.cend(), '=');
                if (pnameend == buffer.cend())
                    throw XMLDocumentError("parser error: incomplete attribute");
                const std::string_view qname(&*pc, pnameend - pc);
                const auto colonpos = qname.find(':');
                std::string empty;
                const std::string_view prefix     = colonpos != std::string::npos ? qname.substr(0, colonpos) : empty;
                const std::string_view local_name = colonpos != std::string::npos ? qname.substr(colonpos + 1) : qname;
                pc = std::next(pnameend);
                pc = std::find_if_not(pc, buffer.cend(), [] (char c) { return isspace(c); });
                if (pc == buffer.cend())
                    throw XMLDocumentError("parser error: incomplete attribute");
                char delim = *pc;
                if (delim != '"' && delim != '\'') {
                    std::string errorMsg = "parser error : attribute ";
                    errorMsg += qname;
                    errorMsg += " missing delimiter";
                    throw XMLDocumentError(errorMsg.c_str());
                }
                std::advance(pc, 1);
                std::vector<char>::const_iterator pvalueend = std::find(pc, buffer.cend(), delim);
                if (pvalueend == buffer.cend())
                    throw XMLDocumentError("parser error: incomplete attribute");
                const std::string_view value(&*pc, pvalueend - pc);

                // attribute callback
                handleAttribute(qname, local_name, prefix, value);

                pc = std::next(pvalueend);
                pc = std::find_if_not(pc, buffer.cend(), [] (char c) { return isspace(c); });
                if (intag && *pc == '>') {
                    std::advance(pc, 1);
                    intag = false;
                }
                if (intag && *pc == '/' && *std::next(pc) == '>') {
                    std::advance(pc, 2);
                    intag = false;
                }

            } else if (*pc == '<' && *std::next(pc) == '!' && *std::next(pc, 2) == '[') {
                // parse CDATA
                const std::string_view endcdata = "]]>";
                std::advance(pc, strlen("<![CDATA["));
                std::vector<char>::const_iterator endpc = std::search(pc, buffer.cend(), endcdata.begin(), endcdata.end());
                if (endpc == buffer.cend()) {
                    pc = refillBuffer(in, pc, buffer, total);
                    endpc = std::search(pc, buffer.cend(), endcdata.begin(), endcdata.end());
                    if (endpc == buffer.cend())
                        throw XMLDocumentError("parser error: incomplete CDATA section");
                }
                std::string_view characters(&*pc, endpc - pc);

                // cdata callback
                handleCDATA(characters);

                pc = std::next(endpc, strlen("]]>"));
            } else if (*pc == '<' && *std::next(pc) == '!' && *std::next(pc, 2) == '-' && *std::next(pc, 3) == '-') {
                // parse XML comment
                const std::string_view endcomment = "-->";
                std::vector<char>::const_iterator endpc = std::search(pc, buffer.cend(), endcomment.begin(), endcomment.end());

                if (endpc == buffer.cend()) {
                    pc = refillBuffer(in, pc, buffer, total);
                    endpc = std::search(pc, buffer.cend(), endcomment.begin(), endcomment.end());
                    if (endpc == buffer.cend())
                        throw XMLDocumentError("parser error: incomplete comment");
                }
                pc = std::next(endpc, strlen("-->"));

                // comment callback
                handleComment("");

                pc = std::find_if_not(pc, buffer.cend(), [] (char c) { return isspace(c); });
            } else if (*pc != '<' && depth == 0) {
                // parse characters before or after XML
                pc = std::find_if_not(pc, buffer.cend(), [] (char c) { return isspace(c); });
                if (pc == buffer.cend() || !isspace(*pc))
                    throw XMLDocumentError("parser error : Start tag expected, '<' not found");
            } else if (*pc == '&') {
                // parse entity references
                std::string characters;
                if (std::distance(pc, buffer.cend()) < 3) {
                    pc = refillBuffer(in, pc, buffer, total);
                    if (std::distance(pc, buffer.cend()) < 3) {
                        std::string partialEntity(pc, buffer.cend());
                        std::string errorMsg = "parser error : Incomplete entity reference, '";
                        errorMsg += partialEntity;
                        errorMsg += "'";
                        throw XMLDocumentError(errorMsg.c_str());                        
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
                        pc = refillBuffer(in, pc, buffer, total);
                        if (std::distance(pc, buffer.cend()) < 4) {
                            std::string partialEntity(pc, buffer.cend());
                            std::string errorMsg = "parser error : Incomplete entity reference, '";
                            errorMsg += partialEntity;
                            errorMsg += "'";
                            throw XMLDocumentError(errorMsg.c_str());                        
                        }
                    }
                    if (*std::next(pc, 4) != ';') {
                        std::string partialEntity(pc, std::next(pc, 4));
                        std::string errorMsg = "parser error : Incomplete entity reference, '";
                        errorMsg += partialEntity;
                        errorMsg += "'";
                        throw XMLDocumentError(errorMsg.c_str());                        
                    }
                    characters += '&';
                    std::advance(pc, strlen("&amp;"));
                } else {
                    characters += '&';
                    std::advance(pc, 1);
                }

                // characters callback for entity references
                handleCharacters(characters);

            } else if (*pc != '<') {
                // parse characters
                std::vector<char>::const_iterator endpc = std::find_if(pc, buffer.cend(), [] (char c) { return c == '<' || c == '&'; });
                std::string_view characters(&*pc, endpc - pc);

                // characters callback
                handleCharacters(characters);

                pc = endpc;
            }
        }
    }
}

#endif
