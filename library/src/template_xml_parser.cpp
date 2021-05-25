/*
    template_xml_parser.cpp

    Partial implementation for an XML parser with template handlers
*/

#include "template_xml_parser.hpp"

/*
    Refill the buffer preserving the unused data.
    Characters [pc, buffer.end()) are shifted left and new data
    is added to the rest of the buffer.
    @param pc Iterator to current position in buffer
    @param buffer Container for characters
    @param totalBytes Updated total bytes read
    @return Iterator to beginning of refilled buffer
*/
std::vector<char>::const_iterator TemplateXMLParser::refillBuffer(std::istream& in, std::vector<char>::const_iterator pc, std::vector<char>& buffer, long& totalBytes) {

    // find number of unprocessed characters [pc, buffer.cend())
    auto d = std::distance(pc, buffer.cend());

    // move unprocessed characters, [pc, buffer.cend()), to start of the buffer
    std::copy(pc, buffer.cend(), buffer.begin());

    // read in trying to read whole blocks
    in.read(buffer.data() + d, (size_t)(BUFFER_SIZE - d));
    auto numbytes = in.gcount();
    // EOF
    if (numbytes == 0)
        return buffer.cend();

    if ((decltype(buffer.size())) numbytes + d < buffer.size())
        buffer.resize(numbytes + d);

    // update with number of bytes read
    totalBytes += (decltype(totalBytes)) numbytes;

    // return iterator to first part of buffer
    return buffer.cbegin();
}
