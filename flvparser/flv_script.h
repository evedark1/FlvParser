// Copyright (c) 2018 winking324

#pragma once // NOLINT(build/header_guard)

#include <inttypes.h>
#include <list>
#include <map>
#include <memory>
#include <string>

#include "flv_data.h"

namespace flv_parser {

enum ScriptDataValueType {
    SCRIPT_DATA_NUMBER = 0,
    SCRIPT_DATA_BOOLEAN = 1,
    SCRIPT_DATA_STRING = 2,
    SCRIPT_DATA_OBJECT = 3,
    SCRIPT_DATA_MOVIE_CLIP = 4,
    SCRIPT_DATA_NULL = 5,
    SCRIPT_DATA_UNDEFINED = 6,
    SCRIPT_DATA_REFERENCE = 7,
    SCRIPT_DATA_ECMA_ARRAY = 8,
    SCRIPT_DATA_OBJECT_END_MARKER = 9,
    SCRIPT_DATA_STRICT_ARRAY = 10,
    SCRIPT_DATA_DATE = 11,
    SCRIPT_DATA_LONG_STRING = 12,

    SCRIPT_DATA_VALUE = UINT8_MAX,
};

struct FlvScriptData {
    virtual ~FlvScriptData() { }
    virtual std::string Info() const { return ""; }
};

struct FlvScriptDataValue : public FlvScriptData {
    uint8_t Type() const { return type_; }
    std::string TypeString() const;
    std::string Info() const { return value_->Info(); }

    template <typename T>
    T* Value() const
    {
        return dynamic_cast<T*>(value_.get());
    }

    uint8_t type_;
    std::shared_ptr<FlvScriptData> value_;
};

struct FlvScriptDataNumber : public FlvScriptData {
    std::string Info() const { return std::to_string(double_data_); }

    double double_data_;
};

struct FlvScriptDataBool : public FlvScriptData {
    std::string Info() const { return std::to_string((bool)bool_data_); }

    uint8_t bool_data_;
};

struct FlvScriptDataReference : public FlvScriptData {
    std::string Info() const { return std::to_string(reference_data_); }

    uint16_t reference_data_;
};

struct FlvScriptDataString : public FlvScriptData {
    std::string Info() const { return string_data_; }

    uint16_t string_length_;
    std::string string_data_;
};

struct FlvScriptDataLongString : public FlvScriptData {
    std::string Info() const { return string_data_; }

    uint32_t string_length_;
    std::string string_data_;
};

struct FlvScriptDataStrictArray : public FlvScriptData {
    std::string Info() const;

    uint32_t strict_array_length_;
    std::list<FlvScriptDataValue> strict_array_value_;
};

struct FlvScriptDataDate : public FlvScriptData {
    double date_time_;
    int16_t local_date_time_offset_;
};

struct FlvScriptDataObjectEnd : public FlvScriptData {
    uint8_t object_end_marker_[3] = { 0x0, 0x0, 0x9 };
};

struct FlvScriptDataObjectProperty : public FlvScriptData {
    std::string Info() const;

    FlvScriptDataString property_name_;
    FlvScriptDataValue property_data_;
};

struct FlvScriptDataObject : public FlvScriptData {
    std::list<FlvScriptDataObjectProperty> object_properties_;
    FlvScriptDataObjectEnd list_terminator_;
};

struct FlvScriptDataEcmaArrary : public FlvScriptData {
    std::string Info() const;

    uint32_t ecma_array_length_;
    std::list<FlvScriptDataObjectProperty> variables_;
    FlvScriptDataObjectEnd list_terminator_;
};

class FlvScript : public FlvData {
    typedef size_t (FlvScript::*ParseFuncPtr)(const std::string&, size_t, size_t,
        FlvScriptData*);
    typedef FlvScriptData* (*NewDataPtr)();

    template <class T>
    struct DataCreator {
        static FlvScriptData* New()
        {
            return (new T());
        }
    };

public:
    FlvScript();

    ~FlvScript();

    virtual int ParseData(const char* data, size_t length);

    const FlvScriptDataValue& Name() { return name_; }
    const FlvScriptDataValue& Value() { return value_; }

private:
    size_t ParseDataValue(const std::string& data, size_t pos, size_t length,
        FlvScriptData* value);

    size_t ParseDataNumber(const std::string& data, size_t pos, size_t length,
        FlvScriptData* value);

    size_t ParseDataBool(const std::string& data, size_t pos, size_t length,
        FlvScriptData* value);

    size_t ParseDataReference(const std::string& data, size_t pos, size_t length,
        FlvScriptData* value);

    size_t ParseDataString(const std::string& data, size_t pos, size_t length,
        FlvScriptData* value);

    size_t ParseDataLongString(const std::string& data, size_t pos, size_t length,
        FlvScriptData* value);

    size_t ParseDataStrictArray(const std::string& data, size_t pos,
        size_t length, FlvScriptData* value);

    size_t ParseDataDate(const std::string& data, size_t pos, size_t length,
        FlvScriptData* value);

    size_t ParseDataObjectProperty(const std::string& data, size_t pos,
        size_t length, FlvScriptData* value);

    size_t ParseDataObject(const std::string& data, size_t pos, size_t length,
        FlvScriptData* value);

    size_t ParseDataEcmaArrary(const std::string& data, size_t pos, size_t length,
        FlvScriptData* value);

private:
    FlvScriptDataValue name_;
    FlvScriptDataValue value_;
    static std::map<uint32_t, ParseFuncPtr> parser_;
    static std::map<uint32_t, NewDataPtr> creator_;
};

} // namespace flv_parser
