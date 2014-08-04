#include "przCorePrivatePCH.h"
#include "model/DungeonLevelGenerator.h"

#include <algorithm>

#include "utils/MatrixHelpers.h"
#include "utils/RandomHelpers.h"

namespace prz {
    namespace mdl {

        const int ZDungeonLevelGenerator::kDungeonLevelWidth = 128;
        const int ZDungeonLevelGenerator::kDungeonLevelHeight = 128;

        const int ZDungeonLevelGenerator::kSubDungeonMinSize = 10;
        const int ZDungeonLevelGenerator::kRoomMinSize = 5;

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

        void ShrinkSubDungeon(SubDungeon* outerSubDungeon, const SubDungeon& lowerSubDungeon, const SubDungeon& higherSubDungeon) {
            outerSubDungeon->x1 = std::min(lowerSubDungeon.x1, higherSubDungeon.x1);
            outerSubDungeon->y1 = std::min(lowerSubDungeon.y1, higherSubDungeon.y1);
            outerSubDungeon->x2 = std::max(lowerSubDungeon.x2, higherSubDungeon.x2);
            outerSubDungeon->y2 = std::max(lowerSubDungeon.y2, higherSubDungeon.y2);
        }

        void ZDungeonLevelGenerator::GenerateBSPTree(BSPTreeNode* rootNode, EDungeonCell::Type*** map, DungeonRooms* rooms) {
            int width = rootNode->dungeon.GetWidth();
            int height = rootNode->dungeon.GetHeight();

            if (width > 2 * kSubDungeonMinSize && height > 2 * kSubDungeonMinSize) {
                if (utl::ZRandomHelpers::FlipCoin()) {
                    SplitSubDungeonVertically(rootNode, rooms);
                } else {
                    SplitSubDungeonHorizontally(rootNode, rooms);
                }
            } else if (width > 2 * kSubDungeonMinSize) {
                SplitSubDungeonVertically(rootNode, rooms);
            } else if (height > 2 * kSubDungeonMinSize) {
                SplitSubDungeonHorizontally(rootNode, rooms);
            } else {
                rooms->push_back(&rootNode->dungeon);
                CreateRoomInsideSubDungeon(map, &rootNode->dungeon);
                rootNode->dungeon.someValidCell = ZPosition(rootNode->dungeon.x1, rootNode->dungeon.y1);
                return;
            }

            GenerateBSPTree(rootNode->lowerSubDungeon, map, rooms);
            GenerateBSPTree(rootNode->higherSubDungeon, map, rooms);
            ShrinkSubDungeon(&rootNode->dungeon, rootNode->lowerSubDungeon->dungeon, rootNode->higherSubDungeon->dungeon);
            rootNode->dungeon.someValidCell = rootNode->lowerSubDungeon->dungeon.someValidCell;
        }

        void ZDungeonLevelGenerator::SplitSubDungeonVertically(BSPTreeNode* rootNode, DungeonRooms* rooms) {
            SubDungeon& dungeon = rootNode->dungeon;

            int maxSubDungeonsWidthDiff = rootNode->dungeon.GetWidth() - 2 * kSubDungeonMinSize;
            int lowerSubDungeonWidth = kSubDungeonMinSize + utl::ZRandomHelpers::GetRandomValue(maxSubDungeonsWidthDiff);
            int lowerSubDungeonX2 = dungeon.x1 + lowerSubDungeonWidth - 1;

            rootNode->lowerSubDungeon = new BSPTreeNode(SubDungeon(dungeon.x1, dungeon.y1, lowerSubDungeonX2, dungeon.y2));
            rootNode->higherSubDungeon = new BSPTreeNode(SubDungeon(lowerSubDungeonX2 + 1, dungeon.y1, dungeon.x2, dungeon.y2));
        }

        void ZDungeonLevelGenerator::SplitSubDungeonHorizontally(BSPTreeNode* rootNode, DungeonRooms* rooms) {
            SubDungeon& dungeon = rootNode->dungeon;

            int maxSubDungeonsHeightDiff = dungeon.GetHeight() - 2 * kSubDungeonMinSize;
            int lowerSubDungeonHeight = kSubDungeonMinSize + utl::ZRandomHelpers::GetRandomValue(maxSubDungeonsHeightDiff);
            int lowerSubDungeonY2 = dungeon.y1 + lowerSubDungeonHeight - 1;

            rootNode->lowerSubDungeon = new BSPTreeNode(SubDungeon(dungeon.x1, dungeon.y1, dungeon.x2, lowerSubDungeonY2));
            rootNode->higherSubDungeon = new BSPTreeNode(SubDungeon(dungeon.x1, lowerSubDungeonY2 + 1, dungeon.x2, dungeon.y2));
        }

        void ZDungeonLevelGenerator::CreateRoomInsideSubDungeon(EDungeonCell::Type*** map, SubDungeon* subDungeon) {
            int roomWidth = utl::ZRandomHelpers::GetRandomValue(kRoomMinSize, subDungeon->GetWidth() - 2);
            int roomX1 = subDungeon->x1 + utl::ZRandomHelpers::GetRandomValue(1, subDungeon->GetWidth() - 1 - roomWidth);
            int roomX2 = roomX1 + roomWidth - 1;

            int roomHeight = utl::ZRandomHelpers::GetRandomValue(kRoomMinSize, subDungeon->GetHeight() - 2);
            int roomY1 = subDungeon->y1 + utl::ZRandomHelpers::GetRandomValue(1, subDungeon->GetHeight() - 1 - roomHeight);
            int roomY2 = roomY1 + roomHeight - 1;

            *subDungeon = SubDungeon(roomX1, roomY1, roomX2, roomY2);

            for (int x = roomX1; x <= roomX2; ++x) {
                for (int y = roomY1; y < roomY2; ++y) {
                    (*map)[x][y] = EDungeonCell::Emptiness;
                }
            }
        }

        ZDungeonLevel* ZDungeonLevelGenerator::GenerateLevel(const ZDungeonLevel::StaircaseList& upStaircases) {
            EDungeonCell::Type** map;
            utl::ZMatrix::Allocate(&map, kDungeonLevelWidth, kDungeonLevelHeight);

            for (int x = 0; x < kDungeonLevelWidth; ++x) {
                for (int y = 0; y < kDungeonLevelHeight; ++y) {
                    map[x][y] = EDungeonCell::SolidRock;
                }
            }

            BSPTreeNode subDungeonsTreeRoot(SubDungeon(0, 0, kDungeonLevelWidth, kDungeonLevelHeight));
            DungeonRooms rooms;
            utl::ZRandomHelpers::Initialize();
            GenerateBSPTree(&subDungeonsTreeRoot, &map, &rooms);

            int startLeafIndex = utl::ZRandomHelpers::GetRandomValue(rooms.size() - 1);
            const SubDungeon* startSubDungeon = rooms[startLeafIndex];

            map[startSubDungeon->x1 + 1][startSubDungeon->y1 + 1] = EDungeonCell::UpStaircase;

            ZDungeonLevel* level = new ZDungeonLevel(kDungeonLevelWidth, kDungeonLevelHeight, &map);

            return level;
        }

    }
}
