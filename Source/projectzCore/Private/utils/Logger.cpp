#include "projectzCorePrivatePCH.h"
#include "Logger.h"

#include "Engine.h"

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

        void ZLogger::Print(ELogVerbosity::Type verbosity, const FColor& color, const ANSICHAR* fileName, int32 lineNum, const FString userMessage) const {
#ifdef VERBOSE
            const FString currentDate = FDateTime::UtcNow().ToString();
#endif
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
