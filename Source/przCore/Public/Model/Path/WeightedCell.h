#pragma once

#include "Model/Position.h"

namespace prz {
namespace mdl {
namespace path {

class ZWeight {
public:
    static const ZWeight kInfinity;
    static const ZWeight kZero;

    ZWeight();
    ZWeight(int value);

    bool operator==(const ZWeight& other) const;
    bool operator!=(const ZWeight& other) const;
    bool operator>(const ZWeight& other) const;
    ZWeight operator+(const ZWeight& other) const;
    ZWeight& operator+=(const ZWeight& other);

private:
    static const int kInfiniteWeight;

    int mValue;
};

struct ZWeightedCell {
    ZWeightedCell();
    ZWeightedCell(const ZPosition& pPosition, const ZWeight& pPathToCellWeight, const ZWeight& pPathFromCellEstimatedWeight);

    ZWeight GetTotalPathWeight() const;

    ZPosition position;

    ZWeight pathToCellWeight;
    ZWeight pathFromCellEstimatedWeight;

};

}
}
}
