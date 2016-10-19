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

#include "Utils/ZDictionary.h"

namespace prz {
namespace utl {

class INotificationCenter {
public:
    virtual ~INotificationCenter() = default;

    typedef std::function<void(const ZDictionary& dict)> ZNotificationEventHandler;

    virtual bool AddObserver(const std::string& name, void* observerOwner, ZNotificationEventHandler handler) = 0;
    virtual bool RemoveObserver(const std::string& name, void* observerOwner) = 0;

    virtual bool PostNotification(const std::string& name) = 0;
    virtual bool PostNotification(const std::string& name, const ZDictionary& dict) = 0;
};

}
}
