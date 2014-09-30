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
    ZDirection();
    virtual ~ZDirection() = default;

    void Turn(ETurnDirection::Type direction);
    ZDirection TurnCopy(ETurnDirection::Type direction) const;

    ZPositionDiff PredictMove() const;
    void Align(const ZPositionDiff& diff);

private:
    typedef std::map<int, ZPositionDiff> ZPredictedMovesMap;
    typedef std::map<ETurnDirection::Type, int> ZTurnDirectionToAngleMap;

    static const double kPi;
    static const ZPredictedMovesMap kPredictedMoves;
    static const ZTurnDirectionToAngleMap kTurnDirectionToAngle;

    static float GetAngleFromDiff(const ZPositionDiff& diff);
    static ZPositionDiff GetPositionDiff(int angle);

    void Turn(int angle);

    int mAngle;
};

}
}

