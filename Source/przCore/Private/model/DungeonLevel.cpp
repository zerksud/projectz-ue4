#include "przCorePrivatePCH.h"

#include <cstring>

#include "model/DungeonLevel.h"
#include "utils/IUniqueIdRegistry.h"
#include "utils/Services.h"
#include "utils/LOG_ANSI.h"
#include "utils/Helpers.h"

namespace prz {
    namespace mdl {

        const ZDungeonLevel::ZMapCell ZDungeonLevel::kSolidCell = '#';
        const ZDungeonLevel::ZMapCell ZDungeonLevel::kHollowCell = '.';
        const ZDungeonLevel::ZMapCell ZDungeonLevel::kUpStaircaseCell = '<';
        const ZDungeonLevel::ZMapCell ZDungeonLevel::kDownStaircaseCell = '>';

        const ZDungeonLevel::ZMapToTerrainCellMap ZDungeonLevel::kMapToTerrainCellMap = {
            {kSolidCell, EDungeonCell::SolidRock},
            {kHollowCell, EDungeonCell::Emptiness},
            {kUpStaircaseCell, EDungeonCell::UpStaircase},
            {kDownStaircaseCell, EDungeonCell::DownStaircase}
        };

        const ZDungeonLevel::ZMoveToTurnDirectionMap ZDungeonLevel::kMoveToTurnDirectionMap = {
            {EMoveDirection::Forward, ETurnDirection::Forward},
            {EMoveDirection::Backward, ETurnDirection::Back},
            {EMoveDirection::Left, ETurnDirection::Left},
            {EMoveDirection::Right, ETurnDirection::Right}
        };

        void ZDungeonLevel::CreateFailSafeDungeon() {
            mWidth = 3;
            mHeight = 3;
            ParseMap(""
            "###"
            "#.#"
            "###");
        }

        ZDungeonLevel::ZDungeonLevel(int width, int height, const ZMapCell* map) {
            if (width <= 0 || height <= 0) {
                LOGE("Can't create dungeon with size %dx%d", width, height);
                CreateFailSafeDungeon();
                return;
            }

            size_t len = strlen(map);
            if (len != width * height) {
                LOGE("Can't create dungeon with size %dx%d from array with size %d", width, height, len);
                CreateFailSafeDungeon();
                return;
            }

            mWidth = width;
            mHeight = height;
            ParseMap(map);
        }

        void ZDungeonLevel::ParseMap(const ZMapCell* map) {
            mTerrain = new EDungeonCell::Type[mWidth * mHeight];

            for (int y = 0; y < mHeight; ++y) {
                for (int x = 0; x < mWidth; ++x) {
                    EDungeonCell::Type cell = EDungeonCell::SolidRock;

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

        ZDungeonLevel::~ZDungeonLevel() {
            using namespace utl;

            for (auto& pair : mMonsterList) {
                delete pair.second;
            }

            delete mTerrain;
        }

        int ZDungeonLevel::GetWidth() const {
            return mWidth;
        }

        int ZDungeonLevel::GetHeight() const {
            return mHeight;
        }

        bool ZDungeonLevel::CellIsEmpty(int x, int y) const {
            if (CellIndicesAreValid(x, y)) {
                return CellIsEmptyImpl(x, y);
            }

            return false;
        }

        bool ZDungeonLevel::CellIsEmpty(const ZPosition& pos) const {
            return CellIsEmpty(pos.GetX(), pos.GetY());
        }

        bool ZDungeonLevel::CellIsSolid(int x, int y) const {
            if (CellIndicesAreValid(x, y)) {
                return CellIsSolidImpl(x, y);
            }

            return true;
        }

        bool ZDungeonLevel::CellIsSolid(const ZPosition& pos) const {
            return CellIsSolid(pos.GetX(), pos.GetY());
        }

        const ZDungeonLevel::StaircaseList& ZDungeonLevel::GetUpStaircases() const {
            return mUpStaircases;
        }

        const ZDungeonLevel::StaircaseList& ZDungeonLevel::GetDownStaircases() const {
            return mDownStaircases;
        }

        EDungeonCell::Type ZDungeonLevel::GetCellType(int x, int y) const {
            if (CellIndicesAreValid(x, y)) {
                return GetCellTypeImpl(x, y);
            }

            return EDungeonCell::SolidRock;
        }

        EDungeonCell::Type ZDungeonLevel::GetCellType(const ZPosition& pos) const {
            return GetCellType(pos.GetX(), pos.GetY());
        }

        EDungeonCell::Type ZDungeonLevel::GetCellTypeImpl(int x, int y) const {
            if (GetPlacedMonster(x, y) != nullptr) {
                return EDungeonCell::Monster;
            }

            EDungeonCell::Type cellType = mTerrain[CalcCellLinearIndex(x, y)];

            return cellType;
        }

        bool ZDungeonLevel::PlaceMonster(ZMonster* monster, const ZPosition& position) {
            if (!monster) {
                LOGE("Placed monster can't be nullptr");
                return false;
            }

            if (!monster->IsRegistered()) {
                LOGE("Can't place non-registered monster");
                return false;
            }

            if (CellIsSolid(position)) {
                LOGE("Can't place monster in solid cell %s", position.ToString().c_str());
                return false;
            }

            auto pos = mMonsterList.find(monster->GetId());
            if (pos != mMonsterList.end()) {
                LOGE("Can't place already placed monster with id = %d", monster->GetId());
                return false;
            }

            int linearPositionIndex = CalcCellLinearIndex(position);
            auto positionMonsterLink = mMonsterIdByPosition.find(linearPositionIndex);
            if (positionMonsterLink != mMonsterIdByPosition.end()) {
                LOGE("Can't place monster in already occupied cell %s", position.ToString().c_str());
                return false;
            }

            mMonsterList[monster->GetId()] = new ZPlacedMonster(monster, position);
            mMonsterIdByPosition[linearPositionIndex] = monster->GetId();

            return true;
        }

        ZMonster* ZDungeonLevel::RemoveMonster(utl::ZIdType monsterId) {
            auto pos = mMonsterList.find(monsterId);
            if (pos == mMonsterList.end()) {
                LOGE("Can't remove not added monster with id = %d", monsterId);
                return nullptr;
            }

            ZPlacedMonster* placedMonster = pos->second;
            ZMonster* monster = placedMonster->monster;
            placedMonster->monster = nullptr;

            int linearPositionIndex = CalcCellLinearIndex(placedMonster->position);
            mMonsterIdByPosition.erase(linearPositionIndex);
            mMonsterList.erase(pos);

            delete placedMonster;

            return monster;
        }

        const ZPosition* ZDungeonLevel::GetMonsterPosition(utl::ZIdType monsterId) const {
            auto pos = mMonsterList.find(monsterId);
            if (pos == mMonsterList.end()) {
                LOGE("Can't return position of not-placed monster with id = %d", monsterId);
                return nullptr;
            }

            return &pos->second->position;
        }

        ZMonster* ZDungeonLevel::GetMonster(utl::ZIdType monsterId) {
            ZPlacedMonster* placedMonster = GetPlacedMonster(monsterId);
            if (placedMonster == nullptr) {
                LOGE("Can't return not-placed monster with id = %d", monsterId);
                return nullptr;
            }

            return placedMonster->monster;
        }

        ZMonster* ZDungeonLevel::GetMonster(const ZPosition& position) {
            return GetMonster(position.GetX(), position.GetY());
        }

        ZMonster* ZDungeonLevel::GetMonster(int x, int y) {
            if (CellIndicesAreValid(x, y)) {
                ZPlacedMonster* placedMonster = GetPlacedMonster(x, y);
                if (placedMonster) {
                    return placedMonster->monster;
                }
            }

            return nullptr;
        }

        ZDungeonLevel::ZPlacedMonster* ZDungeonLevel::GetPlacedMonster(utl::ZIdType monsterId) {
            auto pos = mMonsterList.find(monsterId);
            if (pos == mMonsterList.end()) {
                LOGE("Can't return not-placed monster with id = %d", monsterId);
                return nullptr;
            }

            return pos->second;
        }

        ZDungeonLevel::ZPlacedMonster* ZDungeonLevel::GetPlacedMonster(int x, int y) {
            return const_cast<ZPlacedMonster*>(static_cast<const ZDungeonLevel*>(this)->GetPlacedMonster(x, y));
        }

        const ZDungeonLevel::ZPlacedMonster* ZDungeonLevel::GetPlacedMonster(int x, int y) const {
            int linearIndex = CalcCellLinearIndex(x, y);
            auto pos = mMonsterIdByPosition.find(linearIndex);
            if (pos != mMonsterIdByPosition.end()) {
                return mMonsterList.find(pos->second)->second;
            }

            return nullptr;
        }

        bool ZDungeonLevel::MovementIsDiagonalAroundTheCorner(const ZPosition& origin, const ZPositionDiff& diff) const {
            int sum = abs(diff.GetdX()) + abs(diff.GetdY());
            if (sum <= 1) {
                return false;
            }

            return CellIsSolidImpl(origin.GetX(), origin.GetY() + diff.GetdY()) ||
                CellIsSolidImpl(origin.GetX() + diff.GetdX(), origin.GetY());
        }

        bool ZDungeonLevel::MoveDirectionIsVertical(EMoveDirection::Type direction) {
            return (direction == EMoveDirection::Up || direction == EMoveDirection::Down);
        }

        bool ZDungeonLevel::TryToMoveMonsterVertically(const ZPosition& position, EMoveDirection::Type direction) {
            const StaircaseList* staircasesForMoveDirection = nullptr;
            if (direction == EMoveDirection::Up) {
                staircasesForMoveDirection = &mUpStaircases;
            } else if (direction == EMoveDirection::Down) {
                staircasesForMoveDirection = &mDownStaircases;
            }

            bool movementIsPossible = utl::VectorContains<ZPosition>(*staircasesForMoveDirection, position);

            return movementIsPossible;
        }

        bool ZDungeonLevel::TryToMoveMonsterHorizontally(ZPlacedMonster* placedMonster, EMoveDirection::Type direction, ZPositionDiff* OutExpectedMoveDiff) {
            auto pos = kMoveToTurnDirectionMap.find(direction);
            if (pos == kMoveToTurnDirectionMap.end()) {
                LOGE("Got unsupported move direction %d", direction);
                return false;
            }

            ZDirection alignedDirection = placedMonster->monster->GetDirection();
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
                mMonsterIdByPosition[newLinearIndex] = placedMonster->monster->GetId();
            }

            if (OutExpectedMoveDiff != nullptr) {
                *OutExpectedMoveDiff = expectedDiff;
            }

            return movementIsPossible;
        }

        bool ZDungeonLevel::TryToMoveMonster(utl::ZIdType monsterId, EMoveDirection::Type direction, ZPositionDiff* OutExpectedMoveDiff) {
            ZPlacedMonster* placedMonster = GetPlacedMonster(monsterId);
            if (placedMonster == nullptr) {
                LOGE("Can't move not-placed monster with id = %d", monsterId);
                return false;
            }

            bool movementIsPossible = false;
            if (MoveDirectionIsVertical(direction)) {
                movementIsPossible = TryToMoveMonsterVertically(placedMonster->position, direction);
            } else {
                movementIsPossible = TryToMoveMonsterHorizontally(placedMonster, direction, OutExpectedMoveDiff);
            }

            return movementIsPossible;
        }

        bool ZDungeonLevel::CellIndicesAreValid(int x, int y) const {
            return (x >= 0 && x < mWidth && y >= 0 && y < mHeight);
        }

        int ZDungeonLevel::CalcCellLinearIndex(int x, int y) const {
            return (y * mWidth + x);
        }

        int ZDungeonLevel::CalcCellLinearIndex(const ZPosition& position) const {
            return CalcCellLinearIndex(position.GetX(), position.GetY());
        }

        bool ZDungeonLevel::CellIsSolidImpl(int x, int y) const {
            int index = CalcCellLinearIndex(x, y);
            bool solid = mTerrain[index] == EDungeonCell::SolidRock;

            return solid;
        }

        bool ZDungeonLevel::CellIsEmptyImpl(int x, int y) const {
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
