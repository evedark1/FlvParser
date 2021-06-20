#include <stdio.h>
#include <string>
#include "flvparser/flv_parser.h"

using namespace std;
using namespace flv_parser;

FlvParser flvParser;

int parseFlv(const char *data, size_t len) {
    size_t read_pos = 0;
    while(true) {
        int state = flvParser.state();
        int r = flvParser.parse(data + read_pos, len - read_pos);
        if(r < 0) {
            return -1;
        } else if(r == 0) {
            return read_pos;
        }
        read_pos += r;

        if(state == FlvParser::ParseHeader) {
            FlvHeader header = flvParser.header();
            printf("FlvHeader %s\n", header.Info().c_str());
        } else {  // ParserTag
            shared_ptr<FlvTag> tag = flvParser.tag();
            printf("FlvTag %s %d, %s\n", tag->TypeString().c_str(), tag->Timestamp(), tag->Data()->Info().c_str());
            if(tag->Type() == SCRIPT_TAG) {
                FlvScript *script = dynamic_cast<FlvScript*>(tag->Data());
                const FlvScriptDataValue &name = script->Name();
                printf("name %s: %s\n", name.TypeString().c_str(), name.Info().c_str());
                const FlvScriptDataValue &value = script->Value();
                printf("value %s: %s\n", value.TypeString().c_str(), value.Info().c_str());
            }
        }
    }
}

int main(int argc, const char **argv) {
    if(argc != 2) {
        printf("usage: %s file\n", argv[0]);
        return 0;
    }
    
    FILE *fp = fopen(argv[1], "rb");
    if(fp == NULL) {
        printf("open file error: %s\n", argv[1]);
        return 0;
    }

    std::string buffer;
    while(true) {
        char b[4096];
        size_t rd = fread(b, 1, sizeof(b), fp);
        if(rd == 0) {
            printf("read file finish: %zu\n", rd);
            break;
        }
        buffer.append(b, rd);

        int r = parseFlv(buffer.c_str(), buffer.size());
        if(r < 0) {
            printf("parse flv fail\n");
            break;
        }
        buffer.erase(0, r);
    }

    fclose(fp);
    return 0;
}