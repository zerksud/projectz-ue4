#pragma once

#include "Utils/StandardLibrary/List.h"
#include "Utils/StandardLibrary/String.h"

namespace prz {
namespace utl {
namespace string_helpers {

    ZString Format(const char* format, ...);

    ZString Join(const ZList<ZString>& stringList, const ZString& delimeter = "\n");

}
}
}
