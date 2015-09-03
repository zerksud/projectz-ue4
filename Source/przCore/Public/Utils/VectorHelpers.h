#pragma once

#include "Utils/StandardLibrary/Algorithm.h"
#include "Utils/StandardLibrary/Vector.h"

namespace prz {
namespace utl {

template<typename T>
bool VectorContains(const ZVector<T>& vector, const T& value) {
    auto pos = find(vector.begin(), vector.end(), value);
    bool contains = pos != vector.end();

    return contains;
}

template<typename T>
void VectorInsertAll(ZVector<T>* dst, const ZVector<T>& src) {
    dst->insert(dst->end(), src.begin(), src.end());
}

}
}
