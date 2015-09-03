#pragma once

#include "Utils/StandartLibrary/List.h"
#include "Utils/StandartLibrary/String.h"

namespace prz {
namespace utl {
namespace string_helpers {

    ZString Format(const char* format, ...);

    ZString Join(const ZList<ZString>& stringList, const ZString& delimeter = "\n");

}
}
}
