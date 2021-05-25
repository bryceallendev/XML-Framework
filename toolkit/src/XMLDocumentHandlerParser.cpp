#include "XMLDocumentHandlerParser.hpp"
#include "XMLParserDocumentHandler.hpp"
#include "XMLParser.hpp"
#include <string>

XMLDocument XMLDocumentHandlerParser::parse(const std::string_view filename) {
    std::string url;
    int textsize = 0;
    int loc = 0;
    int expr_count = 0;
    int function_count = 0;
    int class_count = 0;
    int file_count = 0;
    int decl_count = 0;
    int comment_count = 0;
    int line_comment_count = 0;
    int return_count = 0;
    int literal_count = 0;
    long total = 0;
    
    XMLDocument document;
    
    XMLParser parser(handler);
    
    
    parser.parse(total, expr_count, function_count, decl_count, class_count, 
                 file_count, comment_count, return_count, loc, literal_count,
                 line_comment_count, textsize);
    
    return document;
}