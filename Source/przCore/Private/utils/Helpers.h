#pragma once

#include <algorithm>
#include <vector>

namespace prz {
    namespace utl {

        template<typename T>
        bool VectorContains(const std::vector<T>& vector, const T& value) {
            auto pos = std::find(vector.begin(), vector.end(), value);
            bool contains = pos != vector.end();

            return contains;
        }

    }
}
