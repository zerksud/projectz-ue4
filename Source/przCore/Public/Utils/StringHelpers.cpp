#include "przCorePCH.h"
#include "Utils/StringHelpers.h"

#include <stdarg.h>

#include <sstream>

#include "Utils/LOG.h"

namespace prz {
namespace utl {
namespace string_helpers {

ZString Format(const char* format, ...) {
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

    ZString result = userMessage;

    delete[] userMessage;

    return result;
}

ZString Join(const ZList<ZString> &stringList, const ZString &delimeter) {
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
}
