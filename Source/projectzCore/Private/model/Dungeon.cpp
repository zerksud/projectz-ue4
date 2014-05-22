#include "projectzCorePrivatePCH.h"

#include "model/Dungeon.h"
#include "utils/LOG_ANSI.h"

namespace prz {
    namespace mdl {
        ZDungeon::ZDungeon(int width, int height, const ZMapCell* map, const ZPosition& startPosition) {
            mWidth = width;
            mHeight = height;
            ParseMap(map);
        }

        void ZDungeon::ParseMap(const ZMapCell* map) {
            mTerrain = new ZDungeonCell::Type[mWidth * mHeight];

            int w = mWidth;
            int h = mHeight;
            for (int y = 0; y < h; ++y) {
                for (int x = 0; x < w; ++x) {
                    int index = CalcCellLinearIndex(x, y);
                    ZDungeonCell::Type cell = ZDungeonCell::Solid;

                    const ZMapCell cellChar = map[index];
                    if (cellChar == '.') {
                        cell = ZDungeonCell::Hollow;
                    }

                    mTerrain[index] = cell;
                }
            }
        }

        ZDungeon::~ZDungeon() {
            delete mTerrain;
        }

        int ZDungeon::GetWidth() const {
            return mWidth;
        }

        int ZDungeon::GetHeight() const {
            return mHeight;
        }

        bool ZDungeon::CellIsEmpty(int x, int y) const {
            if (CellIndicesAreValid(x, y)) {
                return CellIsEmptyImpl(x, y);
            }

            LOGE("Cell at [%d; %d] is out of bounds", x, y);
            return false;
        }

        bool ZDungeon::CellIsEmpty(const ZPosition& pos) const {
            return CellIsEmpty(pos.GetX(), pos.GetY());
        }

        bool ZDungeon::CellIsSolid(int x, int y) const {
            if (CellIndicesAreValid(x, y)) {
                return CellIsSolidImpl(x, y);
            }

            LOGE("Cell at [%d; %d] is out of bounds", x, y);
            return true;
        }

        bool ZDungeon::CellIsSolid(const ZPosition& pos) const {
            return CellIsSolid(pos.GetX(), pos.GetY());
        }

        bool ZDungeon::CellIndicesAreValid(int x, int y) const {
            return (x >= 0 && x < mWidth && y >= 0 && y < mHeight);
        }

        int ZDungeon::CalcCellLinearIndex(int x, int y) const {
            return (y * mWidth + x);
        }

        bool ZDungeon::CellIsSolidImpl(int x, int y) const {
            bool solid = true;
            int index = CalcCellLinearIndex(x, y);
            switch (mTerrain[index]) {
            case ZDungeonCell::Solid:
                solid = true;
                break;
            case ZDungeonCell::Hollow:
                solid = false;
                break;
            default:
                LOGE("Unknown terrain type %d", mTerrain[index]);
                solid = true;
                break;
            }

            return solid;
        }

        bool ZDungeon::CellIsEmptyImpl(int x, int y) const {
            bool empty = !CellIsSolidImpl(x, y);

            return empty;
        }
    }
}
