// Copyright 2016 Max Molodtsov <zerksud@gmail.com>
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <functional>

namespace prz {
namespace utl {

enum class ELogPriority {
	Debug,
	Error
};
	
class ILogger {
public:
    virtual ~ILogger() = default;

    typedef std::function<void(ELogPriority priority, const char* fileName, int lineNum, const char* message)> ZLogCallback;

    virtual void Log(ELogPriority priority, const char* fileName, int lineNum, const char* format, ...) const = 0;
    virtual void SetLogCallback(ZLogCallback callback) = 0;
};

}
}
