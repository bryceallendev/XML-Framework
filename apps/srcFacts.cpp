/*
    srcFacts.cpp
*/

#include "XMLParser.hpp"
#include "XMLParserHandler.hpp"
#include <iostream>
#include <string>

int main() {
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
    
    XMLParserHandler handler;
    
    XMLParser parser(handler);
    
    //handler.parse();
    
    parser.parse(total, expr_count, function_count, decl_count, class_count, 
                 file_count, comment_count, return_count, loc, literal_count,
                 line_comment_count, textsize);

    return 0;
}
