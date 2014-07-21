#pragma once

#include "model/Minimap.h"
#include "model/Dungeon.h"

namespace prz {
    namespace mdl {

        class IGame {
        public:
            virtual bool TryToMovePlayer(EMoveDirection::Type direction) = 0;
            virtual const ZMinimap GetMinimap() = 0;
        };

    }
}
