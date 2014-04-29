#pragma once

#define LOGD(...)   (projectz::utils::Logger::getInstance().print(__FILE__, __LINE__, __VA_ARGS__))

namespace projectz {
    namespace utils {
        class Logger {
        public:
            static Logger& getInstance() {
                static Logger instance;
                return instance;
            }

            void print(const char* fileName, int32 lineNum, const char* format, ...) const;

        private:
            Logger();
            ~Logger();

            static const char* mLogFormat;
        };
    }
}
