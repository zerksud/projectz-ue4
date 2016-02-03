// Copyright 2016 Max Molodtsov
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

#include "Utils/StandardLibrary/ZUnorderedMap.h"
#include "Utils/StandardLibrary/ZVector.h"

#include "Model/IDungeonLevelGenerator.h"
#include "Model/ZDungeonLevelGenerator.h"

namespace prz {
namespace mdl {

class ZDungeon {
public:
    // ZDungeon owns its instance of IDungeonLevelGenerator
    ZDungeon(IDungeonLevelGenerator* levelGenerator = new ZDungeonLevelGenerator());
    virtual ~ZDungeon();

    const ZDungeonLevel* GetLevel(unsigned int levelIndex);

    bool PlaceMonster(ZMonster* monster, unsigned int levelIndex, const ZPosition& position);
    unsigned int GetMonsterLevelIndex(utl::ZIdType monsterId, bool* successFlag = nullptr) const;
    ZMonster* RemoveMonster(utl::ZIdType monsterId);

    // returns monster by it's id or nullptr if dungeon has no monster with such id
    ZMonster* GetMonster(utl::ZIdType monsterId);
    const ZPosition* GetMonsterPosition(utl::ZIdType monsterId) const;

    bool TryToMoveMonster(utl::ZIdType monsterId, EDirection direction, ZPositionDiff* OutExpectedMoveDiff = nullptr);

private:
    typedef utl::ZVector<ZDungeonLevel*> ZDungeonLevelList;
    typedef utl::ZUnorderedMap<utl::ZIdType, unsigned int> ZMonsterLevelMap;

    ZDungeonLevel* GetExistingLevelOrGenerateNew(unsigned int level);
    void GenerateAbsentLevels(unsigned int maxLevelIndex);

    ZDungeonLevelList mLevels;
    ZMonsterLevelMap mMonsterLevelMap;
    IDungeonLevelGenerator* mLevelGenerator;
};

}
}
