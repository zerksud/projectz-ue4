#pragma once

#include "utils/ILogger.h"
#include "utils/Services.h"

#define LOGD(format, ...)   (prz::utl::ZServices::GetInstance().GetService<prz::utl::ILogger>()->Log(ELogVerbosity::Log, __FILE__, __LINE__, FString::Printf(ANSI_TO_TCHAR(format), __VA_ARGS__)))
#define LOGE(format, ...)   (prz::utl::ZServices::GetInstance().GetService<prz::utl::ILogger>()->Log(ELogVerbosity::Error, __FILE__, __LINE__, FString::Printf(ANSI_TO_TCHAR(format), __VA_ARGS__)))

#define LOGD_ANSI(format, ...)  (prz::utl::ZServices::GetInstance().GetService<prz::utl::ILogger>()->Log(ELogVerbosity::Log, __FILE__, __LINE__, format, __VA_ARGS__))
#define LOGE_ANSI(format, ...)  (prz::utl::ZServices::GetInstance().GetService<prz::utl::ILogger>()->Log(ELogVerbosity::Error, __FILE__, __LINE__, format, __VA_ARGS__))
