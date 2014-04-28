#pragma once

#define LOGD(message)   (projectz::Logger::getInstance().print(message))

namespace projectz {

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
