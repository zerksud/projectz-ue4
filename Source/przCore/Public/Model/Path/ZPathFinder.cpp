// Copyright 2016 Max Molodtsov
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

#include "przCorePCH.h"
#include "Model/Path/ZPathFinder.h"

#include "Utils/StandardLibrary/Math.h"
#include "Utils/StandardLibrary/ZQueue.h"
#include "Utils/StandardLibrary/Utility.h"
#include "Utils/StandardLibrary/ZVector.h"
#include "Utils/MatrixHelpers.h"
#include "Utils/RandomHelpers.h"

namespace prz {
namespace mdl {
namespace path {

const ZWeight ZPathFinder::kSolidRockCellWeight = 100;
const ZWeight ZPathFinder::kEmptyCellWeight = 0;

const ZWeight ZPathFinder::kTunnelTurnPenalty = 1000;
const int ZPathFinder::kEstimatedPathWeightFactor = 25;

class ZWeightedCellPtrAscendingOrder {
public:
    bool operator() (const ZWeightedCell* left, const ZWeightedCell* right) {
        return left->GetTotalPathWeight() > right->GetTotalPathWeight();
    }
};

struct PathCellConnection {
    ZWeight pathToCellWeight;
    ZPosition* previousPathCell;

    PathCellConnection()
        : pathToCellWeight(ZWeight::kInfinity), previousPathCell(nullptr) {
    }

    PathCellConnection(const PathCellConnection& other) {
        pathToCellWeight = other.pathToCellWeight;
        if (other.previousPathCell) {
            previousPathCell = new ZPosition(*other.previousPathCell);
        } else {
            previousPathCell = nullptr;
        }
    }

    friend void swap(PathCellConnection& left, PathCellConnection& right) {
        using utl::swap;

        swap(left.pathToCellWeight, right.pathToCellWeight);
        swap(left.previousPathCell, right.previousPathCell);
    }

    PathCellConnection(PathCellConnection&& other) {
        swap(*this, other);
    }

    PathCellConnection(ZWeight pPathToCellWeight, const ZPosition& pPreviousPathCell)
        : pathToCellWeight(pPathToCellWeight), previousPathCell(new ZPosition(pPreviousPathCell)) {
    }

    ~PathCellConnection() {
        delete previousPathCell;
    }

    PathCellConnection& operator=(PathCellConnection other) {
        swap(*this, other);
        return *this;
    }
};

int ZPathFinder::CalcCellsDistance(const ZPosition& left, const ZPosition& right) {
    ZPositionDiff diff = right - left;
    int distance = utl::abs(diff.GetdX()) + utl::abs(diff.GetdY());

    return distance;
}

bool ZPathFinder::CellMustBeDigged(const ZWeightedMap& map, const ZPosition& position) {
    return CellMustBeDigged(map, position.GetX(), position.GetY());
}

bool ZPathFinder::CellMustBeDigged(const ZWeightedMap& map, int x, int y) {
    return map.GetCellWeight(x, y) != kEmptyCellWeight;
}

void ZPathFinder::BlockCell(ZWeightedMap* map, const ZPosition& position) {
    BlockCell(map, position.GetX(), position.GetY());
}

void ZPathFinder::BlockCell(ZWeightedMap* map, int x, int y) {
    map->SetCellWeight(x, y, path::ZWeight::kInfinity);
}

bool ZPathFinder::CellIsBlocked(const ZWeightedMap& map, const ZPosition& position) {
    return CellIsBlocked(map, position.GetX(), position.GetY());
}

bool ZPathFinder::CellIsBlocked(const ZWeightedMap& map, int x, int y) {
    return (map.GetCellWeight(x, y) == path::ZWeight::kInfinity);
}

ZWeightedCell* ZPathFinder::CreateNextPathCellIfMorePromising(const ZWeightedMap& map, const ZWeightedCell& currentCell, const ZPositionDiff& currentMoveDiff, const ZPosition& finishCellPosition, PathCellConnection** pathConnections) {
    ZPosition nextCellPosition = currentCell.position + currentMoveDiff;
    ZWeight pathToNextCellWeight = currentCell.pathToCellWeight + map.GetCellWeight(nextCellPosition);

    ZPosition* previousCellPositionPtr = pathConnections[currentCell.position.GetX()][currentCell.position.GetY()].previousPathCell;
    if (previousCellPositionPtr) {
        bool currentMoveIsTurning = currentMoveDiff != currentCell.position - *previousCellPositionPtr;
        bool currentMoveIsInsideRock = CellMustBeDigged(map, currentCell.position) && CellMustBeDigged(map, nextCellPosition);
        if (currentMoveIsTurning && currentMoveIsInsideRock) {
            pathToNextCellWeight += kTunnelTurnPenalty;
        }
    }

    PathCellConnection* nextCellConnection = &pathConnections[nextCellPosition.GetX()][nextCellPosition.GetY()];
    if (nextCellConnection->pathToCellWeight > pathToNextCellWeight) {
        int pathFromCellEstimatedWeight = CalcCellsDistance(nextCellPosition, finishCellPosition) * kEstimatedPathWeightFactor;
        *nextCellConnection = PathCellConnection(pathToNextCellWeight, currentCell.position);
        return new ZWeightedCell(nextCellPosition, pathToNextCellWeight, pathFromCellEstimatedWeight);
    }

    return nullptr;
}

ZPathFinder::PathCells ZPathFinder::FindPathBetweenCells(const ZWeightedMap& map, const ZPosition& startCellPosition, const ZPosition& finishCellPosition, bool diggingIsAllowed) {
    PathCellConnection** pathConnections;
    utl::matrix_helpers::Allocate(&pathConnections, map.GetWidth(), map.GetHeight());

    utl::ZPriorityQueue<ZWeightedCell*, utl::ZVector<ZWeightedCell*>, ZWeightedCellPtrAscendingOrder> queue;

    ZWeight pathFromStartCellEstimatedWeight = CalcCellsDistance(startCellPosition, finishCellPosition);
    ZWeightedCell* startCell = new ZWeightedCell(startCellPosition, 0, pathFromStartCellEstimatedWeight);
    queue.push(startCell);

    while (!queue.empty() && queue.top()->position != finishCellPosition) {
        ZWeightedCell* currentCell = queue.top();
        ZPosition currentCellPosition = currentCell->position;
        queue.pop();

        ZWeightedCell* cell;
        ZPositionDiff diff = ZPositionDiff(-1, 0);
        if (!CellIsBlocked(map, currentCellPosition + diff)
            && (diggingIsAllowed || !CellMustBeDigged(map, currentCellPosition + diff))) {
            cell = CreateNextPathCellIfMorePromising(map, *currentCell, diff, finishCellPosition, pathConnections);
            if (cell) {
                queue.push(cell);
            }
        }

        diff = ZPositionDiff(1, 0);
        if (!CellIsBlocked(map, currentCellPosition + diff)
            && (diggingIsAllowed || !CellMustBeDigged(map, currentCellPosition + diff))) {
            cell = CreateNextPathCellIfMorePromising(map, *currentCell, diff, finishCellPosition, pathConnections);
            if (cell) {
                queue.push(cell);
            }
        }

        diff = ZPositionDiff(0, -1);
        if (!CellIsBlocked(map, currentCellPosition + diff)
            && (diggingIsAllowed || !CellMustBeDigged(map, currentCellPosition + diff))) {
            cell = CreateNextPathCellIfMorePromising(map, *currentCell, diff, finishCellPosition, pathConnections);
            if (cell) {
                queue.push(cell);
            }
        }

        diff = ZPositionDiff(0, 1);
        if (!CellIsBlocked(map, currentCellPosition + diff)
            && (diggingIsAllowed || !CellMustBeDigged(map, currentCellPosition + diff))) {
            cell = CreateNextPathCellIfMorePromising(map, *currentCell, diff, finishCellPosition, pathConnections);
            if (cell) {
                queue.push(cell);
            }
        }

        delete currentCell;
    }

    PathCells path;
    if (queue.top()->position == finishCellPosition) {
        path.push_back(finishCellPosition);
        ZPosition previousPathCell = finishCellPosition;
        while (previousPathCell != startCellPosition) {
            previousPathCell = *pathConnections[previousPathCell.GetX()][previousPathCell.GetY()].previousPathCell;
            path.push_back(previousPathCell);
        }
    }

    while (!queue.empty()) {
        ZWeightedCell* cell = queue.top();
        queue.pop();
        delete cell;
    }

    utl::matrix_helpers::Deallocate(&pathConnections, map.GetHeight());

    return path;
}

}
}
}
