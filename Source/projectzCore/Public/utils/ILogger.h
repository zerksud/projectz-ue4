#pragma once

#include <functional>

#include "Core.h"

namespace prz {
    namespace utl {
        class ILogger {
        public:
            typedef std::function<void(ELogVerbosity::Type verbosity, const FString& message)> ZLogCallback;

            virtual void Log(ELogVerbosity::Type verbosity, const ANSICHAR* fileName, int32 lineNum, const FString userMessage) const = 0;
            virtual void Log(ELogVerbosity::Type verbosity, const ANSICHAR* fileName, int32 lineNum, const ANSICHAR* format, ...) const = 0;
            virtual void SetLogCallback(ZLogCallback callback) = 0;
        };
    }
}
