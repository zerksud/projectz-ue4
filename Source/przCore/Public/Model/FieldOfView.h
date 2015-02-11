#pragma once

namespace prz {
namespace mdl {

namespace EDungeonCell {
enum Type {
    Unknown,
    SolidRock,
    Emptiness,
    DownStaircase,
    UpStaircase,
    Monster
};
}

class ZFieldOfView {
public:
    // ZFieldOfView owns fovData
    ZFieldOfView(unsigned int radius, EDungeonCell::Type** fovData);
    virtual ~ZFieldOfView();

    unsigned int GetRadius() const;
    EDungeonCell::Type GetCell(int dx, int dy) const;

private:
    unsigned int mRadius;
    EDungeonCell::Type** mData;
};

}
}
