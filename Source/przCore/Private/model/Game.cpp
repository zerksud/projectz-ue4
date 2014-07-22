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

        const ZMinimap ZGame::GetMinimap() {
            EDungeonCell::Type* cells = new EDungeonCell::Type[kMinimapSize * kMinimapSize];

            unsigned int currentLevel = mDungeon.GetMonsterLevelIndex(mPlayerId);
            const ZDungeonLevel* level = mDungeon.GetLevel(currentLevel);

            const ZPosition* playerPosition = level->GetMonsterPosition(mPlayerId);
            int baseX = playerPosition->GetX();
            int baseY = playerPosition->GetY();

            for (int dx = -kMinimapRadius; dx <= kMinimapRadius; ++dx) {
                for (int dy = -kMinimapRadius; dy <= kMinimapRadius; ++dy) {
                    EDungeonCell::Type minimapCell = level->GetCellType(baseX + dx, baseY + dy);

                    int linearIndex = kMinimapRadius + dx + kMinimapSize * (kMinimapRadius + dy);
                    cells[linearIndex] = minimapCell;
                }
            }

            ZMinimap minimap(kMinimapSize, cells);
            delete cells;

            return minimap;
        }

    }
}
