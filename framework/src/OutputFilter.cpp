/*
    OutputFilter.cpp

    Output filter for XMLFramework
*/

#include "OutputFilter.hpp"
#include "FilterFactory.hpp"
#include <iostream>
#include <fstream>

// Register class in the FilterFactory
bool OutputFilter::registered = FilterFactory::Register("output", "Output to file", OutputFilter::factory);

/*
    Constructor

    @param filename Filename for output
*/
OutputFilter::OutputFilter(std::string_view filename)
    : outputFilename(filename) {
}

/*
    Output the XML document to a file

    @param doc Input XML document
    @return Input XML document
*/
XMLDocument OutputFilter::action(XMLDocument document) {

    // output to standard output if "-", otherwise to file
    if (outputFilename == "-") {
        std::cout << document;
    } else {
        std::ofstream out(outputFilename.c_str());
        out << document;
    }

    return document;
}

/*
    Does the filter perform output
*/
bool OutputFilter::isOutput() {
    return true;
}

 /*
    Factory method

    @param outputFilename Output filename
    @return OutputFilter object
*/
std::unique_ptr<Filter> OutputFilter::factory(std::string_view outputFilename) {

    return std::make_unique<OutputFilter>(outputFilename);
}
