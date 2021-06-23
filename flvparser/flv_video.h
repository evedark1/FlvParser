// Copyright (c) 2018 winking324
//

#pragma once // NOLINT(build/header_guard)

#include "flv_data.h"
#include <inttypes.h>

namespace flv_parser {

enum VideoFrameType {
    VIDEO_KEY_FRAME = 1, // for AVC, a seekable frame
    VIDEO_INTER_FRAME = 2, // for AVC, a non-seekable frame
    VIDEO_DISPOSABLE_INTER_FRAME = 3, // H.263 only
    VIDEO_GENERATED_KEY_FRAME = 4, // reserved for server use only
    VIDEO_INFO_AND_COMMAND_FRAME = 5,
};

enum VideoCodecId {
    VIDEO_CODEC_SORENSON_H263 = 2,
    VIDEO_CODEC_SCREEN = 3,
    VIDEO_CODEC_ON2_VP6 = 4,
    VIDEO_CODEC_ON2_VP6_WITH_ALPHA_CHANNEL = 5,
    VIDEO_CODEC_SCREEN_V2 = 6,
    VIDEO_CODEC_AVC = 7,
    VIDEO_CODEC_HEVC = 12,
};

enum AVCPacketType {
    AVC_SEQUENCE_HEADER = 0,
    AVC_NALU = 1,
    AVC_END_OF_SEQUENCE = 2,
};

class FlvVideo : public FlvData {
public:
    FlvVideo();

    virtual ~FlvVideo();

    virtual int ParseData(const fix_buffer &buffer);
    virtual std::string Info() const;
    virtual fix_buffer Payload() const { return payload_; }

    uint8_t FrameType() const { return frame_type_; }
    uint8_t CodecID() const { return codec_id_; }
    uint8_t AVCPacketType() const { return avc_packet_type_; }
    uint32_t CompositionTime() const { return composition_time_; }

    std::string FrameTypeString() const;
    std::string CodecIDString() const;
    std::string AVCPacketTypeString() const;

private:
    fix_buffer payload_;

    uint8_t frame_type_;
    uint8_t codec_id_;
    uint8_t avc_packet_type_;
    uint32_t composition_time_;
};

} // namespace flv_parser
