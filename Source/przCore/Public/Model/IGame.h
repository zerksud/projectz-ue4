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

#pragma once

#include <string>

#include "Model/ZMinimap.h"
#include "Model/ZDungeon.h"

namespace prz {
namespace mdl {

class IGame {
public:
    virtual ~IGame() = default;

    virtual void StartNewGame() = 0;

    virtual bool TryToMovePlayer(EDirection direction) = 0;
    virtual void TurnPlayer(EDirection direction) = 0;

    virtual const ZMinimap GetMinimap() = 0;
    virtual unsigned int GetMinimapSize() const = 0;

    virtual const std::string& GetLogHistory() = 0;
};

}
}
