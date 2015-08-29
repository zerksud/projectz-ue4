#include "przCorePCH.h"
#include "Utils/RandomHelpers.h"

#include "Utils/StandartLibrary.h"

namespace prz {
namespace utl {

void ZRandomHelpers::Initialize() {
    utl::srand(static_cast<unsigned int>(utl::time(0)));
}

bool ZRandomHelpers::FlipCoin() {
    if (utl::rand() % 2) {
        return true;
    }

    return false;
}

int ZRandomHelpers::GetRandomValue(int maxValue) {
    int value = utl::rand() % (maxValue + 1);
    return value;
}

int ZRandomHelpers::GetRandomValue(int minValue, int maxValue) {
    int value = minValue + utl::rand() % (maxValue - minValue + 1);
    return value;
}

}
}
