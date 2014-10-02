#include "przCorePCH.h"
#include "Model/DungeonLevelGenerator.h"

#include <algorithm>
#include <limits>
#include <numeric>
#include <random>
#include <utility>

#include "Utils/MatrixHelpers.h"
#include "Utils/RandomHelpers.h"

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

void ZDungeonLevelGenerator::DigCellIfSolidAndNotBlocked(const ZPosition& position) {
    DigCellIfSolidAndNotBlocked(position.GetX(), position.GetY());
}

void ZDungeonLevelGenerator::DigCellIfSolidAndNotBlocked(int x, int y) {
    if (mMap[x][y] == EDungeonCell::SolidRock && !path::ZPathFinder::CellIsBlocked(*mWeightedMap, x, y)) {
        mMap[x][y] = EDungeonCell::Emptiness;
        mWeightedMap->SetCellWeight(x, y, path::ZPathFinder::kEmptyCellWeight);
    }
}

void ZDungeonLevelGenerator::ConnectCells(const ZPosition& someCell, const ZPosition& anotherCell) {
    path::ZPathFinder::PathCells path = path::ZPathFinder::FindPathBetweenCells(*mWeightedMap, someCell, anotherCell, true);
    for (auto cell : path) {
        DigCellIfSolidAndNotBlocked(cell);
    }
}

void ShrinkSubDungeon(SubDungeon* outerSubDungeon, const SubDungeon& lowerSubDungeon, const SubDungeon& higherSubDungeon) {
    outerSubDungeon->x1 = std::min(lowerSubDungeon.x1, higherSubDungeon.x1);
    outerSubDungeon->y1 = std::min(lowerSubDungeon.y1, higherSubDungeon.y1);
    outerSubDungeon->x2 = std::max(lowerSubDungeon.x2, higherSubDungeon.x2);
    outerSubDungeon->y2 = std::max(lowerSubDungeon.y2, higherSubDungeon.y2);
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

    bool lowerSubDungeonGenerated = TryToGenerateBSPTree(rootNode->lowerSubDungeon, !tryToSplitVertically);
    bool higherSubDungeonGenerated = TryToGenerateBSPTree(rootNode->higherSubDungeon, !tryToSplitVertically);

    if (lowerSubDungeonGenerated && higherSubDungeonGenerated) {
        const SubDungeon& lowerSubDungeon = rootNode->lowerSubDungeon->dungeon;
        const SubDungeon& higherSubDungeon = rootNode->higherSubDungeon->dungeon;
        ShrinkSubDungeon(&rootSubDungeon, lowerSubDungeon, higherSubDungeon);
        rootSubDungeon.someValidCell = lowerSubDungeon.someValidCell;
        ConnectCells(lowerSubDungeon.someValidCell, higherSubDungeon.someValidCell);
    } else if (lowerSubDungeonGenerated) {
        rootSubDungeon = rootNode->lowerSubDungeon->dungeon;
    } else if (higherSubDungeonGenerated) {
        rootSubDungeon = rootNode->higherSubDungeon->dungeon;
    } else {
        return false;
    }

    return true;
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

bool ZDungeonLevelGenerator::TryToCreateRoomInsideSubDungeon(SubDungeon* subDungeon) {
    int roomWidth = utl::ZRandomHelpers::GetRandomValue(kRoomMinSize, std::min(kRoomMaxSize, subDungeon->GetWidth() - 1 - 2));
    int roomX1 = subDungeon->x1 + utl::ZRandomHelpers::GetRandomValue(1, subDungeon->GetWidth() - 2 - roomWidth);
    int roomX2 = roomX1 + roomWidth - 1;

    int roomHeight = utl::ZRandomHelpers::GetRandomValue(kRoomMinSize, std::min(kRoomMaxSize, subDungeon->GetHeight() - 1 - 2));
    int roomY1 = subDungeon->y1 + utl::ZRandomHelpers::GetRandomValue(1, subDungeon->GetHeight() - 2 - roomHeight);
    int roomY2 = roomY1 + roomHeight - 1;

    bool roomDigged = DigRoomIfAllCellsAreSolidAndNotBlocked(roomX1, roomY1, roomX2, roomY2);
    if (roomDigged) {
        int someValidCellX = utl::ZRandomHelpers::GetRandomValue(roomX1, roomX2);
        int someValidCellY = utl::ZRandomHelpers::GetRandomValue(roomY1, roomY2);
        ZPosition someValidCell = ZPosition(someValidCellX, someValidCellY);
        *subDungeon = SubDungeon(roomX1, roomY1, roomX2, roomY2, someValidCell);
        mRooms.push_back(*subDungeon);
    }

    return roomDigged;
}

bool ZDungeonLevelGenerator::DigRoomIfAllCellsAreSolidAndNotBlocked(int minX, int minY, int maxX, int maxY) {
    for (int x = minX - 1; x <= maxX + 1; ++x) {
        for (int y = minY - 1; y <= maxY + 1; ++y) {
            if (!path::ZPathFinder::CellMustBeDigged(*mWeightedMap, x, y)) {
                return false;
            }
        }
    }

    for (int x = minX; x <= maxX; ++x) {
        for (int y = minY; y <= maxY; ++y) {
            if (path::ZPathFinder::CellIsBlocked(*mWeightedMap, x, y)) {
                return false;
            }
        }
    }

    for (int x = minX; x <= maxX; ++x) {
        for (int y = minY; y <= maxY; ++y) {
            DigCellIfSolidAndNotBlocked(x, y);
        }
    }

    // tunnels should not start at room's corner or touch room's edge, so cells adjacent to corners should be blocked:
    // #x##x#
    // x    x
    // #    #
    // x    x
    // #x##x#
    path::ZPathFinder::BlockCell(mWeightedMap, minX - 1, minY);
    path::ZPathFinder::BlockCell(mWeightedMap, maxX + 1, minY);
    path::ZPathFinder::BlockCell(mWeightedMap, minX - 1, maxY);
    path::ZPathFinder::BlockCell(mWeightedMap, maxX + 1, maxY);

    path::ZPathFinder::BlockCell(mWeightedMap, minX, minY - 1);
    path::ZPathFinder::BlockCell(mWeightedMap, maxX, minY - 1);
    path::ZPathFinder::BlockCell(mWeightedMap, minX, maxY + 1);
    path::ZPathFinder::BlockCell(mWeightedMap, maxX, maxY + 1);

    return true;
}

void ZDungeonLevelGenerator::DigRandomTunnels() {
    int tryCount = mRooms.size() * kRoomCountFractionToDigRandomTunnelsFrom;
    for (int i = 0; i < tryCount; ++i) {
        int fromRoomIndex = utl::ZRandomHelpers::GetRandomValue(mRooms.size() - 1);
        int toRoomIndex = utl::ZRandomHelpers::GetRandomValue(mRooms.size() - 1);
        ConnectCells(mRooms[fromRoomIndex].someValidCell, mRooms[toRoomIndex].someValidCell);
    }
}

int ZDungeonLevelGenerator::CountCellSolidNotBlockedNeighbours(const ZPosition& cell) const {
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
            || path::ZPathFinder::CellMustBeDigged(*mWeightedMap, adjacentCell) && !path::ZPathFinder::CellIsBlocked(*mWeightedMap, adjacentCell)) {
            ++count;
        }
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
        room.someValidCell = mRooms[i].someValidCell;
        room.distanceToClosestStaircase = std::numeric_limits<int>::max();
        for (auto& upStaircase : mUpStaircases) {
            int pathSize = path::ZPathFinder::FindPathBetweenCells(*mWeightedMap, room.someValidCell, upStaircase.position).size();
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
        const SubDungeon& subDungeon = mRooms[currentRoom.index];

        std::vector<ZPosition> staircasePositionVariants;
        if (subDungeon.y1 > kRoomMinSize + 2) {
            staircasePositionVariants.emplace_back(utl::ZRandomHelpers::GetRandomValue(subDungeon.x1 + 1, subDungeon.x2 - 1), subDungeon.y1 - 1);
        }
        if (subDungeon.y2 < kDungeonLevelHeight - 1 - kRoomMinSize - 2) {
            staircasePositionVariants.emplace_back(utl::ZRandomHelpers::GetRandomValue(subDungeon.x1 + 1, subDungeon.x2 - 1), subDungeon.y2 + 1);
        }
        if (subDungeon.x1 > kRoomMinSize + 2) {
            staircasePositionVariants.emplace_back(subDungeon.x1 - 1, utl::ZRandomHelpers::GetRandomValue(subDungeon.y1 + 1, subDungeon.y2 - 1));
        }
        if (subDungeon.x2 < kDungeonLevelWidth - 1 - kRoomMinSize - 2) {
            staircasePositionVariants.emplace_back(subDungeon.x2 + 1, utl::ZRandomHelpers::GetRandomValue(subDungeon.y1 + 1, subDungeon.y2 - 1));
        }
        std::shuffle(staircasePositionVariants.begin(), staircasePositionVariants.end(), std::default_random_engine());

        for (int j = 0; j < staircasePositionVariants.size(); ++j) {
            const ZPosition& staircasePosition = staircasePositionVariants[j];

            bool mustBeDigged = path::ZPathFinder::CellMustBeDigged(*mWeightedMap, staircasePosition);
            bool isLocatedInPocket = CountCellSolidNotBlockedNeighbours(staircasePosition) == 5;
            if (mustBeDigged && isLocatedInPocket) {
                mMap[staircasePosition.GetX()][staircasePosition.GetY()] = EDungeonCell::DownStaircase;
                ++staircasesGeneratedCount;

                for (auto& room : rooms) {
                    int pathSize = path::ZPathFinder::FindPathBetweenCells(*mWeightedMap, staircasePosition, room.someValidCell).size();
                    if (pathSize > 0 && pathSize < room.distanceToClosestStaircase) {
                        room.distanceToClosestStaircase = pathSize;
                    }
                }

                break;
            }
        }

        rooms.pop_back();
    }
}

void ZDungeonLevelGenerator::CalcUpStaircases(const ZDungeonLevel* previousLevel) {
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
        mUpStaircases.emplace_back(ZPosition(startPositionX, startPositionX), ZDirection());
    }
}

void ZDungeonLevelGenerator::BlockStaircaseAdjacentCellsOnDirectionSide(const ZDirectionalStaircase& staircase) {
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
        path::ZPathFinder::BlockCell(mWeightedMap, pos);
    }
}

void ZDungeonLevelGenerator::MarkUpStaircasesAdjacentCellsOnDirectionSideAsBlocked() {
    for (auto& staircase : mUpStaircases) {
        BlockStaircaseAdjacentCellsOnDirectionSide(staircase);

        path::ZPathFinder::BlockCell(mWeightedMap, staircase.position);
    }
}

ZPosition ZDungeonLevelGenerator::CropPositionInsideLevel(const ZPosition& position) {
    return ZPosition(
        std::min(std::max(1, position.GetX()), kDungeonLevelWidth - 1),
        std::min(std::max(1, position.GetY()), kDungeonLevelHeight - 1)
    );
}

const ZDungeonLevel::ZRoom ZDungeonLevelGenerator::CalcRoomNearStaircase(const ZDirectionalStaircase& staircase, int minSize, int maxSize) {
    ZDirection directionInsideRoom = staircase.direction.TurnCopy(ETurnDirection::Back);
    ZPosition someEdgeCell = staircase.position + directionInsideRoom.PredictMove();

    ZDirection nearWallLeftDirection = directionInsideRoom.TurnCopy(ETurnDirection::Left);
    int nearWallLeftPartMaxLength = maxSize - 2;   // cause staircases shouldn't be adjacent to room's corner
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

    int xValues[] = {farWallLeftEnd.GetX(), farWallRightEnd.GetX(), nearWallLeftEnd.GetX(), nearWallRightEnd.GetX()};
    int yValues[] = {farWallLeftEnd.GetY(), farWallRightEnd.GetY(), nearWallLeftEnd.GetY(), nearWallRightEnd.GetY()};

    int minX = *std::min_element(xValues, xValues + 4);
    int minY = *std::min_element(yValues, yValues + 4);

    int maxX = *std::max_element(xValues, xValues + 4);
    int maxY = *std::max_element(yValues, yValues + 4);

    return ZDungeonLevel::ZRoom(minX, minY, maxX, maxY);
}

void ZDungeonLevelGenerator::DigRoomsNearUpStaircases() {
    for (const auto& staircase : mUpStaircases) {
        
        ZDungeonLevel::ZRoom room = CalcRoomNearStaircase(staircase, kRoomMinSize, kRoomMaxSize);

        bool roomDigged = DigRoomIfAllCellsAreSolidAndNotBlocked(room.minX, room.minY, room.maxX, room.maxY);
        if (roomDigged) {
            int someValidCellX = utl::ZRandomHelpers::GetRandomValue(room.minX, room.maxX);
            int someValidCellY = utl::ZRandomHelpers::GetRandomValue(room.minY, room.maxY);
            ZPosition someValidCell = ZPosition(someValidCellX, someValidCellY);

            mRooms.emplace_back(room.minX, room.minY, room.maxX, room.maxY, someValidCell);
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
    MarkUpStaircasesAdjacentCellsOnDirectionSideAsBlocked();
    DigRoomsNearUpStaircases();
    DigUpStaircases();

    BSPTreeNode subDungeonsTreeRoot(0, 0, kDungeonLevelWidth, kDungeonLevelHeight);
    utl::ZRandomHelpers::Initialize();
    bool success = TryToGenerateBSPTree(&subDungeonsTreeRoot);

    DigRandomTunnels();
    ConnectUpStaircasesWithSomeValidCell(subDungeonsTreeRoot.dungeon.someValidCell);
    AddRandomDownStaircases();

    ZDungeonLevel::ZRoomList roomList;
    for (auto room : mRooms) {
        roomList.emplace_back(room.x1, room.y1, room.x2, room.y2);
    }

    ZDungeonLevel* level = new ZDungeonLevel(kDungeonLevelWidth, kDungeonLevelHeight, &mMap, roomList, ZDungeonLevel::ZRoomList());

    delete mWeightedMap;
    mRooms.clear();
    mUpStaircases.clear();

    return level;
}

}
}
