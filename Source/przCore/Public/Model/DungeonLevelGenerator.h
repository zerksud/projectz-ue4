#pragma once

#include "Model/IDungeonLevelGenerator.h"
#include "Model/Path/WeightedCell.h"
#include "Model/Path/PathFinder.h"

namespace prz {
namespace mdl {

struct BSPTreeNode;

struct SubDungeon {
    int x1;
    int y1;
    int x2;
    int y2;

    ZPosition someValidCell;

    SubDungeon(int pX1, int pY1, int pX2, int pY2)
        : x1(pX1), y1(pY1), x2(pX2), y2(pY2), someValidCell(-1, -1) {
    }

    SubDungeon(int pX1, int pY1, int pX2, int pY2, const ZPosition& pSomeValidCell)
        : x1(pX1), y1(pY1), x2(pX2), y2(pY2), someValidCell(pSomeValidCell) {
    }

    int GetWidth() {
        return (x2 - x1 + 1);
    }

    int GetHeight() {
        return (y2 - y1 + 1);
    }
};

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

    bool TryToGenerateBSPTree(BSPTreeNode* rootNode, bool tryToSplitVertically = true);
    void SplitSubDungeonVertically(BSPTreeNode* rootNode);
    void SplitSubDungeonHorizontally(BSPTreeNode* rootNode);
    static void DigCellIfSolidAndNotBlocked(EDungeonCell::Type** map, path::ZWeightedMap* weightedMap, const ZPosition& position);
    static void DigCellIfSolidAndNotBlocked(EDungeonCell::Type** map, path::ZWeightedMap* weightedMap, int x, int y);
    bool TryToCreateRoomInsideSubDungeon(SubDungeon* dungeon);
    static bool CellIsSolidRock(EDungeonCell::Type** map, int x, int y);
    static bool CellIsSolidRock(EDungeonCell::Type** map, const ZPosition& position);

    // returns true if room was successfully digged
    static bool DigRoomIfAllCellsAreSolidAndNotBlocked(EDungeonCell::Type** map, path::ZWeightedMap* weightedMap, const ZDungeonLevel::ZRoom& room);

    void ConnectCells(const ZPosition& someCell, const ZPosition& anotherCell);

    void DigRandomTunnels();
    int CountCellSolidNeighbours(const ZPosition& cell) const;
    static const ZDirectionalStaircase CalcUpStaircase(const ZPosition& downStaircasePosition, const ZDirection& downStaircaseDirection);
    void CalcUpStaircases(const ZDungeonLevel* previousLevel = nullptr);
    void BlockStaircasePocketCells(path::ZWeightedMap* weightedMap, const ZDirectionalStaircase& staircase);
    void BlockUpStaircasesAndTheirPocketCells();
    static ZPosition CropPositionInsideLevel(const ZPosition& position);
    static bool TryToCalcRoomNearStaircase(const ZDirectionalStaircase& staircase, int minSize, int maxSize, ZDungeonLevel::ZRoom* room);
    void DigRoomsNearUpStaircases(const ZDungeonLevel* previousLevel);
    void DigUpStaircases();
    void ConnectUpStaircasesWithSomeValidCell(const ZPosition& someValidCell);
    void DigRandomDownStaircases();

    EDungeonCell::Type** mMap;
    path::ZWeightedMap* mWeightedMap;
    ZDungeonLevel::ZRoomList mRooms;
    ZDungeonLevel::ZRoomList mNextLevelStaircaseRooms;
    ZDirectionalStaircaseList mUpStaircases;
};

}
}
