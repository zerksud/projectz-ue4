#include "przCorePCH.h"
#include "Model/Game.h"

#include "Utils/LOG.h"
#include "Utils/MatrixHelpers.h"
#include "Utils/StringHelpers.h"

namespace prz {
namespace mdl {

const int ZGame::kMinimapRadius = 35;
const int ZGame::kMinimapSize = 1 + kMinimapRadius * 2;
const int ZGame::kLogHistoryMaxSize = 4;

ZGame::ZGame() {
    StartNewGame();
}

ZGame::~ZGame() {
    delete mDungeon;
}

void ZGame::StartNewGame() {
    mDungeon = new ZDungeon();
    const ZDungeonLevel* level = mDungeon->GetLevel(0);
    const ZPosition& startPosition = level->GetUpStaircases().front();
    ZMonster* player = ZMonster::CreateMonster();
    mPlayerId = player->GetId();
    mDungeon->PlaceMonster(player, 0, startPosition);
    mLogHistory.Clear();
    mLogHistory.Log("It's dark in here.");
}

bool ZGame::TryToMovePlayer(EMoveDirection direction) {
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

void ZGame::TurnPlayer(ETurnDirection direction) {
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
    utl::ZMatrix::Allocate(&cells, kMinimapSize);

    unsigned int currentLevel = mDungeon->GetMonsterLevelIndex(mPlayerId);
    const ZDungeonLevel* level = mDungeon->GetLevel(currentLevel);

    const ZPosition* playerPosition = level->GetMonsterPosition(mPlayerId);
    int baseX = playerPosition->GetX();
    int baseY = playerPosition->GetY();

    prz::mdl::ZMonster* player = mDungeon->GetMonster(mPlayerId);
    ZDirection minimapDirection = player->GetDirection();
    minimapDirection.Turn(ETurnDirection::Right);
    const ZPositionDiff minimapDirectionSinCos = minimapDirection.PredictMove();

    const ZFieldOfView& playerFOV = player->GetFieldOfView();
    int viewDistance = player->GetViewDistance();

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

const std::string& ZGame::GetLogHistory() {
    return mLogHistory.GetHistory();
}

}
}
