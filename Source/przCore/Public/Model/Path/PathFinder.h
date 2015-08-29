#pragma once

#include "Utils/StandartLibrary.h"

#include "Model/Position.h"
#include "Model/Path/WeightedMap.h"

namespace prz {
namespace mdl {
namespace path {

struct PathCellConnection;

class ZPathFinder {
public:
    ZPathFinder() = delete;

    static const ZWeight kSolidRockCellWeight;
    static const ZWeight kEmptyCellWeight;

    static const ZWeight kTunnelTurnPenalty;
    static const int kEstimatedPathWeightFactor;

    static int CalcCellsDistance(const ZPosition& left, const ZPosition& right);

    static void BlockCell(ZWeightedMap* map, const ZPosition& position);
    static void BlockCell(ZWeightedMap* map, int x, int y);

    static bool CellIsBlocked(const ZWeightedMap& map, const ZPosition& position);
    static bool CellIsBlocked(const ZWeightedMap& map, int x, int y);

    typedef utl::ZVector<ZPosition> PathCells;
    //returns list of path cells starting from finishCell if path between cells exists, or empty list otherwise
    static PathCells FindPathBetweenCells(const ZWeightedMap& map, const ZPosition& startCellPosition, const ZPosition& finishCellPosition, bool diggingIsAllowed = false);

private:
    static bool CellMustBeDigged(const ZWeightedMap& map, const ZPosition& position);
    static bool CellMustBeDigged(const ZWeightedMap& map, int x, int y);

    // returns true if path from cell to neighbor is shorter than previous path to that cell
    static ZWeightedCell* CreateNextPathCellIfMorePromising(const ZWeightedMap& map, const ZWeightedCell& currentCell, const ZPositionDiff& currentMoveDiff, const ZPosition& finishCellPosition, PathCellConnection** pathConnections);
};

}
}
}
