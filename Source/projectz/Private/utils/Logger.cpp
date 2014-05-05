#include "projectz.h"

#include "Logger.h"

DEFINE_LOG_CATEGORY_STATIC(ProjectZ, All, All)

//#define VERBOSE

namespace projectz {
    namespace utils {

        Logger::Logger() {
        }

        Logger::~Logger() {
        }

        const char* Logger::kLogFormat =
#ifdef VERBOSE
            "[%s][%s:%d] "
#endif
            "%s";

        void Logger::print(ELogVerbosity::Type verbosity, const FColor& color, const ANSICHAR* fileName, int32 lineNum, const FString userMessage) const {
            const FString currentDate = FDateTime::UtcNow().ToString();
            const FString logMessage = FString::Printf(ANSI_TO_TCHAR(kLogFormat),
#ifdef VERBOSE
                *currentDate, ANSI_TO_TCHAR(fileName), lineNum,
#endif
                *userMessage);

            FMsg::Logf(fileName, lineNum, ProjectZ.GetCategoryName(), verbosity, *logMessage);

            if (GEngine) {
                GEngine->AddOnScreenDebugMessage(-1, 5.0f, color, userMessage);
            }
        }
    }
}
