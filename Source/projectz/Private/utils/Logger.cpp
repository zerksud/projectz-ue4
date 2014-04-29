#include "projectz.h"
#include "Logger.h"

namespace projectz {
    namespace utils {

        Logger::Logger() {
        }

        Logger::~Logger() {
        }

        void Logger::print(const char* message) {
            if (GEngine) {
                GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString(message));
            }
        }
    }
}
