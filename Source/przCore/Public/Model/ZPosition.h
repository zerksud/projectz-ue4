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

#include <string>
#include <utility>

namespace prz {
namespace mdl {

class ZPositionDiff final {
public:
    ZPositionDiff();
    ZPositionDiff(int dXValue, int dYValue);

    int GetdX() const;
    int GetdY() const;

    bool operator==(const ZPositionDiff& other) const;
    bool operator!=(const ZPositionDiff& other) const;
    
    int LengthSquare() const;

    std::string ToString() const;

    ZPositionDiff& operator+=(const ZPositionDiff& other);
    
    friend const ZPositionDiff operator*(int mult, const ZPositionDiff& diff);
    friend const ZPositionDiff operator*(const ZPositionDiff& diff, int mult);
    friend const ZPositionDiff operator+(const ZPositionDiff& left, const ZPositionDiff& right);
    friend const ZPositionDiff operator-(const ZPositionDiff& left, const ZPositionDiff& right);

    friend void swap(ZPositionDiff& left, ZPositionDiff& right);

private:
    int mdX;
    int mdY;
};

class ZPosition final {
public:
    ZPosition();
    ZPosition(const ZPosition& other);
    ZPosition(int xValue, int yValue);

    int GetX() const;
    int GetY() const;

    const ZPosition operator+(const ZPositionDiff& diff) const;
    const ZPosition operator-(const ZPositionDiff& diff) const;
    const ZPositionDiff operator-(const ZPosition& other) const;

    bool operator==(const ZPosition& other) const;
    bool operator!=(const ZPosition& other) const;
    
    ZPosition& operator=(const ZPosition& other);
    ZPosition& operator+=(const ZPositionDiff& diff);

    std::string ToString() const;

private:
    int mX;
    int mY;
};

}
}
