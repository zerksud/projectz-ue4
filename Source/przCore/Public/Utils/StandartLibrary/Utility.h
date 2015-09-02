#pragma once

#include <utility>

namespace prz {
namespace utl {

    template<typename T1, typename T2>
    using ZPair = std::pair<T1, T2>;

    using std::swap;

}
}
