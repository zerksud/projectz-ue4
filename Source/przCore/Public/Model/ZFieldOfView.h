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

namespace prz {
namespace mdl {

enum class EDungeonCell {
    Unknown,
    SolidRock,
    Emptiness,
    DownStaircase,
    UpStaircase,
    Monster
};

class ZFieldOfView {
public:
    ZFieldOfView();
    // ZFieldOfView owns fovData
    ZFieldOfView(unsigned int radius, EDungeonCell** fovData);
    ZFieldOfView(const ZFieldOfView& other);
    ZFieldOfView(ZFieldOfView&& other);
    virtual ~ZFieldOfView();

    ZFieldOfView& operator=(ZFieldOfView other);

    friend void swap(ZFieldOfView& left, ZFieldOfView& right);

    unsigned int GetRadius() const;
    EDungeonCell GetCell(int dx, int dy) const;

private:
    unsigned int mRadius;
    EDungeonCell** mData;
};

}
}
