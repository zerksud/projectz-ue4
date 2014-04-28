#include "projectz.h"
#include "Logger.h"

namespace projectz {

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
