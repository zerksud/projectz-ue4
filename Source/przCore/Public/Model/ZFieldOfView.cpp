#include "przCorePCH.h"
#include "ZFieldOfView.h"

#include "Utils/MatrixHelpers.h"
#include "Utils/StandardLibrary/Utility.h"

namespace prz {
namespace mdl {

ZFieldOfView::ZFieldOfView():
mRadius(0) {
    utl::ZMatrix::Allocate(&mData, 1, 1, EDungeonCell::Monster);
}

ZFieldOfView::ZFieldOfView(unsigned int radius, EDungeonCell** fovData) :
mRadius(radius),
mData(fovData) {
}

ZFieldOfView::ZFieldOfView(const ZFieldOfView& other) {
    mRadius = other.mRadius;
    utl::ZMatrix::AllocateAndCopy(&mData, other.mData, mRadius * 2 + 1);
}

ZFieldOfView::ZFieldOfView(ZFieldOfView&& other) {
    swap(*this, other);

    other.mRadius = 0;
    utl::ZMatrix::Allocate(&other.mData, 1, 1, EDungeonCell::Monster);
}

ZFieldOfView::~ZFieldOfView() {
    utl::ZMatrix::Deallocate(&mData, mRadius * 2 + 1);
}

ZFieldOfView& ZFieldOfView::operator=(ZFieldOfView other) {
    swap(*this, other);

    return *this;
}

void swap(ZFieldOfView& left, ZFieldOfView& right) {
    using utl::swap;

    swap(left.mRadius, right.mRadius);
    swap(left.mData, right.mData);
}

EDungeonCell ZFieldOfView::GetCell(int dx, int dy) const {
    if ((unsigned int)(dx * dx + dy * dy) > mRadius * mRadius) {
        return EDungeonCell::Unknown;
    }

    return mData[mRadius + dx][mRadius + dy];
}

}
}
