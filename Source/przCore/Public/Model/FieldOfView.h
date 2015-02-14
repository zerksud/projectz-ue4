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
    ZFieldOfView(const ZFieldOfView& other);
    ZFieldOfView(ZFieldOfView&& other);
    virtual ~ZFieldOfView();

    ZFieldOfView& operator=(ZFieldOfView other);

    friend void swap(ZFieldOfView& left, ZFieldOfView& right);

    unsigned int GetRadius() const;
    EDungeonCell::Type GetCell(int dx, int dy) const;

private:
    unsigned int mRadius;
    EDungeonCell::Type** mData;
};

}
}
