#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "model/DungeonLevel.h"

namespace prz {
    namespace mdl {
        class ZDungeon {
        public:
            ZDungeon() = default;
            virtual ~ZDungeon() = default;

            bool PlaceMonster(ZMonster* monster, unsigned int levelIndex, const ZPosition& position);
            ZMonster* RemoveMonster(utl::ZIdType monsterId);

            // returns monster by it's id or nullptr if dungeon has no monster with such id
            ZMonster* GetMonster(utl::ZIdType monsterId);

            bool TryToMoveMonster(utl::ZIdType monsterId, EMoveDirection::Type direction, ZPositionDiff* OutExpectedMoveDiff = nullptr);

        private:
            typedef std::vector<std::unique_ptr<ZDungeonLevel> > ZDungeonLevelList;
            typedef std::unordered_map<utl::ZIdType, unsigned int> ZMonsterLevelMap;

            ZDungeonLevel* GetLevel(unsigned int level);
            void GenerateAbsentLevels(unsigned int maxLevelIndex);

            ZDungeonLevelList mLevels;
            ZMonsterLevelMap mMonsterLevelMap;
        };
    }
}
