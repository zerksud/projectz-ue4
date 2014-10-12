#include "przCorePCH.h"
#include "Model/DungeonLevelGenerator.h"

#include <algorithm>
#include <limits>
#include <numeric>
#include <random>
#include <utility>

#include "Utils/MatrixHelpers.h"
#include "Utils/RandomHelpers.h"
#include "Utils/LOG_ANSI.h"

namespace prz {
namespace mdl {

const int ZDungeonLevelGenerator::kDungeonLevelWidth = 64;
const int ZDungeonLevelGenerator::kDungeonLevelHeight = 64;

const int ZDungeonLevelGenerator::kSubDungeonMinSize = 8;
const int ZDungeonLevelGenerator::kRoomMaxSize = 10;
const int ZDungeonLevelGenerator::kRoomMinSize = 5;

const float ZDungeonLevelGenerator::kRoomCountFractionToDigRandomTunnelsFrom = 0.25f;

const int ZDungeonLevelGenerator::kStaircaseCount = 3;

struct BSPTreeNode {
    SubDungeon dungeon;

    BSPTreeNode* lowerTreeNode;
    BSPTreeNode* higherTreeNode;

    BSPTreeNode(int x1, int y1, int x2, int y2)
        : dungeon(x1, y1, x2, y2), lowerTreeNode(nullptr), higherTreeNode(nullptr) {
    }

    ~BSPTreeNode() {
        delete lowerTreeNode;
        delete higherTreeNode;
    }
};

void ZDungeonLevelGenerator::DigCellIfSolidAndNotBlocked(EDungeonCell::Type** map, path::ZWeightedMap* weightedMap, const ZPosition& position) {
    DigCellIfSolidAndNotBlocked(map, weightedMap, position.GetX(), position.GetY());
}

void ZDungeonLevelGenerator::DigCellIfSolidAndNotBlocked(EDungeonCell::Type** map, path::ZWeightedMap* weightedMap, int x, int y) {
    if (map[x][y] == EDungeonCell::SolidRock && !path::ZPathFinder::CellIsBlocked(*weightedMap, x, y)) {
        map[x][y] = EDungeonCell::Emptiness;
        weightedMap->SetCellWeight(x, y, path::ZPathFinder::kEmptyCellWeight);
    }
}

void ZDungeonLevelGenerator::ConnectCells(const ZPosition& someCell, const ZPosition& anotherCell) {
    path::ZPathFinder::PathCells path = path::ZPathFinder::FindPathBetweenCells(*mWeightedMap, someCell, anotherCell, true);
    for (auto cell : path) {
        DigCellIfSolidAndNotBlocked(mMap, mWeightedMap, cell);
    }
}

void ShrinkSubDungeon(SubDungeon* outerSubDungeon, const SubDungeon& lowerTreeNode, const SubDungeon& higherTreeNode) {
    outerSubDungeon->x1 = std::min(lowerTreeNode.x1, higherTreeNode.x1);
    outerSubDungeon->y1 = std::min(lowerTreeNode.y1, higherTreeNode.y1);
    outerSubDungeon->x2 = std::max(lowerTreeNode.x2, higherTreeNode.x2);
    outerSubDungeon->y2 = std::max(lowerTreeNode.y2, higherTreeNode.y2);
}

bool ZDungeonLevelGenerator::TryToGenerateBSPTree(BSPTreeNode* rootNode, bool tryToSplitVertically) {
    SubDungeon& rootSubDungeon = rootNode->dungeon;
    int width = rootSubDungeon.GetWidth();
    int height = rootSubDungeon.GetHeight();

    if (tryToSplitVertically && width > 2 * kSubDungeonMinSize) {
        SplitSubDungeonVertically(rootNode);
    } else if (!tryToSplitVertically && height > 2 * kSubDungeonMinSize) {
        SplitSubDungeonHorizontally(rootNode);
    } else {
        return TryToCreateRoomInsideSubDungeon(&rootSubDungeon);
    }

    bool lowerTreeNodeGenerated = TryToGenerateBSPTree(rootNode->lowerTreeNode, !tryToSplitVertically);
    bool higherTreeNodeGenerated = TryToGenerateBSPTree(rootNode->higherTreeNode, !tryToSplitVertically);

    if (lowerTreeNodeGenerated && higherTreeNodeGenerated) {
        const SubDungeon& lowerTreeNode = rootNode->lowerTreeNode->dungeon;
        const SubDungeon& higherTreeNode = rootNode->higherTreeNode->dungeon;
        ShrinkSubDungeon(&rootSubDungeon, lowerTreeNode, higherTreeNode);
        rootSubDungeon.someValidCell = lowerTreeNode.someValidCell;
        ConnectCells(lowerTreeNode.someValidCell, higherTreeNode.someValidCell);
    } else if (lowerTreeNodeGenerated) {
        rootSubDungeon = rootNode->lowerTreeNode->dungeon;
    } else if (higherTreeNodeGenerated) {
        rootSubDungeon = rootNode->higherTreeNode->dungeon;
    } else {
        return false;
    }

    return true;
}

void ZDungeonLevelGenerator::SplitSubDungeonVertically(BSPTreeNode* rootNode) {
    SubDungeon& dungeon = rootNode->dungeon;

    int maxSubDungeonsWidthDiff = rootNode->dungeon.GetWidth() - 2 * kSubDungeonMinSize;
    int lowerTreeNodeWidth = kSubDungeonMinSize + utl::ZRandomHelpers::GetRandomValue(maxSubDungeonsWidthDiff);
    int lowerTreeNodeX2 = dungeon.x1 + lowerTreeNodeWidth - 1;

    rootNode->lowerTreeNode = new BSPTreeNode(dungeon.x1, dungeon.y1, lowerTreeNodeX2, dungeon.y2);
    rootNode->higherTreeNode = new BSPTreeNode(lowerTreeNodeX2 + 1, dungeon.y1, dungeon.x2, dungeon.y2);
}

void ZDungeonLevelGenerator::SplitSubDungeonHorizontally(BSPTreeNode* rootNode) {
    SubDungeon& dungeon = rootNode->dungeon;

    int maxSubDungeonsHeightDiff = dungeon.GetHeight() - 2 * kSubDungeonMinSize;
    int lowerTreeNodeHeight = kSubDungeonMinSize + utl::ZRandomHelpers::GetRandomValue(maxSubDungeonsHeightDiff);
    int lowerTreeNodeY2 = dungeon.y1 + lowerTreeNodeHeight - 1;

    rootNode->lowerTreeNode = new BSPTreeNode(dungeon.x1, dungeon.y1, dungeon.x2, lowerTreeNodeY2);
    rootNode->higherTreeNode = new BSPTreeNode(dungeon.x1, lowerTreeNodeY2 + 1, dungeon.x2, dungeon.y2);
}

bool ZDungeonLevelGenerator::TryToCreateRoomInsideSubDungeon(SubDungeon* subDungeon) {
    int roomWidth = utl::ZRandomHelpers::GetRandomValue(kRoomMinSize, std::min(kRoomMaxSize, subDungeon->GetWidth() - 1 - 2));
    int roomX1 = subDungeon->x1 + utl::ZRandomHelpers::GetRandomValue(1, subDungeon->GetWidth() - 2 - roomWidth);
    int roomX2 = roomX1 + roomWidth - 1;

    int roomHeight = utl::ZRandomHelpers::GetRandomValue(kRoomMinSize, std::min(kRoomMaxSize, subDungeon->GetHeight() - 1 - 2));
    int roomY1 = subDungeon->y1 + utl::ZRandomHelpers::GetRandomValue(1, subDungeon->GetHeight() - 2 - roomHeight);
    int roomY2 = roomY1 + roomHeight - 1;

    bool roomDigged = DigRoomIfAllCellsAreSolidAndNotBlocked(mMap, mWeightedMap, roomX1, roomY1, roomX2, roomY2);
    if (roomDigged) {
        int someValidCellX = utl::ZRandomHelpers::GetRandomValue(roomX1, roomX2);
        int someValidCellY = utl::ZRandomHelpers::GetRandomValue(roomY1, roomY2);
        ZPosition someValidCell = ZPosition(someValidCellX, someValidCellY);
        *subDungeon = SubDungeon(roomX1, roomY1, roomX2, roomY2, someValidCell);
        mRooms.emplace_back(roomX1, roomY1, roomX2, roomY2);
    }

    return roomDigged;
}

bool ZDungeonLevelGenerator::CellIsSolidRock(EDungeonCell::Type** map, int x, int y) {
    if (x < 0 || x >= kDungeonLevelWidth
            || y < 0 || y >= kDungeonLevelHeight) {
        return true;
    }

    return map[x][y] == EDungeonCell::SolidRock;
}

bool ZDungeonLevelGenerator::CellIsSolidRock(EDungeonCell::Type** map, const ZPosition& position) {
    return CellIsSolidRock(map, position.GetX(), position.GetY());
}

void PushBackValidWallCells(std::vector<ZPosition>* wallCells, int roomMinX, int roomMinY, int roomMaxX, int roomMaxY) {
    int x;
    int y = roomMinY - 1;
    for (x = roomMinX - 1; x <= roomMaxX + 1; ++x) {
        wallCells->emplace_back(x, y);
    }
    for (y = roomMinY; y <= roomMaxY + 1; ++y) {
        wallCells->emplace_back(x, y);
    }
    for (x = roomMaxX; x >= roomMinX - 1; --x) {
        wallCells->emplace_back(x, y);
    }
    for (y = roomMaxY; y >= roomMinY; --y) {
        wallCells->emplace_back(x, y);
    }
}

bool ZDungeonLevelGenerator::DigRoomIfAllCellsAreSolidAndNotBlocked(EDungeonCell::Type** map, path::ZWeightedMap* weightedMap, int minX, int minY, int maxX, int maxY) {
    std::vector<ZPosition> wallCells;
    PushBackValidWallCells(&wallCells, minX, minY, maxX, maxY);
    PushBackValidWallCells(&wallCells, minX - 1, minY - 1, maxX + 1, maxY + 1);

    for (auto& cellPosition : wallCells) {
        if (!CellIsSolidRock(map, cellPosition)) {
            return false;
        }
    }

    for (int x = minX; x <= maxX; ++x) {
        for (int y = minY; y <= maxY; ++y) {
            if (!CellIsSolidRock(map, x, y)
                || path::ZPathFinder::CellIsBlocked(*weightedMap, x, y)) {
                return false;
            }
        }
    }

    for (int x = minX; x <= maxX; ++x) {
        for (int y = minY; y <= maxY; ++y) {
            DigCellIfSolidAndNotBlocked(map, weightedMap, x, y);
        }
    }

    // tunnels should not start at room's corner or touch room's edge, so cells adjacent to corners should be blocked:
    // #x##x#
    // x    x
    // #    #
    // x    x
    // #x##x#
    if (minX > 0) {
        path::ZPathFinder::BlockCell(weightedMap, minX - 1, minY);
        path::ZPathFinder::BlockCell(weightedMap, minX - 1, maxY);
    }

    if (maxX < weightedMap->GetWidth() - 1) {
        path::ZPathFinder::BlockCell(weightedMap, maxX + 1, minY);
        path::ZPathFinder::BlockCell(weightedMap, maxX + 1, maxY);
    }

    if (minY > 0) {
        path::ZPathFinder::BlockCell(weightedMap, minX, minY - 1);
        path::ZPathFinder::BlockCell(weightedMap, maxX, minY - 1);
    }

    if (maxY < weightedMap->GetHeight() - 1) {
        path::ZPathFinder::BlockCell(weightedMap, minX, maxY + 1);
        path::ZPathFinder::BlockCell(weightedMap, maxX, maxY + 1);
    }

    return true;
}

void ZDungeonLevelGenerator::DigRandomTunnels() {
    int tryCount = mRooms.size() * kRoomCountFractionToDigRandomTunnelsFrom;
    for (int i = 0; i < tryCount; ++i) {
        int fromRoomIndex = utl::ZRandomHelpers::GetRandomValue(mRooms.size() - 1);
        int toRoomIndex = utl::ZRandomHelpers::GetRandomValue(mRooms.size() - 1);
        ConnectCells(mRooms[fromRoomIndex].GetRandomCell(), mRooms[toRoomIndex].GetRandomCell());
    }
}

int ZDungeonLevelGenerator::CountCellSolidNeighbours(const ZPosition& cell) const {
    static const ETurnDirection::Type adjacentCellsDirections[] = {
        ETurnDirection::BackLeft,
        ETurnDirection::Left,
        ETurnDirection::ForwardLeft,
        ETurnDirection::Forward,
        ETurnDirection::ForwardRight,
        ETurnDirection::Right,
        ETurnDirection::BackRight,
        ETurnDirection::Back
    };
    static const size_t adjacentCellsDirectionsCount = sizeof(adjacentCellsDirections) / sizeof(*adjacentCellsDirections);
    static const ZDirection baseDirection;

    int count = 0;
    for (int i = 0; i < adjacentCellsDirectionsCount; ++i) {
        ZPosition adjacentCell = cell + baseDirection.TurnCopy(adjacentCellsDirections[i]).PredictMove();

        bool adjacentCellIsOnBorder = adjacentCell.GetX() == 0
            || adjacentCell.GetX() == kDungeonLevelWidth
            || adjacentCell.GetY() == 0
            || adjacentCell.GetY() == kDungeonLevelHeight;

        if (adjacentCellIsOnBorder
                || CellIsSolidRock(mMap, adjacentCell)) {
            ++count;
        }
    }

    return count;
}

struct DistancedRoom {
    int index;
    ZPosition someValidCell;
    int distanceToClosestStaircase;
};

void ZDungeonLevelGenerator::DigRandomDownStaircases() {
    const int staircasesToBeGeneratedCount = std::min(kStaircaseCount, (int)mRooms.size());
    int staircasesGeneratedCount = 0;

    std::vector<DistancedRoom> distancedRooms;
    for (int i = 0; i < mRooms.size(); ++i) {
        DistancedRoom room;
        room.index = i;
        room.someValidCell = mRooms[i].center;
        room.distanceToClosestStaircase = std::numeric_limits<int>::max();
        for (auto& upStaircase : mUpStaircases) {
            int pathSize = path::ZPathFinder::FindPathBetweenCells(*mWeightedMap, room.someValidCell, upStaircase.position).size();
            if (pathSize > 0 && pathSize < room.distanceToClosestStaircase) {
                room.distanceToClosestStaircase = pathSize;
            }
        }

        distancedRooms.push_back(room);
    }

    path::ZWeightedMap nextLevelMapTemplate(kDungeonLevelWidth, kDungeonLevelHeight, path::ZPathFinder::kSolidRockCellWeight);
    EDungeonCell::Type** fakeNextLevelMap;
    utl::ZMatrix::Allocate(&fakeNextLevelMap, kDungeonLevelWidth, kDungeonLevelHeight, EDungeonCell::SolidRock);

    while (distancedRooms.size() > 0 && staircasesGeneratedCount < staircasesToBeGeneratedCount) {
        LOGD("got %d staircases, %d more to get", staircasesGeneratedCount, staircasesToBeGeneratedCount - staircasesGeneratedCount);

        std::sort(distancedRooms.begin(), distancedRooms.end(), [](DistancedRoom left, DistancedRoom right) {
            return left.distanceToClosestStaircase < right.distanceToClosestStaircase;
        });

        const DistancedRoom& currentRoom = distancedRooms[distancedRooms.size() - 1];
        const ZDungeonLevel::ZRoom& room = mRooms[currentRoom.index];

        std::vector<ZDirectionalStaircase> staircaseVariants;
        if (room.minY > kRoomMinSize + 2) {
            ZPosition position(utl::ZRandomHelpers::GetRandomValue(room.minX + 1, room.maxX - 1), room.minY - 1);
            staircaseVariants.emplace_back(position, ZDirection::kLeft);
        }
        if (room.maxY < kDungeonLevelHeight - 1 - kRoomMinSize - 2) {
            ZPosition position(utl::ZRandomHelpers::GetRandomValue(room.minX + 1, room.maxX - 1), room.maxY + 1);
            staircaseVariants.emplace_back(position, ZDirection::kRight);
        }
        if (room.minX > kRoomMinSize + 2) {
            ZPosition position(room.minX - 1, utl::ZRandomHelpers::GetRandomValue(room.minY + 1, room.maxY - 1));
            staircaseVariants.emplace_back(position, ZDirection::kBack);
        }
        if (room.maxX < kDungeonLevelWidth - 1 - kRoomMinSize - 2) {
            ZPosition position(room.maxX + 1, utl::ZRandomHelpers::GetRandomValue(room.minY + 1, room.maxY - 1));
            staircaseVariants.emplace_back(position, ZDirection::kForward);
        }
        std::shuffle(staircaseVariants.begin(), staircaseVariants.end(), std::default_random_engine());

        ZDungeonLevel::ZRoom staircaseRoom(0, 0, 0, 0);
        for (int j = 0; j < staircaseVariants.size(); ++j) {
            const ZDirectionalStaircase& staircase = staircaseVariants[j];

            bool mustBeDigged = CellIsSolidRock(mMap, staircase.position);
            bool isNotBlocked = !path::ZPathFinder::CellIsBlocked(*mWeightedMap, staircase.position);
            bool isLocatedInPocket = CountCellSolidNeighbours(staircase.position) == 5;

            path::ZWeightedMap nextLevelMapTemplateCopy(nextLevelMapTemplate);
            LOGD("current map without new staircase:\n%s", nextLevelMapTemplateCopy.ToString().c_str());

            ZDirectionalStaircase nextLevelUpStaircase = CalcUpStaircase(staircase.position, staircase.direction);
            BlockStaircasePocketCells(&nextLevelMapTemplateCopy, nextLevelUpStaircase);
            LOGD("blocked new staircase pocket:\n%s", nextLevelMapTemplateCopy.ToString().c_str());

            path::ZPathFinder::BlockCell(&nextLevelMapTemplateCopy, nextLevelUpStaircase.position);

            bool roomCalcSuccess = TryToCalcRoomNearStaircase(nextLevelUpStaircase, kRoomMinSize, kRoomMaxSize, &staircaseRoom);
            if (!roomCalcSuccess) {
                LOGD("cant calc room near staircase");
                continue;
            }
            bool canDigRoom = DigRoomIfAllCellsAreSolidAndNotBlocked(fakeNextLevelMap, &nextLevelMapTemplateCopy, staircaseRoom.minX, staircaseRoom.minY, staircaseRoom.maxX, staircaseRoom.maxY);
            if (canDigRoom) {
                LOGD("map with new staircase room:\n%s", nextLevelMapTemplateCopy.ToString().c_str());
            }

            LOGD("mustBeDigged = %d; isLocatedInPocket = %d; canDigRoom = %d", mustBeDigged, isLocatedInPocket, canDigRoom);

            if (mustBeDigged && isLocatedInPocket && canDigRoom) {
                mMap[staircase.position.GetX()][staircase.position.GetY()] = EDungeonCell::DownStaircase;
                ++staircasesGeneratedCount;

                nextLevelMapTemplate = nextLevelMapTemplateCopy;
                mNextLevelStaircaseRooms.push_back(staircaseRoom);

                for (auto& room : distancedRooms) {
                    int pathSize = path::ZPathFinder::FindPathBetweenCells(*mWeightedMap, staircase.position, room.someValidCell).size();
                    if (pathSize > 0 && pathSize < room.distanceToClosestStaircase) {
                        room.distanceToClosestStaircase = pathSize;
                    }
                }

                break;
            }
        }

        distancedRooms.pop_back();
    }

    utl::ZMatrix::Deallocate<EDungeonCell::Type>(&fakeNextLevelMap, kDungeonLevelHeight);
}

const ZDungeonLevelGenerator::ZDirectionalStaircase ZDungeonLevelGenerator::CalcUpStaircase(const ZPosition& downStaircasePosition, const ZDirection& downStaircaseDirection) {
    ZPosition upStaircasePosition = downStaircasePosition + downStaircaseDirection.PredictMove();
    ZDirection upStaircaseDirection = downStaircaseDirection.TurnCopy(ETurnDirection::Back);

    return ZDirectionalStaircase(upStaircasePosition, upStaircaseDirection);
}

void ZDungeonLevelGenerator::CalcUpStaircases(const ZDungeonLevel* previousLevel) {
    if (previousLevel) {
        for (const auto& downStaircasePosition : previousLevel->GetDownStaircases()) {
            ZDirection downStaircaseDirection = previousLevel->GetStaircaseDirection(downStaircasePosition);
            ZDirectionalStaircase upStaircase = CalcUpStaircase(downStaircasePosition, downStaircaseDirection);

            mUpStaircases.emplace_back(upStaircase);
        }
    } else {
        int startPositionX = utl::ZRandomHelpers::GetRandomValue(kRoomMaxSize, kDungeonLevelWidth - kRoomMaxSize);
        int startPositionY = utl::ZRandomHelpers::GetRandomValue(kRoomMaxSize, kDungeonLevelHeight - kRoomMaxSize);
        mUpStaircases.emplace_back(ZPosition(startPositionX, startPositionX), ZDirection());
    }
}

void ZDungeonLevelGenerator::BlockStaircasePocketCells(path::ZWeightedMap* weightedMap, const ZDirectionalStaircase& staircase) {
    ZDirection blockedDirection = staircase.direction;

    static ETurnDirection::Type blockedCellsDirections[] = {
        ETurnDirection::Left,
        ETurnDirection::ForwardLeft,
        ETurnDirection::Forward,
        ETurnDirection::ForwardRight,
        ETurnDirection::Right
    };
    static size_t blockedCellsDirectionsSize = sizeof(blockedCellsDirections) / sizeof(*blockedCellsDirections);

    for (int i = 0; i < blockedCellsDirectionsSize; ++i) {
        ZPosition pos = staircase.position + staircase.direction.TurnCopy(blockedCellsDirections[i]).PredictMove();
        path::ZPathFinder::BlockCell(weightedMap, pos);
    }
}

void ZDungeonLevelGenerator::BlockUpStaircasesAndTheirPocketCells() {
    for (auto& staircase : mUpStaircases) {
        BlockStaircasePocketCells(mWeightedMap, staircase);

        path::ZPathFinder::BlockCell(mWeightedMap, staircase.position);
    }
}

ZPosition ZDungeonLevelGenerator::CropPositionInsideLevel(const ZPosition& position) {
    return ZPosition(
        std::min(std::max(1, position.GetX()), kDungeonLevelWidth - 1),
        std::min(std::max(1, position.GetY()), kDungeonLevelHeight - 1)
    );
}

bool ZDungeonLevelGenerator::TryToCalcRoomNearStaircase(const ZDirectionalStaircase& staircase, int minSize, int maxSize, ZDungeonLevel::ZRoom* room) {
    ZDirection directionInsideRoom = staircase.direction.TurnCopy(ETurnDirection::Back);
    ZPosition someEdgeCell = staircase.position + directionInsideRoom.PredictMove();

    ZDirection nearWallLeftDirection = directionInsideRoom.TurnCopy(ETurnDirection::Left);
    int nearWallLeftPartMaxLength = maxSize - 1;   // cause staircases shouldn't be adjacent to room's corner
    int nearWallLeftPartMinLength = 2;
    int nearWallLeftPartLength = utl::ZRandomHelpers::GetRandomValue(nearWallLeftPartMinLength, nearWallLeftPartMaxLength);
    ZPosition nearWallLeftEnd = someEdgeCell + nearWallLeftDirection.PredictMove() * (nearWallLeftPartLength - 1);
    nearWallLeftEnd = CropPositionInsideLevel(nearWallLeftEnd);

    int nearWallLength = utl::ZRandomHelpers::GetRandomValue(std::max(nearWallLeftPartLength + 1, minSize), maxSize);
    ZPosition nearWallRightEnd = nearWallLeftEnd + (nearWallLength - 1) * nearWallLeftDirection.TurnCopy(ETurnDirection::Back).PredictMove();
    nearWallRightEnd = CropPositionInsideLevel(nearWallRightEnd);

    int farWallDistance = utl::ZRandomHelpers::GetRandomValue(minSize, maxSize);
    ZPosition farWallLeftEnd = CropPositionInsideLevel(nearWallLeftEnd + directionInsideRoom.PredictMove() * (farWallDistance - 1));
    ZPosition farWallRightEnd = CropPositionInsideLevel(nearWallRightEnd + directionInsideRoom.PredictMove() * (farWallDistance - 1));

    if (path::ZPathFinder::CalcCellsDistance(someEdgeCell, nearWallLeftEnd) < 1
            || path::ZPathFinder::CalcCellsDistance(someEdgeCell, nearWallRightEnd) < 1
            || path::ZPathFinder::CalcCellsDistance(nearWallLeftEnd, farWallLeftEnd) < kRoomMinSize - 1
            || path::ZPathFinder::CalcCellsDistance(nearWallLeftEnd, nearWallRightEnd) < kRoomMinSize - 1) {
        return false;
    }

    int xValues[] = {farWallLeftEnd.GetX(), farWallRightEnd.GetX(), nearWallLeftEnd.GetX(), nearWallRightEnd.GetX()};
    int yValues[] = {farWallLeftEnd.GetY(), farWallRightEnd.GetY(), nearWallLeftEnd.GetY(), nearWallRightEnd.GetY()};

    int minX = *std::min_element(xValues, xValues + 4);
    int minY = *std::min_element(yValues, yValues + 4);

    int maxX = *std::max_element(xValues, xValues + 4);
    int maxY = *std::max_element(yValues, yValues + 4);

    *room = ZDungeonLevel::ZRoom(minX, minY, maxX, maxY);

    return true;
}

void ZDungeonLevelGenerator::DigRoomsNearUpStaircases(const ZDungeonLevel* previousLevel) {
    ZDungeonLevel::ZRoomList roomList;
    if (previousLevel) {
        roomList = previousLevel->GetNextLevelStaircaseRooms();
    } else {
        ZDungeonLevel::ZRoom room(0, 0, 0, 0);
        for (const auto& staircase : mUpStaircases) {
            bool success = TryToCalcRoomNearStaircase(staircase, kRoomMinSize, kRoomMaxSize, &room);
            if (success) {
                roomList.push_back(room);
            } else {
                ZPosition firstCellOutOfStaircase = staircase.position + staircase.direction.TurnCopy(ETurnDirection::Back).PredictMove();
                DigCellIfSolidAndNotBlocked(mMap, mWeightedMap, firstCellOutOfStaircase);
            }
        }
    }

    for (const auto& room : roomList) {
        bool roomDigged = DigRoomIfAllCellsAreSolidAndNotBlocked(mMap, mWeightedMap, room.minX, room.minY, room.maxX, room.maxY);
        if (roomDigged) {
            mRooms.push_back(room);
        }
    }
}

void ZDungeonLevelGenerator::DigUpStaircases() {
    for (auto& staircase : mUpStaircases) {
        // use direct assignment instead of dig-method cause staircase cell was blocked for room generation:
        mWeightedMap->SetCellWeight(staircase.position, path::ZPathFinder::kEmptyCellWeight);
        mMap[staircase.position.GetX()][staircase.position.GetY()] = EDungeonCell::UpStaircase;
    }
}

void ZDungeonLevelGenerator::ConnectUpStaircasesWithSomeValidCell(const ZPosition& someValidCell) {
    for (auto& staircase : mUpStaircases) {
        int x = staircase.position.GetX();
        int y = staircase.position.GetY();

        if (path::ZPathFinder::CellIsBlocked(*mWeightedMap, x, y)) {
            mWeightedMap->SetCellWeight(x, y, path::ZPathFinder::kEmptyCellWeight);
        }

        ZPosition staircaseConnectedCellPosition = staircase.position + staircase.direction.TurnCopy(ETurnDirection::Back).PredictMove();
        if (path::ZPathFinder::CellIsBlocked(*mWeightedMap, staircaseConnectedCellPosition)) {
            mWeightedMap->SetCellWeight(staircaseConnectedCellPosition, path::ZPathFinder::kEmptyCellWeight);
            path::ZPathFinder::BlockCell(mWeightedMap, staircaseConnectedCellPosition + staircase.direction.TurnCopy(ETurnDirection::Left).PredictMove());
            path::ZPathFinder::BlockCell(mWeightedMap, staircaseConnectedCellPosition + staircase.direction.TurnCopy(ETurnDirection::Right).PredictMove());
        }

        ConnectCells(staircase.position, someValidCell);
    }
}

ZDungeonLevel* ZDungeonLevelGenerator::GenerateLevel(const ZDungeonLevel* previousLevel) {
    utl::ZMatrix::Allocate(&mMap, kDungeonLevelWidth, kDungeonLevelHeight, EDungeonCell::SolidRock);
    mWeightedMap = new path::ZWeightedMap(kDungeonLevelWidth, kDungeonLevelHeight, path::ZPathFinder::kSolidRockCellWeight);

    CalcUpStaircases(previousLevel);
    BlockUpStaircasesAndTheirPocketCells();
    DigRoomsNearUpStaircases(previousLevel);
    DigUpStaircases();

    BSPTreeNode subDungeonsTreeRoot(0, 0, kDungeonLevelWidth, kDungeonLevelHeight);
    utl::ZRandomHelpers::Initialize();
    bool success = TryToGenerateBSPTree(&subDungeonsTreeRoot);

    DigRandomTunnels();
    ConnectUpStaircasesWithSomeValidCell(subDungeonsTreeRoot.dungeon.someValidCell);
    DigRandomDownStaircases();

    ZDungeonLevel* level = new ZDungeonLevel(kDungeonLevelWidth, kDungeonLevelHeight, &mMap, mRooms, mNextLevelStaircaseRooms);

    delete mWeightedMap;
    mRooms.clear();
    mNextLevelStaircaseRooms.clear();
    mUpStaircases.clear();

    return level;
}

}
}
