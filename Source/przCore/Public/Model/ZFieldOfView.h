#pragma once

namespace prz {
namespace mdl {

enum class EDungeonCell {
    Unknown,
    SolidRock,
    Emptiness,
    DownStaircase,
    UpStaircase,
    Monster
};

class ZFieldOfView {
public:
    ZFieldOfView();
    // ZFieldOfView owns fovData
    ZFieldOfView(unsigned int radius, EDungeonCell** fovData);
    ZFieldOfView(const ZFieldOfView& other);
    ZFieldOfView(ZFieldOfView&& other);
    virtual ~ZFieldOfView();

    ZFieldOfView& operator=(ZFieldOfView other);

    friend void swap(ZFieldOfView& left, ZFieldOfView& right);

    unsigned int GetRadius() const;
    EDungeonCell GetCell(int dx, int dy) const;

private:
    unsigned int mRadius;
    EDungeonCell** mData;
};

}
}
