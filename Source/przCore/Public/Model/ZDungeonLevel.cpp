// Copyright 2014-2017 Max Molodtsov <zerksud@gmail.com>
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

#include "przCorePCH.h"
#include "Model/ZDungeonLevel.h"

#include "Utils/LOG.h"
#include "Utils/MatrixHelpers.h"

#include "Model/ZFieldOfViewBuilder.h"

namespace prz {
namespace mdl {

void ZDungeonLevel::CreateFailSafeDungeon() {
    mWidth = 1;
    mHeight = 1;

    EDungeonCell** map;
    utl::matrix_helpers::Allocate(&map, 1);
    map[0][0] = EDungeonCell::DownStaircase;

    ParseMap(&map);
}

ZDungeonLevel::ZDungeonLevel(int width, int height, EDungeonCell*** map, const ZRoomList& rooms, const ZRoomList& nextLevelStaircaseRooms)
    : mWidth(width),
    mHeight(height),
    mRooms(rooms),
    mNextLevelStaircaseRooms(nextLevelStaircaseRooms) {
    if (width <= 0 || height <= 0) {
        LOGE("Can't create dungeon with size %dx%d", width, height);
        CreateFailSafeDungeon();
        return;
    }

    ParseMap(map);
}

void ZDungeonLevel::ParseMap(EDungeonCell*** map) {
    mTerrain = *map;

    for (int x = 0; x < mWidth; ++x) {
        for (int y = 0; y < mHeight; ++y) {
            EDungeonCell mapCell = mTerrain[x][y];

            if (mapCell == EDungeonCell::UpStaircase) {
                mUpStaircases.push_back(ZPosition(x, y));
            } else if (mapCell == EDungeonCell::DownStaircase) {
                mDownStaircases.push_back(ZPosition(x, y));
            }
        }
    }
}

ZDungeonLevel::~ZDungeonLevel() {
    for (auto& pair : mMonsterList) {
        delete pair.second;
    }

    utl::matrix_helpers::Deallocate<EDungeonCell>(&mTerrain, mHeight);
}

int ZDungeonLevel::GetWidth() const {
    return mWidth;
}

int ZDungeonLevel::GetHeight() const {
    return mHeight;
}

bool ZDungeonLevel::CellIsEmpty(int x, int y) const {
    if (CellIndicesAreValid(x, y)) {
        return CellIsEmptyImpl(x, y);
    }

    return false;
}

bool ZDungeonLevel::CellIsEmpty(const ZPosition& pos) const {
    return CellIsEmpty(pos.GetX(), pos.GetY());
}

bool ZDungeonLevel::CellIsSolid(int x, int y) const {
    if (CellIndicesAreValid(x, y)) {
        return CellIsSolidImpl(x, y);
    }

    return true;
}

bool ZDungeonLevel::CellIsSolid(const ZPosition& pos) const {
    return CellIsSolid(pos.GetX(), pos.GetY());
}

const ZDungeonLevel::StaircaseList& ZDungeonLevel::GetUpStaircases() const {
    return mUpStaircases;
}

const ZDungeonLevel::StaircaseList& ZDungeonLevel::GetDownStaircases() const {
    return mDownStaircases;
}

const ZDirection ZDungeonLevel::GetStaircaseDirection(const ZPosition& position) const {
    ZDirection direction;

    if (!CellIsSolid(position.GetX() + 1, position.GetY())) {
        direction.Turn(EDirection::Back);
    } else if (!CellIsSolid(position.GetX(), position.GetY() - 1)) {
        direction.Turn(EDirection::Right);
    } else if (!CellIsSolid(position.GetX(), position.GetY() + 1)) {
        direction.Turn(EDirection::Left);
    }

    return direction;
}

const ZDungeonLevel::ZRoomList& ZDungeonLevel::GetNextLevelStaircaseRooms() const {
    return mNextLevelStaircaseRooms;
}

EDungeonCell ZDungeonLevel::GetCellType(int x, int y) const {
    if (CellIndicesAreValid(x, y)) {
        return GetCellTypeImpl(x, y);
    }

    return EDungeonCell::SolidRock;
}

EDungeonCell ZDungeonLevel::GetCellType(const ZPosition& pos) const {
    return GetCellType(pos.GetX(), pos.GetY());
}

EDungeonCell ZDungeonLevel::GetCellTypeImpl(int x, int y) const {
    if (GetPlacedMonster(x, y) != nullptr) {
        return EDungeonCell::Monster;
    }

    EDungeonCell cellType = mTerrain[x][y];

    return cellType;
}

bool ZDungeonLevel::PlaceMonster(ZMonster* monster, const ZPosition& position) {
    if (!monster) {
        LOGE("Placed monster can't be nullptr");
        return false;
    }

    if (!monster->IsRegistered()) {
        LOGE("Can't place non-registered monster");
        return false;
    }

    if (CellIsSolid(position)) {
        LOGE("Can't place monster in solid cell %s", position.ToString().c_str());
        return false;
    }

    auto pos = mMonsterList.find(monster->GetId());
    if (pos != mMonsterList.end()) {
        LOGE("Can't place already placed monster with id = %d", monster->GetId());
        return false;
    }

    int linearPositionIndex = CalcCellLinearIndex(position);
    auto positionMonsterLink = mMonsterIdByPosition.find(linearPositionIndex);
    if (positionMonsterLink != mMonsterIdByPosition.end()) {
        LOGE("Can't place monster in already occupied cell %s", position.ToString().c_str());
        return false;
    }

    mMonsterList[monster->GetId()] = new ZPlacedMonster(monster, position);
    mMonsterIdByPosition[linearPositionIndex] = monster->GetId();

    return true;
}

ZMonster* ZDungeonLevel::RemoveMonster(utl::ZIdType monsterId) {
    auto pos = mMonsterList.find(monsterId);
    if (pos == mMonsterList.end()) {
        LOGE("Can't remove not added monster with id = %d", monsterId);
        return nullptr;
    }

    ZPlacedMonster* placedMonster = pos->second;
    ZMonster* monster = placedMonster->monster;
    placedMonster->monster = nullptr;

    int linearPositionIndex = CalcCellLinearIndex(placedMonster->position);
    mMonsterIdByPosition.erase(linearPositionIndex);
    mMonsterList.erase(pos);

    delete placedMonster;

    return monster;
}

const ZPosition* ZDungeonLevel::GetMonsterPosition(utl::ZIdType monsterId) const {
    auto pos = mMonsterList.find(monsterId);
    if (pos == mMonsterList.end()) {
        LOGE("Can't return position of not-placed monster with id = %d", monsterId);
        return nullptr;
    }

    return &pos->second->position;
}

ZMonster* ZDungeonLevel::GetMonster(utl::ZIdType monsterId) {
    ZPlacedMonster* placedMonster = GetPlacedMonster(monsterId);
    if (placedMonster == nullptr) {
        LOGE("Can't return not-placed monster with id = %d", monsterId);
        return nullptr;
    }

    return placedMonster->monster;
}

ZMonster* ZDungeonLevel::GetMonster(const ZPosition& position) {
    return GetMonster(position.GetX(), position.GetY());
}

ZMonster* ZDungeonLevel::GetMonster(int x, int y) {
    if (CellIndicesAreValid(x, y)) {
        ZPlacedMonster* placedMonster = GetPlacedMonster(x, y);
        if (placedMonster) {
            return placedMonster->monster;
        }
    }

    return nullptr;
}

bool ZDungeonLevel::UpdateFieldOfView(utl::ZIdType monsterId) {
    ZPlacedMonster* placedMonster = GetPlacedMonster(monsterId);
    if (placedMonster == nullptr) {
        LOGE("Can't update field of view for not-placed monster with id = %d", monsterId);
        return false;
    }

    ZPosition position = placedMonster->position;
    unsigned int viewDistance = placedMonster->monster->GetViewDistance();
    placedMonster->monster->UpdateFieldOfView(ZFieldOfViewBuilder::Build(position, viewDistance, *this));

    return true;
}

ZDungeonLevel::ZPlacedMonster* ZDungeonLevel::GetPlacedMonster(utl::ZIdType monsterId) {
    auto pos = mMonsterList.find(monsterId);
    if (pos == mMonsterList.end()) {
        LOGE("Can't return not-placed monster with id = %d", monsterId);
        return nullptr;
    }

    return pos->second;
}

ZDungeonLevel::ZPlacedMonster* ZDungeonLevel::GetPlacedMonster(int x, int y) {
    return const_cast<ZPlacedMonster*>(static_cast<const ZDungeonLevel*>(this)->GetPlacedMonster(x, y));
}

const ZDungeonLevel::ZPlacedMonster* ZDungeonLevel::GetPlacedMonster(int x, int y) const {
    int linearIndex = CalcCellLinearIndex(x, y);
    auto pos = mMonsterIdByPosition.find(linearIndex);
    if (pos != mMonsterIdByPosition.end()) {
        return mMonsterList.find(pos->second)->second;
    }

    return nullptr;
}

bool ZDungeonLevel::MovementIsDiagonalAroundTheCorner(const ZPosition& origin, const ZPositionDiff& diff) const {
    int sum = abs(diff.GetdX()) + abs(diff.GetdY());
    if (sum <= 1) {
        return false;
    }

    return CellIsSolidImpl(origin.GetX(), origin.GetY() + diff.GetdY()) ||
        CellIsSolidImpl(origin.GetX() + diff.GetdX(), origin.GetY());
}

bool ZDungeonLevel::TryToMoveMonster(utl::ZIdType monsterId, EDirection direction, ZPositionDiff* OutExpectedMoveDiff) {
    ZPlacedMonster* placedMonster = GetPlacedMonster(monsterId);
    if (placedMonster == nullptr) {
        LOGE("Can't move not-placed monster with id = %d", monsterId);
        return false;
    }

    ZDirection alignedDirection = placedMonster->monster->GetDirection();
    alignedDirection.Turn(direction);

    ZPositionDiff expectedDiff = alignedDirection.PredictMove();
    ZPosition expectedPosition = placedMonster->position + expectedDiff;

    bool movementIsPossible = CellIsEmptyImpl(expectedPosition.GetX(), expectedPosition.GetY()) &&
        !MovementIsDiagonalAroundTheCorner(placedMonster->position, expectedDiff);

    if (movementIsPossible) {
        int oldLinearIndex = CalcCellLinearIndex(placedMonster->position);
        mMonsterIdByPosition.erase(oldLinearIndex);

        placedMonster->position = expectedPosition;

        int newLinearIndex = CalcCellLinearIndex(expectedPosition);
        mMonsterIdByPosition[newLinearIndex] = placedMonster->monster->GetId();
    }

    if (OutExpectedMoveDiff != nullptr) {
        *OutExpectedMoveDiff = expectedDiff;
    }

    return movementIsPossible;
}

bool ZDungeonLevel::CellIndicesAreValid(int x, int y) const {
    return (x >= 0 && x < mWidth && y >= 0 && y < mHeight);
}

int ZDungeonLevel::CalcCellLinearIndex(int x, int y) const {
    return (y * mWidth + x);
}

int ZDungeonLevel::CalcCellLinearIndex(const ZPosition& position) const {
    return CalcCellLinearIndex(position.GetX(), position.GetY());
}

bool ZDungeonLevel::CellIsSolidImpl(int x, int y) const {
    bool solid = mTerrain[x][y] == EDungeonCell::SolidRock;

    return solid;
}

bool ZDungeonLevel::CellIsEmptyImpl(int x, int y) const {
    bool empty = !CellIsSolidImpl(x, y);

    if (empty) {
        int linearIndex = CalcCellLinearIndex(x, y);
        auto pos = mMonsterIdByPosition.find(linearIndex);
        if (pos != mMonsterIdByPosition.end()) {
            empty = false;
        }
    }

    return empty;
}

}
}
