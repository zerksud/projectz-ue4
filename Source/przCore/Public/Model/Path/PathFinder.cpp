#include "przCorePCH.h"
#include "Model/Path/PathFinder.h"

#include <cmath>
#include <queue>

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
    };

    PathCellConnection(const PathCellConnection& other) {
        pathToCellWeight = other.pathToCellWeight;
        if (other.previousPathCell) {
            previousPathCell = new ZPosition(*other.previousPathCell);
        } else {
            previousPathCell = nullptr;
        }
    }

    friend void swap(PathCellConnection& left, PathCellConnection& right) {
        using std::swap;

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

int CalcCellsDistance(const ZPosition& left, const ZPosition& right) {
    ZPositionDiff diff = right - left;
    int distance = std::abs(diff.GetdX()) + std::abs(diff.GetdY());

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
    utl::ZMatrix::Allocate(&pathConnections, map.GetWidth(), map.GetHeight());

    std::priority_queue<ZWeightedCell*, std::vector<ZWeightedCell*>, ZWeightedCellPtrAscendingOrder> queue;

    ZWeight pathFromStartCellEstimatedWeight = CalcCellsDistance(startCellPosition, finishCellPosition);
    ZWeightedCell* startCell = new ZWeightedCell(startCellPosition, 0, pathFromStartCellEstimatedWeight);
    queue.push(startCell);

    while (!queue.empty() && queue.top()->position != finishCellPosition) {
        ZWeightedCell* currentCell = queue.top();
        ZPosition currentCellPosition = currentCell->position;
        queue.pop();

        ZWeightedCell* cell;
        ZPositionDiff diff = ZPositionDiff(-1, 0);
        if (currentCellPosition.GetX() > 0 && (diggingIsAllowed || !CellMustBeDigged(map, currentCellPosition + diff))) {
            cell = CreateNextPathCellIfMorePromising(map, *currentCell, diff, finishCellPosition, pathConnections);
            if (cell) {
                queue.push(cell);
            }
        }

        diff = ZPositionDiff(1, 0);
        if (currentCellPosition.GetX() < map.GetWidth() - 1 && (diggingIsAllowed || !CellMustBeDigged(map, currentCellPosition + diff))) {
            cell = CreateNextPathCellIfMorePromising(map, *currentCell, diff, finishCellPosition, pathConnections);
            if (cell) {
                queue.push(cell);
            }
        }

        diff = ZPositionDiff(0, -1);
        if (currentCellPosition.GetY() > 0 && (diggingIsAllowed || !CellMustBeDigged(map, currentCellPosition + diff))) {
            cell = CreateNextPathCellIfMorePromising(map, *currentCell, diff, finishCellPosition, pathConnections);
            if (cell) {
                queue.push(cell);
            }
        }

        diff = ZPositionDiff(0, 1);
        if (currentCellPosition.GetY() < map.GetHeight() - 1 && (diggingIsAllowed || !CellMustBeDigged(map, currentCellPosition + diff))) {
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

    utl::ZMatrix::Deallocate(&pathConnections, map.GetHeight());

    return path;
}

}
}
}
