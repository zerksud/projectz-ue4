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

#include "Utils/StandardLibrary/Algorithm.h"
#include "Utils/StandardLibrary/ZVector.h"

namespace prz {
namespace utl {

template<typename T>
bool VectorContains(const ZVector<T>& vector, const T& value) {
    auto pos = find(vector.begin(), vector.end(), value);
    bool contains = pos != vector.end();

    return contains;
}

template<typename T>
void VectorInsertAll(ZVector<T>* dst, const ZVector<T>& src) {
    dst->insert(dst->end(), src.begin(), src.end());
}

}
}
