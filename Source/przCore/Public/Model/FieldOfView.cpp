#include "przCorePCH.h"
#include "FieldOfView.h"

#include "Utils/MatrixHelpers.h"

namespace prz {
namespace mdl {

ZFieldOfView::ZFieldOfView(unsigned int radius, EDungeonCell::Type** fovData) :
mRadius(radius),
mData(fovData) {
}

ZFieldOfView::~ZFieldOfView() {
    utl::ZMatrix::Deallocate(&mData, mRadius * 2 + 1);
}

EDungeonCell::Type ZFieldOfView::GetCell(int dx, int dy) const {
    if ((unsigned int)(dx * dx + dy * dy) > mRadius * mRadius) {
        return EDungeonCell::Unknown;
    }

    return mData[mRadius + dx][mRadius + dy];
}

}
}
