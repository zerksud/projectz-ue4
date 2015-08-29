#pragma once

#include "Utils/StandartLibrary.h"

#include "Model/Path/WeightedCell.h"
#include "Model/Position.h"

namespace prz {
namespace mdl {
namespace path {

class ZWeightedMap {
public:
    ZWeightedMap(int width, int height, const ZWeight& defaultCellWeight);
    ZWeightedMap(const ZWeightedMap& other);
    ~ZWeightedMap();

    ZWeightedMap& operator=(ZWeightedMap other);

    friend void swap(ZWeightedMap& left, ZWeightedMap& right);

    int GetWidth() const;
    int GetHeight() const;

    void SetCellWeight(const ZPosition& pos, const ZWeight& weight);
    void SetCellWeight(int x, int y, const ZWeight& weight);

    const ZWeight& GetCellWeight(const ZPosition& pos) const;
    const ZWeight& GetCellWeight(int x, int y) const;

    const utl::ZString ToString() const;

private:
    int mWidth;
    int mHeight;
    ZWeight** mCellWeights;
};

}
}
}
