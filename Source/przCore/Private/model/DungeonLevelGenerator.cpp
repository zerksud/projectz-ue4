#include "przCorePrivatePCH.h"
#include "model/DungeonLevelGenerator.h"

#include <algorithm>
#include <queue>
#include <utility>

#include "utils/MatrixHelpers.h"
#include "utils/RandomHelpers.h"

namespace prz {
    namespace mdl {

        const int ZDungeonLevelGenerator::kDungeonLevelWidth = 64;
        const int ZDungeonLevelGenerator::kDungeonLevelHeight = 64;

        const int ZDungeonLevelGenerator::kSubDungeonMinSize = 8;
        const int ZDungeonLevelGenerator::kRoomMaxSize = 10;
        const int ZDungeonLevelGenerator::kRoomMinSize = 5;

        const ZWeight ZDungeonLevelGenerator::kSolidRockCellWeight = 100;
        const ZWeight ZDungeonLevelGenerator::kEmptyCellWeight = 0;

        const ZWeight ZDungeonLevelGenerator::kTunnelTurnPenalty = 1000;
        const int ZDungeonLevelGenerator::kEstimatedPathWeightFactor = 25;
        const float ZDungeonLevelGenerator::kRoomCountFractionToDigRandomTunnelsFrom = 0.25f;

        struct SubDungeon {
            int x1;
            int y1;
            int x2;
            int y2;

            ZPosition someValidCell;

            SubDungeon(int pX1, int pY1, int pX2, int pY2)
                : x1(pX1), y1(pY1), x2(pX2), y2(pY2), someValidCell(-1, -1) {
            }

            SubDungeon(int pX1, int pY1, int pX2, int pY2, const ZPosition& pSomeValidCell)
                : x1(pX1), y1(pY1), x2(pX2), y2(pY2), someValidCell(pSomeValidCell) {
            }

            int GetWidth() {
                return (x2 - x1 + 1);
            }

            int GetHeight() {
                return (y2 - y1 + 1);
            }
        };

        struct BSPTreeNode {
            SubDungeon dungeon;

            BSPTreeNode* lowerSubDungeon;
            BSPTreeNode* higherSubDungeon;

            BSPTreeNode(int x1, int y1, int x2, int y2)
                : dungeon(x1, y1, x2, y2), lowerSubDungeon(nullptr), higherSubDungeon(nullptr) {
            }

            ~BSPTreeNode() {
                delete lowerSubDungeon;
                delete higherSubDungeon;
            }
        };

        class ZWeightedCellPtrAscendingOrder {
        public:
            bool operator() (const ZWeightedCell* left, const ZWeightedCell* right) {
                return left->GetTotalPathWeight() > right->GetTotalPathWeight();
            }
        };

        int CalcCellsDistance(const ZPosition& left, const ZPosition& right) {
            ZPositionDiff diff = right - left;
            int distance = std::abs(diff.GetdX()) + std::abs(diff.GetdY());

            return distance;
        }

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

        bool ZDungeonLevelGenerator::CellMustBeDigged(const ZPosition& position) const {
            return mMapCellWeight[position.GetX()][position.GetY()] != kEmptyCellWeight;
        }

        ZWeightedCell* ZDungeonLevelGenerator::CreateNextPathCell(const ZWeightedCell& currentCell, int dx, int dy, const ZPosition& finishCellPosition, PathCellConnection** pathConnections) {
            ZPositionDiff currentMoveDiff = ZPositionDiff(dx, dy);
            ZPosition nextCellPosition = currentCell.position + currentMoveDiff;
            ZWeight pathToNextCellWeight = currentCell.pathToCellWeight + mMapCellWeight[nextCellPosition.GetX()][nextCellPosition.GetY()];

            ZPosition* previousCellPositionPtr = pathConnections[currentCell.position.GetX()][currentCell.position.GetY()].previousPathCell;
            if (previousCellPositionPtr) {
                bool currentMoveIsTurning = currentMoveDiff != currentCell.position - *previousCellPositionPtr;
                bool currentMoveIsInsideRock = CellMustBeDigged(currentCell.position) && CellMustBeDigged(nextCellPosition);
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

        void ZDungeonLevelGenerator::ConnectCells(const ZPosition& someCell, const ZPosition& anotherCell) {
            PathCellConnection** pathConnections;
            utl::ZMatrix::Allocate(&pathConnections, kDungeonLevelWidth, kDungeonLevelHeight);

            std::priority_queue<ZWeightedCell*, std::vector<ZWeightedCell*>, ZWeightedCellPtrAscendingOrder> queue;
            ZPosition startCellPosition = someCell;
            ZPosition finishCellPosition = anotherCell;

            ZWeight pathFromStartCellEstimatedWeight = CalcCellsDistance(startCellPosition, finishCellPosition);
            ZWeightedCell* startCell = new ZWeightedCell(startCellPosition, 0, pathFromStartCellEstimatedWeight);
            queue.push(startCell);

            while (!queue.empty() && queue.top()->position != finishCellPosition) {
                ZWeightedCell* currentCell = queue.top();
                queue.pop();

                ZWeightedCell* cell;
                if (currentCell->position.GetX() > 0) {
                    cell = CreateNextPathCell(*currentCell, -1, 0, finishCellPosition, pathConnections);
                    if (cell) {
                        queue.push(cell);
                    }
                }

                if (currentCell->position.GetX() < kDungeonLevelWidth - 1) {
                    cell = CreateNextPathCell(*currentCell, 1, 0, finishCellPosition, pathConnections);
                    if (cell) {
                        queue.push(cell);
                    }
                }

                if (currentCell->position.GetY() > 0) {
                    cell = CreateNextPathCell(*currentCell, 0, -1, finishCellPosition, pathConnections);
                    if (cell) {
                        queue.push(cell);
                    }
                }

                if (currentCell->position.GetY() < kDungeonLevelHeight - 1) {
                    cell = CreateNextPathCell(*currentCell, 0, 1, finishCellPosition, pathConnections);
                    if (cell) {
                        queue.push(cell);
                    }
                }

                delete currentCell;
            }

            if (queue.top()->position == finishCellPosition) {
                ZPosition previousPathCell = finishCellPosition;
                while (previousPathCell != startCellPosition) {
                    mMap[previousPathCell.GetX()][previousPathCell.GetY()] = EDungeonCell::Emptiness;
                    mMapCellWeight[previousPathCell.GetX()][previousPathCell.GetY()] = kEmptyCellWeight;
                    previousPathCell = *pathConnections[previousPathCell.GetX()][previousPathCell.GetY()].previousPathCell;
                }
            }

            while (!queue.empty()) {
                ZWeightedCell* cell = queue.top();
                queue.pop();
                delete cell;
            }

            utl::ZMatrix::Deallocate(&pathConnections, kDungeonLevelHeight);
        }

        void ShrinkSubDungeon(SubDungeon* outerSubDungeon, const SubDungeon& lowerSubDungeon, const SubDungeon& higherSubDungeon) {
            outerSubDungeon->x1 = std::min(lowerSubDungeon.x1, higherSubDungeon.x1);
            outerSubDungeon->y1 = std::min(lowerSubDungeon.y1, higherSubDungeon.y1);
            outerSubDungeon->x2 = std::max(lowerSubDungeon.x2, higherSubDungeon.x2);
            outerSubDungeon->y2 = std::max(lowerSubDungeon.y2, higherSubDungeon.y2);
        }

        void ZDungeonLevelGenerator::GenerateBSPTree(BSPTreeNode* rootNode, bool tryToSplitVertically) {
            SubDungeon& rootSubDungeon = rootNode->dungeon;
            int width = rootSubDungeon.GetWidth();
            int height = rootSubDungeon.GetHeight();

            if (tryToSplitVertically && width > 2 * kSubDungeonMinSize) {
                SplitSubDungeonVertically(rootNode);
            } else if (!tryToSplitVertically && height > 2 * kSubDungeonMinSize) {
                SplitSubDungeonHorizontally(rootNode);
            } else {
                CreateRoomInsideSubDungeon(&rootSubDungeon);
                mRooms.push_back(&rootSubDungeon);

                return;
            }

            GenerateBSPTree(rootNode->lowerSubDungeon, !tryToSplitVertically);
            GenerateBSPTree(rootNode->higherSubDungeon, !tryToSplitVertically);
            SubDungeon& lowerSubDungeon = rootNode->lowerSubDungeon->dungeon;
            SubDungeon& higherSubDungeon = rootNode->higherSubDungeon->dungeon;
            ShrinkSubDungeon(&rootSubDungeon, lowerSubDungeon, higherSubDungeon);
            rootSubDungeon.someValidCell = lowerSubDungeon.someValidCell;
            ConnectCells(lowerSubDungeon.someValidCell, higherSubDungeon.someValidCell);
        }

        void ZDungeonLevelGenerator::SplitSubDungeonVertically(BSPTreeNode* rootNode) {
            SubDungeon& dungeon = rootNode->dungeon;

            int maxSubDungeonsWidthDiff = rootNode->dungeon.GetWidth() - 2 * kSubDungeonMinSize;
            int lowerSubDungeonWidth = kSubDungeonMinSize + utl::ZRandomHelpers::GetRandomValue(maxSubDungeonsWidthDiff);
            int lowerSubDungeonX2 = dungeon.x1 + lowerSubDungeonWidth - 1;

            rootNode->lowerSubDungeon = new BSPTreeNode(dungeon.x1, dungeon.y1, lowerSubDungeonX2, dungeon.y2);
            rootNode->higherSubDungeon = new BSPTreeNode(lowerSubDungeonX2 + 1, dungeon.y1, dungeon.x2, dungeon.y2);
        }

        void ZDungeonLevelGenerator::SplitSubDungeonHorizontally(BSPTreeNode* rootNode) {
            SubDungeon& dungeon = rootNode->dungeon;

            int maxSubDungeonsHeightDiff = dungeon.GetHeight() - 2 * kSubDungeonMinSize;
            int lowerSubDungeonHeight = kSubDungeonMinSize + utl::ZRandomHelpers::GetRandomValue(maxSubDungeonsHeightDiff);
            int lowerSubDungeonY2 = dungeon.y1 + lowerSubDungeonHeight - 1;

            rootNode->lowerSubDungeon = new BSPTreeNode(dungeon.x1, dungeon.y1, dungeon.x2, lowerSubDungeonY2);
            rootNode->higherSubDungeon = new BSPTreeNode(dungeon.x1, lowerSubDungeonY2 + 1, dungeon.x2, dungeon.y2);
        }

        void ZDungeonLevelGenerator::CreateRoomInsideSubDungeon(SubDungeon* subDungeon) {
            int roomWidth = utl::ZRandomHelpers::GetRandomValue(kRoomMinSize, std::min(kRoomMaxSize, subDungeon->GetWidth() - 1 - 2));
            int roomX1 = subDungeon->x1 + utl::ZRandomHelpers::GetRandomValue(1, subDungeon->GetWidth() - 2 - roomWidth);
            int roomX2 = roomX1 + roomWidth - 1;

            int roomHeight = utl::ZRandomHelpers::GetRandomValue(kRoomMinSize, std::min(kRoomMaxSize, subDungeon->GetHeight() - 1 - 2));
            int roomY1 = subDungeon->y1 + utl::ZRandomHelpers::GetRandomValue(1, subDungeon->GetHeight() - 2 - roomHeight);
            int roomY2 = roomY1 + roomHeight - 1;

            int someValidCellX = utl::ZRandomHelpers::GetRandomValue(roomX1, roomX2);
            int someValidCellY = utl::ZRandomHelpers::GetRandomValue(roomY1, roomY2);
            ZPosition someValidCell = ZPosition(someValidCellX, someValidCellY);

            *subDungeon = SubDungeon(roomX1, roomY1, roomX2, roomY2, someValidCell);

            for (int x = roomX1; x <= roomX2; ++x) {
                for (int y = roomY1; y <= roomY2; ++y) {
                    mMap[x][y] = EDungeonCell::Emptiness;
                    mMapCellWeight[x][y] = kEmptyCellWeight;
                }
            }

            // tunnels should not start at room's corner or touch room's edge, so cells adjacent to corners have infinite weights:
            // #+##+#
            // +    +
            // #    #
            // +    +
            // #+##+#
            const ZWeight& forbiddenCellWeight = ZWeight::kInfinity;
            mMapCellWeight[roomX1 - 1][roomY1] = forbiddenCellWeight;
            mMapCellWeight[roomX2 + 1][roomY1] = forbiddenCellWeight;
            mMapCellWeight[roomX1 - 1][roomY2] = forbiddenCellWeight;
            mMapCellWeight[roomX2 + 1][roomY2] = forbiddenCellWeight;

            mMapCellWeight[roomX1][roomY1 - 1] = forbiddenCellWeight;
            mMapCellWeight[roomX2][roomY1 - 1] = forbiddenCellWeight;
            mMapCellWeight[roomX1][roomY2 + 1] = forbiddenCellWeight;
            mMapCellWeight[roomX2][roomY2 + 1] = forbiddenCellWeight;
        }

        void ZDungeonLevelGenerator::DigRandomTunnels() {
            int tryCount = mRooms.size() * kRoomCountFractionToDigRandomTunnelsFrom;
            for (int i = 0; i < tryCount; ++i) {
                int fromRoomIndex = utl::ZRandomHelpers::GetRandomValue(mRooms.size() - 1);
                int toRoomIndex = utl::ZRandomHelpers::GetRandomValue(mRooms.size() - 1);
                ConnectCells(mRooms[fromRoomIndex]->someValidCell, mRooms[toRoomIndex]->someValidCell);
            }
        }

        ZDungeonLevel* ZDungeonLevelGenerator::GenerateLevel(const ZDungeonLevel::StaircaseList& upStaircases) {
            utl::ZMatrix::Allocate(&mMap, kDungeonLevelWidth, kDungeonLevelHeight, EDungeonCell::SolidRock);
            utl::ZMatrix::Allocate(&mMapCellWeight, kDungeonLevelWidth, kDungeonLevelHeight, kSolidRockCellWeight);

            BSPTreeNode subDungeonsTreeRoot(0, 0, kDungeonLevelWidth, kDungeonLevelHeight);
            utl::ZRandomHelpers::Initialize();
            GenerateBSPTree(&subDungeonsTreeRoot);

            DigRandomTunnels();

            int startLeafIndex = utl::ZRandomHelpers::GetRandomValue(mRooms.size() - 1);
            const SubDungeon* startSubDungeon = mRooms[startLeafIndex];
            mMap[startSubDungeon->x1 + 1][startSubDungeon->y1 + 1] = EDungeonCell::UpStaircase;

            ZDungeonLevel* level = new ZDungeonLevel(kDungeonLevelWidth, kDungeonLevelHeight, &mMap);

            utl::ZMatrix::Deallocate(&mMapCellWeight, kDungeonLevelHeight);

            return level;
        }

    }
}
