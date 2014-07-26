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
            virtual unsigned int GetMinimapSize() const override;

            virtual const LogMessages& GetLogHistory() const override;

        private:
            static const int kMinimapRadius;
            static const int kMinimapSize;

            static const int kLogHistoryMaxSize;

            void AddLogMessage(const std::string& message);

            ZDungeon mDungeon;
            utl::ZIdType mPlayerId;

            LogMessages mLogHistory;
        };

    }
}
