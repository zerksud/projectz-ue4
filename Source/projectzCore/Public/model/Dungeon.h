#pragma once

#include <unordered_map>

#include "model/Position.h"

namespace prz {
    namespace mdl {

        namespace ZDungeonCell {
            enum Type {
                Solid,
                Hollow
            };
        }

        class ZDungeon {
        public:
            typedef char ZMapCell;

            ZDungeon(int width, int height, const ZMapCell* map, const ZPosition& startPosition);
            ZDungeon(const ZDungeon& other);
            ZDungeon& operator=(const ZDungeon& other);
            virtual ~ZDungeon();

            int GetWidth() const;
            int GetHeight() const;

            bool CellIsEmpty(int x, int y) const;
            bool CellIsEmpty(const ZPosition& pos) const;

            bool CellIsSolid(int x, int y) const;
            bool CellIsSolid(const ZPosition& pos) const;

        private:
            static const ZMapCell kSolidCell = '#';
            static const ZMapCell kHollowCell = '.';
            
            typedef std::unordered_map<ZMapCell, ZDungeonCell::Type> ZMapToTerrainCellMap;
            static const ZMapToTerrainCellMap kMapToTerrainCellMap;

            void ParseMap(const ZMapCell* terrain);
            bool CellIndicesAreValid(int x, int y) const;
            int CalcCellLinearIndex(int x, int y) const;
            bool CellIsSolidImpl(int x, int y) const;
            bool CellIsEmptyImpl(int x, int y) const;

            int mWidth;
            int mHeight;
            ZDungeonCell::Type* mTerrain;
        };
    }
}
