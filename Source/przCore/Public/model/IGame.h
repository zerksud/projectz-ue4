#pragma once

#include "model/Dungeon.h"

namespace prz {
    namespace mdl {
        class IGame {
        public:
            virtual ZDungeon& GetDungeon() = 0;
            virtual utl::ZIdType GetPlayerId() const = 0;
        };
    }
}
