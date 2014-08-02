#pragma once

namespace prz {
    namespace utl {

        class ZRandomHelpers {
        public:
            ZRandomHelpers() = delete;

            static void Initialize();
            static bool FlipCoin();
            static int GetRandomValue(int maxValue);
            static int GetRandomValue(int minValue, int maxValue);
        };

    }
}
