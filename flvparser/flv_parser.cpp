#include "flv_parser.h"
#include "read_bytes.h"

namespace flv_parser {

inline int skipPreviousTagSize(int r, int len)
{
    r = r + 4; // skip previous tag size
    return (r <= len) ? r : 0;
}

FlvParser::FlvParser()
{
    _state = StateHeader;
    _previousSize = 0;
}

int FlvParser::parse(const char* data, size_t len, size_t &rlen, std::shared_ptr<FlvTag> &tag)
{
    rlen = 0;
    tag = nullptr;

    switch (_state) {
    case StateHeader: {
        int r = _header.ParseData(data, len);
        if (r > 0) {
            _state = StatePreviousSize;
            _previousSize = 0;
            rlen = r;
            return ParseHeader;
        } else if (r < 0) {
            _state = StateError;
            return ErrorStream;
        } else {
            return ErrorMoreData;
        }
    } break;

    case StateTag: {
        std::shared_ptr<FlvTag> ntag(new FlvTag);
        int r = ntag->ParseData(data, len);
        if (r > 0) {
            _state = StatePreviousSize;
            _previousSize = ntag->BodySize();
            rlen = r;
            tag = ntag;
            return ParseTag;
        } else if (r < 0) {
            if (ntag->BodySize() > 0) {
                _state = StatePreviousSize;
                _previousSize = ntag->BodySize();
                rlen = ntag->BodySize();
                tag = ntag;
                return ErrorTagBody;
            } 

            _state = StateError;
            return ErrorStream;
        } else {
            return ErrorMoreData;
        }
    } break;

    case StatePreviousSize: {
        if(len >= 4) {
            /* don't check previous size
            if(Read4Bytes(data) != _previousSize) {
                _state = StateError;
                return ErrorStream;
            }
            */
            _state = StateTag;
            rlen = 4; // skip previous tag size
            return ParsePreviousSize;
        } else {
            return ErrorMoreData;
        }
    } break;

    default:
        break;
    }

    return ErrorStream;
}

}