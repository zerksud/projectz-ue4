#include "przCorePCH.h"

#include <algorithm>
#include <cmath>

#include "Model/Direction.h"
#include "Utils/LOG.h"

namespace prz {
namespace mdl {
static const double kPi = 3.14159265359;

typedef std::map<int, ZPositionDiff> ZPredictedMovesMap;

static const ZPredictedMovesMap kPredictedMoves = {
        {0, ZPositionDiff(1.0f, 0.0f)},
        {45, ZPositionDiff(1.0f, -1.0f)},
        {90, ZPositionDiff(0.0f, -1.0f)},
        {135, ZPositionDiff(-1.0f, -1.0f)},
        {180, ZPositionDiff(-1.0f, 0.0f)},
        {225, ZPositionDiff(-1.0f, 1.0f)},
        {270, ZPositionDiff(0.0f, 1.0f)},
        {315, ZPositionDiff(1.0f, 1.0f)}};

static const int kTurnDirectionToAngle[] = {
        0,      // Forward
        90,     // Left
        -90,    // Right
        180,    // Back
        45,     // ForwardLeft
        -45,    // ForwardRight
        135,    // BackLeft
        -135    // BackRight
};

const ZDirection ZDirection::kForward;
const ZDirection ZDirection::kLeft(ETurnDirection::Left);
const ZDirection ZDirection::kRight(ETurnDirection::Right);
const ZDirection ZDirection::kBack(ETurnDirection::Back);

ZDirection::ZDirection()
    : mAngle(0) {
}

ZDirection::ZDirection(ETurnDirection::Type direction)
    : mAngle(0) {
    Turn(direction);
}

void ZDirection::Turn(ETurnDirection::Type direction) {
    if (direction > ETurnDirection::BackRight) {
        LOGE("Got unsupported ETurnDirection = %d", direction);
    } else {
        Turn(kTurnDirectionToAngle[direction]);
    }
}

ZDirection ZDirection::TurnCopy(ETurnDirection::Type direction) const {
    ZDirection directionCopy(*this);
    directionCopy.Turn(direction);

    return directionCopy;
}

float ZDirection::GetAngleFromDiff(const ZPositionDiff& diff) {
    int dx = diff.GetdX();
    int dy = -diff.GetdY();
    float alpha = atan2(dy, dx);
    alpha = alpha / kPi * 180.0;

    return alpha;
}

ZPositionDiff ZDirection::GetPositionDiff(int angle) {
    ZPredictedMovesMap::const_iterator pos = kPredictedMoves.find(angle);

    ZPositionDiff diff;
    if (pos != kPredictedMoves.end()) {
        diff = pos->second;
    } else {
        LOGE("Got unsupported angle %d", angle);
    }

    return diff;
}

ZPositionDiff ZDirection::PredictMove() const {
    return GetPositionDiff(mAngle);
}

void ZDirection::Align(const ZPositionDiff& diff) {
    int angle = GetAngleFromDiff(diff);
    int discreteDiff = round((angle - mAngle) / 45.0f) * 45;

    Turn(discreteDiff);
}

void ZDirection::Turn(int angle) {
    int newAngle = mAngle + angle;
    while (newAngle < 0) {
        newAngle += 360;
    }
    newAngle %= 360;

    mAngle = newAngle;
}

}
}
