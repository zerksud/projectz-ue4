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
#include "Model/Path/ZWeightedMap.h"

#include "Utils/MatrixHelpers.h"

namespace prz {
namespace mdl {
namespace path {

ZWeightedMap::ZWeightedMap(int width, int height, const ZWeight& defaultCellWeight)
    : mWidth(width),
        mHeight(height) {
    utl::matrix_helpers::Allocate(&mCellWeights, mWidth, mHeight, defaultCellWeight);
}

ZWeightedMap::ZWeightedMap(const ZWeightedMap& other)
    : mWidth(other.mWidth),
        mHeight(other.mHeight) {
    utl::matrix_helpers::AllocateAndCopy(&mCellWeights, other.mCellWeights, mWidth, mHeight);
}

ZWeightedMap::~ZWeightedMap() {
    utl::matrix_helpers::Deallocate(&mCellWeights, mHeight);
}

ZWeightedMap& ZWeightedMap::operator=(ZWeightedMap other) {
    swap(*this, other);

    return *this;
}

void swap(ZWeightedMap& left, ZWeightedMap& right) {
    using utl::swap;

    swap(left.mWidth, right.mWidth);
    swap(left.mHeight, right.mHeight);
    swap(left.mCellWeights, right.mCellWeights);
}

int ZWeightedMap::GetWidth() const {
    return mWidth;
}

int ZWeightedMap::GetHeight() const {
    return mHeight;
}

void ZWeightedMap::SetCellWeight(const ZPosition& pos, const ZWeight& weight) {
    SetCellWeight(pos.GetX(), pos.GetY(), weight);
}

void ZWeightedMap::SetCellWeight(int x, int y, const ZWeight& weight) {
    mCellWeights[x][y] = weight;
}

const ZWeight& ZWeightedMap::GetCellWeight(const ZPosition& pos) const {
    return GetCellWeight(pos.GetX(), pos.GetY());
}

const ZWeight& ZWeightedMap::GetCellWeight(int x, int y) const {
    if (x < 0 || x >= mWidth
        || y < 0 || y >= mHeight) {
        return ZWeight::kInfinity;
    }

    return mCellWeights[x][y];
}

char GetWeightChar(const ZWeight& weight) {
    static const char kZeroWeight = '.';
    static const char kInfinityWeight = 'x';
    static const char kPositiveWeight = '#';

    if (weight == ZWeight::kZero) {
        return kZeroWeight;
    } else if (weight == ZWeight::kInfinity) {
        return kInfinityWeight;
    } 
    
    return kPositiveWeight;
}

const utl::ZString ZWeightedMap::ToString() const {
    char* resultBuffer = new char[(mWidth + 1) * mHeight + 1];
    char* currentCharPos = resultBuffer;
    
    for (int x = mWidth - 1; x >= 0; --x) {
        for (int y = 0; y < mHeight; ++y) {
            *(currentCharPos++) = GetWeightChar(mCellWeights[x][y]);
        }
        *(currentCharPos++) = '\n';
    }
    *currentCharPos = 0;
    
    utl::ZString result(resultBuffer);
    delete[] resultBuffer;
    
    return result;
}

}
}
}