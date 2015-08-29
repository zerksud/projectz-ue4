#pragma once

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <functional>
#include <limits>
#include <list>
#include <map>
#include <string>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <utility>

namespace prz {
namespace utl {

    template<typename T>
    using ZFunction = std::function<T>;

    template<typename T>
    using ZList = std::list<T>;

    template<typename TKey, typename TValue>
    using ZMap = std::map<TKey, TValue>;

    template<typename T>
    using ZNumericLimits = std::numeric_limits<T>;

    template<typename T1, typename T2>
    using ZPair = std::pair<T1, T2>;

    template<typename T, typename TContainer, typename TCompare>
    using ZPriorityQueue = std::priority_queue<T, TContainer, TCompare>;

    using ZString = std::string;

    template<typename TKey, typename TValue>
    using ZUnorderedMap = std::unordered_map<TKey, TValue>;

    template<typename T>
    using ZUnorderedSet = std::unordered_set<T>;

    template<typename T>
    using ZVector = std::vector<T>;

    using std::abs;
    using std::floor;

    using std::min;
    using std::max;

    using std::min_element;
    using std::max_element;

    using std::fill_n;
    using std::find;
    using std::find_if;
    using std::sort;
    using std::shuffle;

    using std::default_random_engine;
    using std::srand;
    using std::rand;

    using std::time;

    using std::swap;

}
}
