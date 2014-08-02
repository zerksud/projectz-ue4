#include "przCorePrivatePCH.h"
#include "model/DungeonLevelGenerator.h"

#include "utils/MatrixHelpers.h"
#include "utils/RandomHelpers.h"

namespace prz {
    namespace mdl {

        const int ZDungeonLevelGenerator::kDungeonLevelWidth = 128;
        const int ZDungeonLevelGenerator::kDungeonLevelHeight = 128;

        const int ZDungeonLevelGenerator::kSubDungeonMinSize = 10;
        const int ZDungeonLevelGenerator::kRoomMinSize = 5;

        struct SubDungeon {
            int x;
            int y;
            int width;
            int height;

            SubDungeon(int pX, int pY, int pWidth, int pHeight)
                : x(pX), y(pY), width(pWidth), height(pHeight) {
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


        void ZDungeonLevelGenerator::GenerateBSPTree(BSPTreeNode* rootNode, BSPTreeNodes* leafs) {
            int width = rootNode->dungeon.width;
            int height = rootNode->dungeon.height;

            if (width > 2 * kSubDungeonMinSize && height > 2 * kSubDungeonMinSize) {
                if (utl::ZRandomHelpers::FlipCoin()) {
                    SplitSubDungeonVertically(rootNode, leafs);
                } else {
                    SplitSubDungeonHorizontally(rootNode, leafs);
                }
            } else if (width > 2 * kSubDungeonMinSize) {
                SplitSubDungeonVertically(rootNode, leafs);
            } else if (height > 2 * kSubDungeonMinSize) {
                SplitSubDungeonHorizontally(rootNode, leafs);
            } else {
                leafs->push_back(rootNode);
            }
        }

        void ZDungeonLevelGenerator::SplitSubDungeonVertically(BSPTreeNode* rootNode, BSPTreeNodes* leafs) {
            int x = rootNode->dungeon.x;
            int y = rootNode->dungeon.y;
            int width = rootNode->dungeon.width;
            int height = rootNode->dungeon.height;

            int maxSubDungeonsWidthDiff = width - 2 * kSubDungeonMinSize;
            int lowerSubDungeonWidth = kSubDungeonMinSize + utl::ZRandomHelpers::GetRandomValue(maxSubDungeonsWidthDiff);

            rootNode->lowerSubDungeon = new BSPTreeNode(SubDungeon(x, y, lowerSubDungeonWidth, height));
            rootNode->higherSubDungeon = new BSPTreeNode(SubDungeon(x + lowerSubDungeonWidth, y, width - lowerSubDungeonWidth, height));

            GenerateBSPTree(rootNode->lowerSubDungeon, leafs);
            GenerateBSPTree(rootNode->higherSubDungeon, leafs);
        }

        void ZDungeonLevelGenerator::SplitSubDungeonHorizontally(BSPTreeNode* rootNode, BSPTreeNodes* leafs) {
            int x = rootNode->dungeon.x;
            int y = rootNode->dungeon.y;
            int width = rootNode->dungeon.width;
            int height = rootNode->dungeon.height;

            int maxSubDungeonsHeightDiff = height - 2 * kSubDungeonMinSize;
            int lowerSubDungeonHeight = kSubDungeonMinSize + utl::ZRandomHelpers::GetRandomValue(maxSubDungeonsHeightDiff);

            rootNode->lowerSubDungeon = new BSPTreeNode(SubDungeon(x, y, width, lowerSubDungeonHeight));
            rootNode->higherSubDungeon = new BSPTreeNode(SubDungeon(x, y + lowerSubDungeonHeight, width, height - lowerSubDungeonHeight));

            GenerateBSPTree(rootNode->lowerSubDungeon, leafs);
            GenerateBSPTree(rootNode->higherSubDungeon, leafs);
        }

        void ZDungeonLevelGenerator::CreateRoomInsideSubDungeon(EDungeonCell::Type*** map, SubDungeon* subDungeon) {
            int roomWidth = utl::ZRandomHelpers::GetRandomValue(kRoomMinSize, subDungeon->width - 2);
            int roomX = subDungeon->x + utl::ZRandomHelpers::GetRandomValue(1, subDungeon->width - 1 - roomWidth);

            int roomHeight = utl::ZRandomHelpers::GetRandomValue(kRoomMinSize, subDungeon->height - 2);
            int roomY = subDungeon->y + utl::ZRandomHelpers::GetRandomValue(1, subDungeon->height - 1 - roomHeight);

            *subDungeon = SubDungeon(roomX, roomY, roomWidth, roomHeight);

            for (int dx = 0; dx < roomWidth - 1; ++dx) {
                for (int dy = 0; dy < roomHeight - 1; ++dy) {
                    (*map)[roomX + dx][roomY + dy] = EDungeonCell::Emptiness;
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
            BSPTreeNodes leafs;
            utl::ZRandomHelpers::Initialize();
            GenerateBSPTree(&subDungeonsTreeRoot, &leafs);

            for (auto& item : leafs) {
                CreateRoomInsideSubDungeon(&map, &item->dungeon);
            }

            int startLeafIndex = utl::ZRandomHelpers::GetRandomValue(leafs.size() - 1);
            const SubDungeon& startSubDungeon = leafs[startLeafIndex]->dungeon;

            map[startSubDungeon.x + 1][startSubDungeon.y + 1] = EDungeonCell::UpStaircase;

            ZDungeonLevel* level = new ZDungeonLevel(kDungeonLevelWidth, kDungeonLevelHeight, &map);

            return level;
        }

    }
}
