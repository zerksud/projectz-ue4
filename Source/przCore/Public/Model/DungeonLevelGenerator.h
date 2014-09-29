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

typedef std::vector<SubDungeon> DungeonRooms;

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
    void DiggCellIfSolidAndNotBlocked(const ZPosition& position);
    void DiggCellIfSolidAndNotBlocked(int x, int y);
    // assignes infinite weight to cell with position
    void BlockCell(const ZPosition& position);
    // assignes infinite weight to cell [x, y]
    void BlockCell(int x, int y);
    bool CellIsBlocked(int x, int y) const;
    bool TryToCreateRoomInsideSubDungeon(SubDungeon* dungeon);
    // returns true if room was successfully digged
    bool DiggRoomIfAllCellsAreSolidAndNotBlocked(int minX, int minY, int maxX, int maxY);

    void ConnectCells(const ZPosition& someCell, const ZPosition& anotherCell);

    void DigRandomTunnels();
    int CountCellSolidNeighbours(const ZPosition& cell) const;
    void CalcUpStaircases(const ZDungeonLevel* previousLevel = nullptr);
    static ZPosition CropPositionInsideLevel(const ZPosition& position);
    void DiggRoomsNearUpStaircases();
    void DiggUpStaircases();
    void ConnectUpStaircasesWithSomeValidCell(const ZPosition& someValidCell);
    void AddRandomDownStaircases();

    EDungeonCell::Type** mMap;
    path::ZWeightedMap* mWeightedMap;
    DungeonRooms mRooms;
    ZDirectionalStaircaseList mUpStaircases;
};

}
}
