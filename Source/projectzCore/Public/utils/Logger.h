#pragma once

#include "Engine.h"

#define LOGD(format, ...)   (prz::utils::ZLogger::GetInstance().Print(ELogVerbosity::Log, FColor::Yellow, __FILE__, __LINE__, FString::Printf(ANSI_TO_TCHAR(format), __VA_ARGS__)))
#define LOGE(format, ...)   (prz::utils::ZLogger::GetInstance().Print(ELogVerbosity::Error, FColor::Red, __FILE__, __LINE__, FString::Printf(ANSI_TO_TCHAR(format), __VA_ARGS__)))

namespace prz {
    namespace utils {
        class ZLogger {
        public:
            static ZLogger& GetInstance() {
                static ZLogger instance;
                return instance;
            }

            void Print(ELogVerbosity::Type verbosity, const FColor& color, const ANSICHAR* fileName, int32 lineNum, const FString userMessage) const;

        private:
            ZLogger();
            ~ZLogger();

            static const char* kLogFormat;
        };
    }
}
