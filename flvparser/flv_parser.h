#pragma once
#include <string>

#include "flv_header.h"
#include "flv_tag.h"

namespace flv_parser
{

class FlvParser
{
public:
	enum State {
		ParseError = 0,
		ParseHeader = 1,
		ParseTag = 2,
	};

	FlvParser();

	// parse 每次只解析一段数据，解析结果通过 tag 获取
	// 返回解析成功的字节数， =0 数据不足, < 0 解析失败
	int parse(const char *data, size_t len);

	int state() const {return _state;}
	const FlvHeader &header() const {return _header;};
	std::shared_ptr<FlvTag> tag() const {return _tag;};

private:
	int parseHeader(const char *data, size_t len);
	int parseTag(const char *data, size_t len);

private:
	State _state;
	FlvHeader _header;
	std::shared_ptr<FlvTag> _tag;
};

}