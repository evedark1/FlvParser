#include "flv_parser.h"
#include "read_bytes.h"

namespace flv_parser
{

FlvParser::FlvParser() {
    _state = ParseHeader;
}

int FlvParser::parse(const char *data, size_t len) {
    if(_state == ParseHeader)
        return parseHeader(data, len);
    else if(_state == ParseTag)
        return parseTag(data, len);
    else
        return -1;
}

int FlvParser::parseHeader(const char *data, size_t len) {
    int r = _header.ParseData(data, len);
    if (r == -1)
        _state = ParseError;
    else if (r > 0)
        _state = ParseTag;
    return r;
}

int FlvParser::parseTag(const char *data, size_t len) {
    if(len < 4 + 11) // presize + tag header size
        return 0;

    // 跳过 previous tag size 不做解析
    int read_pos = 4;

    std::shared_ptr<FlvTag> tag(new FlvTag);
    int r = tag->ParseData(data + read_pos, len - read_pos);
    if(r == -1) {
        _state = ParseError;
        return r;
    } else if(r == 0) {
        return 0;
    } else {
        _tag = tag;
        return r + read_pos;
    }
}

}