#include "przCorePrivatePCH.h"
#include "model/Game.h"

#include "utils/LOG_ANSI.h"

namespace prz {
    namespace mdl {

        const int ZGame::kMinimapRadius = 14;
        const int ZGame::kMinimapSize = 1 + kMinimapRadius * 2;

        ZGame::ZGame() {
            const ZDungeonLevel* level = mDungeon.GetLevel(0);
            const ZPosition& startPosition = level->GetUpStaircases().front();
            ZMonster* player = ZMonster::CreateMonster();
            mPlayerId = player->GetId();
            mDungeon.PlaceMonster(player, 0, startPosition);
        }

        bool ZGame::TryToMovePlayer(EMoveDirection::Type direction) {
            return mDungeon.TryToMoveMonster(mPlayerId, direction);
        }

        void ZGame::TurnPlayer(ETurnDirection::Type direction) {
            mDungeon.GetMonster(mPlayerId)->GetDirection().Turn(direction);
        }

        ZPosition GetRotatedPosition(const ZPositionDiff& sinCosOfReversedAngle, int x, int y) {
            int cos = sinCosOfReversedAngle.GetdX();
            int sin = -sinCosOfReversedAngle.GetdY();

            int rotatedX = cos * x - sin * y;
            int rotatedY = sin * x + cos * y;

            return ZPosition(rotatedX, rotatedY);
        }

        const ZMinimap ZGame::GetMinimap() {
            EDungeonCell::Type* cells = new EDungeonCell::Type[kMinimapSize * kMinimapSize];

            unsigned int currentLevel = mDungeon.GetMonsterLevelIndex(mPlayerId);
            const ZDungeonLevel* level = mDungeon.GetLevel(currentLevel);

            const ZPosition* playerPosition = level->GetMonsterPosition(mPlayerId);
            int baseX = playerPosition->GetX();
            int baseY = playerPosition->GetY();

            ZDirection minimapDirection = mDungeon.GetMonster(mPlayerId)->GetDirection();
            minimapDirection.Turn(ETurnDirection::Right);
            const ZPositionDiff minimapDirectionSinCos = minimapDirection.PredictMove();

            for (int dx = -kMinimapRadius; dx <= kMinimapRadius; ++dx) {
                for (int dy = -kMinimapRadius; dy <= kMinimapRadius; ++dy) {
                    EDungeonCell::Type minimapCell = level->GetCellType(baseX + dx, baseY + dy);

                    ZPosition rotatedCell = GetRotatedPosition(minimapDirectionSinCos, dx, dy);
                    int linearIndex = kMinimapRadius + rotatedCell.GetX() + kMinimapSize * (kMinimapRadius + rotatedCell.GetY());
                    cells[linearIndex] = minimapCell;
                }
            }

            ZMinimap minimap(kMinimapSize, cells);
            delete cells;

            return minimap;
        }

        unsigned int ZGame::GetMinimapSize() const {
            return kMinimapSize;
        }

    }
}
