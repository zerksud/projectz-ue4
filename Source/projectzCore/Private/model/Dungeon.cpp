#include "projectzCorePrivatePCH.h"

#include <cstring>

#include "model/Dungeon.h"
#include "utils/IUniqueIdRegistry.h"
#include "utils/Services.h"
#include "utils/LOG_ANSI.h"

namespace prz {
    namespace mdl {

        const ZDungeon::ZMapToTerrainCellMap ZDungeon::kMapToTerrainCellMap = {
            {kSolidCell, EDungeonCell::Solid},
            {kHollowCell, EDungeonCell::Hollow},
            {kUpStaircaseCell, EDungeonCell::Hollow},
            {kDownStaircaseCell, EDungeonCell::Hollow}
        };

        const ZDungeon::ZMoveToTurnDirectionMap ZDungeon::kMoveToTurnDirectionMap = {
            {EMoveDirection::Forward, ETurnDirection::Forward},
            {EMoveDirection::Backward, ETurnDirection::Back},
            {EMoveDirection::Left, ETurnDirection::Left},
            {EMoveDirection::Right, ETurnDirection::Right}
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
            mTerrain = new EDungeonCell::Type[mWidth * mHeight];

            for (int y = 0; y < mHeight; ++y) {
                for (int x = 0; x < mWidth; ++x) {
                    EDungeonCell::Type cell = EDungeonCell::Solid;

                    int index = CalcCellLinearIndex(x, y);
                    const ZMapCell mapCell = map[index];

                    if (mapCell == kUpStaircaseCell) {
                        mUpStaircases.push_back(ZPosition(x, y));
                    } else if (mapCell == kDownStaircaseCell) {
                        mDownStaircases.push_back(ZPosition(x, y));
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
            using namespace utl;

            IUniqueIdRegistry* registry = ZServices::GetInstance().GetService<IUniqueIdRegistry>();
            if (registry) {
                for (auto& pair : mMonsterList) {
                    ZMonster& monster = pair.second->monster;
                    if (monster.IsRegistered()) {
                        registry->ReleaseUniqueId(&monster);
                    }

                    delete pair.second;
                }
            }

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

        const ZDungeon::StaircaseList& ZDungeon::GetUpStaircases() const {
            return mUpStaircases;
        }

        const ZDungeon::StaircaseList& ZDungeon::GetDownStaircases() const {
            return mDownStaircases;
        }

        bool ZDungeon::PlaceMonster(const ZMonster& monster, const ZPosition& position) {
            if (!monster.IsRegistered()) {
                LOGE("Can't place non-registered monster");
                return false;
            }

            if (CellIsSolid(position)) {
                LOGE("Can't place monster in solid cell %s", position.ToString().c_str());
                return false;
            }

            auto pos = mMonsterList.find(monster.GetId());
            if (pos != mMonsterList.end()) {
                LOGE("Can't place already placed monster with id = %d", monster.GetId());
                return false;
            }

            int linearPositionIndex = CalcCellLinearIndex(position);
            auto positionMonsterLink = mMonsterIdByPosition.find(linearPositionIndex);
            if (positionMonsterLink != mMonsterIdByPosition.end()) {
                LOGE("Can't place monster in already occupied cell %s", position.ToString().c_str());
                return false;
            }

            mMonsterList[monster.GetId()] = new ZPlacedMonster(monster, position);
            mMonsterIdByPosition[linearPositionIndex] = monster.GetId();

            return true;
        }

        const ZPosition* ZDungeon::GetMonsterPosition(utl::ZIdType monsterId) const {
            auto pos = mMonsterList.find(monsterId);
            if (pos == mMonsterList.end()) {
                LOGE("Can't return position of not-placed monster with id = %d", monsterId);
                return nullptr;
            }

            return &pos->second->position;
        }

        ZMonster* ZDungeon::GetMonster(utl::ZIdType monsterId) {
            ZPlacedMonster* placedMonster = GetPlacedMonster(monsterId);
            if (placedMonster == nullptr) {
                LOGE("Can't return not-placed monster with id = %d", monsterId);
                return nullptr;
            }

            return &placedMonster->monster;
        }

        ZDungeon::ZPlacedMonster* ZDungeon::GetPlacedMonster(utl::ZIdType monsterId) {
            auto pos = mMonsterList.find(monsterId);
            if (pos == mMonsterList.end()) {
                LOGE("Can't return not-placed monster with id = %d", monsterId);
                return nullptr;
            }

            return pos->second;
        }

        bool ZDungeon::MovementIsDiagonalAroundTheCorner(const ZPosition& origin, const ZPositionDiff& diff) const {
            int sum = abs(diff.GetdX()) + abs(diff.GetdY());
            if (sum <= 1) {
                return false;
            }

            return CellIsSolidImpl(origin.GetX(), origin.GetY() + diff.GetdY()) ||
                CellIsSolidImpl(origin.GetX() + diff.GetdX(), origin.GetY());
        }

        bool ZDungeon::TryToMoveMonster(utl::ZIdType monsterId, EMoveDirection::Type direction, ZPositionDiff* OutExpectedMoveDiff) {
            ZPlacedMonster* placedMonster = GetPlacedMonster(monsterId);
            if (placedMonster == nullptr) {
                LOGE("Can't move not-placed monster with id = %d", monsterId);
                return false;
            }

            auto pos = kMoveToTurnDirectionMap.find(direction);
            if (pos == kMoveToTurnDirectionMap.end()) {
                LOGE("Got unsupported move direction %d", direction);
                return false;
            }

            ZDirection alignedDirection = placedMonster->monster.GetDirection();
            alignedDirection.Turn(pos->second);

            ZPositionDiff expectedDiff = alignedDirection.PredictMove();
            ZPosition expectedPosition = placedMonster->position + expectedDiff;

            bool movementIsPossible = CellIsEmptyImpl(expectedPosition.GetX(), expectedPosition.GetY()) &&
                !MovementIsDiagonalAroundTheCorner(placedMonster->position, expectedDiff);

            if (movementIsPossible) {
                int oldLinearIndex = CalcCellLinearIndex(placedMonster->position);
                mMonsterIdByPosition.erase(oldLinearIndex);

                placedMonster->position = expectedPosition;

                int newLinearIndex = CalcCellLinearIndex(expectedPosition);
                mMonsterIdByPosition[newLinearIndex] = monsterId;
            }

            if (OutExpectedMoveDiff != nullptr) {
                *OutExpectedMoveDiff = expectedDiff;
            }

            return movementIsPossible;
        }

        bool ZDungeon::CellIndicesAreValid(int x, int y) const {
            return (x >= 0 && x < mWidth && y >= 0 && y < mHeight);
        }

        int ZDungeon::CalcCellLinearIndex(int x, int y) const {
            return (y * mWidth + x);
        }

        int ZDungeon::CalcCellLinearIndex(const ZPosition& position) const {
            return CalcCellLinearIndex(position.GetX(), position.GetY());
        }

        bool ZDungeon::CellIsSolidImpl(int x, int y) const {
            bool solid = true;
            int index = CalcCellLinearIndex(x, y);
            switch (mTerrain[index]) {
            case EDungeonCell::Solid:
                solid = true;
                break;
            case EDungeonCell::Hollow:
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

            if (empty) {
                int linearIndex = CalcCellLinearIndex(x, y);
                auto pos = mMonsterIdByPosition.find(linearIndex);
                if (pos != mMonsterIdByPosition.end()) {
                    empty = false;
                }
            }

            return empty;
        }
    }
}
