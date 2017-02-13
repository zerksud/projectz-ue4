// Copyright 2014-2017 Max Molodtsov <zerksud@gmail.com>
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

#include <string.h>

#include <algorithm>

namespace prz {
namespace utl {
namespace matrix_helpers {

template<typename T>
void Allocate(T*** matrix, unsigned int width, unsigned int height) {
    *matrix = new T*[height];
    for (unsigned int i = 0; i < height; ++i) {
        (*matrix)[i] = new T[width];
    }
}

template<typename T>
void Allocate(T*** matrix, unsigned int width, unsigned int height, T defaultValue) {
    Allocate(matrix, width, height);
    for (unsigned int i = 0; i < height; ++i) {
        std::fill_n((*matrix)[i], width, defaultValue);
    }
}

template<typename T>
void Allocate(T*** matrix, unsigned int size) {
    Allocate<T>(matrix, size, size);
}

template<typename T>
void AllocateAndCopy(T*** dst, T const * const * src, unsigned int width, unsigned int height) {
    Allocate(dst, width, height);
    for (unsigned int i = 0; i < height; ++i) {
        memcpy((*dst)[i], src[i], width * sizeof(T));
    }
}

template<typename T>
void AllocateAndCopy(T*** dst, T const * const * src, unsigned int size) {
    AllocateAndCopy<T>(dst, src, size, size);
}

template<typename T>
void Deallocate(T*** matrix, unsigned int height) {
    for (unsigned int i = 0; i < height; ++i) {
        delete[](*matrix)[i];
    }
    delete[](*matrix);
    *matrix = nullptr;
}

}
}
}
