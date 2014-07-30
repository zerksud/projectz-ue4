#include "przCorePrivatePCH.h"
#include "model/DungeonLevelGenerator.h"

#include "utils/MatrixHelpers.h"

namespace prz {
    namespace mdl {

        const int ZDungeonLevelGenerator::kDungeonLevelWidth = 256;
        const int ZDungeonLevelGenerator::kDungeonLevelHeight = 256;

        ZDungeonLevel* ZDungeonLevelGenerator::GenerateLevel(const ZDungeonLevel::StaircaseList& upStaircases) {
            EDungeonCell::Type** map;
            utl::ZMatrix::Allocate(&map, kDungeonLevelWidth, kDungeonLevelHeight);

            for (int x = 0; x < kDungeonLevelWidth; ++x) {
                for (int y = 0; y < kDungeonLevelHeight; ++y) {
                    map[x][y] = EDungeonCell::SolidRock;
                }
            }

            for (int x = 5; x < 5 + 3; ++x) {
                for (int y = 5; y < 5 + 4; ++y) {
                    map[x][y] = EDungeonCell::Emptiness;
                }
            }
            map[6][7] = EDungeonCell::UpStaircase;

            ZDungeonLevel* level = new ZDungeonLevel(kDungeonLevelWidth, kDungeonLevelHeight, &map);

            return level;
        }

    }
}
