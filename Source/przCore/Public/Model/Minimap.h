#pragma once

#include <utility>

#include "Core.h"

#include "Model/DungeonLevel.h"

namespace prz {
namespace mdl {

class PRZCORE_API ZMinimap {
public:
    // holds cells matrix
    ZMinimap(unsigned int size, EDungeonCell::Type*** cells);
    ZMinimap(const ZMinimap& other);
    ZMinimap(ZMinimap&& other);
    ZMinimap& operator=(ZMinimap other);
    virtual ~ZMinimap();

    friend void swap(ZMinimap& left, ZMinimap& right) {
        using std::swap;

        swap(left.mSize, right.mSize);
        swap(left.mCells, right.mCells);
    }

    unsigned int GetSize() const;
    EDungeonCell::Type GetCell(unsigned int x, unsigned int y) const;

private:
    bool IndicesAreValid(unsigned int x, unsigned int y) const;

    unsigned int mSize;
    EDungeonCell::Type** mCells;
};

}
}

