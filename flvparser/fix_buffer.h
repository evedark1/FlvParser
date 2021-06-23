#pragma once

#include <array>
#include <memory>
#include <string.h>

namespace flv_parser {

class fix_buffer {
    struct buffer {
        char* data;
        size_t len;

        buffer(const char* d, size_t l)
        {
            data = (char*)malloc(l);
            memcpy(data, d, l);
            len = l;
        }

        ~buffer()
        {
            free(data);
        }
    };

public:
    static const size_t npos = -1;

    fix_buffer() { init(NULL, 0); }
    explicit fix_buffer(const std::string& str) { init(str.data(), str.size()); }
    explicit fix_buffer(const char* data, size_t len) { init(data, len); }

    fix_buffer sub(size_t pos, size_t len = npos) const;
    fix_buffer whole() const;
    fix_buffer deepcopy() const;

    const char* data() const { return _ref ? _ref->data + _pos : NULL; }
    size_t size() const { return _len; }
    bool empty() const { return _len == 0; }

    std::string str() { return std::string(data(), size()); }

    int compare(const fix_buffer& other) const;
    bool operator==(const fix_buffer& other) const { return compare(other) == 0; }
    bool operator!=(const fix_buffer& other) const { return compare(other) != 0; }
    bool operator<(const fix_buffer& other) const { return compare(other) < 0; }
    bool operator<=(const fix_buffer& other) const { return compare(other) <= 0; }
    bool operator>(const fix_buffer& other) const { return compare(other) > 0; }
    bool operator>=(const fix_buffer& other) const { return compare(other) >= 0; }

private:
    void init(const char* data, size_t len);

private:
    size_t _pos;
    size_t _len;
    std::shared_ptr<buffer> _ref;
};

}