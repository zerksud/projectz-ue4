#include "przCorePrivatePCH.h"
#include "model/Minimap.h"

#include "utils/LOG_ANSI.h"

namespace prz {
    namespace mdl {

        ZMinimap::ZMinimap(unsigned int sideSize, const EMinimapCell::Type* cells) {
            mSize = sideSize;

            mCells = new EMinimapCell::Type[mSize];
            for (unsigned int i = 0; i < mSize * mSize; ++i) {
                mCells[i] = cells[i];
            }
        }

        ZMinimap::~ZMinimap() {
            delete[] mCells;
        }

        unsigned int ZMinimap::GetSize() const {
            return mSize;
        }

        bool ZMinimap::IndicesAreValid(unsigned int x, unsigned int y) const {
            return (x < mSize && y < mSize);
        }

        unsigned int ZMinimap::CalcLinearIndex(unsigned int x, unsigned int y) const {
            return (x + y * mSize);
        }

        EMinimapCell::Type ZMinimap::GetCell(unsigned int x, unsigned int y) const {
            if (!IndicesAreValid(x, y)) {
                LOGE("Can't get cell %dx%d from minimap of size %d", x, y, mSize);
                return EMinimapCell::Unknown;
            }

            return mCells[CalcLinearIndex(x, y)];
        }
    }
}
