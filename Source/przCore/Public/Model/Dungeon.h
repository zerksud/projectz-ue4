#pragma once

#include "Utils/StandartLibrary.h"

#include "Model/IDungeonLevelGenerator.h"
#include "Model/DungeonLevelGenerator.h"

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
