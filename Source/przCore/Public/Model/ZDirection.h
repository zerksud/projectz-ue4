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

#include "Model/ZPosition.h"

namespace prz {
namespace mdl {

enum class EDirection {
    Forward,
    Left,
    Right,
    Back,
    ForwardLeft,
    ForwardRight,
    BackLeft,
    BackRight
};

class ZDirection {
public:
    static const ZDirection kForward;
    static const ZDirection kLeft;
    static const ZDirection kRight;
    static const ZDirection kBack;

    ZDirection();
    ZDirection(EDirection direction);
    virtual ~ZDirection() = default;

    EDirection GetDirection() const;
    
    ZDirection& Turn(EDirection direction);
    ZDirection TurnCopy(EDirection direction) const;

    ZPositionDiff PredictMove() const;
    void Align(const ZPositionDiff& diff);

private:
    static int GetAngleFromDiff(const ZPositionDiff& diff);
    static ZPositionDiff GetPositionDiff(int angle);

    void Turn(int angle);

    int mAngle;
};

}
}

