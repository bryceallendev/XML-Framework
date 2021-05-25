/*
    Filter.hpp

    Filter for XMLFramework
*/

#ifndef INCLUDED_FILTER_HPP
#define INCLUDED_FILTER_HPP

#include "XMLDocument.hpp"

// @pattern Template Method @role Abstract Class
class Filter {
public:

    /*
        Performs the action on the XML document

        @param doc Input XML document
        @return XML document after action performed on input
    */
    virtual XMLDocument action(const XMLDocument document) = 0;

    /*
        Does the filter perform output
    */
    virtual bool isOutput() const { return false; }

    // destructor
    virtual ~Filter() = default;
};

#endif
