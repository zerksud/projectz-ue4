#include "przCorePrivatePCH.h"
#include "model/DungeonLevelGenerator.h"

#include <cstdlib>
#include <ctime>

#include "utils/MatrixHelpers.h"

namespace prz {
    namespace mdl {

        const int ZDungeonLevelGenerator::kDungeonLevelWidth = 256;
        const int ZDungeonLevelGenerator::kDungeonLevelHeight = 256;

        const int ZDungeonLevelGenerator::kSubDungeonMinSize = 10;

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
                if (std::rand() % 2 == 0) {
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
            int lowerSubDungeonWidth = kSubDungeonMinSize + std::rand() % (maxSubDungeonsWidthDiff + 1);

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
            int lowerSubDungeonHeight = kSubDungeonMinSize + std::rand() % (maxSubDungeonsHeightDiff + 1);

            rootNode->lowerSubDungeon = new BSPTreeNode(SubDungeon(x, y, width, lowerSubDungeonHeight));
            rootNode->higherSubDungeon = new BSPTreeNode(SubDungeon(x, y + lowerSubDungeonHeight, width, height - lowerSubDungeonHeight));

            GenerateBSPTree(rootNode->lowerSubDungeon, leafs);
            GenerateBSPTree(rootNode->higherSubDungeon, leafs);
        }

        void ZDungeonLevelGenerator::CreateRoomInsideSubDungeon(EDungeonCell::Type*** map, SubDungeon* dungeon) {
            for (int dx = 1; dx < dungeon->width - 1; ++dx) {
                for (int dy = 1; dy < dungeon->height - 1; ++dy) {
                    (*map)[dungeon->x + dx][dungeon->y + dy] = EDungeonCell::Emptiness;
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
            std::srand(std::time(0));
            GenerateBSPTree(&subDungeonsTreeRoot, &leafs);

            for (auto& item : leafs) {
                CreateRoomInsideSubDungeon(&map, &item->dungeon);
            }

            int startLeafIndex = std::rand() % leafs.size();
            const SubDungeon& startSubDungeon = leafs[startLeafIndex]->dungeon;

            map[startSubDungeon.x + 2][startSubDungeon.y + 2] = EDungeonCell::UpStaircase;

            ZDungeonLevel* level = new ZDungeonLevel(kDungeonLevelWidth, kDungeonLevelHeight, &map);

            return level;
        }

    }
}
