// Copyright (c) 2018 winking324
//


#include "flv_header.h"
#include <sstream>
#include "read_bytes.h"


namespace flv_parser {


FlvHeader::FlvHeader() {
}

int FlvHeader::ParseData(const char *data, size_t size) {
  // FLV header size 9
  if(size < 9) {
    return 0;
  }

  size_t read_pos = 0;
  signature_[0] = ReadByte(&data[read_pos++]);
  signature_[1] = ReadByte(&data[read_pos++]);
  signature_[2] = ReadByte(&data[read_pos++]);

  // Signature byte always 'F'(0x46) 'L'(0x4C) 'V'(0x56)
  if (signature_[0] != 0x46 || signature_[1] != 0x4C || signature_[2] != 0x56) {
    return -1;
  }

  // File version (for example, 0x01 for FLV version 1)
  version_ = ReadByte(&data[read_pos++]);
  if(version_ != 1) {
    return -1;
  }

  uint8_t type_byte = ReadByte(&data[read_pos++]);
  reserved1_ = (type_byte & 0xF8) >> 3;  // Shall be 0
  audio_flag_ = (type_byte & 0x04) >> 2;  // 1 = Audio tags are present
  reserved2_ = (type_byte & 0x02) >> 1;  // Shall be 0
  video_flag_ = (type_byte & 0x01);  // 1 = Video tags are present

  // The length of this header in bytes
  data_offset_ = Read4Bytes(&data[read_pos]);
  read_pos += 4;
  if (read_pos != data_offset_) {
    return -1;
  }
  return read_pos;
}

std::string FlvHeader::Info() {
  std::stringstream ss;
  ss << std::string(reinterpret_cast<char *>(signature_), 3) << "|"
     << "Version:" << static_cast<uint32_t>(version_) << "|"
     << (audio_flag_ == 1 ? "Audio|" : "")
     << (video_flag_ == 1 ? "Video|" : "")
     << data_offset_;
  return ss.str();
}

}  // namespace flv_parser

