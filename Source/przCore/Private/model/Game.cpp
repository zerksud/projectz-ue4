#include "przCorePrivatePCH.h"
#include "model/Game.h"

namespace prz {
    namespace mdl {

        ZGame::ZGame() {
            const ZDungeonLevel* level = mDungeon.GetLevel(0);
            const ZPosition& startPosition = level->GetUpStaircases().front();
            ZMonster* player = ZMonster::CreateMonster();
            mPlayerId = player->GetId();
            mDungeon.PlaceMonster(player, 0, startPosition);
        }

        ZDungeon& ZGame::GetDungeon() {
            return mDungeon;
        }

    }
}
