#pragma once

#include "Utils/StandartLibrary.h"

#include "Model/Minimap.h"
#include "Model/Dungeon.h"

namespace prz {
namespace mdl {

class IGame {
public:
    virtual ~IGame() = default;

    virtual void StartNewGame() = 0;

    virtual bool TryToMovePlayer(EDirection direction) = 0;
    virtual void TurnPlayer(EDirection direction) = 0;

    virtual const ZMinimap GetMinimap() = 0;
    virtual unsigned int GetMinimapSize() const = 0;

    virtual const utl::ZString& GetLogHistory() = 0;
};

}
}
