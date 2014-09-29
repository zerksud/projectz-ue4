#pragma once

#include "Model/Path/WeightedCell.h"
#include "Model/Position.h"

namespace prz {
namespace mdl {
namespace path {

class ZWeightedMap {
public:
    ZWeightedMap(int width, int height, const ZWeight& defaultCellWeight);
    ~ZWeightedMap();

    int GetWidth() const;
    int GetHeight() const;

    void SetCellWeight(const ZPosition& pos, const ZWeight& weight);
    void SetCellWeight(int x, int y, const ZWeight& weight);

    ZWeight GetCellWeight(const ZPosition& pos) const;
    ZWeight GetCellWeight(int x, int y) const;

private:
    int mWidth;
    int mHeight;
    ZWeight** mCellWeights;
};

}
}
}
