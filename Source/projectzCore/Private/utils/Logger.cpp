#include "projectzCorePrivatePCH.h"
#include "Logger.h"

DEFINE_LOG_CATEGORY_STATIC(ProjectZ, All, All)

//#define VERBOSE

namespace prz {
    namespace utils {

        ZLogger::ZLogger() {
        }

        ZLogger::~ZLogger() {
        }

        const char* ZLogger::kLogFormat =
#ifdef VERBOSE
            "[%s][%s:%d] "
#endif
            "%s";

        void ZLogger::Log(ELogVerbosity::Type verbosity, const ANSICHAR* fileName, int32 lineNum, const FString userMessage) const {
#ifdef VERBOSE
            const FString currentDate = FDateTime::UtcNow().ToString();
#endif
            const FString logMessage = FString::Printf(ANSI_TO_TCHAR(kLogFormat),
#ifdef VERBOSE
                *currentDate, ANSI_TO_TCHAR(fileName), lineNum,
#endif
                *userMessage);

            FMsg::Logf(fileName, lineNum, ProjectZ.GetCategoryName(), verbosity, *logMessage);

            if (mLogCallback) {
                mLogCallback(verbosity, userMessage);
            }
        }

        void ZLogger::Log(ELogVerbosity::Type verbosity, const ANSICHAR* fileName, int32 lineNum, const ANSICHAR* format, ...) const {
            va_list args;
            va_start(args, format);
            int32 messageSize = vsnprintf(nullptr, 0, format, args);
            va_end(args);

            if (messageSize < 0) {
                UE_LOG(ProjectZ, Error, TEXT("Can't use format '%s' for logging from %s:%d"), ANSI_TO_TCHAR(format), ANSI_TO_TCHAR(fileName), lineNum);
                return;
            }

            char* userMessage = new char[messageSize + 1];
            va_start(args, format);
            vsprintf(userMessage, format, args);
            va_end(args);

            Log(verbosity, fileName, lineNum, ANSI_TO_TCHAR(userMessage));

            delete userMessage;
        }

        void ZLogger::SetLogCallback(ZLogCallback callback) {
            mLogCallback = callback;
        }
    }
}
