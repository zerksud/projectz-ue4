// Copyright 2016 Max Molodtsov <zerksud@gmail.com>
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
#include "ZLogger.h"

#include <stdarg.h>

namespace prz {
namespace utl {

void ZLogger::Log(ELogPriority priority, const char* fileName, int lineNum, const char* format, ...) const {
    va_list args;
    va_start(args, format);
    int messageSize = vsnprintf(nullptr, 0, format, args);
    va_end(args);

    if (messageSize < 0) {
        return;
    }

    char* userMessage = new char[messageSize + 1];
    va_start(args, format);
    vsprintf(userMessage, format, args);
    va_end(args);

    if (mLogCallback) {
        mLogCallback(priority, fileName, lineNum, userMessage);
    }

    delete[] userMessage;
}

void ZLogger::SetLogCallback(ZLogCallback callback) {
    mLogCallback = callback;
}

}
}
