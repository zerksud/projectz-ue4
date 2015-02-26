#ifndef MATRIX2D_H
#define MATRIX2D_H

#include "Model/Position.h"

namespace prz {
namespace mdl {

class ZMatrix2D {
public:
    ZMatrix2D();
    ZMatrix2D(const ZMatrix2D& other);
    ZMatrix2D& operator=(ZMatrix2D other);
    friend void swap(ZMatrix2D& left, ZMatrix2D& right);
    
    ZMatrix2D& operator*(const ZMatrix2D& other);
    ZPositionDiff operator*(const ZPositionDiff& diff) const;
    
    static ZMatrix2D FlipX();
    static ZMatrix2D FlipY();
    static ZMatrix2D Rotate(int rightAnglesCount);
    
private:
    ZMatrix2D(int v00, int v01, int v10, int v11);
    
    enum { size = 2 };
    int mData[size][size];
};

}
}
#endif // MATRIX2D_H
