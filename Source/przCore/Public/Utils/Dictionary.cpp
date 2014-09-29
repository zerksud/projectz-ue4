#include "przCorePCH.h"
#include "Utils/Dictionary.h"

#include "Utils/LOG_ANSI.h"

namespace prz {
namespace utl {

ZDictionary::ZDictionary(const ZDictionary& other) : mValueType(other.mValueType) {
    switch (other.mValueType) {
    case intValue:
    case boolValue:
        mValue = other.mValue;
        break;
    case mapValue:
        mValue.asMap = new ZDictionaryMap();
        mValue.asMap->insert(other.mValue.asMap->begin(), other.mValue.asMap->end());
        break;
    default:
        LOGE("Got unsupported ZDIctionary of ValueType %d", other.mValueType);
        mValueType = undefinedValue;
        break;
    }
}

ZDictionary::ZDictionary(ZDictionary&& other) {
    swap(*this, other);
}

ZDictionary::ZDictionary(const int pInt) : mValueType(intValue) {
    mValue.asInt = pInt;
}

ZDictionary::ZDictionary(const bool pBool) : mValueType(boolValue) {
    mValue.asBool = pBool;
}

ZDictionary& ZDictionary::operator=(ZDictionary other) {
    swap(*this, other);
    return *this;
}

ZDictionary::~ZDictionary() {
    if (mValueType == mapValue) {
        delete mValue.asMap;
    }
}

ZDictionary& ZDictionary::operator[](const char* key) {
    if (mValueType == undefinedValue) {
        mValueType = mapValue;
        mValue.asMap = new ZDictionaryMap();
    }

    if (mValueType == mapValue) {
        return (*mValue.asMap)[key];
    }

    LOGE("Can't access ZDictionary value of type %d by key %s", mValueType, key);
    *this = ZDictionary();
    return (*this)[key];
}

const ZDictionary& ZDictionary::operator[](const char* key) const {
    if (mValueType == mapValue) {
        return (*mValue.asMap)[key];
    }

    LOGE("Can't access const ZDictionary value of type %d by key %s", mValueType, key);
    return *this;
}

const int ZDictionary::AsInt() const {
    if (mValueType == intValue) {
        return mValue.asInt;
    }

    LOGE("Can't get ZDictionary value of type %d AsInt", mValueType);
    return 0;
}
const bool ZDictionary::AsBool() const {
    if (mValueType == boolValue) {
        return mValue.asBool;
    }

    LOGE("Can't get ZDictionary value of type %d AsBool", mValueType);
    return false;
}

}
}
