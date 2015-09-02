#pragma once

#include <queue>

namespace prz {
namespace utl {

    template<typename T, typename TContainer, typename TCompare>
    using ZPriorityQueue = std::priority_queue<T, TContainer, TCompare>;

}
}
