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

#include "Model/ZPosition.h"

namespace prz {
namespace mdl {

class ZMatrix2D {
public:
    ZMatrix2D();
    ZMatrix2D(const ZMatrix2D& other);
    ZMatrix2D& operator=(ZMatrix2D other);
    friend void swap(ZMatrix2D& left, ZMatrix2D& right);
    
    ZMatrix2D& operator*(const ZMatrix2D& other);
    ZPositionDiff operator*(const ZPositionDiff& diff) const;
    
    static ZMatrix2D FlipX();
    static ZMatrix2D FlipY();
    static ZMatrix2D Rotate(int rightAnglesCount);
    
private:
    ZMatrix2D(int v00, int v01, int v10, int v11);
    
    enum { size = 2 };
    int mData[size][size];
};

}
}
