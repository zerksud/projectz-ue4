#pragma once

#include "Model/Position.h"

#include <map>

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
    static float GetAngleFromDiff(const ZPositionDiff& diff);
    static ZPositionDiff GetPositionDiff(int angle);

    void Turn(int angle);

    int mAngle;
};

}
}

