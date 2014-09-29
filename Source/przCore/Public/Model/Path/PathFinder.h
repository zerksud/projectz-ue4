#pragma once

#include "Model/Position.h"
#include "Model/DungeonLevel.h"
#include "Model/Path/WeightedCell.h"

#include "Utils/MatrixHelpers.h"

namespace prz {
namespace mdl {

struct PathCellConnection;

class ZWeightedMap {
public:
    ZWeightedMap(int width, int height, const ZWeight& defaultCellWeight) {
        mWidth = width;
        mHeight = height;
        utl::ZMatrix::Allocate(&mCellWeights, mWidth, mHeight, defaultCellWeight);
    };

    ~ZWeightedMap() {
        utl::ZMatrix::Deallocate(&mCellWeights, mHeight);
    };

    int GetWidth() const {
        return mWidth;
    }

    int GetHeight() const {
        return mHeight;
    }

    void SetCellWeight(const ZPosition& pos, const ZWeight& weight) {
        SetCellWeight(pos.GetX(), pos.GetY(), weight);
    }

    void SetCellWeight(int x, int y, const ZWeight& weight) {
        mCellWeights[x][y] = weight;
    }

    ZWeight GetCellWeight(const ZPosition& pos) const {
        return GetCellWeight(pos.GetX(), pos.GetY());
    }

    ZWeight GetCellWeight(int x, int y) const {
        return mCellWeights[x][y];
    }

private:
    int mWidth;
    int mHeight;
    ZWeight** mCellWeights;
};

class ZPathFinder {
public:
    ZPathFinder() = delete;

    static const ZWeight kSolidRockCellWeight;
    static const ZWeight kEmptyCellWeight;

    static const ZWeight kTunnelTurnPenalty;
    static const int kEstimatedPathWeightFactor;

    static bool CellMustBeDigged(const ZWeightedMap& map, const ZPosition& position);
    static bool CellMustBeDigged(const ZWeightedMap& map, int x, int y);

    typedef std::vector<ZPosition> PathCells;
    //returns list of path cells starting from finishCell if path between cells exists, or empty list otherwise
    static PathCells FindPathBetweenCells(const ZWeightedMap& map, const ZPosition& startCellPosition, const ZPosition& finishCellPosition, bool diggingIsAllowed = false);

private:
    // returns true if path from cell to neighbor is shorter than previous path to that cell
    static ZWeightedCell* ZPathFinder::CreateNextPathCellIfMorePromising(const ZWeightedMap& map, const ZWeightedCell& currentCell, const ZPositionDiff& currentMoveDiff, const ZPosition& finishCellPosition, PathCellConnection** pathConnections);
};
}
}
