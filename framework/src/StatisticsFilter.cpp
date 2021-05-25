/*
    StatisticsFilter.hpp

    Statistics filter for XMLFramework
*/

#include "FilterFactory.hpp"
#include "StatisticsFilter.hpp"
#include <string>

// Register class in the FilterFactory
bool StatisticsFilter::registered = FilterFactory::isRegistered("statistics");

// Outputs stats of XMLDocument
std::unique_ptr<Filter> StatisticsFilter::displayStats() {
    int decl_count = 0;
    int start_tag_count = 0;
    int end_tag_count = 0;
    int attribute_count = 0;
    int character_section_count = 0;
    int reference_count = 0;
    int namespace_count = 0;
    int comment_count = 0;
    int cdata_count = 0;

    /*
    XMLParser statsParser (nullptr, nullptr);

    statsParser.parse(total, expr_count, function_count, decl_count, class_count, 
                 file_count, comment_count, return_count, loc, literal_count,
                 line_comment_count, textsize);
    */

    // Number of declarations
    std::cout << "Declarations: " << decl_count << std::endl;

    // Number of start tags
    std::cout << "Start Tags: " << start_tag_count << std::endl;
    
    // Number of end tags
    std::cout << "End Tags: " << end_tag_count << std::endl;
    
    // Number of attributes
    std::cout << "Attributes: " << attribute_count << std::endl;
    
    // Number of character sections (not just characters)
    std::cout << "Character Sections: " << character_section_count << std::endl;
    
    // Number of entity references
    std::cout << "Entity References: " << reference_count << std::endl;
    
    // Number of XML namespaces
    std::cout << "Namespaces: " << namespace_count << std::endl;
    
    // Number of XML comments
    std::cout << "Comments: " << comment_count << std::endl;
    
    // Number of CDATA
    std::cout << "CDATA: " << cdata_count << std::endl;
    
    return nullptr;

}
