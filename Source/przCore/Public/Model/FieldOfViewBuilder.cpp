#include "przCorePCH.h"
#include "Model/FieldOfViewBuilder.h"

#include "Utils/MatrixHelpers.h"

namespace prz {
namespace mdl {

ZFieldOfView ZFieldOfViewBuilder::Build(const ZPosition& position, int viewDistance, const ZDungeonLevel& dungeonLevel) {
    EDungeonCell::Type** fieldOfViewData;
    utl::ZMatrix::Allocate(&fieldOfViewData, viewDistance * 2 + 1);

    int x = position.GetX();
    int y = position.GetY();

    for (int dx = -viewDistance; dx <= viewDistance; ++dx) {
        for (int dy = -viewDistance; dy <= viewDistance; ++dy) {
            if (dx * dx + dy * dy <= viewDistance * viewDistance) {
                fieldOfViewData[dx + viewDistance][dy + viewDistance] = dungeonLevel.GetCellType(x + dx, y + dy);
            }
        }
    }

    return ZFieldOfView(viewDistance, fieldOfViewData);
}

}
}
