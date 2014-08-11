#pragma once

#include "model/IDungeonLevelGenerator.h"

namespace prz {
    namespace mdl {

        struct BSPTreeNode;
        struct SubDungeon;
        typedef std::vector<SubDungeon*> DungeonRooms;

        class ZDungeonLevelGenerator : public IDungeonLevelGenerator {
        public:
            ZDungeonLevelGenerator() = default;
            virtual ~ZDungeonLevelGenerator() = default;

            virtual ZDungeonLevel* GenerateLevel(const ZDungeonLevel::StaircaseList& upStaircases = ZDungeonLevel::StaircaseList()) override;

        private:
            static const int kDungeonLevelWidth;
            static const int kDungeonLevelHeight;

            static const int kSubDungeonMinSize;
            static const int kRoomMaxSize;
            static const int kRoomMinSize;

            static const int kSolidRockCellWeight;
            static const int kEmptyCellWeight;
            static const int kForbiddenCellWeight;

            void GenerateBSPTree(BSPTreeNode* rootNode, bool tryToSplitVertically = true);
            void SplitSubDungeonVertically(BSPTreeNode* rootNode);
            void SplitSubDungeonHorizontally(BSPTreeNode* rootNode);
            void CreateRoomInsideSubDungeon(SubDungeon* dungeon);
            void ConnectDirectSubDungeons(const SubDungeon& lowerSubDungeon, const SubDungeon& higherSubDungeon);

            EDungeonCell::Type** mMap;
            int** mMapCellWeight;
            DungeonRooms mRooms;
        };
    }
}
