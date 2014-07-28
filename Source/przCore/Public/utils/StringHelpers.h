#pragma once

#include <list>
#include <string>

namespace prz {
    namespace utl {

        class ZString {
        public:
            ZString() = delete;

            static std::string Format(const char* format, ...);

            static std::string Join(const std::list<std::string>& stringList, const std::string& delimeter = "\n");

        };

    }
}
