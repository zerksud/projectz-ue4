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

            virtual ZDungeonLevel* GenerateLevel(const ZDungeonLevel* previousLevel = nullptr) override;

        private:
            static const int kDungeonLevelWidth;
            static const int kDungeonLevelHeight;

            static const int kSubDungeonMinSize;
            static const int kRoomMaxSize;
            static const int kRoomMinSize;

            static const ZWeight kSolidRockCellWeight;
            static const ZWeight kEmptyCellWeight;
            static const ZWeight kTunnelTurnPenalty;
            static const int kEstimatedPathWeightFactorIfDigging;
            static const int kEstimatedPathWeightFactorWithoutDigging;
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

            typedef std::vector<ZPosition> PathCells;
            //returns list of path cells starting from finishCell if path between cells exists, or empty list otherwise
            PathCells FindPathBetweenCells(const ZPosition& startCellPosition, const ZPosition& finishCellPosition, bool diggingIsAllowed = false);

            void ConnectCells(const ZPosition& someCell, const ZPosition& anotherCell);
            bool CellMustBeDigged(const ZPosition& position) const;
            bool CellMustBeDigged(int x, int y) const;
            // returns true if path from cell to neighbor is shorter than previous path to that cell
            ZWeightedCell* CreateNextPathCellIfMorePromising(const ZWeightedCell& currentCell, int dx, int dy, const ZPosition& finishCellPosition, PathCellConnection** pathConnections, int estimatedPathWeightFactor);
            void DigRandomTunnels();
            int CountCellSolidNeighbours(const ZPosition& cell) const;
            void PlaceUpStaircases(const ZDungeonLevel* previousLevel = nullptr);
            void ConnectUpStaircasesWithSomeValidCell(const ZPosition& someValidCell);
            void AddRandomDownStaircases();

            EDungeonCell::Type** mMap;
            ZWeight** mMapCellWeight;
            DungeonRooms mRooms;
            ZDirectionalStaircaseList mUpStaircases;
        };
    }
}
