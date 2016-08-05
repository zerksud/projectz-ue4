// Copyright 2016 Max Molodtsov <zerksud@gmail.com>
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include "Model/IGame.h"
#include "Model/ZGameLog.h"

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
