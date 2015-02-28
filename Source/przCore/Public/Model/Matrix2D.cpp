#include "przCorePCH.h"
#include "Matrix2D.h"

#include "Model/Direction.h"

namespace prz {
namespace mdl {

ZMatrix2D::ZMatrix2D() {
    mData[0][0] = 1;
    mData[0][1] = 0;
    mData[1][0] = 0;
    mData[1][1] = 1;
}

ZMatrix2D::ZMatrix2D(int v00, int v01, int v10, int v11) {
    mData[0][0] = v00;
    mData[0][1] = v01;
    mData[1][0] = v10;
    mData[1][1] = v11;
}

ZMatrix2D::ZMatrix2D(const ZMatrix2D& other) {
    mData[0][0] = other.mData[0][0];
    mData[0][1] = other.mData[0][1];
    mData[1][0] = other.mData[1][0];
    mData[1][1] = other.mData[1][1];
}

ZMatrix2D& ZMatrix2D::operator=(ZMatrix2D other) {
    swap(*this, other);
    
    return *this;
}

void swap(ZMatrix2D& left, ZMatrix2D& right) {
    using std::swap;
    
    swap(left.mData, right.mData);
}

ZMatrix2D& ZMatrix2D::operator*(const ZMatrix2D& other) {
    ZMatrix2D result;
    result.mData[0][0] = mData[0][0] * other.mData[0][0] + mData[0][1] * other.mData[1][0];
    result.mData[0][1] = mData[0][0] * other.mData[0][1] + mData[0][1] * other.mData[1][1];
    result.mData[1][0] = mData[1][0] * other.mData[0][0] + mData[1][1] * other.mData[1][0];
    result.mData[1][1] = mData[1][0] * other.mData[0][1] + mData[1][1] * other.mData[1][1];
    
    *this = result;
    
    return *this;
}

ZPositionDiff ZMatrix2D::operator*(const ZPositionDiff& diff) const {
    int dx = mData[0][0] * diff.GetdX() + mData[0][1] * diff.GetdY();
    int dy = mData[1][0] * diff.GetdX() + mData[1][1] * diff.GetdY();
    
    return ZPositionDiff(dx, dy);
}

ZMatrix2D ZMatrix2D::FlipX() {
    return ZMatrix2D(-1, 0, 0, 1);;
}

ZMatrix2D ZMatrix2D::FlipY() {
    return ZMatrix2D(1, 0, 0, -1);
}

ZMatrix2D ZMatrix2D::Rotate(int rightAnglesCount) {
    ZDirection direction;
    ETurnDirection turnDirection = rightAnglesCount > 0 ? ETurnDirection::Left : ETurnDirection::Right;
    for (int turnIndex = 0; turnIndex < abs(rightAnglesCount); ++turnIndex) {
        direction.Turn(turnDirection);
    }
    ZPositionDiff sinCos = direction.PredictMove();
    int sin = sinCos.GetdY();
    int cos = sinCos.GetdX();
    
    ZMatrix2D matrix;
    matrix.mData[0][0] = cos;
    matrix.mData[0][1] = -sin;
    matrix.mData[1][0] = sin;
    matrix.mData[1][1] = cos;
    
    return matrix;
}


}
}
