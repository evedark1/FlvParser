// Copyright (c) 2018 winking324
//


#pragma once  // NOLINT(build/header_guard)


#include <inttypes.h>
#include "flv_data.h"


namespace flv_parser {


enum AudioFormat {
  AUDIO_FORMAT_PLATFORM_ENDIAN_LINEAR_PCM = 0,
  AUDIO_FORMAT_ADPCM = 1,
  AUDIO_FORMAT_MP3 = 2,
  AUDIO_FORMAT_LITTLE_ENDIAN_LINEAR_PCM = 3,
  AUDIO_FORMAT_NELLYMOSER_16_KHZ_MONO = 4,
  AUDIO_FORMAT_NELLYMOSER_8_KHZ_MONO = 5,
  AUDIO_FORMAT_NELLYMOSER = 6,
  AUDIO_FORMAT_G_711_A_LAW_LOGARITHMIC_PCM = 7,
  AUDIO_FORMAT_G_711_MU_LAW_LOGARITHMIC_PCM = 8,
  AUDIO_FORMAT_RESERVED = 9,
  AUDIO_FORMAT_AAC = 10,
  AUDIO_FORMAT_SPEEX = 11,
  AUDIO_FORMAT_MP3_8_KHZ = 14,
  AUDIO_FORMAT_DEVICE_SPECIFIC_SOUND = 15,
};


enum AudioSampleRate {
  AUDIO_RATE_5_5_KHZ = 0,
  AUDIO_RATE_11_KHZ = 1,
  AUDIO_RATE_22_KHZ = 2,
  AUDIO_RATE_44_KHZ = 3,
};


enum AuidoSampleSize {
  AUDIO_SIZE_8_BIT_SAMPLES = 0,
  AUDIO_SIZE_16_BIT_SAMPLES = 1,
};


enum AudioChannel {
  AUDIO_CHANNEL_MONO = 0,
  AUDIO_CHANNEL_STEREO = 1,
};


enum AACPacketType {
  AAC_SEQUENCE_HEADER = 0,
  AAC_RAW = 1,
};


class FlvAudio : public FlvData {
 public:
  FlvAudio();

  ~FlvAudio();

  virtual int ParseData(const char *data, size_t length);
  virtual std::string Info() const;

  uint8_t SoundFormat() const {return format_;}
  uint8_t SoundRate() const {return rate_;}
  uint8_t SoundSize() const {return size_;}
  uint8_t SoundType() const {return type_;}
  uint8_t AACPacketType() const {return aac_packet_type_;}

  std::string SoundFormatString() const;
  std::string SoundRateString() const;
  std::string SoundSizeString() const;
  std::string SoundTypeString() const;
  std::string AACPacketTypeString() const;

 private:
  uint8_t format_;
  uint8_t rate_;
  uint8_t size_;
  uint8_t type_;
  uint8_t aac_packet_type_;
};


}  // namespace flv_parser

