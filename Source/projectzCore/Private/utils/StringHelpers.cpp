#include "projectzCorePrivatePCH.h"
#include "utils/StringHelpers.h"

#include "utils/LOG_ANSI.h"

namespace prz {
    namespace utl {
        namespace ZString {

            std::string Format(const ANSICHAR* format, ...) {
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

        }
    }
}
