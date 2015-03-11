#pragma once

#include "Utils/ILogger.h"

#include "Platform.h"

namespace prz {
namespace utl {

class PRZCORE_API ZLogger : public ILogger {
public:
    ZLogger() = default;
    virtual ~ZLogger() = default;

    virtual void Log(ELogPriority priority, const char* fileName, int lineNum, const char* format, ...) const override;
    virtual void SetLogCallback(ZLogCallback callback) override;

private:
    ZLogCallback mLogCallback;
};

}
}
