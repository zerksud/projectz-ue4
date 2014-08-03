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
            static const int kRoomMinSize;

            static void GenerateBSPTree(BSPTreeNode* rootNode, DungeonRooms* rooms);
            static void SplitSubDungeonVertically(BSPTreeNode* rootNode, DungeonRooms* rooms);
            static void SplitSubDungeonHorizontally(BSPTreeNode* rootNode, DungeonRooms* rooms);
            static void CreateRoomInsideSubDungeon(EDungeonCell::Type*** map, SubDungeon* dungeon);
        };
    }
}
