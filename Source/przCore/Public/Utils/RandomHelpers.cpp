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
#include "Utils/RandomHelpers.h"

#include "Utils/StandardLibrary/StdLib.h"
#include "Utils/StandardLibrary/Time.h"

namespace prz {
namespace utl {
namespace random_helpers {

void Initialize() {
    utl::srand(static_cast<unsigned int>(utl::time(0)));
}

bool FlipCoin() {
    if (utl::rand() % 2) {
        return true;
    }

    return false;
}

int GetRandomValue(int maxValue) {
    int value = utl::rand() % (maxValue + 1);
    return value;
}

int GetRandomValue(int minValue, int maxValue) {
    int value = minValue + utl::rand() % (maxValue - minValue + 1);
    return value;
}

}
}
}
