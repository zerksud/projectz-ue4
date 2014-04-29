#pragma once

#define LOGD(message)   (projectz::utils::Logger::getInstance().print(message))

namespace projectz {
    namespace utils {
        class Logger {
        public:
            static Logger& getInstance() {
                static Logger instance;
                return instance;
            }

            void print(const char* message);

        private:
            Logger();
            ~Logger();
        };
    }
}
