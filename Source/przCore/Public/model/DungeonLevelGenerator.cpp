#include "przCorePCH.h"
#include "model/DungeonLevelGenerator.h"

#include <algorithm>
#include <limits>
#include <numeric>
#include <random>
#include <utility>

#include "model/PathFinder.h"

#include "utils/MatrixHelpers.h"
#include "utils/RandomHelpers.h"

namespace prz {
    namespace mdl {

        const int ZDungeonLevelGenerator::kDungeonLevelWidth = 64;
        const int ZDungeonLevelGenerator::kDungeonLevelHeight = 64;

        const int ZDungeonLevelGenerator::kSubDungeonMinSize = 8;
        const int ZDungeonLevelGenerator::kRoomMaxSize = 10;
        const int ZDungeonLevelGenerator::kRoomMinSize = 5;

        const float ZDungeonLevelGenerator::kRoomCountFractionToDigRandomTunnelsFrom = 0.25f;

        const int ZDungeonLevelGenerator::kStaircaseCount = 3;

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

        void ZDungeonLevelGenerator::DiggCellIfSolidAndNotBlocked(const ZPosition& position) {
            DiggCellIfSolidAndNotBlocked(position.GetX(), position.GetY());
        }

        void ZDungeonLevelGenerator::DiggCellIfSolidAndNotBlocked(int x, int y) {
            if (mMap[x][y] == EDungeonCell::SolidRock && mMapCellWeight[x][y] != ZWeight::kInfinity) {
                mMap[x][y] = EDungeonCell::Emptiness;
                mMapCellWeight[x][y] = ZPathFinder::kEmptyCellWeight;
            }
        }

        void ZDungeonLevelGenerator::ConnectCells(const ZPosition& someCell, const ZPosition& anotherCell) {
            ZPathFinder::PathCells path = ZPathFinder::FindPathBetweenCells(mMapCellWeight, kDungeonLevelWidth, kDungeonLevelHeight, someCell, anotherCell, true);
            for (auto cell : path) {
                DiggCellIfSolidAndNotBlocked(cell);
            }
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

        void ZDungeonLevelGenerator::BlockCell(const ZPosition& position) {
            BlockCell(position.GetX(), position.GetY());
        }

        void ZDungeonLevelGenerator::BlockCell(int x, int y) {
            static const ZWeight forbiddenCellWeight = ZWeight::kInfinity;
            mMapCellWeight[x][y] = forbiddenCellWeight;
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
                    DiggCellIfSolidAndNotBlocked(x, y);
                }
            }

            // tunnels should not start at room's corner or touch room's edge, so cells adjacent to corners should be blocked:
            // #+##+#
            // +    +
            // #    #
            // +    +
            // #+##+#
            BlockCell(roomX1 - 1, roomY1);
            BlockCell(roomX2 + 1, roomY1);
            BlockCell(roomX1 - 1, roomY2);
            BlockCell(roomX2 + 1, roomY2);

            BlockCell(roomX1, roomY1 - 1);
            BlockCell(roomX2, roomY1 - 1);
            BlockCell(roomX1, roomY2 + 1);
            BlockCell(roomX2, roomY2 + 1);
        }

        void ZDungeonLevelGenerator::DigRandomTunnels() {
            int tryCount = mRooms.size() * kRoomCountFractionToDigRandomTunnelsFrom;
            for (int i = 0; i < tryCount; ++i) {
                int fromRoomIndex = utl::ZRandomHelpers::GetRandomValue(mRooms.size() - 1);
                int toRoomIndex = utl::ZRandomHelpers::GetRandomValue(mRooms.size() - 1);
                ConnectCells(mRooms[fromRoomIndex]->someValidCell, mRooms[toRoomIndex]->someValidCell);
            }
        }

        int ZDungeonLevelGenerator::CountCellSolidNeighbours(const ZPosition& cell) const {
            int count = 0;
            const int x = cell.GetX();
            const int y = cell.GetY();

            if (x == 0 || x > 0 && ZPathFinder::CellMustBeDigged(mMapCellWeight, x - 1, y)) {
                ++count;
            }

            if (x == kDungeonLevelWidth || x < kDungeonLevelWidth && ZPathFinder::CellMustBeDigged(mMapCellWeight, x + 1, y)) {
                ++count;
            }

            if (y == 0 || y > 0 && ZPathFinder::CellMustBeDigged(mMapCellWeight, x, y - 1)) {
                ++count;
            }

            if (y == kDungeonLevelHeight || y < kDungeonLevelHeight && ZPathFinder::CellMustBeDigged(mMapCellWeight, x, y + 1)) {
                ++count;
            }

            return count;
        }

        struct Room {
            int index;
            ZPosition someValidCell;
            int distanceToClosestStaircase;
        };

        void ZDungeonLevelGenerator::AddRandomDownStaircases() {
            const int staircasesToBeGeneratedCount = std::min(kStaircaseCount, (int)mRooms.size());
            int staircasesGeneratedCount = 0;

            std::vector<Room> rooms;
            for (int i = 0; i < mRooms.size(); ++i) {
                Room room;
                room.index = i;
                room.someValidCell = mRooms[i]->someValidCell;
                room.distanceToClosestStaircase = std::numeric_limits<int>::max();
                for (auto& upStaircase : mUpStaircases) {
                    int pathSize = ZPathFinder::FindPathBetweenCells(mMapCellWeight, kDungeonLevelWidth, kDungeonLevelHeight, room.someValidCell, upStaircase.position).size();
                    if (pathSize > 0 && pathSize < room.distanceToClosestStaircase) {
                        room.distanceToClosestStaircase = pathSize;
                    }
                }

                rooms.push_back(room);
            }

            while (rooms.size() > 0 && staircasesGeneratedCount < staircasesToBeGeneratedCount) {
                std::sort(rooms.begin(), rooms.end(), [](Room left, Room right) {
                    return left.distanceToClosestStaircase < right.distanceToClosestStaircase;
                });

                const Room& currentRoom = rooms[rooms.size() - 1];
                const SubDungeon* subDungeon = mRooms[currentRoom.index];

                std::vector<ZPosition> staircasePositionVariants;
                if (subDungeon->y1 > kRoomMinSize + 2) {
                    staircasePositionVariants.emplace_back(utl::ZRandomHelpers::GetRandomValue(subDungeon->x1, subDungeon->x2), subDungeon->y1 - 1);
                }
                if (subDungeon->y2 < kDungeonLevelHeight - 1 - kRoomMinSize - 2) {
                    staircasePositionVariants.emplace_back(utl::ZRandomHelpers::GetRandomValue(subDungeon->x1, subDungeon->x2), subDungeon->y2 + 1);
                }
                if (subDungeon->x1 > kRoomMinSize + 2) {
                    staircasePositionVariants.emplace_back(subDungeon->x1 - 1, utl::ZRandomHelpers::GetRandomValue(subDungeon->y1, subDungeon->y2));
                }
                if (subDungeon->x2 < kDungeonLevelWidth - 1 - kRoomMinSize - 2) {
                    staircasePositionVariants.emplace_back(subDungeon->x2 + 1, utl::ZRandomHelpers::GetRandomValue(subDungeon->y1, subDungeon->y2));
                }
                std::shuffle(staircasePositionVariants.begin(), staircasePositionVariants.end(), std::default_random_engine());

                for (int j = 0; j < staircasePositionVariants.size(); ++j) {
                    const ZPosition& cellPosition = staircasePositionVariants[j];

                    if (ZPathFinder::CellMustBeDigged(mMapCellWeight, cellPosition) && CountCellSolidNeighbours(cellPosition) == 3) {
                        mMap[cellPosition.GetX()][cellPosition.GetY()] = EDungeonCell::DownStaircase;
                        ++staircasesGeneratedCount;

                        rooms.pop_back();
                        for (auto& room : rooms) {
                            int pathSize = ZPathFinder::FindPathBetweenCells(mMapCellWeight, kDungeonLevelWidth, kDungeonLevelHeight, currentRoom.someValidCell, room.someValidCell).size();
                            if (pathSize > 0 && pathSize < room.distanceToClosestStaircase) {
                                room.distanceToClosestStaircase = pathSize;
                            }
                        }

                        break;
                    }
                }
            }
        }

        void ZDungeonLevelGenerator::PlaceUpStaircases(const ZDungeonLevel* previousLevel) {
            if (previousLevel) {
                for (auto& previousLevelStaircasePosition : previousLevel->GetDownStaircases()) {
                    ZDirection downStaircaseDirection = previousLevel->GetStaircaseDirection(previousLevelStaircasePosition);
                    ZPosition upStaircasePosition = previousLevelStaircasePosition + downStaircaseDirection.PredictMove();
                    ZDirection upStaircaseDirection = downStaircaseDirection.TurnCopy(ETurnDirection::Back);

                    mUpStaircases.emplace_back(upStaircasePosition, upStaircaseDirection);
                }
            } else {
                int startPositionX = utl::ZRandomHelpers::GetRandomValue(5, kDungeonLevelWidth - 5);
                int startPositionY = utl::ZRandomHelpers::GetRandomValue(5, kDungeonLevelHeight - 5);
                mMap[startPositionX][startPositionX] = EDungeonCell::UpStaircase;
                mUpStaircases.emplace_back(ZPosition(startPositionX, startPositionX), ZDirection());
            }

            for (auto& staircase : mUpStaircases) {
                int x = staircase.position.GetX();
                int y = staircase.position.GetY();

                ZDirection blockedDirection = staircase.direction;

                ETurnDirection::Type blockedCellsDirections[] = {
                    ETurnDirection::Left,
                    ETurnDirection::ForwardLeft,
                    ETurnDirection::Forward,
                    ETurnDirection::ForwardRight,
                    ETurnDirection::Right
                };
                size_t blockedCellsDirectionsSize = sizeof(blockedCellsDirections) / sizeof(*blockedCellsDirections);

                for (int i = 0; i < blockedCellsDirectionsSize; ++i) {
                    ZPosition pos = staircase.position + blockedDirection.TurnCopy(blockedCellsDirections[i]).PredictMove();
                    BlockCell(pos.GetX(), pos.GetY());
                }

                DiggCellIfSolidAndNotBlocked(staircase.position);
                mMap[x][y] = EDungeonCell::UpStaircase;
            }
        }

        void ZDungeonLevelGenerator::ConnectUpStaircasesWithSomeValidCell(const ZPosition& someValidCell) {
             for (auto& staircase : mUpStaircases) {
                int x = staircase.position.GetX();
                int y = staircase.position.GetY();

                if (mMapCellWeight[x][y] == ZWeight::kInfinity) {
                    mMapCellWeight[x][y] = ZPathFinder::kEmptyCellWeight;
                }

                ZPosition staircaseConnectedCellPosition = staircase.position + staircase.direction.TurnCopy(ETurnDirection::Back).PredictMove();
                if (mMapCellWeight[staircaseConnectedCellPosition.GetX()][staircaseConnectedCellPosition.GetY()] == ZWeight::kInfinity) {
                    mMapCellWeight[staircaseConnectedCellPosition.GetX()][staircaseConnectedCellPosition.GetY()] = ZPathFinder::kEmptyCellWeight;
                    BlockCell(staircaseConnectedCellPosition + staircase.direction.TurnCopy(ETurnDirection::Left).PredictMove());
                    BlockCell(staircaseConnectedCellPosition + staircase.direction.TurnCopy(ETurnDirection::Right).PredictMove());
                }

                ConnectCells(staircase.position, someValidCell);
            }
        }

        ZDungeonLevel* ZDungeonLevelGenerator::GenerateLevel(const ZDungeonLevel* previousLevel) {
            utl::ZMatrix::Allocate(&mMap, kDungeonLevelWidth, kDungeonLevelHeight, EDungeonCell::SolidRock);
            utl::ZMatrix::Allocate(&mMapCellWeight, kDungeonLevelWidth, kDungeonLevelHeight, ZPathFinder::kSolidRockCellWeight);

            PlaceUpStaircases(previousLevel);

            BSPTreeNode subDungeonsTreeRoot(0, 0, kDungeonLevelWidth, kDungeonLevelHeight);
            utl::ZRandomHelpers::Initialize();
            GenerateBSPTree(&subDungeonsTreeRoot);

            DigRandomTunnels();
            ConnectUpStaircasesWithSomeValidCell(subDungeonsTreeRoot.dungeon.someValidCell);
            AddRandomDownStaircases();

            ZDungeonLevel::ZRoomList roomList;
            for (auto room : mRooms) {
                roomList.emplace_back(room->x1, room->y1, room->x2, room->y2);
            }

            ZDungeonLevel* level = new ZDungeonLevel(kDungeonLevelWidth, kDungeonLevelHeight, &mMap, roomList);

            utl::ZMatrix::Deallocate(&mMapCellWeight, kDungeonLevelHeight);
            mRooms.clear();
            mUpStaircases.clear();

            return level;
        }

    }
}
