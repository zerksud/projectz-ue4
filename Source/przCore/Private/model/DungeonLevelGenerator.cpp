#include "przCorePrivatePCH.h"
#include "model/DungeonLevelGenerator.h"

#include <algorithm>
#include <limits>
#include <queue>

#include "utils/MatrixHelpers.h"
#include "utils/RandomHelpers.h"

namespace prz {
    namespace mdl {

        struct WeightedCell {
            static const int kInfiniteWeight;

            static int SumWeights(int leftWeight, int rightWeight) {
                int sum = leftWeight + rightWeight;
                if (sum >= kInfiniteWeight) {
                    return kInfiniteWeight;
                }

                return sum;
            }

            ZPosition position;

            int pathToCellWeight;
            int pathFromCellEstimatedWeight;

            WeightedCell()
                : position(ZPosition(0, 0)), pathToCellWeight(kInfiniteWeight), pathFromCellEstimatedWeight(kInfiniteWeight) {
            }

            WeightedCell(const ZPosition& pPosition, int pPathToCellWeight, int pPathFromCellEstimatedWeight)
                : position(pPosition), pathToCellWeight(pPathToCellWeight), pathFromCellEstimatedWeight(pPathFromCellEstimatedWeight) {
            }

            int GetTotalPathWeight() const {
                return SumWeights(pathToCellWeight, pathFromCellEstimatedWeight);
            }
        };

        const int WeightedCell::kInfiniteWeight = std::numeric_limits<int>::max() / 2;

        const int ZDungeonLevelGenerator::kDungeonLevelWidth = 16;
        const int ZDungeonLevelGenerator::kDungeonLevelHeight = 16;

        const int ZDungeonLevelGenerator::kSubDungeonMinSize = 8;
        const int ZDungeonLevelGenerator::kRoomMinSize = 5;

        const int ZDungeonLevelGenerator::kSolidRockCellWeight = 100;
        const int ZDungeonLevelGenerator::kEmptyCellWeight = 0;
        const int ZDungeonLevelGenerator::kForbiddenCellWeight = WeightedCell::kInfiniteWeight;

        struct SubDungeon {
            int x1;
            int y1;
            int x2;
            int y2;

            ZPosition someValidCell;

            SubDungeon(int pX1, int pY1, int pX2, int pY2)
                : x1(pX1), y1(pY1), x2(pX2), y2(pY2), someValidCell(-1, -1) {
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

            BSPTreeNode(SubDungeon pDungeon)
                : dungeon(pDungeon), lowerSubDungeon(nullptr), higherSubDungeon(nullptr) {
            }

            ~BSPTreeNode() {
                delete lowerSubDungeon;
                delete higherSubDungeon;
            }
        };

        class WeightedCellAscendingOrder {
        public:
            bool operator() (const WeightedCell& left, const WeightedCell& right) {
                return left.GetTotalPathWeight() > right.GetTotalPathWeight();
            }
        };

        int CalcCellsDistance(const ZPosition& left, const ZPosition& right) {
            ZPositionDiff diff = right - left;
            int distance = std::abs(diff.GetdX()) + std::abs(diff.GetdY());

            return distance;
        }

        struct PathCellConnection {
            int pathToCellWeight;
            ZPosition previousPathCell;

            PathCellConnection() = default;

            PathCellConnection(int pPathToCellWeight, const ZPosition& pPreviousPathCell)
                : pathToCellWeight(pPathToCellWeight), previousPathCell(pPreviousPathCell) {
            }
        };

        // returns true if path from cell to neighbor is shorter than previous path to that cell
        bool CreateNeighborCell(const WeightedCell& cell, int dx, int dy, const ZPosition& finishCellPosition, int** mapCellWeight, PathCellConnection** pathConnections, WeightedCell* createdCell) {
            ZPosition cellPosition = cell.position + ZPositionDiff(dx, dy);
            int pathToCellWeight = WeightedCell::SumWeights(cell.pathToCellWeight, mapCellWeight[cellPosition.GetX()][cellPosition.GetY()]);

            PathCellConnection* connection = &pathConnections[cellPosition.GetX()][cellPosition.GetY()];
            if (connection->pathToCellWeight > pathToCellWeight) {
                int pathFromCellEstimatedWeight = CalcCellsDistance(cellPosition, finishCellPosition) * 99;
                *createdCell = WeightedCell(cellPosition, pathToCellWeight, pathFromCellEstimatedWeight);
                *connection = PathCellConnection(pathToCellWeight, cell.position);
                return true;
            }

            return false;
        }

        void ZDungeonLevelGenerator::ConnectDirectSubDungeons(const SubDungeon& lowerSubDungeon, const SubDungeon& higherSubDungeon) {
            PathCellConnection defaultPathCellConnection = PathCellConnection(WeightedCell::kInfiniteWeight, ZPosition(0, 0));
            PathCellConnection** pathConnections;
            utl::ZMatrix::Allocate(&pathConnections, kDungeonLevelWidth, kDungeonLevelHeight, defaultPathCellConnection);

            std::priority_queue<WeightedCell, std::vector<WeightedCell>, WeightedCellAscendingOrder> queue;
            ZPosition startCellPosition = lowerSubDungeon.someValidCell;
            ZPosition finishCellPosition = higherSubDungeon.someValidCell;

            queue.emplace(startCellPosition, 0, CalcCellsDistance(startCellPosition, finishCellPosition));

            while (!queue.empty() && queue.top().position != finishCellPosition) {
                WeightedCell currentCell = queue.top();
                queue.pop();

                WeightedCell cell = WeightedCell(currentCell);
                if (currentCell.position.GetX() > 0
                    && CreateNeighborCell(currentCell, -1, 0, finishCellPosition, mMapCellWeight, pathConnections, &cell)) {
                    queue.push(cell);
                }

                if (currentCell.position.GetX() < kDungeonLevelWidth - 1
                    && CreateNeighborCell(currentCell, 1, 0, finishCellPosition, mMapCellWeight, pathConnections, &cell)) {
                    queue.push(cell);
                }

                if (currentCell.position.GetY() > 0
                    && CreateNeighborCell(currentCell, 0, -1, finishCellPosition, mMapCellWeight, pathConnections, &cell)) {
                    queue.push(cell);
                }

                if (currentCell.position.GetY() < kDungeonLevelHeight - 1
                    && CreateNeighborCell(currentCell, 0, 1, finishCellPosition, mMapCellWeight, pathConnections, &cell)) {
                    queue.push(cell);
                }
            }

            if (queue.top().position == finishCellPosition) {
                ZPosition& previousPathCell = pathConnections[finishCellPosition.GetX()][finishCellPosition.GetY()].previousPathCell;
                while (previousPathCell != startCellPosition) {
                    mMap[previousPathCell.GetX()][previousPathCell.GetY()] = EDungeonCell::Emptiness;
                    mMapCellWeight[previousPathCell.GetX()][previousPathCell.GetY()] = kEmptyCellWeight;
                    previousPathCell = pathConnections[previousPathCell.GetX()][previousPathCell.GetY()].previousPathCell;
                }
            }

            utl::ZMatrix::Deallocate(&pathConnections, kDungeonLevelHeight);
        }

        void ShrinkSubDungeon(SubDungeon* outerSubDungeon, const SubDungeon& lowerSubDungeon, const SubDungeon& higherSubDungeon) {
            outerSubDungeon->x1 = std::min(lowerSubDungeon.x1, higherSubDungeon.x1);
            outerSubDungeon->y1 = std::min(lowerSubDungeon.y1, higherSubDungeon.y1);
            outerSubDungeon->x2 = std::max(lowerSubDungeon.x2, higherSubDungeon.x2);
            outerSubDungeon->y2 = std::max(lowerSubDungeon.y2, higherSubDungeon.y2);
        }

        void ZDungeonLevelGenerator::GenerateBSPTree(BSPTreeNode* rootNode) {
            int width = rootNode->dungeon.GetWidth();
            int height = rootNode->dungeon.GetHeight();

            if (width > 2 * kSubDungeonMinSize && height > 2 * kSubDungeonMinSize) {
                if (utl::ZRandomHelpers::FlipCoin()) {
                    SplitSubDungeonVertically(rootNode);
                } else {
                    SplitSubDungeonHorizontally(rootNode);
                }
            } else if (width > 2 * kSubDungeonMinSize) {
                SplitSubDungeonVertically(rootNode);
            } else if (height > 2 * kSubDungeonMinSize) {
                SplitSubDungeonHorizontally(rootNode);
            } else {
                mRooms.push_back(&rootNode->dungeon);
                CreateRoomInsideSubDungeon(&rootNode->dungeon);
                rootNode->dungeon.someValidCell = ZPosition(rootNode->dungeon.x1, rootNode->dungeon.y1);
                return;
            }

            GenerateBSPTree(rootNode->lowerSubDungeon);
            GenerateBSPTree(rootNode->higherSubDungeon);
            ShrinkSubDungeon(&rootNode->dungeon, rootNode->lowerSubDungeon->dungeon, rootNode->higherSubDungeon->dungeon);
            rootNode->dungeon.someValidCell = rootNode->lowerSubDungeon->dungeon.someValidCell;
            ConnectDirectSubDungeons(rootNode->lowerSubDungeon->dungeon, rootNode->higherSubDungeon->dungeon);
        }

        void ZDungeonLevelGenerator::SplitSubDungeonVertically(BSPTreeNode* rootNode) {
            SubDungeon& dungeon = rootNode->dungeon;

            int maxSubDungeonsWidthDiff = rootNode->dungeon.GetWidth() - 2 * kSubDungeonMinSize;
            int lowerSubDungeonWidth = kSubDungeonMinSize + utl::ZRandomHelpers::GetRandomValue(maxSubDungeonsWidthDiff);
            int lowerSubDungeonX2 = dungeon.x1 + lowerSubDungeonWidth - 1;

            rootNode->lowerSubDungeon = new BSPTreeNode(SubDungeon(dungeon.x1, dungeon.y1, lowerSubDungeonX2, dungeon.y2));
            rootNode->higherSubDungeon = new BSPTreeNode(SubDungeon(lowerSubDungeonX2 + 1, dungeon.y1, dungeon.x2, dungeon.y2));
        }

        void ZDungeonLevelGenerator::SplitSubDungeonHorizontally(BSPTreeNode* rootNode) {
            SubDungeon& dungeon = rootNode->dungeon;

            int maxSubDungeonsHeightDiff = dungeon.GetHeight() - 2 * kSubDungeonMinSize;
            int lowerSubDungeonHeight = kSubDungeonMinSize + utl::ZRandomHelpers::GetRandomValue(maxSubDungeonsHeightDiff);
            int lowerSubDungeonY2 = dungeon.y1 + lowerSubDungeonHeight - 1;

            rootNode->lowerSubDungeon = new BSPTreeNode(SubDungeon(dungeon.x1, dungeon.y1, dungeon.x2, lowerSubDungeonY2));
            rootNode->higherSubDungeon = new BSPTreeNode(SubDungeon(dungeon.x1, lowerSubDungeonY2 + 1, dungeon.x2, dungeon.y2));
        }

        void ZDungeonLevelGenerator::CreateRoomInsideSubDungeon(SubDungeon* subDungeon) {
            int roomWidth = utl::ZRandomHelpers::GetRandomValue(kRoomMinSize, subDungeon->GetWidth() - 1 - 2);
            int roomX1 = subDungeon->x1 + utl::ZRandomHelpers::GetRandomValue(1, subDungeon->GetWidth() - 2 - roomWidth);
            int roomX2 = roomX1 + roomWidth - 1;

            int roomHeight = utl::ZRandomHelpers::GetRandomValue(kRoomMinSize, subDungeon->GetHeight() - 1 - 2);
            int roomY1 = subDungeon->y1 + utl::ZRandomHelpers::GetRandomValue(1, subDungeon->GetHeight() - 2 - roomHeight);
            int roomY2 = roomY1 + roomHeight - 1;

            *subDungeon = SubDungeon(roomX1, roomY1, roomX2, roomY2);

            for (int x = roomX1; x <= roomX2; ++x) {
                for (int y = roomY1; y < roomY2; ++y) {
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
            mMapCellWeight[roomX1 - 1][roomY1] = kForbiddenCellWeight;
            mMapCellWeight[roomX2 + 1][roomY1] = kForbiddenCellWeight;
            mMapCellWeight[roomX1 - 1][roomY2] = kForbiddenCellWeight;
            mMapCellWeight[roomX2 + 1][roomY2] = kForbiddenCellWeight;

            mMapCellWeight[roomX1][roomY1 - 1] = kForbiddenCellWeight;
            mMapCellWeight[roomX2][roomY1 - 1] = kForbiddenCellWeight;
            mMapCellWeight[roomX1][roomY2 + 1] = kForbiddenCellWeight;
            mMapCellWeight[roomX2][roomY2 + 1] = kForbiddenCellWeight;
        }

        ZDungeonLevel* ZDungeonLevelGenerator::GenerateLevel(const ZDungeonLevel::StaircaseList& upStaircases) {
            utl::ZMatrix::Allocate(&mMap, kDungeonLevelWidth, kDungeonLevelHeight, EDungeonCell::SolidRock);
            utl::ZMatrix::Allocate(&mMapCellWeight, kDungeonLevelWidth, kDungeonLevelHeight, kSolidRockCellWeight);

            BSPTreeNode subDungeonsTreeRoot(SubDungeon(0, 0, kDungeonLevelWidth, kDungeonLevelHeight));
            utl::ZRandomHelpers::Initialize();
            GenerateBSPTree(&subDungeonsTreeRoot);

            int startLeafIndex = utl::ZRandomHelpers::GetRandomValue(mRooms.size() - 1);
            const SubDungeon* startSubDungeon = mRooms[startLeafIndex];

            mMap[startSubDungeon->x1 + 1][startSubDungeon->y1 + 1] = EDungeonCell::UpStaircase;

            ZDungeonLevel* level = new ZDungeonLevel(kDungeonLevelWidth, kDungeonLevelHeight, &mMap);

            utl::ZMatrix::Deallocate(&mMapCellWeight, kDungeonLevelHeight);

            return level;
        }

    }
}
