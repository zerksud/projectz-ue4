#include "przCorePCH.h"
#include "Logger.h"

namespace prz {
namespace utl {

void ZLogger::Log(ELogPriority priority, const char* fileName, int lineNum, const char* format, ...) const {
    va_list args;
    va_start(args, format);
    int32 messageSize = vsnprintf(nullptr, 0, format, args);
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
