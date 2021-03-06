// Copyright 2014-2017 Max Molodtsov <zerksud@gmail.com>
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "przCorePCH.h"
#include "Utils/StringHelpers.h"

#include <stdarg.h>
#include <string.h>

#include "Utils/LOG.h"

namespace prz {
namespace utl {
namespace string_helpers {

std::string Format(const char* format, ...) {
    va_list args;
    va_start(args, format);
    int messageSize = vsnprintf(nullptr, 0, format, args);
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

std::string Join(const std::list<std::string>& stringList, const std::string& delimeter) {
    size_t resultLength = 0;
    for (const std::string& str : stringList) {
        resultLength += strlen(str.c_str());
    }
    resultLength += strlen(delimeter.c_str()) * (stringList.size() - 1) + 1;
    
    char* resultBuffer = new char[resultLength];
    if (stringList.size() > 0) {
        const char* firstString = stringList.front().c_str();
        strcpy(resultBuffer, firstString);
        char* currentBufferEnd = resultBuffer + strlen(firstString);
        
        const char* delimeterStr = delimeter.c_str();
        const size_t delimeterLength = strlen(delimeterStr);
        
        for (auto i = ++stringList.begin(), end = stringList.end(); i != end; ++i) {
            strcat(currentBufferEnd, delimeterStr);
            currentBufferEnd += delimeterLength;
            
            const char* str = (*i).c_str();
            strcat(currentBufferEnd, str);
            currentBufferEnd += strlen(str);
        }
    }

    std::string result(resultBuffer);
    delete[] resultBuffer;
    
    return result;
}

}
}
}
