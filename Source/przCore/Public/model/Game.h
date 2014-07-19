#pragma once

#include "model/IGame.h"
#include "model/Dungeon.h"

namespace prz {
    namespace mdl {
        class PRZCORE_API ZGame : public IGame {
        public:
            ZGame();
            virtual ~ZGame() = default;

            ZDungeon& GetDungeon();

            utl::ZIdType GetPlayerId() const;

        private:
            ZDungeon mDungeon;
            utl::ZIdType mPlayerId;
        };
    }
}
