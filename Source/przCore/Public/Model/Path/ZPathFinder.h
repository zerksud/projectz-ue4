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

#include <vector>

#include "Model/ZPosition.h"
#include "Model/Path/ZWeightedMap.h"

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

    typedef std::vector<ZPosition> PathCells;
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
