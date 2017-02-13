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
#include "Model/ZGameLog.h"

#include <stdarg.h>

#include "Utils/LOG.h"
#include "Utils/StringHelpers.h"

namespace prz {
namespace mdl {

ZGameLog::ZGameLog()
    : mHistoryMaxSize(6),
    mCachedHistoryIsValid(false) {
}

void ZGameLog::Clear() {
    mHistory.clear();
    mCachedHistoryIsValid = false;
}

void ZGameLog::Log(const char* format, ...) {
    va_list args;
    va_start(args, format);
    int messageSize = vsnprintf(nullptr, 0, format, args);
    va_end(args);

    if (messageSize < 0) {
        LOGE("Can't use format '%s' for loging game event");
        return;
    }

    char* userMessage = new char[messageSize + 1];
    va_start(args, format);
    vsprintf(userMessage, format, args);
    va_end(args);

    AddMessage(userMessage);

    delete[] userMessage;
}

void ZGameLog::AddMessage(const std::string& message) {
    if (mHistory.size() >= mHistoryMaxSize) {
        mHistory.pop_back();
    }

    mHistory.push_front(message);
    mCachedHistoryIsValid = false;
}

void ZGameLog::SetHistoryMaxSize(unsigned int size) {
    if (size > 0) {
        mHistoryMaxSize = size;
    } else {
        LOGE("Can't set HistoryMaxSize to %d", size);
    }
}

const std::string& ZGameLog::GetHistory(const std::string& delimeter) {
    if (!mCachedHistoryIsValid) {
        mCachedHistory = utl::string_helpers::Join(mHistory, delimeter);
        mCachedHistoryIsValid = true;
    }

    return mCachedHistory;
}

}
}
