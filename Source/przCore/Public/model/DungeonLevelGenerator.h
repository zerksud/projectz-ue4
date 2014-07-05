#pragma once

#include <memory>
#include <vector>

#include "model/DungeonLevel.h"

namespace prz {
    namespace mdl {
        class ZDungeonLevelGenerator {
        public:
            ZDungeonLevelGenerator() = delete;
            virtual ~ZDungeonLevelGenerator() = delete;

            static ZDungeonLevel* GenerateLevel(const ZDungeonLevel::StaircaseList& upStaircases = ZDungeonLevel::StaircaseList());

        private:
            static ZDungeonLevel::ZMapCell* kDefaultLevelMap;
            static int kDefaultLevelWidth;
            static int kDefaultLevelHeight;
        };
    }
}
