// Copyright 2016 Max Molodtsov <zerksud@gmail.com>
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include "Utils/StandardLibrary/ZUnorderedMap.h"
#include "Utils/StandardLibrary/ZVector.h"
#include "Utils/RandomHelpers.h"

#include "Model/ZFieldOfView.h"
#include "Model/ZPosition.h"
#include "Model/ZMonster.h"

namespace prz {
namespace mdl {

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
            int randomX = utl::random_helpers::GetRandomValue(minX, maxX);
            int randomY = utl::random_helpers::GetRandomValue(minY, maxY);

            return ZPosition(randomX, randomY);
        }
    };
    typedef utl::ZVector<ZRoom> ZRoomList;

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

    typedef utl::ZVector<ZPosition> StaircaseList;
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

    bool TryToMoveMonster(utl::ZIdType monsterId, EDirection direction, ZPositionDiff* OutExpectedMoveDiff = nullptr);

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

    typedef utl::ZUnorderedMap<int, utl::ZIdType> ZMonsterIdByPositionMap;
    typedef utl::ZUnorderedMap<utl::ZIdType, ZPlacedMonster*> ZMonsterList;

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
