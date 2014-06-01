#include "projectzCorePrivatePCH.h"

#include <algorithm>
#include <cmath>

#include "model/Direction.h"
#include "utils/LOG_ANSI.h"

namespace prz {
    namespace mdl {
        const double ZDirection::kPi = 3.14159265359;

        ZDirection::ZPredictedMovesMap ZDirection::kPredictedMoves = {
            {0, ZPositionDiff(1.0f, 0.0f)},
            {45, ZPositionDiff(1.0f, -1.0f)},
            {90, ZPositionDiff(0.0f, -1.0f)},
            {135, ZPositionDiff(-1.0f, -1.0f)},
            {180, ZPositionDiff(-1.0f, 0.0f)},
            {225, ZPositionDiff(-1.0f, 1.0f)},
            {270, ZPositionDiff(0.0f, 1.0f)},
            {315, ZPositionDiff(1.0f, 1.0f)}};

        ZDirection::ZDirection() {
            mAngle = 0;
        }

        ZDirection::~ZDirection() {
        }

        void ZDirection::TurnLeft() {
            Rotate(90);
        }

        void ZDirection::TurnRight() {
            Rotate(-90);
        }

        void ZDirection::TurnBack() {
            Rotate(180);
        }

        void ZDirection::TurnForwardLeft() {
            Rotate(45);
        }

        void ZDirection::TurnForwardRight() {
            Rotate(-45);
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

            Rotate(discreteDiff);
        }

        void ZDirection::Rotate(int angle) {
            int newAngle = mAngle + angle;
            while (newAngle < 0) {
                newAngle += 360;
            }
            newAngle %= 360;

            mAngle = newAngle;
        }
    }
}
