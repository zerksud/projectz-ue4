#pragma once

#include "Utils/StandartLibrary.h"

namespace prz {
namespace utl {
namespace string_helpers {

    ZString Format(const char* format, ...);

    ZString Join(const ZList<ZString>& stringList, const ZString& delimeter = "\n");

}
}
}
