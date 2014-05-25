#include "projectzCorePrivatePCH.h"

#include <cstring>

#include "model/Dungeon.h"
#include "utils/LOG_ANSI.h"

namespace prz {
    namespace mdl {

        const ZDungeon::ZMapToTerrainCellMap ZDungeon::kMapToTerrainCellMap = {
            {kSolidCell, ZDungeonCell::Solid},
            {kHollowCell, ZDungeonCell::Hollow},
            {kStairsUpCell, ZDungeonCell::Hollow},
            {kStairsDownCell, ZDungeonCell::Hollow}
        };

        void ZDungeon::CreateFailSafeDungeon() {
            mWidth = 3;
            mHeight = 3;
            ParseMap(""
            "###"
            "#.#"
            "###");
        }

        ZDungeon::ZDungeon(int width, int height, const ZMapCell* map) {
            if (width <= 0 || height <= 0) {
                LOGE("Can't create dungeon with size %dx%d", width, height);
                CreateFailSafeDungeon();
                return;
            }

            if (strlen(map) != width * height) {
                LOGE("Can't create dungeon with size %dx%d from %s", width, height, map);
                CreateFailSafeDungeon();
                return;
            }

            mWidth = width;
            mHeight = height;
            ParseMap(map);
        }

        void ZDungeon::ParseMap(const ZMapCell* map) {
            mTerrain = new ZDungeonCell::Type[mWidth * mHeight];

            for (int y = 0; y < mHeight; ++y) {
                for (int x = 0; x < mWidth; ++x) {
                    ZDungeonCell::Type cell = ZDungeonCell::Solid;

                    int index = CalcCellLinearIndex(x, y);
                    const ZMapCell mapCell = map[index];

                    if (mapCell == kStairsUpCell) {
                        mStairsUp.push_back(ZPosition(x, y));
                    } else if (mapCell == kStairsDownCell) {
                        mStairsDown.push_back(ZPosition(x, y));
                    }

                    auto mapCellReplacement = kMapToTerrainCellMap.find(mapCell);

                    if (mapCellReplacement != kMapToTerrainCellMap.end()) {
                        cell = mapCellReplacement->second;
                    } else {
                        LOGE("Got unknown map cell type '%c'", mapCell);
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

        const ZDungeon::StairsList& ZDungeon::GetStairsUp() const {
            return mStairsUp;
        }

        const ZDungeon::StairsList& ZDungeon::GetStairsDown() const {
            return mStairsDown;
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
