#pragma once

#include "model/Dungeon.h"

namespace prz {
    namespace mdl {
        class ZGame {
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
