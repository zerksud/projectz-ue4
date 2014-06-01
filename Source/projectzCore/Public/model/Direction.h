#pragma once

#include "model/Position.h"

#include <map>

namespace prz {
    namespace mdl {
        class ZDirection {
        public:
            ZDirection();
            virtual ~ZDirection();

            void TurnLeft();
            void TurnRight();
            void TurnBack();
            void TurnForwardLeft();
            void TurnForwardRight();

            ZPositionDiff PredictMove() const;
            void Align(const ZPositionDiff& diff);

        private:
            typedef std::map<int, ZPositionDiff> ZPredictedMovesMap;

            static const double kPi;
            static ZPredictedMovesMap kPredictedMoves;
            static float GetAngleFromDiff(const ZPositionDiff& diff);
            static ZPositionDiff GetPositionDiff(int angle);

            void Rotate(int angle);

            int mAngle;
        };
    }
}

