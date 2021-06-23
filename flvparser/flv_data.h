// Copyright (c) 2018 winking324
//

#pragma once // NOLINT(build/header_guard)

#include "fix_buffer.h"
#include <inttypes.h>
#include <string>

namespace flv_parser {

class FlvData {
public:
    FlvData() { }
    virtual ~FlvData() { }

    virtual int ParseData(const fix_buffer& buffer) = 0;
    virtual std::string Info() const { return ""; }
    virtual fix_buffer Payload() const { return fix_buffer(); }
};

} // namespace flv_parser
