#include "projectz.h"
#include "Logger.h"

DEFINE_LOG_CATEGORY_STATIC(ProjectZ, All, All)

namespace projectz {
    namespace utils {

        Logger::Logger() {
        }

        Logger::~Logger() {
        }

        const char* Logger::mLogFormat = "[%s:%d] %s";

        void Logger::print(const char* fileName, int32 lineNum, const char* format, ...) const {
            va_list args;
            va_start(args, format);
            int32 messageSize = vsnprintf(nullptr, 0, format, args);
            va_end(args);

            if (messageSize > 0) {
                UE_LOG(ProjectZ, Error, TEXT("Can't use format '%s' for logging from %s:%d"), ANSI_TO_TCHAR(format), ANSI_TO_TCHAR(fileName), lineNum);
                return;
            }

            char* userMessage = new char[messageSize + 1];
            va_start(args, format);
            vsprintf(userMessage, format, args);
            va_end(args);

            const FString logMessage = FString::Printf(ANSI_TO_TCHAR(mLogFormat), ANSI_TO_TCHAR(fileName), lineNum, ANSI_TO_TCHAR(userMessage));

            UE_LOG(ProjectZ, Log, TEXT("%s"), *logMessage);

            if (GEngine) {
                GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, ANSI_TO_TCHAR(userMessage));
            }

            delete[] userMessage;
        }
    }
}
