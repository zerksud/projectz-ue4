#pragma once

#include "Utils/ILogger.h"
#include "Utils/Services.h"

#define LOGD(format, ...) \
    (GET_SERVICE(prz::utl::ILogger)->Log(ELogVerbosity::Log, __FILE__, __LINE__, FString::Printf(ANSI_TO_TCHAR(format), ##__VA_ARGS__)))

#define LOGE(format, ...) \
    (GET_SERVICE(prz::utl::ILogger)->Log(ELogVerbosity::Error, __FILE__, __LINE__, FString::Printf(ANSI_TO_TCHAR(format), ##__VA_ARGS__)))

#define LOGD_ANSI(...) \
    (GET_SERVICE(prz::utl::ILogger)->Log(ELogVerbosity::Log, __FILE__, __LINE__, __VA_ARGS__))

#define LOGE_ANSI(...) \
    (GET_SERVICE(prz::utl::ILogger)->Log(ELogVerbosity::Error, __FILE__, __LINE__, __VA_ARGS__))
