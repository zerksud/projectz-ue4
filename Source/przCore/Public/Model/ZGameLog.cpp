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

void ZGameLog::AddMessage(const utl::ZString &message) {
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

const utl::ZString &ZGameLog::GetHistory(const utl::ZString &delimeter) {
    if (!mCachedHistoryIsValid) {
        mCachedHistory = utl::string_helpers::Join(mHistory, delimeter);
        mCachedHistoryIsValid = true;
    }

    return mCachedHistory;
}

}
}
