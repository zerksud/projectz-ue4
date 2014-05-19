#pragma once

#include "model/Position.h"

#include <map>

namespace prz {
    namespace mdl {
        class ZDirection {
        public:
            ZDirection();
            virtual ~ZDirection();

            void RotateLeft();
            void RotateRight();
            void RotateBack();
            void RotateHalfLeft();
            void RotateHalfRight();

            ZPositionDiff PredictMove() const;
            void Align(const ZPositionDiff& diff);

        private:
            typedef std::map<int, ZPositionDiff> ZPredictedMovesMap;

            static const double kPi;
            static ZPredictedMovesMap kPredictedMoves;
            static int GetAngleFromDiff(const ZPositionDiff& diff);
            static ZPositionDiff GetPositionDiff(int angle);

            void Rotate(int angle);

            int mAngle;
        };
    }
}

