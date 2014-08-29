#pragma once

#include <unordered_map>
#include <vector>

#include "model/Position.h"
#include "model/Monster.h"

namespace prz {
    namespace mdl {

        namespace EDungeonCell {
            enum Type {
                Unknown,
                SolidRock,
                Emptiness,
                DownStaircase,
                UpStaircase,
                Monster
            };
        }

        namespace EMoveDirection {
            enum Type {
                Forward,
                Backward,
                Left,
                Right
            };
        }

        class ZDungeonLevel {
        public:
            typedef std::vector<ZPosition> StaircaseList;

            // holds map matrix
            ZDungeonLevel(int width, int height, EDungeonCell::Type*** map);
            ZDungeonLevel(const ZDungeonLevel& other) = delete;
            ZDungeonLevel& operator=(const ZDungeonLevel& other) = delete;
            virtual ~ZDungeonLevel();

            int GetWidth() const;
            int GetHeight() const;

            bool CellIsEmpty(int x, int y) const;
            bool CellIsEmpty(const ZPosition& pos) const;

            bool CellIsSolid(int x, int y) const;
            bool CellIsSolid(const ZPosition& pos) const;

            const StaircaseList& GetUpStaircases() const;
            const StaircaseList& GetDownStaircases() const;

            EDungeonCell::Type GetCellType(int x, int y) const;
            EDungeonCell::Type GetCellType(const ZPosition& pos) const;

            bool PlaceMonster(ZMonster* monster, const ZPosition& position);
            ZMonster* RemoveMonster(utl::ZIdType monsterId);

            // returns monster's position by it's id or nullptr if this level has no monster with such id
            const ZPosition* GetMonsterPosition(utl::ZIdType monsterId) const;

            // returns monster by it's id or nullptr if this level has no monster with such id
            ZMonster* GetMonster(utl::ZIdType monsterId);

            ZMonster* GetMonster(const ZPosition& position);
            ZMonster* GetMonster(int x, int y);

            bool TryToMoveMonster(utl::ZIdType monsterId, EMoveDirection::Type direction, ZPositionDiff* OutExpectedMoveDiff = nullptr);

        private:
            typedef std::map<EMoveDirection::Type, ETurnDirection::Type> ZMoveToTurnDirectionMap;
            static const ZMoveToTurnDirectionMap kMoveToTurnDirectionMap;

            void CreateFailSafeDungeon();

            void ParseMap(EDungeonCell::Type*** map);
            bool CellIndicesAreValid(int x, int y) const;

            int CalcCellLinearIndex(int x, int y) const;
            int CalcCellLinearIndex(const ZPosition& position) const;

            bool CellIsSolidImpl(int x, int y) const;
            bool CellIsEmptyImpl(int x, int y) const;

            EDungeonCell::Type GetCellTypeImpl(int x, int y) const;

            struct ZPlacedMonster {
                ZMonster* monster;
                ZPosition position;

                ZPlacedMonster(ZMonster* pMonster, const ZPosition& pPosition) :
                    monster(pMonster), position(pPosition) {
                }

                ~ZPlacedMonster() {
                    ZMonster::DestroyMonster(&monster);
                }
            };

            bool MovementIsDiagonalAroundTheCorner(const ZPosition& origin, const ZPositionDiff& diff) const;

            typedef std::unordered_map<int, utl::ZIdType> ZMonsterIdByPositionMap;
            typedef std::unordered_map<utl::ZIdType, ZPlacedMonster*> ZMonsterList;

            ZPlacedMonster* GetPlacedMonster(utl::ZIdType monsterId);
            ZPlacedMonster* GetPlacedMonster(int x, int y);
            const ZPlacedMonster* GetPlacedMonster(int x, int y) const;

            int mWidth;
            int mHeight;
            EDungeonCell::Type** mTerrain;

            StaircaseList mUpStaircases;
            StaircaseList mDownStaircases;

            ZMonsterIdByPositionMap mMonsterIdByPosition;
            ZMonsterList mMonsterList;
        };
    }
}
