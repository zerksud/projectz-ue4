#include "projectz.h"

#include "Logger.h"

DEFINE_LOG_CATEGORY_STATIC(ProjectZ, All, All)

namespace projectz {
    namespace utils {

        Logger::Logger() {
        }

        Logger::~Logger() {
        }

        const char* Logger::kLogFormat = "[%s:%d] %s";

        void Logger::print(ELogVerbosity::Type verbosity, const FColor& color, const ANSICHAR* fileName, int32 lineNum, const ANSICHAR* format, ...) const {
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

            const FString logMessage = FString::Printf(ANSI_TO_TCHAR(kLogFormat), ANSI_TO_TCHAR(fileName), lineNum, ANSI_TO_TCHAR(userMessage));

            FMsg::Logf(fileName, lineNum, ProjectZ.GetCategoryName(), verbosity, TEXT("%s"), *logMessage);

            if (GEngine) {
                GEngine->AddOnScreenDebugMessage(-1, 5.0f, color, ANSI_TO_TCHAR(userMessage));
            }

            delete[] userMessage;
        }
    }
}
