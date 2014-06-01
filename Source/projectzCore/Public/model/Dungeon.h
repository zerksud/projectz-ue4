#pragma once

#include <unordered_map>
#include <vector>

#include "model/Position.h"
#include "model/Monster.h"

namespace prz {
    namespace mdl {

        namespace EDungeonCell {
            enum Type {
                Solid,
                Hollow
            };
        }

        class ZDungeon {
        public:
            typedef char ZMapCell;
            typedef std::vector<ZPosition> StaircaseList;

            ZDungeon(int width, int height, const ZMapCell* map);
            virtual ~ZDungeon();

            int GetWidth() const;
            int GetHeight() const;

            bool CellIsEmpty(int x, int y) const;
            bool CellIsEmpty(const ZPosition& pos) const;

            bool CellIsSolid(int x, int y) const;
            bool CellIsSolid(const ZPosition& pos) const;

            const StaircaseList& GetUpStaircases() const;
            const StaircaseList& GetDownStaircases() const;

            bool PlaceMonster(const ZMonster& monster, const ZPosition& position);
            const ZPosition* GetMonsterPosition(utl::ZIdType monsterId) const;
            ZMonster* GetMonster(utl::ZIdType monsterId);

        private:
            ZDungeon(const ZDungeon& other);
            ZDungeon& operator=(const ZDungeon& other);

            static const ZMapCell kSolidCell = '#';
            static const ZMapCell kHollowCell = '.';
            static const ZMapCell kUpStaircaseCell = '<';
            static const ZMapCell kDownStaircaseCell = '>';

            typedef std::unordered_map<ZMapCell, EDungeonCell::Type> ZMapToTerrainCellMap;
            static const ZMapToTerrainCellMap kMapToTerrainCellMap;

            void CreateFailSafeDungeon();

            void ParseMap(const ZMapCell* terrain);
            bool CellIndicesAreValid(int x, int y) const;

            int CalcCellLinearIndex(int x, int y) const;
            int CalcCellLinearIndex(const ZPosition& position) const;

            bool CellIsSolidImpl(int x, int y) const;
            bool CellIsEmptyImpl(int x, int y) const;

            int mWidth;
            int mHeight;
            EDungeonCell::Type* mTerrain;

            StaircaseList mUpStaircases;
            StaircaseList mDownStaircases;

            struct ZPlacedMonster {
                ZMonster monster;
                ZPosition position;

                ZPlacedMonster(const ZMonster& pMonster, const ZPosition& pPosition) :
                    monster(pMonster), position(pPosition) {
                }
            };
            typedef std::unordered_map<int, utl::ZIdType> ZMonsterIdByPositionMap;
            typedef std::unordered_map<utl::ZIdType, ZPlacedMonster*> ZMonsterList;


            ZMonsterIdByPositionMap mMonsterIdByPosition;
            ZMonsterList mMonsterList;
        };
    }
}
