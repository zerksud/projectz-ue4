#include "przCorePCH.h"
#include "Utils/RandomHelpers.h"

#include "Utils/StandardLibrary/StdLib.h"
#include "Utils/StandardLibrary/Time.h"

namespace prz {
namespace utl {
namespace random_helpers {

void Initialize() {
    utl::srand(static_cast<unsigned int>(utl::time(0)));
}

bool FlipCoin() {
    if (utl::rand() % 2) {
        return true;
    }

    return false;
}

int GetRandomValue(int maxValue) {
    int value = utl::rand() % (maxValue + 1);
    return value;
}

int GetRandomValue(int minValue, int maxValue) {
    int value = minValue + utl::rand() % (maxValue - minValue + 1);
    return value;
}

}
}
}
