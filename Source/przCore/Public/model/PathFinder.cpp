#include "przCorePCH.h"
#include "model/PathFinder.h"

#include <cmath>
#include <queue>

#include "utils/MatrixHelpers.h"
#include "utils/RandomHelpers.h"

namespace prz {
    namespace mdl {

        const ZWeight ZPathFinder::kSolidRockCellWeight = 100;
        const ZWeight ZPathFinder::kEmptyCellWeight = 0;

        const ZWeight ZPathFinder::kTunnelTurnPenalty = 1000;
        const int ZPathFinder::kEstimatedPathWeightFactor = 25;

        ZWeight** ZPathFinder::sMapCellWeight = 0;

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

        bool ZPathFinder::CellMustBeDigged(ZWeight** mapCellWeight, const ZPosition& position) {
            return CellMustBeDigged(mapCellWeight, position.GetX(), position.GetY());
        }

        bool ZPathFinder::CellMustBeDigged(ZWeight** mapCellWeight, int x, int y) {
            return mapCellWeight[x][y] != kEmptyCellWeight;
        }

        ZWeightedCell* ZPathFinder::CreateNextPathCellIfMorePromising(const ZWeightedCell& currentCell, const ZPositionDiff& currentMoveDiff, const ZPosition& finishCellPosition, PathCellConnection** pathConnections) {
            ZPosition nextCellPosition = currentCell.position + currentMoveDiff;
            ZWeight pathToNextCellWeight = currentCell.pathToCellWeight + sMapCellWeight[nextCellPosition.GetX()][nextCellPosition.GetY()];

            ZPosition* previousCellPositionPtr = pathConnections[currentCell.position.GetX()][currentCell.position.GetY()].previousPathCell;
            if (previousCellPositionPtr) {
                bool currentMoveIsTurning = currentMoveDiff != currentCell.position - *previousCellPositionPtr;
                bool currentMoveIsInsideRock = CellMustBeDigged(sMapCellWeight, currentCell.position) && CellMustBeDigged(sMapCellWeight, nextCellPosition);
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

        ZPathFinder::PathCells ZPathFinder::FindPathBetweenCells(ZWeight** mapCellWeight, int levelWidth, int levelHeight, const ZPosition& startCellPosition, const ZPosition& finishCellPosition, bool diggingIsAllowed) {
            sMapCellWeight = mapCellWeight;

            PathCellConnection** pathConnections;
            utl::ZMatrix::Allocate(&pathConnections, levelWidth, levelHeight);

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
                if (currentCellPosition.GetX() > 0 && (diggingIsAllowed || !CellMustBeDigged(sMapCellWeight, currentCellPosition + diff))) {
                    cell = CreateNextPathCellIfMorePromising(*currentCell, diff, finishCellPosition, pathConnections);
                    if (cell) {
                        queue.push(cell);
                    }
                }

                diff = ZPositionDiff(1, 0);
                if (currentCellPosition.GetX() < levelWidth - 1 && (diggingIsAllowed || !CellMustBeDigged(sMapCellWeight, currentCellPosition + diff))) {
                    cell = CreateNextPathCellIfMorePromising(*currentCell, diff, finishCellPosition, pathConnections);
                    if (cell) {
                        queue.push(cell);
                    }
                }

                diff = ZPositionDiff(0, -1);
                if (currentCellPosition.GetY() > 0 && (diggingIsAllowed || !CellMustBeDigged(sMapCellWeight, currentCellPosition + diff))) {
                    cell = CreateNextPathCellIfMorePromising(*currentCell, diff, finishCellPosition, pathConnections);
                    if (cell) {
                        queue.push(cell);
                    }
                }

                diff = ZPositionDiff(0, 1);
                if (currentCellPosition.GetY() < levelHeight - 1 && (diggingIsAllowed || !CellMustBeDigged(sMapCellWeight, currentCellPosition + diff))) {
                    cell = CreateNextPathCellIfMorePromising(*currentCell, diff, finishCellPosition, pathConnections);
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

            utl::ZMatrix::Deallocate(&pathConnections, levelHeight);

            return path;
        }

    }
}
