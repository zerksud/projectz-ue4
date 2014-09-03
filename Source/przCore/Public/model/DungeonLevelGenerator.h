#pragma once

#include "model/IDungeonLevelGenerator.h"

#include "model/WeightedCell.h"

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

            virtual ZDungeonLevel* GenerateLevel(const ZDungeonLevel::StaircaseList& upStaircases = ZDungeonLevel::StaircaseList()) override;

        private:
            static const int kDungeonLevelWidth;
            static const int kDungeonLevelHeight;

            static const int kSubDungeonMinSize;
            static const int kRoomMaxSize;
            static const int kRoomMinSize;

            static const ZWeight kSolidRockCellWeight;
            static const ZWeight kEmptyCellWeight;
            static const ZWeight kTunnelTurnPenalty;
            static const int kEstimatedPathWeightFactor;
            static const float kRoomCountFractionToDigRandomTunnelsFrom;

            static const int kStaircaseCount;

            void GenerateBSPTree(BSPTreeNode* rootNode, bool tryToSplitVertically = true);
            void SplitSubDungeonVertically(BSPTreeNode* rootNode);
            void SplitSubDungeonHorizontally(BSPTreeNode* rootNode);
            void DiggCellIfSolid(const ZPosition& position, EDungeonCell::Type cellType = EDungeonCell::Emptiness);
            void DiggCellIfSolid(int x, int y, EDungeonCell::Type cellType = EDungeonCell::Emptiness);
            // assignes infinite weight to cell [x, y]
            void BlockCell(int x, int y);
            void CreateRoomInsideSubDungeon(SubDungeon* dungeon);

            typedef std::vector<ZPosition> PathCells;
            //returns list of path cells starting from finishCell if path between cells exists, or empty list otherwise
            PathCells FindPathBetweenCells(const ZPosition& startCellPosition, const ZPosition& finishCellPosition);

            void ConnectCells(const ZPosition& someCell, const ZPosition& anotherCell);
            bool CellMustBeDigged(const ZPosition& position) const;
            bool CellMustBeDigged(int x, int y) const;
            // returns true if path from cell to neighbor is shorter than previous path to that cell
            ZWeightedCell* CreateNextPathCell(const ZWeightedCell& currentCell, int dx, int dy, const ZPosition& finishCellPosition, PathCellConnection** pathConnections);
            void DigRandomTunnels();
            int CountCellSolidNeighbours(const ZPosition& cell) const;
            void AddRandomDownStaircases();

            EDungeonCell::Type** mMap;
            ZWeight** mMapCellWeight;
            DungeonRooms mRooms;
        };
    }
}
