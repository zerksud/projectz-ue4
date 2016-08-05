// Copyright 2016 Max Molodtsov <zerksud@gmail.com>
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
#include "Model/Path/ZWeightedCell.h"

namespace prz {
namespace mdl {
namespace path {

static const int kInfiniteWeight = -1;

const ZWeight ZWeight::kInfinity(kInfiniteWeight);
const ZWeight ZWeight::kZero(0);

ZWeight::ZWeight()
    : mValue(kInfiniteWeight) {
}

ZWeight::ZWeight(int value) {
    if (value >= 0) {
        mValue = value;
    } else {
        mValue = kInfiniteWeight;
    }
}

bool ZWeight::operator==(const ZWeight& other) const {
    return this->mValue == other.mValue;
}

bool ZWeight::operator!=(const ZWeight& other) const {
    return !(*this == other);
}

bool ZWeight::operator>(const ZWeight& other) const {
    if (other.mValue == kInfiniteWeight) {
        return false;
    }

    if (this->mValue == kInfiniteWeight) {
        return true;
    }

    return this->mValue > other.mValue;
}

ZWeight ZWeight::operator+(const ZWeight& other) const {
    if (this->mValue == kInfiniteWeight || other.mValue == kInfiniteWeight) {
        return kInfinity;
    } else {
        return this->mValue + other.mValue;
    }
}

ZWeight& ZWeight::operator+=(const ZWeight& other) {
    *this = *this + other;
    return *this;
}

ZWeightedCell::ZWeightedCell()
    : position(ZPosition(0, 0)), pathToCellWeight(ZWeight::kInfinity), pathFromCellEstimatedWeight(ZWeight::kInfinity) {
}

ZWeightedCell::ZWeightedCell(const ZPosition& pPosition, const ZWeight& pPathToCellWeight, const ZWeight& pPathFromCellEstimatedWeight)
    : position(pPosition), pathToCellWeight(pPathToCellWeight), pathFromCellEstimatedWeight(pPathFromCellEstimatedWeight) {
}

ZWeight ZWeightedCell::GetTotalPathWeight() const {
    return pathToCellWeight + pathFromCellEstimatedWeight;
}

}
}
}
