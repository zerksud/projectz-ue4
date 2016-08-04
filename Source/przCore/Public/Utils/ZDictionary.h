// Copyright 2016 Max Molodtsov
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include "Platform.h"

#include "Utils/StandardLibrary/ZString.h"
#include "Utils/StandardLibrary/ZUnorderedMap.h"
#include "Utils/StandardLibrary/ZUtility.h"

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
