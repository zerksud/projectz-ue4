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

#include "Utils/StandardLibrary/ZString.h"
#include "Utils/StandardLibrary/Utility.h"

#include "Model/Path/ZWeightedCell.h"
#include "Model/ZPosition.h"

namespace prz {
namespace mdl {
namespace path {

class ZWeightedMap {
public:
    ZWeightedMap(int width, int height, const ZWeight& defaultCellWeight);
    ZWeightedMap(const ZWeightedMap& other);
    ~ZWeightedMap();

    ZWeightedMap& operator=(ZWeightedMap other);

    friend void swap(ZWeightedMap& left, ZWeightedMap& right);

    int GetWidth() const;
    int GetHeight() const;

    void SetCellWeight(const ZPosition& pos, const ZWeight& weight);
    void SetCellWeight(int x, int y, const ZWeight& weight);

    const ZWeight& GetCellWeight(const ZPosition& pos) const;
    const ZWeight& GetCellWeight(int x, int y) const;

    const utl::ZString ToString() const;

private:
    int mWidth;
    int mHeight;
    ZWeight** mCellWeights;
};

}
}
}
