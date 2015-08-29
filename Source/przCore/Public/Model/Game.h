#pragma once

#include "Model/IGame.h"
#include "Model/GameLog.h"

namespace prz {
namespace mdl {

class PRZCORE_API ZGame : public IGame {
public:
    ZGame();
    virtual ~ZGame();

    virtual void StartNewGame() override;

    virtual bool TryToMovePlayer(EDirection direction) override;
    virtual void TurnPlayer(EDirection direction) override;

    virtual const ZMinimap GetMinimap() override;
    virtual unsigned int GetMinimapSize() const override;

    virtual const utl::ZString& GetLogHistory() override;

private:
    ZDungeon* mDungeon;
    utl::ZIdType mPlayerId;

    ZGameLog mLogHistory;
};

}
}
