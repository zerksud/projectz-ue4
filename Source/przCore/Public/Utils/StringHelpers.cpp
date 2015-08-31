#include "przCorePCH.h"
#include "Utils/StringHelpers.h"

#include <stdarg.h>
#include <string.h>

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
    size_t result_length = 0;
    for (const ZString& str : stringList) {
        result_length += strlen(str.c_str());
    }
    result_length += strlen(delimeter.c_str()) * (stringList.size() - 1) + 1;
    
    char* buffer = new char[result_length];
    if (stringList.size() > 0) {
        const char* first_string = stringList.front().c_str();
        strcpy(buffer, first_string);
        char* current_buffer_end = buffer + strlen(first_string);
        
        const char* delimeter_str = delimeter.c_str();
        const size_t delimeter_length = strlen(delimeter_str);
        
        for (auto i = ++stringList.begin(), end = stringList.end(); i != end; ++i) {
            strcat(current_buffer_end, delimeter_str);
            current_buffer_end += delimeter_length;
            
            const char* str = (*i).c_str();
            strcat(current_buffer_end, str);
            current_buffer_end += strlen(str);
        }
    }

    ZString result(buffer);
    delete[] buffer;
    
    return result;
}

}
}
}
