#pragma once

#include "model/IDungeonLevelGenerator.h"

namespace prz {
    namespace mdl {

        struct BSPTreeNode;
        typedef std::vector<BSPTreeNode*> BSPTreeNodes;

        class ZDungeonLevelGenerator : public IDungeonLevelGenerator {
        public:
            ZDungeonLevelGenerator() = default;
            virtual ~ZDungeonLevelGenerator() = default;

            virtual ZDungeonLevel* GenerateLevel(const ZDungeonLevel::StaircaseList& upStaircases = ZDungeonLevel::StaircaseList()) override;

        private:
            static const int kDungeonLevelWidth;
            static const int kDungeonLevelHeight;

            static const int kSubDungeonMinSize;

            static void GenerateBSPTree(BSPTreeNode* rootNode, BSPTreeNodes* leafs);
            static void SplitSubDungeonVertically(BSPTreeNode* rootNode, BSPTreeNodes* leafs);
            static void SplitSubDungeonHorizontally(BSPTreeNode* rootNode, BSPTreeNodes* leafs);
        };
    }
}
