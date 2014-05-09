#pragma once

#define LOGD(format, ...)   (prz::utils::Logger::GetInstance().Print(ELogVerbosity::Log, FColor::Yellow, __FILE__, __LINE__, FString::Printf(ANSI_TO_TCHAR(format), __VA_ARGS__)))
#define LOGE(format, ...)   (prz::utils::Logger::GetInstance().Print(ELogVerbosity::Error, FColor::Red, __FILE__, __LINE__, FString::Printf(ANSI_TO_TCHAR(format), __VA_ARGS__)))

namespace prz {
    namespace utils {
        class Logger {
        public:
            static Logger& GetInstance() {
                static Logger instance;
                return instance;
            }

            void Print(ELogVerbosity::Type verbosity, const FColor& color, const ANSICHAR* fileName, int32 lineNum, const FString userMessage) const;

        private:
            Logger();
            ~Logger();

            static const char* kLogFormat;
        };
    }
}
