#pragma once

#include "Utils/StandardLibrary/Vector.h"

#include "Model/IDungeonLevelGenerator.h"
#include "Model/Path/WeightedCell.h"
#include "Model/Path/PathFinder.h"

namespace prz {
namespace mdl {

struct BSPTreeNode;
struct SubDungeon;
struct PathCellConnection;

class ZDungeonLevelGenerator : public IDungeonLevelGenerator {
public:
    ZDungeonLevelGenerator() = default;
    virtual ~ZDungeonLevelGenerator() = default;

    virtual ZDungeonLevel* GenerateLevel(const ZDungeonLevel* previousLevel = nullptr) override;

private:
    struct ZDirectionalStaircase {
        ZPosition position;
        ZDirection direction;

        ZDirectionalStaircase(const ZPosition& pPosition, const ZDirection& pDirection)
            : position(pPosition), direction(pDirection) {
        }
    };
    typedef utl::ZVector<ZDirectionalStaircase> ZDirectionalStaircaseList;

    bool TryToGenerateBSPTree(BSPTreeNode* rootNode, bool tryToSplitVertically = true);
    void SplitSubDungeonVertically(BSPTreeNode* rootNode);
    void SplitSubDungeonHorizontally(BSPTreeNode* rootNode);
    static void DigCellIfSolidAndNotBlocked(EDungeonCell** map, path::ZWeightedMap* weightedMap, const ZPosition& position);
    static void DigCellIfSolidAndNotBlocked(EDungeonCell** map, path::ZWeightedMap* weightedMap, int x, int y);
    bool TryToCreateRoomInsideSubDungeon(SubDungeon* dungeon);
    static bool CellIsSolidRock(EDungeonCell** map, int x, int y);
    static bool CellIsSolidRock(EDungeonCell** map, const ZPosition& position);

    // returns true if room was successfully digged
    static bool DigRoomIfAllCellsAreSolidAndNotBlocked(EDungeonCell** map, path::ZWeightedMap* weightedMap, const ZDungeonLevel::ZRoom& room);

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

    EDungeonCell** mMap;
    path::ZWeightedMap* mWeightedMap;
    ZDungeonLevel::ZRoomList mRooms;
    ZDungeonLevel::ZRoomList mNextLevelStaircaseRooms;
    ZDirectionalStaircaseList mUpStaircases;
};

}
}
