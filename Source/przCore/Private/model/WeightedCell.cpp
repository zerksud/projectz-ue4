#include "przCorePrivatePCH.h"
#include "model/WeightedCell.h"

#include <limits>

namespace prz {
    namespace mdl {

        const int ZWeightedCell::kInfiniteWeight = std::numeric_limits<int>::max() / 2;

        int ZWeightedCell::SumWeights(int leftWeight, int rightWeight) {
            int sum = leftWeight + rightWeight;
            if (sum >= kInfiniteWeight) {
                return kInfiniteWeight;
            }

            return sum;
        }

        ZWeightedCell::ZWeightedCell()
            : position(ZPosition(0, 0)), pathToCellWeight(kInfiniteWeight), pathFromCellEstimatedWeight(kInfiniteWeight) {
        }

        ZWeightedCell::ZWeightedCell(const ZPosition& pPosition, int pPathToCellWeight, int pPathFromCellEstimatedWeight)
            : position(pPosition), pathToCellWeight(pPathToCellWeight), pathFromCellEstimatedWeight(pPathFromCellEstimatedWeight) {
        }

        int ZWeightedCell::GetTotalPathWeight() const {
            return SumWeights(pathToCellWeight, pathFromCellEstimatedWeight);
        }

    }
}
