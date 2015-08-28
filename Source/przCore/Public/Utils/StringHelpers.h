#pragma once

#include <list>
#include <string>

namespace prz {
namespace utl {
namespace string_helpers {

    std::string Format(const char* format, ...);

    std::string Join(const std::list<std::string>& stringList, const std::string& delimeter = "\n");

}
}
}
