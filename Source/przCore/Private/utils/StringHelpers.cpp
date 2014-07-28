#include "przCorePrivatePCH.h"
#include "utils/StringHelpers.h"

#include <sstream>

#include "utils/LOG_ANSI.h"

namespace prz {
    namespace utl {

        std::string ZString::Format(const ANSICHAR* format, ...) {
            va_list args;
            va_start(args, format);
            int32 messageSize = vsnprintf(nullptr, 0, format, args);
            va_end(args);

            if (messageSize < 0) {
                LOGE("Can't format '%s' string with arguments", format);
                return "";
            }

            char* userMessage = new char[messageSize + 1];
            va_start(args, format);
            vsprintf(userMessage, format, args);
            va_end(args);

            std::string result = userMessage;

            delete[] userMessage;

            return result;
        }

        std::string ZString::Join(const std::list<std::string> & stringList, const std::string& delimeter) {
            std::stringstream ss;
            if (stringList.size() > 0) {
                ss << stringList.front();
                for (auto i = ++stringList.begin(), end = stringList.end(); i != end; ++i) {
                    ss << delimeter << *i;
                }
            }

            return ss.str();
        }

    }
}
