// Copyright (c) 2018 winking324
//

#include "flv_tag.h"

#include <memory>
#include <sstream>

#include "flv_audio.h"
#include "flv_script.h"
#include "flv_video.h"
#include "read_bytes.h"

namespace flv_parser {

static const size_t kTagHeaderSize = 11;

FlvTag::FlvTag()
{
    reserved_ = 0;
    filter_ = 0;
    type_ = UNKNOWN_TAG;
    size_ = 0;
    timestamp_ = 0;
    timestamp_extended_ = 0;
    stream_id_ = 0;
}

int FlvTag::ParseData(const char* data, size_t len)
{
    if (len < kTagHeaderSize) {
        return 0;
    }

    size_t read_pos = 0;
    uint8_t type_byte = ReadByte(&data[read_pos++]);
    reserved_ = (type_byte & 0xC0) >> 6;
    filter_ = (type_byte & 0x40) >> 5;
    type_ = (type_byte & 0x1F);

    size_ = Read3Bytes(&data[read_pos]);
    read_pos += 3;

    timestamp_ = Read3Bytes(&data[read_pos]);
    read_pos += 3;

    timestamp_extended_ = ReadByte(&data[read_pos++]);

    stream_id_ = Read3Bytes(&data[read_pos]);
    read_pos += 3;

    if (size_ == 0)
        return -1;
    if (size_ + read_pos > len)
        return 0;

    body_ = fix_buffer(data, size_ + kTagHeaderSize);
    payload_ = body_.sub(read_pos);

    int read_data_size = 0;
    switch (type_) {
    case AUDIO_TAG:
        data_.reset(new FlvAudio());
        read_data_size = data_->ParseData(payload_);
        break;
    case VIDEO_TAG:
        data_.reset(new FlvVideo());
        read_data_size = data_->ParseData(payload_);
        break;
    case SCRIPT_TAG:
        data_.reset(new FlvScript());
        read_data_size = data_->ParseData(payload_);
        break;
    default:
        return -1;
    }

    if (read_data_size <= 0)
        return -1;
    else
        return read_data_size + kTagHeaderSize;
}

std::string FlvTag::TypeString() const
{
    switch (type_) {
    case AUDIO_TAG:
        return "Audio";
    case VIDEO_TAG:
        return "Video";
    case SCRIPT_TAG:
        return "Script";
    default:
        return "Unknown";
    }
}

std::string FlvTag::Info()
{
    std::string data_info = data_->Info();
    std::stringstream ss;
    ss << "Filter:" << static_cast<uint32_t>(filter_) << "|"
       << "Size:" << size_ << "|"
       << "Timestamp:" << Timestamp()
       << (data_info.empty() ? "" : "|") << data_info;
    return ss.str();
}

uint32_t FlvTag::Timestamp() const
{
    return static_cast<uint32_t>(timestamp_extended_) << 24 | timestamp_;
}

} // namespace flv_parser
