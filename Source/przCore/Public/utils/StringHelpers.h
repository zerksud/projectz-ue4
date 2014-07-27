#pragma once

#include <list>
#include <string>

#include "Core.h"

namespace prz {
    namespace utl {
        class PRZCORE_API ZString {
        public:
            static std::string Format(const char* format, ...);

            static std::string Join(const std::list<std::string>& stringList, std::string delimeter = "\n");

        };
    }
}
