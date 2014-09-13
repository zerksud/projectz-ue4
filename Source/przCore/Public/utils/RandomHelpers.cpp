#include "przCorePCH.h"
#include "utils/RandomHelpers.h"

#include <cstdlib>
#include <ctime>

namespace prz {
    namespace utl {

        void ZRandomHelpers::Initialize() {
            std::srand(std::time(0));
        }

        bool ZRandomHelpers::FlipCoin() {
            if (std::rand() % 2) {
                return true;
            }

            return false;
        }

        int ZRandomHelpers::GetRandomValue(int maxValue) {
            int value = std::rand() % (maxValue + 1);
            return value;
        }

        int ZRandomHelpers::GetRandomValue(int minValue, int maxValue) {
            int value = minValue + std::rand() % (maxValue - minValue + 1);
            return value;
        }

    }
}
