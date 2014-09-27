#pragma once

#include "model/Position.h"
#include "model/DungeonLevel.h"
#include "model/WeightedCell.h"

namespace prz {
    namespace mdl {

        struct PathCellConnection;

        class ZPathFinder {
        public:
            ZPathFinder() = delete;

            static const ZWeight kSolidRockCellWeight;
            static const ZWeight kEmptyCellWeight;

            static const ZWeight kTunnelTurnPenalty;
            static const int kEstimatedPathWeightFactor;

            static bool CellMustBeDigged(ZWeight** mapCellWeight, const ZPosition& position);
            static bool CellMustBeDigged(ZWeight** mapCellWeight, int x, int y);

            typedef std::vector<ZPosition> PathCells;
            //returns list of path cells starting from finishCell if path between cells exists, or empty list otherwise
            static PathCells FindPathBetweenCells(ZWeight** mapCellWeight, int levelWidth, int levelHeight, const ZPosition& startCellPosition, const ZPosition& finishCellPosition, bool diggingIsAllowed = false);

        private:
            // returns true if path from cell to neighbor is shorter than previous path to that cell
            static ZWeightedCell* ZPathFinder::CreateNextPathCellIfMorePromising(const ZWeightedCell& currentCell, const ZPositionDiff& currentMoveDiff, const ZPosition& finishCellPosition, PathCellConnection** pathConnections);

            static ZWeight** sMapCellWeight;
        };
    }
}
