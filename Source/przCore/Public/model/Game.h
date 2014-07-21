#pragma once

#include "model/IGame.h"

namespace prz {
    namespace mdl {

        class PRZCORE_API ZGame : public IGame {
        public:
            ZGame();
            virtual ~ZGame() = default;

            virtual bool TryToMovePlayer(EMoveDirection::Type direction) override;
            virtual void TurnPlayer(ETurnDirection::Type direction) override;
            virtual const ZMinimap GetMinimap() override;

        private:
            static const int kMinimapRadius;
            static const int kMinimapSize;

            ZDungeon mDungeon;
            utl::ZIdType mPlayerId;
        };

    }
}
