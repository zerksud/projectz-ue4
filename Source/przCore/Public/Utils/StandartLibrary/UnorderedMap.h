#pragma once

#include <unordered_map>

namespace prz {
namespace utl {

    template<typename TKey, typename TValue>
    using ZUnorderedMap = std::unordered_map<TKey, TValue>;

}
}
