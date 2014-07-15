#pragma once

#include <memory>
#include <vector>

#include "model/IDungeonLevelGenerator.h"

namespace prz {
    namespace mdl {
        class ZDungeonLevelGenerator : public IDungeonLevelGenerator {
        public:
            ZDungeonLevelGenerator() = default;
            virtual ~ZDungeonLevelGenerator() = default;

            virtual ZDungeonLevel* GenerateLevel(const ZDungeonLevel::StaircaseList& upStaircases = ZDungeonLevel::StaircaseList()) override;

        private:
            static const ZDungeonLevel::ZMapCell* kDefaultLevelMap;
            static int kDefaultLevelWidth;
            static int kDefaultLevelHeight;
        };
    }
}
