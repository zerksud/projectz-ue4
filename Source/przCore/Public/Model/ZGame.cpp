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

#include "przCorePCH.h"
#include "Model/ZGame.h"

#include "Utils/LOG.h"
#include "Utils/MatrixHelpers.h"
#include "Utils/StringHelpers.h"

namespace prz {
namespace mdl {

static const int kMinimapRadius = 14;
static const int kMinimapSize = 1 + kMinimapRadius * 2;
static const int kLogHistoryMaxSize = 4;

ZGame::ZGame() {
    StartNewGame();
}

ZGame::~ZGame() {
    delete mDungeon;
}

void ZGame::StartNewGame() {
    mDungeon = new ZDungeon();
    const ZDungeonLevel* level = mDungeon->GetLevel(0);
    
    ZMonster* player = ZMonster::CreateMonster();
    mPlayerId = player->GetId();
    
    const ZPosition& startPosition = level->GetUpStaircases().front();
    player->GetDirection() = level->GetStaircaseDirection(startPosition).TurnCopy(EDirection::Back);
    
    mDungeon->PlaceMonster(player, 0, startPosition + player->GetDirection().PredictMove());
    
    mLogHistory.Clear();
    mLogHistory.Log("It's dark in here.");
}

bool ZGame::TryToMovePlayer(EDirection direction) {
    unsigned int levelIndexBeforeMove = mDungeon->GetMonsterLevelIndex(mPlayerId);
    bool success = mDungeon->TryToMoveMonster(mPlayerId, direction);
    if (success) {
        unsigned int levelIndexAfterMove = mDungeon->GetMonsterLevelIndex(mPlayerId);
        const ZPosition* playerPosition = mDungeon->GetLevel(levelIndexAfterMove)->GetMonsterPosition(mPlayerId);
        mLogHistory.Log("You moved to [%d;%d].", playerPosition->GetX(), playerPosition->GetY());

        if (levelIndexBeforeMove != levelIndexAfterMove) {
            mLogHistory.Log("You are now at level %d", levelIndexAfterMove);
        }
    } else {
        mLogHistory.Log("You can't move there.");
    }

    return success;
}

void ZGame::TurnPlayer(EDirection direction) {
    mDungeon->GetMonster(mPlayerId)->GetDirection().Turn(direction);
}

ZPosition GetRotatedPosition(const ZPositionDiff& sinCosOfReversedAngle, int x, int y) {
    int cos = sinCosOfReversedAngle.GetdX();
    int sin = -sinCosOfReversedAngle.GetdY();

    int rotatedX = cos * x - sin * y;
    int rotatedY = sin * x + cos * y;

    return ZPosition(rotatedX, rotatedY);
}

const ZMinimap ZGame::GetMinimap() {
    EDungeonCell** cells;
    utl::matrix_helpers::Allocate(&cells, kMinimapSize);

    prz::mdl::ZMonster* player = mDungeon->GetMonster(mPlayerId);
    ZDirection minimapDirection = player->GetDirection();
    minimapDirection.Turn(EDirection::Right);
    const ZPositionDiff minimapDirectionSinCos = minimapDirection.PredictMove();

    const ZFieldOfView& playerFOV = player->GetFieldOfView();

    for (int dx = -kMinimapRadius; dx <= kMinimapRadius; ++dx) {
        for (int dy = -kMinimapRadius; dy <= kMinimapRadius; ++dy) {
            EDungeonCell minimapCell = playerFOV.GetCell(dx, dy);

            ZPosition rotatedCell = GetRotatedPosition(minimapDirectionSinCos, dx, dy);
            int x = kMinimapRadius + rotatedCell.GetX();
            int y = kMinimapRadius + rotatedCell.GetY();
            cells[x][y] = minimapCell;
        }
    }

    ZMinimap minimap(kMinimapSize, &cells);

    return minimap;
}

unsigned int ZGame::GetMinimapSize() const {
    return kMinimapSize;
}

const utl::ZString& ZGame::GetLogHistory() {
    return mLogHistory.GetHistory();
}

}
}
