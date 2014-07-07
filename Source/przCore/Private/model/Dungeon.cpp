#include "przCorePrivatePCH.h"
#include "model/Dungeon.h"

#include "model/DungeonLevelGenerator.h"
#include "utils/LOG_ANSI.h"

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
            delete mLevelGenerator;
        }

        const ZDungeonLevel* ZDungeon::GetLevel(unsigned int levelIndex) {
            return GetExistingLevelOrGenerateNew(levelIndex);
        }

        bool ZDungeon::PlaceMonster(ZMonster* monster, unsigned int levelIndex, const ZPosition& position) {
            if (!monster) {
                LOGE("Placed monster can't be nullptr");
                return false;
            }

            if (!monster->IsRegistered()) {
                LOGE("Can't place non-registered monster");
                return false;
            }

            auto pos = mMonsterLevelMap.find(monster->GetId());
            if (pos != mMonsterLevelMap.end()) {
                LOGE("Can't place already placed monster with id = %d", monster->GetId());
                return false;
            }

            ZDungeonLevel* level = GetExistingLevelOrGenerateNew(levelIndex);
            bool success = level->PlaceMonster(monster, position);
            if (success) {
                mMonsterLevelMap[monster->GetId()] = levelIndex;
            }

            return success;
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

        bool ZDungeon::TryToMoveMonster(utl::ZIdType monsterId, EMoveDirection::Type direction, ZPositionDiff* OutExpectedMoveDiff) {
            auto monsterLevelIterator = mMonsterLevelMap.find(monsterId);
            if (monsterLevelIterator == mMonsterLevelMap.end()) {
                LOGE("Can't move not-placed monster with id = %d", monsterId);
                return false;
            }

            unsigned int monsterLevelIndex = monsterLevelIterator->second;

            bool moveIsSuccessful = mLevels[monsterLevelIndex]->TryToMoveMonster(monsterId, direction, OutExpectedMoveDiff);
            if (moveIsSuccessful) {
                ZDungeonLevel* nextLevel = nullptr;
                unsigned int nextLevelIndex = 0;
                if (direction == EMoveDirection::Down) {
                    nextLevelIndex = monsterLevelIndex + 1;
                    nextLevel = GetExistingLevelOrGenerateNew(nextLevelIndex);
                } else if (direction == EMoveDirection::Up) {
                    if (monsterLevelIndex > 0) {
                        nextLevelIndex = monsterLevelIndex - 1;
                        nextLevel = mLevels[nextLevelIndex].get();
                    } else {
                        return false;   // monster can't leave dungeon
                    }
                }

                if (nextLevel) {
                    const ZPosition* monsterPosition = mLevels[monsterLevelIndex]->GetMonsterPosition(monsterId);
                    bool targetCellIsEmpty = nextLevel->CellIsEmpty(*monsterPosition);

                    if (targetCellIsEmpty) {
                        ZMonster* monster = mLevels[monsterLevelIndex]->RemoveMonster(monsterId);
                        nextLevel->PlaceMonster(monster, *monsterPosition);

                        monsterLevelIterator->second = nextLevelIndex;
                    } else {
                        moveIsSuccessful = false;
                    }
                }
            }

            return moveIsSuccessful;
        }

        ZDungeonLevel* ZDungeon::GetExistingLevelOrGenerateNew(unsigned int needLevelIndex) {
            if (needLevelIndex >= mLevels.size()) {
                GenerateAbsentLevels(needLevelIndex);
            }

            return mLevels[needLevelIndex].get();
        }

        void ZDungeon::GenerateAbsentLevels(unsigned int maxLevelIndex) {
            if (mLevels.empty()) {
                ZDungeonLevel* level = mLevelGenerator->GenerateLevel();
                mLevels.push_back(std::unique_ptr<ZDungeonLevel>(level));
            }

            mLevels.reserve(maxLevelIndex + 1);
            for (unsigned int newLevelIndex = mLevels.size(); newLevelIndex <= maxLevelIndex; ++newLevelIndex) {
                const ZDungeonLevel::StaircaseList& aboveLevelDownStaircases = mLevels[newLevelIndex - 1]->GetDownStaircases();
                ZDungeonLevel* level = mLevelGenerator->GenerateLevel(aboveLevelDownStaircases);
                mLevels.push_back(std::unique_ptr<ZDungeonLevel>(level));
            }
        }

    }
}
