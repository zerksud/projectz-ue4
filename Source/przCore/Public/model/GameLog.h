#pragma once

#include <list>
#include <string>

namespace prz {
    namespace mdl {

        class ZGameLog {
        public:
            ZGameLog();
            virtual ~ZGameLog() = default;

            void Log(const char* format, ...);
            void SetHistoryMaxSize(unsigned int size);

            typedef std::list<std::string> LogHistory;
            const std::string& GetHistory(const std::string& delimeter = "\n");

        private:
            void AddMessage(const std::string& message);

            LogHistory mHistory;
            unsigned int mHistoryMaxSize;

            bool mCachedHistoryIsValid;
            std::string mCachedHistory;
        };

    }
}
