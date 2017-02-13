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

#include "przCorePCH.h"
#include "Model/ZPosition.h"

#include "Utils/StringHelpers.h"

namespace prz {
namespace mdl {

ZPositionDiff::ZPositionDiff() {
    mdX = 0;
    mdY = 0;
}

ZPositionDiff::ZPositionDiff(int dXValue, int dYValue) {
    mdX = dXValue;
    mdY = dYValue;
}

int ZPositionDiff::GetdX() const {
    return mdX;
}

int ZPositionDiff::GetdY() const {
    return mdY;
}

bool ZPositionDiff::operator==(const ZPositionDiff& other) const {
    return (mdX == other.mdX && mdY == other.mdY);
}

bool ZPositionDiff::operator!=(const ZPositionDiff& other) const {
    return !(*this == other);
}

int ZPositionDiff::LengthSquare() const {
    return mdX * mdX + mdY * mdY;
}

std::string ZPositionDiff::ToString() const {
    return utl::string_helpers::Format("[%d;%d]", mdX, mdY);
}

ZPositionDiff& ZPositionDiff::operator+=(const ZPositionDiff& other) {
    *this = *this + other;
    return *this;
}

const ZPositionDiff operator*(int mult, const ZPositionDiff& diff) {
    return ZPositionDiff(diff.mdX * mult, diff.mdY * mult);
}

const ZPositionDiff operator*(const ZPositionDiff& diff, int mult) {
    return mult * diff;
}

const ZPositionDiff operator+(const ZPositionDiff& left, const ZPositionDiff& right) {
    return ZPositionDiff(left.mdX + right.mdX, left.mdY + right.mdY);
}

const ZPositionDiff operator-(const ZPositionDiff& left, const ZPositionDiff& right) {
    return ZPositionDiff(left.mdX - right.mdX, left.mdY - right.mdY);
}

void swap(ZPositionDiff& left, ZPositionDiff&right) {
    using std::swap;

    swap(left.mdX, right.mdX);
    swap(left.mdY, right.mdY);
}

ZPosition::ZPosition() {
    mX = 0;
    mY = 0;
}

ZPosition::ZPosition(const ZPosition& other) {
    mX = other.mX;
    mY = other.mY;
}

ZPosition::ZPosition(int xValue, int yValue) {
    mX = xValue;
    mY = yValue;
}

int ZPosition::GetX() const {
    return mX;
}

int ZPosition::GetY() const {
    return mY;
}

const ZPosition ZPosition::operator+(const ZPositionDiff& diff) const {
    int x = mX + diff.GetdX();
    int y = mY + diff.GetdY();

    return ZPosition(x, y);
}

const ZPosition ZPosition::operator-(const ZPositionDiff& diff) const {
    int x = mX - diff.GetdX();
    int y = mY - diff.GetdY();

    return ZPosition(x, y);
}

const ZPositionDiff ZPosition::operator-(const ZPosition& other) const {
    int dx = mX - other.mX;
    int dy = mY - other.mY;

    return ZPositionDiff(dx, dy);
}

bool ZPosition::operator==(const ZPosition& other) const {
    return (mX == other.mX && mY == other.mY);
}

bool ZPosition::operator!=(const ZPosition& other) const {
    return !(*this == other);
}

ZPosition& ZPosition::operator=(const ZPosition& other) {
    mX = other.mX;
    mY = other.mY;

    return *this;
}

ZPosition& ZPosition::operator+=(const ZPositionDiff& diff) {
    *this = *this + diff;

    return *this;
}

std::string ZPosition::ToString() const {
    return utl::string_helpers::Format("[%d;%d]", mX, mY);
}

}
}
