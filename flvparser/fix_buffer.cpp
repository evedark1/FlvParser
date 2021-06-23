#include "fix_buffer.h"

namespace flv_parser {

void fix_buffer::init(const char* data, size_t len)
{
    _pos = 0;
    _len = len;
    if (len > 0) {
        _ref = std::make_shared<buffer>(data, len);
    }
}

fix_buffer fix_buffer::sub(size_t pos, size_t len) const
{
    fix_buffer r;
    r._pos = _pos + pos;
    if (len == npos) {
        r._len = _len - pos;
    } else {
        if (pos + len > _len)
            throw std::out_of_range("fix_buffer sub out of range");
        r._len = len;
    }
    if (r._len > 0)
        r._ref = _ref;
    return r;
}

fix_buffer fix_buffer::whole() const
{
    fix_buffer r;
    if (_ref) {
        r._pos = 0;
        r._len = _ref->len;
        r._ref = _ref;
    }
    return r;
}

fix_buffer fix_buffer::deepcopy() const
{
    return fix_buffer(data(), size());
}

int fix_buffer::compare(const fix_buffer& other) const
{
    size_t s1 = size();
    size_t s2 = other.size();
    int ret = memcmp(data(), other.data(), std::min(s1, s2));
    if (ret == 0) {
        ret = s1 - s2;
    }
    return ret;
}

}