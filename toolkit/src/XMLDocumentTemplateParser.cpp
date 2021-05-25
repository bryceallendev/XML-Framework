/*
    XMLDocumentTemplateParser.cpp

    Parses XML into an XMLDocument
*/

#include "XMLDocumentTemplateParser.hpp"
#include "template_xml_parser.hpp"

#if !defined(_MSC_VER)
#include <sys/uio.h>
#include <unistd.h>
#else
#include <BaseTsd.h>
#include <io.h>
#endif

#include <fstream>
#include <string_view>
#include <fcntl.h>
#include <memory>
#include <iostream>
#include <fstream>
#include <sstream>

// parse the input XML into an XMLDocument
XMLDocument XMLDocumentTemplateParser::parse(std::string_view filename) {

    // open the file as an input stream
    std::string sfilename(filename);
    std::ifstream in(sfilename);

    // parse into XML document
    std::string document;
    bool inStartTag = false;
    TemplateXMLParser::template_xml_parser(in,

        // XML declaration
        [&document](std::string_view version, std::string_view encoding, std::string_view standalone) {

            // add XML declaration
            document += "<?xml version=\"";
            document += version;
            document += "\" encoding=\"";
            document += encoding;
            document += "\" standalone=\"";
            document += standalone;
            document += "\"?>\n";
        },

        // start tag
        [&document, &inStartTag](std::string_view qname, std::string_view /* local_name */, std::string_view /* prefix */) {

            // end open start tag
            if (inStartTag) {
                document += ">";
                inStartTag = false;
            }

            // add start tag name
            document += "<";
            document += qname;

            inStartTag = true;
        },

        // end tag
        [&document, &inStartTag](std::string_view qname, std::string_view /* local_name */, std::string_view /* prefix */) {

            // end empty element
            if (inStartTag) {
                document += "/>";
                inStartTag = false;
                return;
            }

            // add end tag
            document += "</";
            document += qname;
            document += ">";
        },

        // characters
        [&document, &inStartTag](std::string_view content) {

            // end open start tag
            if (inStartTag) {
                document += ">";
                inStartTag = false;
            }

            // escape content
            for (char c : content) {
                if (c == '<')
                    document += "&lt;";
                else if (c == '>')
                    document += "&gt;";
                else if (c == '&')
                    document += "&amp;";
                else
                    document += c;
            }
        },

        // attribute
        [&document](std::string_view qname, std::string_view /* local_name */, std::string_view /* prefix */, std::string_view value) {

            // add attribute
            document += " ";
            document += qname;
            document += "=\"";
            document += value;
            document += "\"";
        },

        // namespace declaration
        [&document](std::string_view prefix, std::string_view /* name */, std::string_view url) {

            // add namespace declaration
            document += " xmlns";
            if (!prefix.empty()) {
                document += ":";
                document += prefix;
            }
            document += "=\"";
            document += url;
            document += "\"";
        },

        // XML comment
        [&document, &inStartTag](std::string_view content) {

            // end open start tag
            if (inStartTag) {
                document += ">";
                inStartTag = false;
            }

            // add XML comment
            document += "<!--";
            document += content;
            document += "-->";
        },

        // CDATA
        [&document, &inStartTag](std::string_view content) {

            // end open start tag
            if (inStartTag) {
                document += ">";
                inStartTag = false;
            }

            // add CDATA section
            document += "<![CDATA[";
            document += content;
            document += "]]>";
        }
    );

    // add final newline
    document += "\n";

    return XMLDocument(std::move(document));
}
