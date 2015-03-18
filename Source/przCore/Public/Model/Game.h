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

    virtual bool TryToMovePlayer(EMoveDirection direction) override;
    virtual void TurnPlayer(ETurnDirection direction) override;

    virtual const ZMinimap GetMinimap() override;
    virtual unsigned int GetMinimapSize() const override;

    virtual const std::string& GetLogHistory() override;

private:
    ZDungeon* mDungeon;
    utl::ZIdType mPlayerId;

    ZGameLog mLogHistory;
};

}
}
