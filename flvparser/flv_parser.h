#pragma once
#include <string>

#include "flv_header.h"
#include "flv_tag.h"

namespace flv_parser {

class FlvParser {
public:

    FlvParser();

    enum ParseResult {
        ParseTag = 0,           // len > 0 && tag != null
        ParseHeader = 1,        // len > 0 && tag == null
        ParsePreviousSize = 2,  // len > 0 && tag == null
        ErrorMoreData = 3,      // len == 0 && tag == null
        ErrorTagBody = 4,       // len > 0 && tag != null
        ErrorStream = 5,        // len == 0 && tag == null
    };

    /**
     * @brief parse binary data to flv tag
     * 
     * @param data input data
     * @param len input data length
     * @param rlen output parsed length
     * @param tag output flv tag when completed
     * @return see ParseResult
     */
    int parse(const char* data, size_t len, size_t &rlen, std::shared_ptr<FlvTag> &tag);

    const FlvHeader& header() const { return _header; };

private:
    enum State {
        StateError = 0,
        StateHeader = 1,
        StateTag = 2,
        StatePreviousSize = 3,
    };

    State _state;
    FlvHeader _header;
    size_t _previousSize;
};

}