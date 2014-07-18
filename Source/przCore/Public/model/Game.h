#pragma once

#include "model/IGame.h"

namespace prz {
    namespace mdl {
        class PRZCORE_API ZGame : public IGame {
        public:
            ZGame();
            virtual ~ZGame() = default;

            virtual ZDungeon& GetDungeon() override;

            virtual utl::ZIdType GetPlayerId() const override;

        private:
            ZDungeon mDungeon;
            utl::ZIdType mPlayerId;
        };
    }
}
