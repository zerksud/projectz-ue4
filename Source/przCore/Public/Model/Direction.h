#pragma once

#include "Model/Position.h"

#include <map>

namespace prz {
namespace mdl {

namespace ETurnDirection {
enum Type {
    Forward = 0,
    Left,
    Right,
    Back,
    ForwardLeft,
    ForwardRight,
    BackLeft,
    BackRight
};
}

class ZDirection {
public:
    static const ZDirection kForward;
    static const ZDirection kLeft;
    static const ZDirection kRight;
    static const ZDirection kBack;

    ZDirection();
    ZDirection(ETurnDirection::Type direction);
    virtual ~ZDirection() = default;

    void Turn(ETurnDirection::Type direction);
    ZDirection TurnCopy(ETurnDirection::Type direction) const;

    ZPositionDiff PredictMove() const;
    void Align(const ZPositionDiff& diff);

private:
    static float GetAngleFromDiff(const ZPositionDiff& diff);
    static ZPositionDiff GetPositionDiff(int angle);

    void Turn(int angle);

    int mAngle;
};

}
}

