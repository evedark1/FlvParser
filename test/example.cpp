#include "flvparser/flv_parser.h"
#include <stdio.h>
#include <string>

using namespace std;
using namespace flv_parser;

FlvParser flvParser;

int parseFlv(const char* data, size_t len)
{
    size_t read_pos = 0;
    while (true) {
        size_t sz;
        shared_ptr<FlvTag> tag;
        int result = flvParser.parse(data + read_pos, len - read_pos, sz, tag);
        if (sz > 0)
            read_pos += sz;

        if (result == FlvParser::ParseHeader) {
            FlvHeader header = flvParser.header();
            printf("FlvHeader %s\n", header.Info().c_str());
        } else if (result == FlvParser::ParseTag) {
            printf("FlvTag %s %d, %s\n", tag->TypeString().c_str(), tag->Timestamp(), tag->Data()->Info().c_str());
            if (tag->Type() == SCRIPT_TAG) {
                FlvScript *script = dynamic_cast<FlvScript *>(tag->Data());
                const FlvScriptDataValue &name = script->Name();
                printf("name %s: %s\n", name.TypeString().c_str(), name.Info().c_str());
                const FlvScriptDataValue &value = script->Value();
                printf("value %s: %s\n", value.TypeString().c_str(), value.Info().c_str());
            }
        } else if (result == FlvParser::ParsePreviousSize) {
            continue;
        } else if (result == FlvParser::ErrorMoreData) {
            return read_pos;
        } else if(result == FlvParser::ErrorTagBody) {
            printf("FlvTag error body %s %d\n", tag->TypeString().c_str(), tag->Timestamp());
        } else { // result == FlvParser::ErrorStream
            return -1;
        }
    }
}

int main(int argc, const char** argv)
{
    if (argc != 2) {
        printf("usage: %s file\n", argv[0]);
        return 0;
    }

    FILE* fp = fopen(argv[1], "rb");
    if (fp == NULL) {
        printf("open file error: %s\n", argv[1]);
        return 0;
    }

    std::string buffer;
    while (true) {
        char b[4096];
        size_t rd = fread(b, 1, sizeof(b), fp);
        if (rd == 0) {
            printf("read file finish: %zu\n", rd);
            break;
        }
        buffer.append(b, rd);

        int r = parseFlv(buffer.c_str(), buffer.size());
        if (r < 0) {
            printf("parse flv fail\n");
            break;
        }
        buffer.erase(0, r);
    }

    fclose(fp);
    return 0;
}