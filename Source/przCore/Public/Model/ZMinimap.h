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

#include "Model/ZDungeonLevel.h"

namespace prz {
namespace mdl {

class PRZCORE_API ZMinimap {
public:
    // holds cells matrix
    ZMinimap(unsigned int size, EDungeonCell*** cells);
    ZMinimap(const ZMinimap& other);
    ZMinimap(ZMinimap&& other);
    ZMinimap& operator=(ZMinimap other);
    virtual ~ZMinimap();

    friend void swap(ZMinimap& left, ZMinimap& right) {
        using utl::swap;

        swap(left.mSize, right.mSize);
        swap(left.mCells, right.mCells);
    }

    unsigned int GetSize() const;
    EDungeonCell GetCell(unsigned int x, unsigned int y) const;

private:
    bool IndicesAreValid(unsigned int x, unsigned int y) const;

    unsigned int mSize;
    EDungeonCell** mCells;
};

}
}

