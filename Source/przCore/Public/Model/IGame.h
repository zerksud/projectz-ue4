#pragma once

#include "Model/Minimap.h"
#include "Model/Dungeon.h"

namespace prz {
    namespace mdl {

        class IGame {
        public:
            virtual ~IGame() = default;

            virtual void StartNewGame() = 0;

            virtual bool TryToMovePlayer(EMoveDirection::Type direction) = 0;
            virtual void TurnPlayer(ETurnDirection::Type direction) = 0;

            virtual const ZMinimap GetMinimap() = 0;
            virtual unsigned int GetMinimapSize() const = 0;

            virtual const std::string& GetLogHistory() = 0;
        };

    }
}
