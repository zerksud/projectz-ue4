#pragma once

#include "Utils/StandardLibrary/ZList.h"
#include "Utils/StandardLibrary/ZString.h"

namespace prz {
namespace utl {
namespace string_helpers {

    ZString Format(const char* format, ...);

    ZString Join(const ZList<ZString>& stringList, const ZString& delimeter = "\n");

}
}
}
