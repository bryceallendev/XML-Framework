/*
    XMLDocument.cpp

    XPath filter for XMLFramework
*/

#include "XMLDocument.hpp"

// output operator
std::ostream& operator<<(std::ostream& out, const XMLDocument& document) {

    out << document.contents;

    return out;
}

// constructor
XMLDocument::XMLDocument() {
}

// constructor with initial contents
XMLDocument::XMLDocument(std::string_view contents)
    : contents(contents) {
}

// move constructor from initial contents
XMLDocument::XMLDocument(std::string&& contents)
    : contents(std::move(contents)) {
}

// move copy constructor
XMLDocument::XMLDocument(XMLDocument&& other)
    : contents(std::move(other.contents)) {
}

// move assignment operator
XMLDocument& XMLDocument::operator=(XMLDocument&& other) {

    if (this == &other)
        return *this;

    contents = std::move(other.contents);

    return *this;
}

// get contents
std::string& XMLDocument::get() {
    return contents;
}
