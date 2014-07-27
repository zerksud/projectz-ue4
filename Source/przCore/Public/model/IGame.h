#pragma once

#include "model/Minimap.h"
#include "model/Dungeon.h"

namespace prz {
    namespace mdl {

        class IGame {
        public:
            virtual ~IGame() = default;

            virtual bool TryToMovePlayer(EMoveDirection::Type direction) = 0;
            virtual void TurnPlayer(ETurnDirection::Type direction) = 0;

            virtual const ZMinimap GetMinimap() = 0;
            virtual unsigned int GetMinimapSize() const = 0;

            typedef std::list<std::string> LogMessages;
            virtual const LogMessages& GetLogHistory() const = 0;
        };

    }
}
