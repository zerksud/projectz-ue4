#pragma once

#include <unordered_map>
#include <vector>

#include "Model/FieldOfView.h"
#include "Model/Position.h"
#include "Model/Monster.h"
#include "Utils/RandomHelpers.h"

namespace prz {
namespace mdl {

enum class EMoveDirection {
    Forward,
    Backward,
    Left,
    Right
};

struct ZBounds {
    int minX;
    int minY;
    int maxX;
    int maxY;

    ZBounds(int pMinX, int pMinY, int pMaxX, int pMaxY)
        : minX(pMinX),
        minY(pMinY),
        maxX(pMaxX),
        maxY(pMaxY) {
    }
};

class ZDungeonLevel {
public:
    struct ZRoom : public ZBounds {
        ZPosition center;

        ZRoom(int pMinX, int pMinY, int pMaxX, int pMaxY)
            : ZBounds(pMinX, pMinY, pMaxX, pMaxY),
            center((pMinX + pMaxX) / 2, (pMinY + pMaxY) / 2) {
        }

        const ZPosition GetRandomCell() const {
            int randomX = utl::ZRandomHelpers::GetRandomValue(minX, maxX);
            int randomY = utl::ZRandomHelpers::GetRandomValue(minY, maxY);

            return ZPosition(randomX, randomY);
        }
    };
    typedef std::vector<ZRoom> ZRoomList;

    // holds map matrix
    ZDungeonLevel(int width, int height, EDungeonCell*** map, const ZRoomList& rooms = ZRoomList(), const ZRoomList& nextLevelStaircaseRooms = ZRoomList());
    ZDungeonLevel(const ZDungeonLevel& other) = delete;
    ZDungeonLevel& operator=(const ZDungeonLevel& other) = delete;
    virtual ~ZDungeonLevel();

    int GetWidth() const;
    int GetHeight() const;

    bool CellIsEmpty(int x, int y) const;
    bool CellIsEmpty(const ZPosition& pos) const;

    bool CellIsSolid(int x, int y) const;
    bool CellIsSolid(const ZPosition& pos) const;

    typedef std::vector<ZPosition> StaircaseList;
    const StaircaseList& GetUpStaircases() const;
    const StaircaseList& GetDownStaircases() const;

    const ZDirection GetStaircaseDirection(const ZPosition& position) const;

    const ZRoomList& GetNextLevelStaircaseRooms() const;

    EDungeonCell GetCellType(int x, int y) const;
    EDungeonCell GetCellType(const ZPosition& pos) const;

    bool PlaceMonster(ZMonster* monster, const ZPosition& position);
    ZMonster* RemoveMonster(utl::ZIdType monsterId);

    // returns monster's position by it's id or nullptr if this level has no monster with such id
    const ZPosition* GetMonsterPosition(utl::ZIdType monsterId) const;

    // returns monster by it's id or nullptr if this level has no monster with such id
    ZMonster* GetMonster(utl::ZIdType monsterId);

    ZMonster* GetMonster(const ZPosition& position);
    ZMonster* GetMonster(int x, int y);

    bool UpdateFieldOfView(utl::ZIdType monsterId);

    bool TryToMoveMonster(utl::ZIdType monsterId, EMoveDirection direction, ZPositionDiff* OutExpectedMoveDiff = nullptr);

private:
    void CreateFailSafeDungeon();

    void ParseMap(EDungeonCell*** map);
    bool CellIndicesAreValid(int x, int y) const;

    int CalcCellLinearIndex(int x, int y) const;
    int CalcCellLinearIndex(const ZPosition& position) const;

    bool CellIsSolidImpl(int x, int y) const;
    bool CellIsEmptyImpl(int x, int y) const;

    EDungeonCell GetCellTypeImpl(int x, int y) const;

    struct ZPlacedMonster {
        ZMonster* monster;
        ZPosition position;

        ZPlacedMonster(ZMonster* pMonster, const ZPosition& pPosition) :
            monster(pMonster), position(pPosition) {
        }

        ~ZPlacedMonster() {
            ZMonster::DestroyMonster(&monster);
        }
    };

    bool MovementIsDiagonalAroundTheCorner(const ZPosition& origin, const ZPositionDiff& diff) const;

    typedef std::unordered_map<int, utl::ZIdType> ZMonsterIdByPositionMap;
    typedef std::unordered_map<utl::ZIdType, ZPlacedMonster*> ZMonsterList;

    ZPlacedMonster* GetPlacedMonster(utl::ZIdType monsterId);
    ZPlacedMonster* GetPlacedMonster(int x, int y);
    const ZPlacedMonster* GetPlacedMonster(int x, int y) const;

    int mWidth;
    int mHeight;
    EDungeonCell** mTerrain;
    ZRoomList mRooms;
    ZRoomList mNextLevelStaircaseRooms;

    StaircaseList mUpStaircases;
    StaircaseList mDownStaircases;

    ZMonsterIdByPositionMap mMonsterIdByPosition;
    ZMonsterList mMonsterList;
};

}
}
