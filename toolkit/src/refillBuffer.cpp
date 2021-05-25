/*
    refillBuffer.cpp

    Implement refillBuffer function
*/

#include "refillBuffer.hpp"

#include <string>
#include <errno.h>

#if !defined(_MSC_VER)
#include <sys/uio.h>
#include <unistd.h>
#define READ read
#else
#include <BaseTsd.h>
#include <io.h>
typedef SSIZE_T ssize_t;
#define READ _read
#endif

const int BUFFER_SIZE = 16 * 16 * 4096;

/*
    Refill the buffer preserving the unused data.
    Characters [pc, buffer.end()) are shifted left and new data
    is added to the rest of the buffer.

    @param pc Iterator to current position in buffer
    @param buffer Container for characters
    @param totalBytes Updated total bytes read
    @return Iterator to beginning of refilled buffer
*/
std::string::const_iterator refillBuffer(std::string::const_iterator pc, std::string& buffer, long& totalBytes) {

    // find number of unprocessed characters [pc, buffer.cend())
    auto d = std::distance(pc, buffer.cend());

    // move unprocessed characters, [pc, buffer.cend()), to start of the buffer
    std::copy(pc, buffer.cend(), buffer.begin());

    // read in trying to read whole blocks
    ssize_t numbytes = 0;
    while (((numbytes = READ(0, (void*)(buffer.data() + d), (size_t)(BUFFER_SIZE - d))) == (ssize_t) -1) &&
        (errno == EINTR)) {
    }
    // error in read
    if (numbytes == -1)
        return buffer.cend();
    // EOF
    if (numbytes == 0)
        return buffer.cend();

    if ((std::string::size_type) (numbytes + d) < buffer.size())
        buffer.resize(numbytes + d);

    // update with number of bytes read
    totalBytes += (long) numbytes;

    // return iterator to first part of buffer
    return buffer.cbegin();
}
