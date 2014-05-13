#pragma once

#include "utils/ILogger.h"
#include "utils/Services.h"

#define LOGD(format, ...)   (prz::utils::ZServices::GetInstance().GetService<prz::utils::ILogger>()->Log(ELogVerbosity::Log, __FILE__, __LINE__, FString::Printf(ANSI_TO_TCHAR(format), __VA_ARGS__)))
#define LOGE(format, ...)   (prz::utils::ZServices::GetInstance().GetService<prz::utils::ILogger>()->Log(ELogVerbosity::Error, __FILE__, __LINE__, FString::Printf(ANSI_TO_TCHAR(format), __VA_ARGS__)))

#define LOGD_ANSI(format, ...)  (prz::utils::ZServices::GetInstance().GetService<prz::utils::ILogger>()->Log(ELogVerbosity::Log, __FILE__, __LINE__, format, __VA_ARGS__))
#define LOGE_ANSI(format, ...)  (prz::utils::ZServices::GetInstance().GetService<prz::utils::ILogger>()->Log(ELogVerbosity::Error, __FILE__, __LINE__, format, __VA_ARGS__))
