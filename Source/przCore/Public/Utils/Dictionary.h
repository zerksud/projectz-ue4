#pragma once

#include "Platform.h"

#include "Utils/StandartLibrary.h"

namespace prz {
namespace utl {

class PRZCORE_API ZDictionary {
public:
    ZDictionary() = default;
    ZDictionary(const ZDictionary& other);
    ZDictionary(ZDictionary&& other);
    ZDictionary(int pInt);
    ZDictionary(bool pBool);
    ZDictionary& operator=(ZDictionary other);
    virtual ~ZDictionary();

    ZDictionary& operator[](const char* key);
    const ZDictionary& operator[](const char* key) const;

    int AsInt() const;
    bool AsBool() const;

    friend void swap(ZDictionary& left, ZDictionary& right) {
        using utl::swap;

        swap(left.mValue, right.mValue);
        swap(left.mValueType, right.mValueType);
    }

private:
    typedef utl::ZUnorderedMap<ZString, ZDictionary> ZDictionaryMap;

    typedef union {
        int asInt;
        bool asBool;
        ZDictionaryMap* asMap;
    } ZValueBucket;

    typedef enum {
        undefinedValue,
        intValue,
        boolValue,
        mapValue
    } ZValueType;

    ZValueBucket mValue;
    ZValueType mValueType = undefinedValue;
};

}
}
