/*
    refillBuffer.hpp

    Declaration of refillBuffer function
*/

#ifndef INCLUDE_REFILLBUFFER_HPP
#define INCLUDE_REFILLBUFFER_HPP

#include <string>

/*
    Refill the buffer preserving the unused data.
    Characters [pc, buffer.end()) are shifted left and new data
    is added to the rest of the buffer.

    @param pc Iterator to current position in buffer
    @param buffer Container for characters
    @param totalBytes Updated total bytes read
    @return Iterator to beginning of refilled buffer
*/
std::string::const_iterator refillBuffer(std::string::const_iterator pc, std::string& buffer, long& totalBytes);

#endif
