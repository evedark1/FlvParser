// Copyright (c) 2018 winking324
//

#pragma once // NOLINT(build/header_guard)

#include "flv_audio.h"
#include "flv_data.h"
#include "flv_script.h"
#include "flv_video.h"
#include <inttypes.h>
#include <memory>

namespace flv_parser {

enum TagType {
    UNKNOWN_TAG = 0,
    AUDIO_TAG = 8,
    VIDEO_TAG = 9,
    SCRIPT_TAG = 18
};

class FlvTag {
public:
    FlvTag();

    int ParseData(const char* data, size_t len);

    int Type() const { return type_; }
    std::string TypeString() const;
    std::string Info();

    fix_buffer Body() const { return body_; }
    size_t BodySize() const { return body_.size(); }
    fix_buffer Payload() const { return payload_; }
    FlvData* Data() { return data_.get(); }
    uint32_t Timestamp() const;

private:
    fix_buffer body_;
    fix_buffer payload_;

    uint8_t reserved_;
    uint8_t filter_;
    uint8_t type_;
    uint32_t size_;
    uint32_t timestamp_;
    uint8_t timestamp_extended_;
    uint32_t stream_id_;
    std::shared_ptr<FlvData> data_;
};

} // namespace flv_parser
