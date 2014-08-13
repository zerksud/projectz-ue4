#pragma once

#include "model/Position.h"

namespace prz {
    namespace mdl {

        struct ZWeightedCell {
            static const int kInfiniteWeight;

            static int SumWeights(int leftWeight, int rightWeight);

            ZWeightedCell();
            ZWeightedCell(const ZPosition& pPosition, int pPathToCellWeight, int pPathFromCellEstimatedWeight);

            int GetTotalPathWeight() const;

            ZPosition position;

            int pathToCellWeight;
            int pathFromCellEstimatedWeight;

        };

    }
}
