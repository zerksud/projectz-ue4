#pragma once

#include <functional>

#include "Core.h"

#define LOGD(format, ...)   (prz::utils::ZLogger::GetInstance().Log(ELogVerbosity::Log, __FILE__, __LINE__, FString::Printf(ANSI_TO_TCHAR(format), __VA_ARGS__)))
#define LOGE(format, ...)   (prz::utils::ZLogger::GetInstance().Log(ELogVerbosity::Error, __FILE__, __LINE__, FString::Printf(ANSI_TO_TCHAR(format), __VA_ARGS__)))

#define LOGD_ANSI(format, ...)  (prz::utils::ZLogger::GetInstance().Log(ELogVerbosity::Log, __FILE__, __LINE__, format, __VA_ARGS__))
#define LOGE_ANSI(format, ...)  (prz::utils::ZLogger::GetInstance().Log(ELogVerbosity::Error, __FILE__, __LINE__, format, __VA_ARGS__))

namespace prz {
    namespace utils {
        class ZLogger {
        public:
            static ZLogger& GetInstance() {
                static ZLogger instance;
                return instance;
            }

            typedef std::function<void(ELogVerbosity::Type verbosity, const FString& message)> ZLogCallback;

            void Log(ELogVerbosity::Type verbosity, const ANSICHAR* fileName, int32 lineNum, const FString userMessage) const;
            void Log(ELogVerbosity::Type verbosity, const ANSICHAR* fileName, int32 lineNum, const ANSICHAR* format, ...) const;
            void SetLogCallback(ZLogCallback callback);

        private:
            ZLogger();
            ~ZLogger();

            static const char* kLogFormat;
            ZLogCallback mLogCallback;
        };
    }
}
