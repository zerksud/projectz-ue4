#pragma once

#include "Utils/StandardLibrary/String.h"
#include "Utils/StandardLibrary/Utility.h"

namespace prz {
namespace mdl {

class ZPositionDiff {
public:
    ZPositionDiff();
    ZPositionDiff(int dXValue, int dYValue);
    virtual ~ZPositionDiff();

    int GetdX() const;
    int GetdY() const;

    bool operator==(const ZPositionDiff& other) const;
    bool operator!=(const ZPositionDiff& other) const;
    
    int LengthSquare() const;

    utl::ZString ToString() const;

    ZPositionDiff& operator+=(const ZPositionDiff& other);
    
    friend const ZPositionDiff operator*(int mult, const ZPositionDiff& diff);
    friend const ZPositionDiff operator*(const ZPositionDiff& diff, int mult);
    friend const ZPositionDiff operator+(const ZPositionDiff& left, const ZPositionDiff& right);
    friend const ZPositionDiff operator-(const ZPositionDiff& left, const ZPositionDiff& right);

    friend void swap(ZPositionDiff& left, ZPositionDiff& right);

private:
    int mdX;
    int mdY;
};

class ZPosition {
public:
    ZPosition();
    ZPosition(const ZPosition& other);
    ZPosition(int xValue, int yValue);
    virtual ~ZPosition();

    int GetX() const;
    int GetY() const;

    const ZPosition operator+(const ZPositionDiff& diff) const;
    const ZPosition operator-(const ZPositionDiff& diff) const;
    const ZPositionDiff operator-(const ZPosition& other) const;

    bool operator==(const ZPosition& other) const;
    bool operator!=(const ZPosition& other) const;
    
    ZPosition& operator=(const ZPosition& other);
    ZPosition& operator+=(const ZPositionDiff& diff);

    utl::ZString ToString() const;

private:
    int mX;
    int mY;
};

}
}
