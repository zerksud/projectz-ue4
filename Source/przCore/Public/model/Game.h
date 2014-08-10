#pragma once

#include "model/IGame.h"
#include "model/GameLog.h"

namespace prz {
    namespace mdl {

        class PRZCORE_API ZGame : public IGame {
        public:
            ZGame();
            virtual ~ZGame();

            virtual void StartNewGame() override;

            virtual bool TryToMovePlayer(EMoveDirection::Type direction) override;
            virtual void TurnPlayer(ETurnDirection::Type direction) override;

            virtual const ZMinimap GetMinimap() override;
            virtual unsigned int GetMinimapSize() const override;

            virtual const std::string& GetLogHistory() override;

        private:
            static const int kMinimapRadius;
            static const int kMinimapSize;

            static const int kLogHistoryMaxSize;

            ZDungeon* mDungeon;
            utl::ZIdType mPlayerId;

            ZGameLog mLogHistory;
        };

    }
}
