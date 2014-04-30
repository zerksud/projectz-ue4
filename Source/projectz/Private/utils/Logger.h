#pragma once

#define LOGD(...)   (projectz::utils::Logger::getInstance().print(ELogVerbosity::Log, FColor::Yellow, __FILE__, __LINE__, __VA_ARGS__))
#define LOGE(...)   (projectz::utils::Logger::getInstance().print(ELogVerbosity::Error, FColor::Red, __FILE__, __LINE__, __VA_ARGS__))

namespace projectz {
    namespace utils {
        class Logger {
        public:
            static Logger& getInstance() {
                static Logger instance;
                return instance;
            }

            void print(ELogVerbosity::Type verbosity, const FColor& color, const ANSICHAR* fileName, int32 lineNum, const ANSICHAR* format, ...) const;

        private:
            Logger();
            ~Logger();

            static const char* kLogFormat;
        };
    }
}
