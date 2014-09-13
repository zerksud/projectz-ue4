#include "przCorePCH.h"
#include "model/Minimap.h"

#include "utils/LOG_ANSI.h"
#include "utils/MatrixHelpers.h"

namespace prz {
    namespace mdl {

        ZMinimap::ZMinimap(unsigned int sideSize, EDungeonCell::Type*** cells) {
            mSize = sideSize;
            mCells = *cells;
            *cells = nullptr;
        }

        ZMinimap::ZMinimap(const ZMinimap& other) {
            mSize = other.mSize;
            utl::ZMatrix::AllocateAndCopy<EDungeonCell::Type>(&mCells, other.mCells, mSize);
        }

        ZMinimap::ZMinimap(ZMinimap&& other) {
            mSize = 0;
            mCells = nullptr;

            swap(*this, other);
        }

        ZMinimap& ZMinimap::operator=(ZMinimap other) {
            swap(*this, other);
            return *this;
        }

        ZMinimap::~ZMinimap() {
            utl::ZMatrix::Deallocate(&mCells, mSize);
        }

        unsigned int ZMinimap::GetSize() const {
            return mSize;
        }

        bool ZMinimap::IndicesAreValid(unsigned int x, unsigned int y) const {
            return (x < mSize && y < mSize);
        }

        EDungeonCell::Type ZMinimap::GetCell(unsigned int x, unsigned int y) const {
            if (!IndicesAreValid(x, y)) {
                LOGE("Can't get cell %dx%d from minimap of size %d", x, y, mSize);
                return EDungeonCell::Unknown;
            }

            return mCells[x][y];
        }
    }
}
