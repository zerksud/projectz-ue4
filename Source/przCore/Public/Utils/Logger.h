#pragma once

#include "Utils/ILogger.h"

namespace prz {
    namespace utl {
        class PRZCORE_API ZLogger : public ILogger {
        public:
            ZLogger();
            virtual ~ZLogger();

            virtual void Log(ELogVerbosity::Type verbosity, const ANSICHAR* fileName, int32 lineNum, const FString userMessage) const override;
            virtual void Log(ELogVerbosity::Type verbosity, const ANSICHAR* fileName, int32 lineNum, const ANSICHAR* format, ...) const override;
            virtual void SetLogCallback(ZLogCallback callback) override;

        private:
            static const char* kLogFormat;
            static FString FormatUserMessage(const ANSICHAR* fileName, int32 lineNum, const TCHAR* userMessage);

            ZLogCallback mLogCallback;
        };
    }
}
