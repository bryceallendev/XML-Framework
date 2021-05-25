/*
    xmlpipeline.cpp

    A pipeline of XML processing. Commands are of the form:

    ./xmlpipeline fragment.cpp.xml --output foo.xml

    Filter names are prefixed by a "--". Filters are registered automatically.
*/

#include <string>
#include <vector>
#include <CLI11.hpp>
#include "XMLPipeline.hpp"
#include "FilterFactory.hpp"

int main(int argc, char* argv[]) {

    // main pipeline to create and execute
    XMLPipeline pipeline;

    // process command-line options
    CLI::App app{"XML pipeline"};

    // positional arguments, i.e., input files
    // done separately so it can be hidden
    app.add_option_function<std::vector<std::string>>("InputFiles", [&](const std::vector<std::string>&) {}, "")
    // hides it in help
    ->group("")
    ->each([&](std::string_view filename) {

        pipeline.addFilter("input", filename);
    });

    // add options for all filters
    for (const auto& filterInfo : FilterFactory::getFilterNames()) {

        // option name is filter name with prefix "--"
        std::string optionName = "--";
        optionName += filterInfo.first;

        // create the option using the possible filters
        app.add_option(optionName, filterInfo.second)
        ->each([&pipeline, optionName](std::string_view value) {

            // strip of "--" option prefix to get option name
            std::string_view name(optionName);
            name.remove_prefix(2);

            // add the filter of this name and value to the pipeline
            bool status = pipeline.addFilter(name, value);
            if (!status) {
                std::cerr << "xmlpipeline: Invalid filter '" << name << "'\n";
                exit(1);
            }
        });
    }

    // parse the command-line arguments
    CLI11_PARSE(app, argc, argv);

    // run the pipeline
    pipeline.run();

    return 0;
}
