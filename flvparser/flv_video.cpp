// Copyright (c) 2018 winking324
//

#include "flv_video.h"
#include "read_bytes.h"
#include <sstream>

namespace flv_parser {

FlvVideo::FlvVideo()
{
}

FlvVideo::~FlvVideo()
{
}

int FlvVideo::ParseData(const fix_buffer &buffer)
{
    const char *data = buffer.data();
    size_t read_pos = 0;
    uint8_t video_byte = ReadByte(&data[read_pos++]);

    frame_type_ = (video_byte & 0xF0) >> 4;
    codec_id_ = (video_byte & 0x0F);

    if (codec_id_ == VIDEO_CODEC_AVC || codec_id_ == VIDEO_CODEC_HEVC || codec_id_ == VIDEO_CODEC_AV1) {
        avc_packet_type_ = ReadByte(&data[read_pos++]);
        composition_time_ = Read3Bytes(&data[read_pos]);
        read_pos += 3;
    }

    payload_ = buffer.sub(read_pos);
    return buffer.size();
}

std::string FlvVideo::Info() const
{
    std::stringstream ss;
    ss << "codec: " << CodecIDString() << ", " << FrameTypeString();
    if (codec_id_ == VIDEO_CODEC_AVC || codec_id_ == VIDEO_CODEC_HEVC || codec_id_ == VIDEO_CODEC_AV1) {
        ss << ", " << AVCPacketTypeString();
    }
    return ss.str();
}

std::string FlvVideo::FrameTypeString() const
{
    switch (frame_type_) {
    case VIDEO_KEY_FRAME:
        return "key frame (for AVC, a seekable frame)";
    case VIDEO_INTER_FRAME:
        return "inter frame (for AVC, a non-seekable frame)";
    case VIDEO_DISPOSABLE_INTER_FRAME:
        return "disposable inter frame (H.263 only)";
    case VIDEO_GENERATED_KEY_FRAME:
        return "generated key frame (reserved for server use only)";
    case VIDEO_INFO_AND_COMMAND_FRAME:
        return "video info/command frame";
    default:
        return "Unknown frame type: " + std::to_string(static_cast<uint32_t>(frame_type_));
    }
}

std::string FlvVideo::CodecIDString() const
{
    switch (codec_id_) {
    case VIDEO_CODEC_SORENSON_H263:
        return "Sorenson H.263";
    case VIDEO_CODEC_SCREEN:
        return "Screen video";
    case VIDEO_CODEC_ON2_VP6:
        return "On2 VP6";
    case VIDEO_CODEC_ON2_VP6_WITH_ALPHA_CHANNEL:
        return "On2 VP6 with alpha channel";
    case VIDEO_CODEC_SCREEN_V2:
        return "Screen video version 2";
    case VIDEO_CODEC_AVC:
        return "AVC";
    case VIDEO_CODEC_HEVC:
        return "HEVC";
    case VIDEO_CODEC_AV1:
        return "AV1";
    default:
        return "Unknown codec id: " + std::to_string(static_cast<uint32_t>(codec_id_));
    }
}

std::string FlvVideo::AVCPacketTypeString() const
{
    switch (avc_packet_type_) {
    case AVC_SEQUENCE_HEADER:
        return "AVC sequence header";
    case AVC_NALU:
        return "AVC NALU";
    case AVC_END_OF_SEQUENCE:
        return "AVC end of sequence";
    default:
        return "Unknown AVC packet type: " + std::to_string(static_cast<uint32_t>(avc_packet_type_));
    }
}

} // namespace flv_parser
