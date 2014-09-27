#pragma once

#include "model/IDungeonLevelGenerator.h"
#include "model/WeightedCell.h"
#include "model/PathFinder.h"

namespace prz {
    namespace mdl {

        struct BSPTreeNode;
        struct SubDungeon;
        typedef std::vector<SubDungeon*> DungeonRooms;
        struct PathCellConnection;

        class ZDungeonLevelGenerator : public IDungeonLevelGenerator {
        public:
            ZDungeonLevelGenerator() = default;
            virtual ~ZDungeonLevelGenerator() = default;

            virtual ZDungeonLevel* GenerateLevel(const ZDungeonLevel* previousLevel = nullptr) override;

        private:
            static const int kDungeonLevelWidth;
            static const int kDungeonLevelHeight;

            static const int kSubDungeonMinSize;
            static const int kRoomMaxSize;
            static const int kRoomMinSize;

            static const float kRoomCountFractionToDigRandomTunnelsFrom;

            static const int kStaircaseCount;

            struct ZDirectionalStaircase {
                ZPosition position;
                ZDirection direction;

                ZDirectionalStaircase(const ZPosition& pPosition, const ZDirection& pDirection)
                    : position(pPosition), direction(pDirection) {
                }
            };
            typedef std::vector<ZDirectionalStaircase> ZDirectionalStaircaseList;

            void GenerateBSPTree(BSPTreeNode* rootNode, bool tryToSplitVertically = true);
            void SplitSubDungeonVertically(BSPTreeNode* rootNode);
            void SplitSubDungeonHorizontally(BSPTreeNode* rootNode);
            void DiggCellIfSolidAndNotBlocked(const ZPosition& position);
            void DiggCellIfSolidAndNotBlocked(int x, int y);
            // assignes infinite weight to cell with position
            void BlockCell(const ZPosition& position);
            // assignes infinite weight to cell [x, y]
            void BlockCell(int x, int y);
            void CreateRoomInsideSubDungeon(SubDungeon* dungeon);
            
            void ConnectCells(const ZPosition& someCell, const ZPosition& anotherCell);
            
            void DigRandomTunnels();
            int CountCellSolidNeighbours(const ZPosition& cell) const;
            void PlaceUpStaircases(const ZDungeonLevel* previousLevel = nullptr);
            void ConnectUpStaircasesWithSomeValidCell(const ZPosition& someValidCell);
            void AddRandomDownStaircases();

            EDungeonCell::Type** mMap;
            ZWeightedMap* mWeightedMap;
            DungeonRooms mRooms;
            ZDirectionalStaircaseList mUpStaircases;
        };
    }
}
