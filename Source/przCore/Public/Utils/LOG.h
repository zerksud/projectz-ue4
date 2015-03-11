#pragma once

#include "Utils/ILogger.h"
#include "Utils/Services.h"

#define LOGD(...) \
    (GET_SERVICE(prz::utl::ILogger)->Log(prz::utl::ELogPriority::Debug, __FILE__, __LINE__, __VA_ARGS__))

#define LOGE(...) \
    (GET_SERVICE(prz::utl::ILogger)->Log(prz::utl::ELogPriority::Error, __FILE__, __LINE__, __VA_ARGS__))
