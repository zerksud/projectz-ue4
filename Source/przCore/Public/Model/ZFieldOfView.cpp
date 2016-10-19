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
#include "ZFieldOfView.h"

#include <utility>

#include "Utils/MatrixHelpers.h"

namespace prz {
namespace mdl {

ZFieldOfView::ZFieldOfView():
mRadius(0) {
    utl::matrix_helpers::Allocate(&mData, 1, 1, EDungeonCell::Monster);
}

ZFieldOfView::ZFieldOfView(unsigned int radius, EDungeonCell** fovData) :
mRadius(radius),
mData(fovData) {
}

ZFieldOfView::ZFieldOfView(const ZFieldOfView& other) {
    mRadius = other.mRadius;
    utl::matrix_helpers::AllocateAndCopy(&mData, other.mData, mRadius * 2 + 1);
}

ZFieldOfView::ZFieldOfView(ZFieldOfView&& other) {
    swap(*this, other);

    other.mRadius = 0;
    utl::matrix_helpers::Allocate(&other.mData, 1, 1, EDungeonCell::Monster);
}

ZFieldOfView::~ZFieldOfView() {
    utl::matrix_helpers::Deallocate(&mData, mRadius * 2 + 1);
}

ZFieldOfView& ZFieldOfView::operator=(ZFieldOfView other) {
    swap(*this, other);

    return *this;
}

void swap(ZFieldOfView& left, ZFieldOfView& right) {
    using std::swap;

    swap(left.mRadius, right.mRadius);
    swap(left.mData, right.mData);
}

EDungeonCell ZFieldOfView::GetCell(int dx, int dy) const {
    if ((unsigned int)(dx * dx + dy * dy) > mRadius * mRadius) {
        return EDungeonCell::Unknown;
    }

    return mData[mRadius + dx][mRadius + dy];
}

}
}
