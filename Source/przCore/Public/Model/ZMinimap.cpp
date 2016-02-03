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

#include "przCorePCH.h"
#include "Model/ZMinimap.h"

#include "Utils/LOG.h"
#include "Utils/MatrixHelpers.h"

namespace prz {
namespace mdl {

ZMinimap::ZMinimap(unsigned int sideSize, EDungeonCell*** cells) {
    mSize = sideSize;
    mCells = *cells;
    *cells = nullptr;
}

ZMinimap::ZMinimap(const ZMinimap& other) {
    mSize = other.mSize;
    utl::matrix_helpers::AllocateAndCopy<EDungeonCell>(&mCells, other.mCells, mSize);
}

ZMinimap::ZMinimap(ZMinimap&& other) {
    mSize = 0;
    mCells = nullptr;

    swap(*this, other);
}

ZMinimap& ZMinimap::operator=(ZMinimap other) {
    swap(*this, other);
    return *this;
}

ZMinimap::~ZMinimap() {
    utl::matrix_helpers::Deallocate(&mCells, mSize);
}

unsigned int ZMinimap::GetSize() const {
    return mSize;
}

bool ZMinimap::IndicesAreValid(unsigned int x, unsigned int y) const {
    return (x < mSize && y < mSize);
}

EDungeonCell ZMinimap::GetCell(unsigned int x, unsigned int y) const {
    if (!IndicesAreValid(x, y)) {
        LOGE("Can't get cell %dx%d from minimap of size %d", x, y, mSize);
        return EDungeonCell::Unknown;
    }

    return mCells[x][y];
}

}
}
