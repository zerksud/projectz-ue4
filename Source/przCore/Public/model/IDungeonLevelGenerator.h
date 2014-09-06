#pragma once

#include "model/DungeonLevel.h"

namespace prz {
    namespace mdl {
        class IDungeonLevelGenerator {
        public:
            virtual ~IDungeonLevelGenerator() = default;
            virtual ZDungeonLevel* GenerateLevel(const ZDungeonLevel* previousLevel = nullptr) = 0;
        };
    }
}
