// Copyright (c) 2018 winking324
//


#pragma once  // NOLINT(build/header_guard)


#include <string>
#include <inttypes.h>

namespace flv_parser {


class FlvData {
 public:
  FlvData() {}
  virtual ~FlvData() {}

  virtual int ParseData(const char *data, size_t length) = 0;
  virtual std::string Info() const {return "";}
};


}  // namespace flv_parser

