#include "przCorePCH.h"
#include "Model/Path/WeightedMap.h"

#include "Utils/MatrixHelpers.h"

namespace prz {
namespace mdl {

ZWeightedMap::ZWeightedMap(int width, int height, const ZWeight& defaultCellWeight) {
    mWidth = width;
    mHeight = height;
    utl::ZMatrix::Allocate(&mCellWeights, mWidth, mHeight, defaultCellWeight);
};

ZWeightedMap::~ZWeightedMap() {
    utl::ZMatrix::Deallocate(&mCellWeights, mHeight);
};

int ZWeightedMap::GetWidth() const {
    return mWidth;
}

int ZWeightedMap::GetHeight() const {
    return mHeight;
}

void ZWeightedMap::SetCellWeight(const ZPosition& pos, const ZWeight& weight) {
    SetCellWeight(pos.GetX(), pos.GetY(), weight);
}

void ZWeightedMap::SetCellWeight(int x, int y, const ZWeight& weight) {
    mCellWeights[x][y] = weight;
}

ZWeight ZWeightedMap::GetCellWeight(const ZPosition& pos) const {
    return GetCellWeight(pos.GetX(), pos.GetY());
}

ZWeight ZWeightedMap::GetCellWeight(int x, int y) const {
    return mCellWeights[x][y];
}

}
}
