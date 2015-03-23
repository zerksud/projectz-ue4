#include "przCorePCH.h"
#include "Model/Dungeon.h"

#include "Model/DungeonLevelGenerator.h"
#include "Utils/VectorHelpers.h"
#include "Utils/LOG.h"

namespace prz {
namespace mdl {

ZDungeon::ZDungeon(IDungeonLevelGenerator* levelGenerator) {
    if (levelGenerator) {
        mLevelGenerator = levelGenerator;
    } else {
        LOGE("Can't use nullptr as levelGenerator. Creating default one.");
        mLevelGenerator = new ZDungeonLevelGenerator();
    }
}

ZDungeon::~ZDungeon() {
    for (ZDungeonLevel* level : mLevels) {
        delete level;
    }
    delete mLevelGenerator;
}

const ZDungeonLevel* ZDungeon::GetLevel(unsigned int levelIndex) {
    return GetExistingLevelOrGenerateNew(levelIndex);
}

bool ZDungeon::PlaceMonster(ZMonster* monster, unsigned int levelIndex, const ZPosition& position) {
    ZDungeonLevel* level = GetExistingLevelOrGenerateNew(levelIndex);
    bool success = level->PlaceMonster(monster, position);
    if (success) {
        mMonsterLevelMap[monster->GetId()] = levelIndex;
        level->UpdateFieldOfView(monster->GetId());     // TODO: move FOV update to end of turn; update all FOV's together or on request
    }

    return success;
}

unsigned int ZDungeon::GetMonsterLevelIndex(utl::ZIdType monsterId, bool* successFlag) const {
    bool success = false;
    unsigned int index = 0;

    auto pos = mMonsterLevelMap.find(monsterId);
    if (pos != mMonsterLevelMap.end()) {
        success = true;
        index = pos->second;
    } else {
        LOGE("Can't get level index for not-placed monster with id = %d", monsterId);
    }

    if (successFlag) {
        *successFlag = success;
    }

    return index;
}

ZMonster* ZDungeon::RemoveMonster(utl::ZIdType monsterId) {
    auto pos = mMonsterLevelMap.find(monsterId);
    if (pos == mMonsterLevelMap.end()) {
        LOGE("Can't remove not added monster with id = %d", monsterId);
        return nullptr;
    }

    ZMonster* removedMonster = mLevels[pos->second]->RemoveMonster(monsterId);
    mMonsterLevelMap.erase(pos);

    return removedMonster;
}

ZMonster* ZDungeon::GetMonster(utl::ZIdType monsterId) {
    auto pos = mMonsterLevelMap.find(monsterId);
    if (pos == mMonsterLevelMap.end()) {
        LOGE("Can't return not-placed monster with id = %d", monsterId);
        return nullptr;
    }

    ZMonster* monster = mLevels[pos->second]->GetMonster(monsterId);

    return monster;
}

const ZPosition* ZDungeon::GetMonsterPosition(utl::ZIdType monsterId) const {
    auto pos = mMonsterLevelMap.find(monsterId);
    if (pos == mMonsterLevelMap.end()) {
        LOGE("Can't get position of not-placed monster with id = %d", monsterId);
        return nullptr;
    }

    const ZPosition* position = mLevels[pos->second]->GetMonsterPosition(monsterId);

    return position;
}

bool ZDungeon::TryToMoveMonster(utl::ZIdType monsterId, EDirection direction, ZPositionDiff* OutExpectedMoveDiff) {
    auto monsterLevelIterator = mMonsterLevelMap.find(monsterId);
    if (monsterLevelIterator == mMonsterLevelMap.end()) {
        LOGE("Can't move not-placed monster with id = %d", monsterId);
        return false;
    }

    unsigned int monsterLevelIndex = monsterLevelIterator->second;

    bool moveIsSuccessful = mLevels[monsterLevelIndex]->TryToMoveMonster(monsterId, direction, OutExpectedMoveDiff);
    if (moveIsSuccessful) {
        ZDungeonLevel* nextLevel = nullptr;
        const ZPosition* monsterPosition = mLevels[monsterLevelIndex]->GetMonsterPosition(monsterId);
        unsigned int nextLevelIndex = 0;
        if (utl::VectorContains(mLevels[monsterLevelIndex]->GetDownStaircases(), *monsterPosition)) {
            nextLevelIndex = monsterLevelIndex + 1;
            nextLevel = GetExistingLevelOrGenerateNew(nextLevelIndex);
        } else if (utl::VectorContains(mLevels[monsterLevelIndex]->GetUpStaircases(), *monsterPosition)) {
            if (monsterLevelIndex > 0) {
                nextLevelIndex = monsterLevelIndex - 1;
                nextLevel = mLevels[nextLevelIndex];
            } else {
                return false;   // monster can't leave dungeon
            }
        }

        if (nextLevel) {
            ZPositionDiff staircaseDirectionMove = mLevels[monsterLevelIndex]->GetStaircaseDirection(*monsterPosition).PredictMove();
            ZPosition nextLevelCellPosition = *monsterPosition + staircaseDirectionMove + staircaseDirectionMove;
            bool targetCellIsEmpty = nextLevel->CellIsEmpty(nextLevelCellPosition);

            if (targetCellIsEmpty) {
                ZMonster* monster = mLevels[monsterLevelIndex]->RemoveMonster(monsterId);
                nextLevel->PlaceMonster(monster, nextLevelCellPosition);

                monsterLevelIterator->second = nextLevelIndex;
            } else {
                moveIsSuccessful = false;
            }
        }

        mLevels[monsterLevelIterator->second]->UpdateFieldOfView(monsterId);    // TODO: move FOV update to end of turn; update all FOV's together or on request
    }

    return moveIsSuccessful;
}

ZDungeonLevel* ZDungeon::GetExistingLevelOrGenerateNew(unsigned int needLevelIndex) {
    if (needLevelIndex >= mLevels.size()) {
        GenerateAbsentLevels(needLevelIndex);
    }

    return mLevels[needLevelIndex];
}

void ZDungeon::GenerateAbsentLevels(unsigned int maxLevelIndex) {
    if (mLevels.empty()) {
        ZDungeonLevel* level = mLevelGenerator->GenerateLevel();
        mLevels.push_back(level);
    }

    mLevels.reserve(maxLevelIndex + 1);
    for (unsigned int newLevelIndex = mLevels.size(); newLevelIndex <= maxLevelIndex; ++newLevelIndex) {
        const ZDungeonLevel::StaircaseList& aboveLevelDownStaircases = mLevels[newLevelIndex - 1]->GetDownStaircases();
        ZDungeonLevel* level = mLevelGenerator->GenerateLevel(mLevels[newLevelIndex - 1]);
        mLevels.push_back(level);
    }
}

}
}
